#include "executor.h"

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "builtins.h"

Executor::Executor(ShellEnv& shellEnv, int& exitCode)
    : exitCode(exitCode), shellEnv(shellEnv), commandLookup(shellEnv.commandLookup) {};

[[nodiscard]] bool Executor::executeCommands(const std::vector<Command>& commands) {
    bool success = true;
    const auto cmdsLen = commands.size();

    for (size_t i = 0; i < cmdsLen; ++i) {
        const auto& cmd = commands[i];
        if (cmd.executable.empty()) continue;

         // Saves from forking and unnecessary execvp
        if (!cmd.isBuiltin && !commandLookup.findExecutable(cmd.executable.c_str(), shellEnv.PATH)) {
            // Currently makes no distinction between not found and no permission
            std::cout << cmd.executable << ": Command not found or no permission" << std::endl;
            continue;
        }

        switch (cmd.op) {
            case Operator::None:
                success = !cmd.isBuiltin ? execBasic(cmdToCharVec(cmd)) : execBuiltin(cmd);
                break;
            case Operator::Pipe: {
                std::vector<Command> pipeline{ cmd };

                // Support a multi-command pipeline
                while (i + 1 < cmdsLen && commands[i + 1].op == Operator::Pipe) {
                    pipeline.push_back(commands[++i]);
                }
                if (i + 1 < cmdsLen) {
                    pipeline.push_back(commands[++i]); // Last element that only reads
                } 
                
                success = execPipe(pipeline);
                break;
            }
            case Operator::Background:
                success = execBg(cmd);
                break;
            case Operator::Redirection:
                execRedir(cmd);
                break;
            default:
                success = false;
                break;
        }
    }
    return success;
}

// Forks the current process and executes the specified command in the child.
// Takes a reference to a pid_t to store the child’s PID, allowing the caller to wait or manage it.
bool Executor::exec(const std::vector<const char*>& args, pid_t& childPid) {
    // execvp() requires null-terminated arrays.
    if (args.empty() || args.back() != nullptr) {
        return false;
    }

    childPid = fork();
    if (childPid == 0) { // Child process
        execvp(args[0], const_cast<char* const*>(args.data()));
        // In case of an error
        perror("execvp");
        _exit(1);
    } else if (childPid == -1) { // Fork failed
        perror("fork");
        return false;
    }

    return true;
}

bool Executor::execBasic(const std::vector<const char*>& args) {
    pid_t childPid;
    
    if (exec(args, childPid)) {
        // Main process
        waitChildPid(childPid);
        return true;
    } else {
        return false;
    }
}

// Starts a child process in the backgorund.
bool Executor::execBg(const Command& cmd) {
    // At the moment this doesn't release the terminal to the parent
    pid_t childPid;
    if (cmd.isBuiltin) {
        childPid = fork();
        if (childPid == 0) {
            return execBuiltin(cmd);
        } else if (childPid == -1) {
            return false;
        }
    }
    return exec(cmdToCharVec(cmd), childPid);
}

// Executes the given command with a specified fd out and/or fd in
// Requires open fds in an array of integers
bool Executor::execDup(
    pid_t& childPid,
    const Command& cmd,
    const std::vector<int>& fds, // Makes easier to use in both pipe and redirection
    int fdOut,
    int fdIn)
{

    if (cmd.executable.empty()) return false;

    // Can be used both from redirection and pipes
    childPid = fork();
    if (childPid == 0) { // Child process
        if (fdOut >= 0) {
            if (dup2(fdOut, STDOUT_FILENO) == -1) {
                perror("dup2 fdOut");
                _exit(1);
            }
        }

        if (fdIn >= 0) {
            if (dup2(fdIn, STDIN_FILENO) == -1) {
                perror("dup2 fdIn");
                _exit(1);
            }
        }

        for (auto fd: fds) {
            close(fd);
        }

        if (cmd.isBuiltin) {
            _exit(execBuiltin(cmd) ? 0 : 1); // Exit the child process
        } else {
            auto args = cmdToCharVec(cmd);
            execvp(args[0], const_cast<char* const*>(args.data()));
            perror("execvp");
            _exit(1);
        }
    } else if (childPid == -1) { // Fork failed
        perror("fork");
        return false;
    }
    return true;
}

// Executes the specified pipeline of commands. Creates N-1 pipes between commands.
// Currently doesn't support other operations at the ends of the pipeline.
bool Executor::execPipe(const std::vector<Command>& commands) {
    std::vector<pid_t> pipePids;
    std::vector<int> pipes;

    const auto len = commands.size();
    // Create N-1 pipes
    for (size_t j = 0; j < len - 1; ++j) {
        int fd[2];
        if (pipe(fd) == -1) {
            exitCode = 1;
            return false;
        }
        // Not the most elegant, but I'll allow it for now
        pipes.push_back(fd[0]);
        pipes.push_back(fd[1]);
    }
    
    for (size_t i = 0; i < len; ++i) {
        execDup(
            pipePids.emplace_back(),
            commands[i],
            pipes,
            i < (len - 1) ? pipes[i * 2 + 1] : -1, // Current write end. If last, no outfile.
            i != 0 ? pipes[(i - 1) * 2] : -1 // Last read end. If first, no infile.
        );
    }

    for (auto pipe: pipes) {
        close(pipe);
    }
    
    for (auto childPid: pipePids) {
        waitChildPid(childPid);
    }
    return true;
}

// Redirects command output to the file specified
// At the moment only works for output and not input 
void Executor::execRedir(const Command& cmd) {
    if (cmd.outFile.empty()) {
        std::cerr << "No output file specified" << std::endl;
        exitCode = 1;
        return;
    }
    // Note: Could break the following branches into seperate functions
    if (cmd.isBuiltin) {
        std::ofstream outFile(cmd.outFile);
        if (!outFile) {
            exitCode = 1;
            return;
        }

        // Change the cout buffer to the file outputstream and save original
        auto* originalBuf = std::cout.rdbuf(outFile.rdbuf());
        bool success = execBuiltin(cmd);

        std::cout.flush();
        std::cout.rdbuf(originalBuf);
        exitCode = success != 0;

    } else {
        int outFile = open(cmd.outFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // -rw-r--r--
        if (outFile < 0) {
            perror("open");
            exitCode = 1;
            return;
        }
        
        pid_t childPid;
        if (!execDup(childPid, cmd, { outFile }, outFile, -1)) {
            close(outFile);
            exitCode = 1;
            return;
        }
        close(outFile);

        waitChildPid(childPid);
    }
}

bool Executor::execBuiltin(const Command& cmd) {
    return Builtins::handleBuiltin(cmd, shellEnv) == Builtins::Control::BREAK
        ? false
        : true;
}

// Waits for the given pid and sets exit status accordingly
void Executor::waitChildPid(pid_t childPid) {
    int status;
    waitpid(childPid, &status, 0);
    // Get the childs status code
    if (WIFEXITED(status)) {
        exitCode = WEXITSTATUS(status);
    } else {
        exitCode = 1;
    }
}

// Converts a command struct (exec and args) into a char* vector.
// The passed struct must outlive the function.
const std::vector<const char*> Executor::cmdToCharVec(const Command& cmd) const {
    std::vector<const char*> commands;
    commands.push_back(cmd.executable.data());
    for (auto& arg: cmd.args) {
        commands.push_back(arg.data());
    }
    commands.push_back(nullptr);
    return commands;
}
