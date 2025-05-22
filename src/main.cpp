// WestShell

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

inline vector<char *> parser(const string& command);
inline void freeCommand(const vector<char *>& command);

int main() {
    // Get required information about the user
    const struct passwd* pw = getpwuid(geteuid());
    const string user = pw->pw_name;
    const string homeDir = pw->pw_dir;

    const pid_t mainPid = getpid();
    
    string command;
    
    while (true) {
        // Get the path and modify it to be printed in the shell
        auto path = filesystem::current_path().string();
        if (path.find(homeDir) == 0) {
            path.replace(0, homeDir.length(), "~");
        }
        
        cout << user << ":" << path << "$ ";

        getline(std::cin, command);
        
        // Get the space parsed command and arguments
        const vector<char *> commands = parser(command);
        
        // Just a quick hack for some builtins now
        if (command.empty()) {
            continue;
        } else if (strcmp(commands[0], "exit") == 0) {
            break;
        } else if (strcmp(commands[0], "cd") == 0) {
            // If no path provided, go to home dir
            chdir(commands[1] ? commands[1] : homeDir.c_str());

            // Free the dynamically allocated c-strings
            freeCommand(commands);
            continue;
        }
        
        const pid_t childPid = fork();
        if (childPid == 0) { // Child process
            execvp(commands[0], commands.data());

            // In case of an error
            perror("execvp");
            freeCommand(commands);
            exit(1);
        } else if (childPid == -1) { // Failed to create
            break;
        } else { // Main process
            waitpid(childPid, nullptr, 0);
        }
    }

    return 0;
}

/* A simple whitespace parser.
   Will need to rethink this later as it is not that safe.
*/
inline vector<char *> parser(const string& command) {
    vector<char *> commands;

    stringstream ss(command);
    
    string token;
    while (getline(ss, token, ' ')) {
        commands.push_back(strdup(token.c_str()));
    }

    commands.push_back(nullptr);
    
    return commands;
    
}

inline void freeCommand(const vector<char *>& command) {
    for (auto* arg: command) {
        free(arg); // Now also performed on nullptr
    }
}
