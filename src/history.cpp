#include "history.h"

#include <fstream>
#include <iostream>
#include <filesystem>

// Initializes the history object by loading entries from the history file
History::History(const std::string& homeDir): homeDir(homeDir) {
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

// On program end, write to the history file
History::~History() {
    std::ofstream historyFile;
    historyFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    // The following always writes all again, even when history lower than max
    try {
        historyFile.open(fileLocation);
        int lines = history.size();
        int index = lines > maxLines ? lines - maxLines : 0;
        // Write only maxLines amount ignoring the old commands
        for (int i = index; i < lines; ++i) {
            historyFile << history[i] << std::endl;
        }
    } catch (const std::ofstream::failure& e) {
        std::cerr << "File open/write failed: " << e.what() << std::endl;
    }
}

// Saves a new history entry to memory
void History::saveEntry(const std::string& command) {
    if (command.empty()) return;

    history.push_back(command);
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
