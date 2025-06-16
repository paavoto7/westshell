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
    };

    enum class Builtin { cd, echo, history, hash, exit, jobs };

    // Could also have functions as values here
    inline const std::unordered_map<std::string, Builtin> builtin_commands {
        {"cd", Builtin::cd},
        {"echo", Builtin::echo},
        {"history", Builtin::history},
        {"hash", Builtin::hash}, 
        {"exit", Builtin::exit},
        {"jobs", Builtin::jobs}
    };

    Control handleBuiltin(const Command& cmd, ShellEnv& shellEnv);

    inline bool isBuiltin(const Command& cmd) {
        return builtin_commands.count(cmd.executable);
    }

    inline bool isBuiltin(const std::string& executable) {
        return builtin_commands.count(executable);
    }
    
    void cd(const std::string& path);
    void echo(const std::string& message);
    void echo(const std::vector<std::string>& messages);
    void history(const History& history);
    void hash(const std::unordered_map<std::string, bool>& lookupCache);
    void exit(const Command& cmd, ShellEnv& shellEnv);
    void jobs(const ShellEnv& shellEnv);
}

#endif // BUILTINS_H
