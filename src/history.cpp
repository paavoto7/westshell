#include "history.h"

#include <fstream>
#include <iostream>
#include <filesystem>

// Initializes the history object by loading entries from the history file
History::History(std::string homeDir): homeDir(homeDir) {
    const std::string filename = ".westshell_history";
    fileLocation = homeDir + "/" + filename;
    
    std::ifstream historyFile;
    historyFile.exceptions(std::ifstream::badbit);
    
    // Create the history file if missing
    if (!std::filesystem::exists(fileLocation)) {
        saveEntry("");
    }
    
    std::string line;
    try {
        historyFile.open(fileLocation);
        // Read the history file commands into a vector
        while (getline(historyFile, line)) {
            history.push_back(line);
        }

    } catch (const std::ifstream::failure& e) {
        std::cerr << "File open/read failed: " << e.what() << std::endl;
    }
}

// Saves a new history entry to both memory and the history file
void History::saveEntry(const std::string& command) {
    if (command.empty()) return;

    history.push_back(command);
    
    std::ofstream historyFile;
    historyFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    try {
        historyFile.open(fileLocation, std::ios::app); // Append mode
        historyFile << command << std::endl;
    } catch (const std::ifstream::failure& e) {
        std::cerr << "File open/write failed: " << e.what() << std::endl;
    }
}

// Returns the history entry at the given index, clamped to valid bounds
std::string History::getEntry(int index) {
    if (index < 0) {
        return history[0];
    } else if (index >= history.size()) {
        return history[history.size() - 1];
    } else {
        return history[index];
    }
}

// Only a view into the history vector is required
const std::vector<std::string>& History::getAll() const {
    return history;
}
