#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <vector>

#include "command.h"
#include "shell_env.h"
#include "command_lookup.h"

// Is responsible for the execution of the parsed commands
class Executor {
public:
    Executor(ShellEnv& shellEnv, int& exitCode);

    [[nodiscard]] bool executeCommands(const std::vector<Command>& commands);

private:
    int& exitCode;
    ShellEnv& shellEnv;
    CommandLookup& commandLookup;
    
    // Note: All executor functions could technically be `const`,
    // because they do not modify Executor’s member variables themselves.
    // However, they produce significant side effects (running processes,
    // modifying file descriptors, changing the shell exit code)
    // and are therefore intentionally marked non-const.
    bool exec(const std::vector<const char*>& args, pid_t& pid);
    bool execDup(
        pid_t& childPid,
        const Command& cmd,
        const std::vector<int>& fds,
        int fdOut = -1,
        int fdIn = -1
    );
    
    // These could be non-member functions, but for clarity they are here
    bool execBasic(const std::vector<const char*>& args);
    bool execPipe(const std::vector<Command>& commands);
    bool execBg(const Command& cmd);
    void execRedir(const Command& cmd);
    bool execBuiltin(const Command& cmd);
    bool execLogical(const Command& cmd);
    
    void waitChildPid(pid_t childPid);

    const std::vector<const char*> cmdToCharVec(const Command& cmd) const;
};

#endif // EXECUTOR_H
