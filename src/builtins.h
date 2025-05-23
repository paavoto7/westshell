#ifndef BUILTINS_H
#define BUILTINS_H

#include <string>
#include <vector>

#include "shell_env.h"

// Simple namespace for builtins.
namespace Builtins {
    enum class Control {
        CONTINUE,
        BREAK,
        NONE
    };

    Control handleBuiltin(
        const std::string& command, const std::vector<char *>& args, const ShellEnv& shellEnv
    );
    void cd(const char* path, const std::string& homeDir);
    void echo(const char* message);
}

#endif // BUILTINS_H