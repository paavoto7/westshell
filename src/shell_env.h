#ifndef SHELLENV_H
#define SHELLENV_H

#include <string>

#include "history.h"
#include "command_lookup.h"

// Encapsulates global shell environment.
class ShellEnv {
public:
    // These are all currently global, but that might/should change
    std::string user;
    std::string homeDir;
    pid_t mainPid;
    std::string PATH;

    History history;
    CommandLookup commandLookup;

    ShellEnv();
};


#endif // SHELLENV_H