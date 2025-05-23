#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>

/* Simple wrapper class for history functionality
   TODO: Maximum history size
*/
class History {
public:
    History() = default;
    History(std::string homeDir);
    
    void saveEntry(const std::string& command);
    std::string getEntry(int index = 0);

    const std::vector<std::string>& getAll() const;

private:
    std::vector<std::string> history;
    std::string fileLocation;
    std::string homeDir;
};

#endif // HISTORY_H
