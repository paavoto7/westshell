#ifndef SHELL_H
#define SHELL_H

#include <string>

#include "shell_env.h"
#include "executor.h"

class Shell {
public:
    Shell();
    int run();

private:
    // The shell environment also owns the history and commandLookup object
    ShellEnv shellEnv;
    Executor executor;

    int exitCode = 0;

    const std::string primaryColour = "primaryColour";
    const std::string secondaryColour = "secondaryColour";

    void getPromptPath(std::string& promptPath);
    void printPrompt(const std::string& path) const;
    
};

#endif // SHELL_H
