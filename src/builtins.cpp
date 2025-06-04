#include "builtins.h"

#include <iostream>
#include <unistd.h>

#include "history.h"

namespace Builtins {
    
    /* Calls a builtin if command is one and returns the appropriate Control code.
       Return Control::NONE if command is not a builtin.
    */
    Control handleBuiltin(const Command& cmd, const ShellEnv& shellEnv) {
        auto iter = builtin_commands.find(cmd.executable);
        if (iter != builtin_commands.end()) {
            switch (iter->second) {
                case Builtin::exit:
                    return Control::BREAK;

                case Builtin::cd:
                    // If no path provided, go to home dir
                    Builtins::cd(!cmd.args.empty() ? cmd.args[0] : shellEnv.homeDir);
                    return Control::CONTINUE;

                case Builtin::echo:
                    // If no arguments, print an empty line
                    Builtins::echo(!cmd.args.empty() ? cmd.args[0] : "");
                    return Control::CONTINUE;

                case Builtin::history:
                    Builtins::history(shellEnv.history);
                    return Control::CONTINUE;

                case Builtin::hash:
                    Builtins::hash(shellEnv.commandLookup.getLookup());
                    return Control::CONTINUE;
            }
        }
        return Control::NONE;
    }
    
    void cd(const std::string& path) {
        if (chdir(path.c_str()) == -1)
            perror("cd");
    }

    void echo(const std::string& message) {
        std::cout << message << std::endl;
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
