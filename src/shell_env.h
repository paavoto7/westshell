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
    // Note: Some responsibilities of this class should probably
    // be split into distinct, more focused classes as the project evolves.
    // ShellEnv should still own them, but the responsibilities
    // would then be more maintanable and clearer.

    // These are all currently global, but that might/should change
    std::string user;
    std::string homeDir;
    const std::string PATH;
    std::string currentPath;
    int exitCode = 0;
    bool shouldExit = false;

    History history;
    CommandLookup commandLookup;

    ShellEnv();

    const std::unordered_map<std::string, std::string>& getSettings() const;
    const std::string& getSetting(const std::string& key) const;

    void addBackgroundJob(pid_t pid);
    void reapBackgroundJobs();
    const std::unordered_set<pid_t>& getBackgroundJobs() const;

    void setCurrentPath();
    
private:
    const pid_t mainPid;

    std::unordered_map<std::string, std::string> settings;
    std::unordered_set<pid_t> background_jobs;
    
    void parseSettings();
};

#endif // SHELLENV_H
