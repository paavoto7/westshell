#include "builtins.h"

#include <iostream>
#include <cstring>
#include <unistd.h>

#include "history.h"

namespace Builtins {
    
    /* Calls a builtin if command is one and returns the appropriate Control code.
       Return Control::NONE if command is not a builtin.
    */
    Control handleBuiltin(const std::vector<char*>& args, const ShellEnv& shellEnv) {
        // The following is a bit inefficient and unmaintainable so rethink is in order
        // Could use an unordered map and functions as values for example
        if (std::strcmp(args[0], "exit") == 0) {
            return Control::BREAK;
        }
        else if (std::strcmp(args[0], "cd") == 0) {
            Builtins::cd(args[1], shellEnv.homeDir);
            return Control::CONTINUE;
        }
        else if (std::strcmp(args[0], "echo") == 0) {
            // If no path provided, go to home dir
            Builtins::echo(args[1]);
            return Control::CONTINUE;
        }
        else if (std::strcmp(args[0], "history") == 0) {
            // If no path provided, go to home dir
            Builtins::history(shellEnv.history);
            return Control::CONTINUE;
        }
        else if (std::strcmp(args[0], "hash") == 0) {
            // If no path provided, go to home dir
            Builtins::hash(shellEnv.commandLookup.getLookup());
            return Control::CONTINUE;
        }
        else {
            return Control::NONE;
        }
    }
    
    void cd(const char* path, const std::string& homeDir) {
        // If no path provided, go to home dir
        if (chdir(path ? path : homeDir.c_str()) == -1)
            perror("cd");
    }

    void echo(const char* message) {
        std::cout << (message ? message : "") << std::endl;
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
        for (auto& [command, found]: lookupCache) {
            if (found) {
                std::cout << command << '\n';
            }
        }
        std::cout << std::flush;
    }
}
