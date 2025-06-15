#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <vector>
#include <array>

#include "command.h"
#include "shell_env.h"
#include "command_lookup.h"

// Is responsible for the execution of the parsed commands
class Executor {
public:
    Executor(ShellEnv& shellEnv, int& exitCode);

    [[nodiscard]] bool executeExternalCommand(const std::vector<Command>& commands) const;

private:
    int& exitCode;
    ShellEnv& shellEnv;
    CommandLookup& commandLookup;
    
    bool exec(const std::vector<const char*>& args, pid_t& pid) const;
    bool execDup(
        pid_t& childPid,
        const Command& cmd,
        bool isBuiltin,
        const std::vector<int>& fds,
        int fdOut = -1,
        int fdIn = -1)
    const;
    
    // These could be non-member functions, but for clarity they are here
    bool execBasic(const std::vector<const char*>& args) const;
    bool execPipe(const std::vector<Command>& commands) const;
    bool execBg(const Command& cmd, bool isBuiltin = false) const;
    void execRedir(const Command& cmd, bool isBuiltin = false) const;
    bool execBuiltin(const Command& cmd) const;

    const std::vector<const char*> cmdToCharVec(const Command& cmd) const;
};

#endif // EXECUTOR_H
