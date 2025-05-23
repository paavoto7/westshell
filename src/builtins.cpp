#include "builtins.h"

#include <iostream>
#include <unistd.h>
#include <string.h>

namespace Builtins {
    
    Control handleBuiltin(
        const std::string& command, const std::vector<char *>& args, const ShellEnv& shellEnv
    ) {
        if (command.empty()) {
            return Control::CONTINUE;
        } else if (strcmp(args[0], "exit") == 0) {
            return Control::BREAK;
        } else if (strcmp(args[0], "cd") == 0) {
            // If no path provided, go to home dir
            Builtins::cd(args[1], shellEnv.homeDir);
            return Control::CONTINUE;
        } else if (strcmp(args[0], "echo") == 0) {
            // If no path provided, go to home dir
            Builtins::echo(args[1]);
            return Control::CONTINUE;
        } else {
            return Control::NONE;
        }
    }
    
    void cd(const char* path, const std::string& homeDir) {
        chdir(path ? path : homeDir.c_str());
    }

    void echo(const char* message) {
        std::cout << message << std::endl;
    }
}