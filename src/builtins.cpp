#include "builtins.h"

#include <iostream>
#include <unistd.h>
#include <string.h>

#include "history.h"

namespace Builtins {
    
    /* Calls a builtin if command is one and returns the appropriate Control code.
       Return Control::NONE if command not empty or builtin.
    */
    Control handleBuiltin(
        const std::string& command, const std::vector<char *>& args, const ShellEnv& shellEnv
    ) {
        // The following is a bit inefficient and unmaintainable so rethink is in order
        // Could use an unorder map and functions as values for example
        if (command.empty()) {
            return Control::EMPTY;
        }
        else if (strcmp(args[0], "exit") == 0) {
            return Control::BREAK;
        }
        else if (strcmp(args[0], "cd") == 0) {
            Builtins::cd(args[1], shellEnv.homeDir);
            return Control::CONTINUE;
        }
        else if (strcmp(args[0], "echo") == 0) {
            // If no path provided, go to home dir
            Builtins::echo(args[1]);
            return Control::CONTINUE;
        }
        else if (strcmp(args[0], "history") == 0) {
            // If no path provided, go to home dir
            Builtins::history(shellEnv.history);
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
            std::cout << command << "\n";
        }
        std::cout << std::flush;
    }
}