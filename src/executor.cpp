#include "executor.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "builtins.h"

Executor::Executor(ShellEnv& shellEnv, int& exitCode)
    : exitCode(exitCode), shellEnv(shellEnv), commandLookup(shellEnv.commandLookup) {};

[[nodiscard]] bool Executor::executeExternalCommand(
    std::vector<Command>& commands) const 
{
    bool success = true;
    for (Command& cmd: commands) {
        if (cmd.executable.empty()) continue;

        bool isBuiltin = Builtins::isBuiltin(cmd);
         // Saves from forking and unnecessary execvp
        if (!isBuiltin && !commandLookup.findExecutable(cmd.executable.c_str(), shellEnv.PATH)) {
            // Currently makes no distinction between not found and no permission
            std::cout << cmd.executable << ": Command not found or no permission" << std::endl;
            continue;
        }
        switch (cmd.op) {
            case Operator::None:
                success = !isBuiltin ? execBasic(cmdToCharVec(cmd)) : execBuiltin(cmd);
                break;
            case Operator::Pipe:
                success = execPipe(cmd, isBuiltin);
                break;
            case Operator::Background:
                success = execBg(cmd, isBuiltin);
                break;
            case Operator::Redirection:
                execRedir(cmd, isBuiltin);
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
bool Executor::exec(const std::vector<const char*>& args, pid_t& childPid) const {
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

bool Executor::execBasic(const std::vector<const char*>& args) const {
    pid_t childPid;
    
    if (exec(args, childPid)) {
        // Main process
        int status = 0;
        waitpid(childPid, &status, 0);
        // Get the childs status code
        if (WIFEXITED(status)) {
            exitCode = WEXITSTATUS(status);
        }
        return true;
    } else {
        return false;
    }
}

// Starts a child process in the backgorund.
bool Executor::execBg(Command& cmd, bool isBuiltin) const {
    // At the moment this doesn't release the terminal to the parent
    pid_t childPid;
    if (isBuiltin) {
        childPid = fork();
        if (childPid == 0) {
            return execBuiltin(cmd);
        } else if (childPid == -1) {
            return false;
        }
    }
    return exec(cmdToCharVec(cmd), childPid);
}

// Executes the given command with a specified fd
// Can be used with fd2 and with isPipe closes both
static bool execDup(
    pid_t& childPid,
    const std::vector<const char*>& args,
    int& exitCode,
    int fd,
    int fd2 = STDOUT_FILENO,
    int pipeOthrEnd = -1)
{
    // Can be used both from redirection and pipes
    childPid = fork();
    if (childPid == 0) { // Child process
        dup2(fd, fd2);
        close(fd);
        if (pipeOthrEnd >= 0) close(pipeOthrEnd);

        execvp(args[0], const_cast<char* const*>(args.data()));
        
        perror("execvp");
        _exit(1);
    } else if (childPid == -1) { // Fork failed
        perror("fork");
        return false;
    }
    return true;
}

bool Executor::execPipe(Command& cmd, bool isBuiltin) const {
    return true;
}

// Redirects command output to the file specified
// At the moment only works for output and not input 
void Executor::execRedir(Command& cmd, bool isBuiltin) const {
    if (cmd.outFile.empty()) {
        std::cerr << "No output file specified" << std::endl;
        exitCode = 1;
        return;
    }
    // Note: Could break the following branches into seperate functions
    if (isBuiltin) {
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
        if (!execDup(childPid, cmdToCharVec(cmd), exitCode, outFile)) {
            close(outFile);
            exitCode = 1;
            return;
        }
        close(outFile);

        int status;
        waitpid(childPid, &status, 0);
        // Get the childs status code
        if (WIFEXITED(status)) {
            exitCode = WEXITSTATUS(status);
        } else {
            exitCode = 1;
        }
    }
}

bool Executor::execBuiltin(const Command& cmd) const {
    auto control = Builtins::handleBuiltin(cmd, shellEnv);
    using Builtins::Control;

    // Go in if command was a builtin
    if (control != Control::NONE) {
        if (control == Control::BREAK) {
            // Need to add support for exit codes at some point
            return false;
        }
    }
    return true;
}

// Converts a command struct (exec and args) into a char* vector.
// The passed struct must outlive the function.
const std::vector<const char*> Executor::cmdToCharVec(Command& cmd) const {
    std::vector<const char*> commands;
    commands.push_back(cmd.executable.data());
    for (auto& arg: cmd.args) {
        commands.push_back(arg.data());
    }
    commands.push_back(nullptr);
    return commands;
}

std::vector<char*> Executor::strVecToCharVec(std::vector<std::string>& args) const {
    std::vector<char*> commands;
    for (auto& cmdstr: args) {
        commands.push_back(cmdstr.data());
    }
    commands.push_back(nullptr);
    return commands;
}
