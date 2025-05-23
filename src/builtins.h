#ifndef BUILTINS_H
#define BUILTINS_H

#include <string>
#include <vector>

#include "shell_env.h"

// Simple namespace for builtins.
namespace Builtins {
    // Meaningful return values for handleBuiltin
    enum class Control {
        CONTINUE,
        BREAK,
        NONE,
        EMPTY
    };

    Control handleBuiltin(
        const std::string& command, const std::vector<char *>& args, const ShellEnv& shellEnv
    );
    
    void cd(const char* path, const std::string& homeDir);
    void echo(const char* message);
    void history(const History& history);
}

#endif // BUILTINS_H