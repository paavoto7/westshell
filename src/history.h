#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>

/* Simple wrapper class for history functionality
   Loads history file on startup, and writes it
   again after exiting.
*/
class History {
public:
    History() = default;
    History(const std::string& homeDir);

    ~History();
    
    void saveEntry(const std::string& command);
    std::string getEntry(int index = 0);

    const std::vector<std::string>& getAll() const;

private:
    std::vector<std::string> history;
    std::string filename = ".westshell_history";
    std::string fileLocation;
    std::string homeDir;
    // Need to implement a settings file
    int maxLines = 100;
};

#endif // HISTORY_H
