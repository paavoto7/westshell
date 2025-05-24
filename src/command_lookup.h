#ifndef COMMANDLOOKUP_H
#define COMMANDLOOKUP_H

#include <string>
#include <unordered_map>

// Encapsulated the command lookup and caching into one
class CommandLookup {
public:
    bool findExecutable(const char* command, const std::string& PATH);
    void clearCache();

    const std::unordered_map<std::string, bool>& getLookup() const;
    
private:
    // Could also hold the amount of times called
    std::unordered_map<std::string, bool> lookupCache;
};

#endif // COMMANDLOOKUP_H
