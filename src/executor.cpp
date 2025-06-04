#include "executor.h"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include "builtins.h"

Executor::Executor(ShellEnv& shellEnv, int& exitCode)
    : shellEnv(shellEnv), commandLookup(shellEnv.commandLookup), exitCode(exitCode) {};

[[nodiscard]] bool Executor::executeExternalCommand(
    std::vector<Command>& commands) const 
{
    bool success = false;
    for (Command& cmd: commands) {
        bool isBuiltin = Builtins::isBuiltin(cmd);
         // Saves from forking and unnecessary execvp
        if (!isBuiltin && !commandLookup.findExecutable(cmd.executable.c_str(), shellEnv.PATH)) {
            // Currently makes no distinction between not found and no permission
            std::cout << cmd.executable << ": Command not found or no permission" << std::endl;
            continue;
        }
        switch (cmd.op) {
            case Operator::None:
                success = !isBuiltin ? exec(cmdToCharVec(cmd)) : execBuiltin(cmd);
                break;
            case Operator::Pipe:
                success = execPipe(cmdToCharVec(cmd));
                break;
            case Operator::Background:
                success = execBg(cmdToCharVec(cmd));
                break;
            case Operator::Redirection:
                success = execRedir(cmdToCharVec(cmd));
                break;
            default:
                success = false;
                break;
        }
        if (!success)
            break;
    }
    return success;
}

// Forks and then executes the provided command.
// Returns true if successful, false othwerise.
bool Executor::exec(const std::vector<char*>& args) const {
    const pid_t childPid = fork();
    if (childPid == 0) { // Child process
        execvp(args[0], args.data());
        // In case of an error
        perror("execvp");
        exit(1);
    } else if (childPid == -1) { // Failed to create
        return false;
    } else { // Main process
        int status = 0;
        waitpid(childPid, &status, 0);
        // Get the childs status code
        if (WIFEXITED(status)) {
            exitCode = WEXITSTATUS(status);
        }
    }
    return true;
}

bool Executor::execPipe(const std::vector<char*>& args) const {
    return true;
}

bool Executor::execBg(const std::vector<char*>& args) const {
    return true;
}

bool Executor::execRedir(const std::vector<char*>& args) const {
    return true;
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

std::vector<char*> Executor::cmdToCharVec(Command& cmd) const {
    std::vector<char*> commands;
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

