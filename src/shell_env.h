#ifndef SHELLENV_H
#define SHELLENV_H

#include <unordered_map>
#include <string>
#include <unordered_set>

#include "history.h"
#include "command_lookup.h"

// Encapsulates global shell environment.
class ShellEnv {
public:
    // These are all currently global, but that might/should change
    std::string user;
    std::string homeDir;
    const pid_t mainPid;
    const std::string PATH;
    int exitCode = 0;

    History history;
    CommandLookup commandLookup;

    ShellEnv();

    const std::unordered_map<std::string, std::string>& getSettings() const;
    const std::string& getSetting(const std::string& key) const;

    void addBackgroundJob(pid_t pid);
    void reapBackgroundJobs();
    const std::unordered_set<pid_t>& getBackgroundJobs() const;
    
private:
    std::unordered_map<std::string, std::string> settings;
    std::unordered_set<pid_t> background_jobs;
    
    void parseSettings();
};

#endif // SHELLENV_H
