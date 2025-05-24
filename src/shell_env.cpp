#include "shell_env.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <pwd.h>

ShellEnv::ShellEnv() {
    // Get required information about the user
    struct passwd* pw = getpwuid(geteuid());
    user = pw->pw_name;
    homeDir = pw->pw_dir;
    parseSettings();

    PATH = getenv("PATH");
    mainPid = getpid();

    // Needed to be done here as we need the home dir
    history = History(homeDir);
}

// Settings should stay unmodified at least in this version
const std::unordered_map<std::string, std::string>& ShellEnv::getSettings() const {
    return settings;
}

// Get a setting loaded from the ini file
std::string_view ShellEnv::getSetting(const std::string& key) const {
    // Might do the function with std::optional<std::string_view> later
    static const std::string_view empty("");
    auto item = settings.find(key);
    // Not the safest solution at the moment, but will do for now
    return item != settings.end() ? item->second : empty;
}


// Reads the specified initialisation file into memory
void ShellEnv::parseSettings() {
    std::string fileLocation = homeDir + "/" + ".westshell.ini";
    
    std::ifstream initFile;
    initFile.exceptions(std::ifstream::badbit);
    
    std::string line;
    try {
        initFile.open(fileLocation);
        // Read the history file commands into a vector
        while (getline(initFile, line)) {
            // The format in the ini file is key=value so split at =
            int ind = line.find('=');
            settings[line.substr(0, ind)] = line.substr(ind + 1, line.length() - ind);
        }

    } catch (const std::ifstream::failure& e) {
        std::cerr << "File open/read failed: " << e.what() << std::endl;
    }
}
