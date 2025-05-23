#ifndef SHELLENV_H
#define SHELLENV_H

#include <string>

#include "history.h"

// Encapsulates global shell environment.
class ShellEnv {
public:
    std::string user;
    std::string homeDir;
    pid_t mainPid;
    History history;

    ShellEnv();
};


#endif // SHELLENV_H