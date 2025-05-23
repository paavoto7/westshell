#ifndef SHELLENV_H
#define SHELLENV_H

#include <string>

/* Encapsulates global shell environment.
*/
class ShellEnv {
public:
    std::string user;
    std::string homeDir;
    pid_t mainPid;

    ShellEnv();
};


#endif // SHELLENV_H