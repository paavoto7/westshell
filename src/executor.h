#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <vector>
#include <string>

#include "command.h"
#include "shell_env.h"
#include "command_lookup.h"

class Executor {
public:
    Executor(ShellEnv& shellEnv, int& exitCode);

    [[nodiscard]] bool executeExternalCommand(std::vector<Command>& commands) const;

private:
    int& exitCode;
    ShellEnv& shellEnv;
    CommandLookup& commandLookup;
    
    bool exec(const std::vector<const char*>& args, pid_t& pid) const;
    bool execBasic(const std::vector<const char*>& args) const;
    bool execPipe(Command& cmd, bool isBuiltin = false) const;
    bool execBg(Command& cmd, bool isBuiltin = false) const;
    void execRedir(Command& cmd, bool isBuiltin = false) const;
    bool execBuiltin(const Command& cmd) const;

    const std::vector<const char*> cmdToCharVec(Command& cmd) const;
    std::vector<char*> strVecToCharVec(std::vector<std::string>& args) const;
};

#endif // EXECUTOR_H
