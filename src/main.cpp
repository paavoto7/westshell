// WestShell

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <sys/wait.h>
#include <string.h>

#include "command_lookup.h"
#include "builtins.h"
#include "shell_env.h"
#include "style.h"

inline std::vector<char *> parser(const std::string& command);
inline void freeCommand(const std::vector<char *>& command);

int main() {
    
    // The shell environment also owns the history and commandLookup object
    ShellEnv shellEnv;
    CommandLookup& commandLookup = shellEnv.commandLookup;
    
    std::string command;

    int exitCode = 0;
    
    // Might want to create a seperate function or class running main loop
    while (true) {
        // Get the path and modify it to be printed in the shell
        auto path = std::filesystem::current_path().string();
        if (path.find(shellEnv.homeDir) == 0) {
            path.replace(0, shellEnv.homeDir.length(), "~");
        }
        
        std::cout
            << Style::bright_green << shellEnv.user << Style::reset
            << ":" << Style::bright_blue << path << Style::reset << "$ ";

        getline(std::cin, command);
        
        // Get the space parsed command and arguments
        const std::vector<char *> commands = parser(command);
        
        // Just a quick hack for some builtins now
        auto control = Builtins::handleBuiltin(command, commands, shellEnv);
        using Builtins::Control;

        // Add to history if not empty
        if (control != Control::EMPTY) {
            shellEnv.history.saveEntry(command);
        } else {
            continue;
        }

        // Go in if command was a builtin
        if (control != Control::NONE) {
            // Free the dynamically allocated c-strings
            freeCommand(commands);

            if (control == Control::CONTINUE) {
                continue;
            } else if (control == Control::BREAK) {
                // Need to add support for exit codes at some point
                break;
            }
        }
        
        // Saves from forking and unnecessary execvp
        if (!commandLookup.findExecutable(commands[0], shellEnv.PATH)) {
            // Currently makes no distinction between not found and no permission
            std::cout << commands[0] << ": Command not found or no permission" << std::endl;
            continue;
        }
        
        const pid_t childPid = fork();
        if (childPid == 0) { // Child process
            execvp(commands[0], commands.data());

            // In case of an error
            perror("execvp");
            freeCommand(commands);
            exit(1);
        } else if (childPid == -1) { // Failed to create
            break;
        } else { // Main process
            waitpid(childPid, nullptr, 0);
        }
    }

    return exitCode;
}

/* A simple whitespace parser.
   Will need to rethink this later as it is not that safe.
*/
inline std::vector<char *> parser(const std::string& command) {
    std::vector<char *> commands;

    std::stringstream ss(command);
    
    std::string token;
    while (getline(ss, token, ' ')) {
        commands.push_back(strdup(token.c_str()));
    }

    commands.push_back(nullptr);
    
    return commands;
    
}

inline void freeCommand(const std::vector<char *>& command) {
    for (auto* arg: command) {
        free(arg); // Now also performed on nullptr
    }
}
