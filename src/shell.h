#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>

#include "command_lookup.h"
#include "shell_env.h"

class Shell {
public:
    Shell();
    int run();

private:
    // The shell environment also owns the history and commandLookup object
    ShellEnv shellEnv;
    CommandLookup& commandLookup;

    const std::string primaryColour = "primaryColour";
    const std::string secondaryColour = "secondaryColour";

    void getPromptPath(std::string& promptPath);
    void printPrompt(const std::string& path) const;
    bool executeExternalCommand(const std::vector<char*>& args, int& exitCode) const;
    
    // This is here for now before/if I make a command class or similar
    void freeCommand(const std::vector<char*>& command) const;
};

#endif // SHELL_H