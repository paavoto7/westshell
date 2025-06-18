#include "builtins.h"

#include <iostream>
#include <unistd.h>

#include "history.h"

namespace {
    bool isNumber(const std::string& arg) {
        if (arg.empty()) return false;
        for (auto c: arg) {
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;
    }
}

namespace Builtins {
    
    // Calls a builtin if command is one and returns the appropriate Control code.
    // Return Control::NONE if command is not a builtin.
    Control handleBuiltin(const Command& cmd, ShellEnv& shellEnv) {
        auto iter = builtin_commands.find(cmd.executable);
        if (iter == builtin_commands.end()) {
            std::cerr << "Command not found, use isBuiltin first" << std::endl;
            return Control::CONTINUE;
        }
        switch (iter->second) {
            case Builtin::exit:
                Builtins::exit(cmd, shellEnv);
                return Control::BREAK;

            case Builtin::cd:
                // If no path provided, go to home dir
                cd(!cmd.args.empty() ? cmd.args[0] : shellEnv.homeDir);
                shellEnv.setCurrentPath(); // Change current path
                return Control::CONTINUE;

            case Builtin::echo:
                // If no arguments, print an empty line
                echo(cmd.args);
                return Control::CONTINUE;

            case Builtin::history:
                history(shellEnv.history);
                return Control::CONTINUE;

            case Builtin::hash:
                hash(shellEnv.commandLookup.getLookup());
                return Control::CONTINUE;

            case Builtin::jobs:
                jobs(shellEnv);
                return Control::CONTINUE;
            
            default:
                // Something unexpected
                return Control::BREAK;
        }
    }
    
    void cd(const std::string& path) {
        if (chdir(path.c_str()) == -1)
            perror("cd");
    }

    void echo(const std::string& message) {
        std::cout << message << std::endl;
    }

    void echo(const std::vector<std::string>& messages) {
        for (const auto& msg: messages) {
            std::cout << msg << ' ';
        }
        std::cout << std::endl;
    }

    void history(const History& history) {
        // Print all commands of the history vector
        for (const auto& command: history.getAll()) {
            std::cout << command << '\n';
        }
        std::cout << std::flush;
    }

    // Print the cached commands
    void hash(const std::unordered_map<std::string, bool>& lookupCache) {
        std::cout << "command:\n";
        for (const auto& [command, found]: lookupCache) {
            if (found) {
                std::cout << command << '\n';
            }
        }
        std::cout << std::flush;
    }

    // Sets only the exit code for now
    void exit(const Command& cmd, ShellEnv& shellEnv) {
        shellEnv.shouldExit = true;
        if (!cmd.args.empty() && isNumber(cmd.args[0])) {
            shellEnv.exitCode = std::stoi(cmd.args[0]);
        }
    }
    
    void jobs(const ShellEnv& shellEnv) {
        // At the moment the background jobs only save the pid
        for (auto job: shellEnv.getBackgroundJobs()) {
            std::cout << job << '\n';
        }
        std::cout << std::flush;
    }
}
