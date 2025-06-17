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

    const std::string primaryColour = "primaryColour";
    const std::string secondaryColour = "secondaryColour";

    void printPrompt() const;
    
};

#endif // SHELL_H
