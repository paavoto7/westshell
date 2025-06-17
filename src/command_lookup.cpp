#include "command_lookup.h"

#include <sstream>
#include <unistd.h>

// Finds whether an executable is in the path or not.
// Should handle absolute paths and cases where PATH is null.
bool CommandLookup::findExecutable(const char* command, const std::string& PATH) {
    const std::string cmdStr(command);
    
    // Check if present in cache
    auto item = lookupCache.find(cmdStr);
    if (item != lookupCache.end()) {
        return item->second;
    }

    // Check for absolute paths
    if (cmdStr.find('/') != std::string::npos) {
        bool exists = access(command, X_OK) == 0;
        lookupCache[cmdStr] = exists;
        return exists;
    }
    
    std::stringstream ss(PATH);
    std::string directory;
    
    while (std::getline(ss, directory, ':')) {
        if (access((directory + "/" + command).c_str(), X_OK) == 0) {
            lookupCache[cmdStr] = true;
            return true;
        }
    }
    lookupCache[cmdStr] = false;
    return false;
}

// Get a view into the cached commands
const std::unordered_map<std::string, bool>& CommandLookup::getLookup() const {
    return lookupCache;
}

// Clear the cache
void CommandLookup::clearCache() {
    lookupCache.clear();
}
