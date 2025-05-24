#ifndef SHELLENV_H
#define SHELLENV_H

#include <unordered_map>
#include <string>
#include <string_view>

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

    const std::unordered_map<std::string, std::string>& getSettings() const;
    std::string_view getSetting(const std::string& key) const;
    
private:
    std::unordered_map<std::string, std::string> settings;
    
    void parseSettings();
};

#endif // SHELLENV_H
