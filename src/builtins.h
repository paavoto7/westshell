#ifndef BUILTINS_H
#define BUILTINS_H

#include <string>
#include <unordered_map>

#include "shell_env.h"
#include "command.h"

// Simple namespace for builtins.
namespace Builtins {
    // Meaningful return values for handleBuiltin
    enum class Control {
        CONTINUE,
        BREAK,
        NONE,
        EMPTY
    };

    enum class Builtin { cd, echo, history, hash, exit };

    // Could also have functions as values here
    inline const std::unordered_map<std::string, Builtin> builtin_commands {
        {"cd", Builtin::cd},
        {"echo", Builtin::echo},
        {"history", Builtin::history},
        {"hash", Builtin::hash}, 
        {"exit", Builtin::exit}
    };

    Control handleBuiltin(const Command& cmd, const ShellEnv& shellEnv);

    inline bool isBuiltin(const Command& cmd) {
        return builtin_commands.count(cmd.executable);
    }
    
    void cd(const std::string& path);
    void echo(const std::string& message);
    void history(const History& history);
    void hash(const std::unordered_map<std::string, bool>& lookupCache);
}

#endif // BUILTINS_H
