#include "shell.h"

#include <iostream>
#include <filesystem>

#include "parsing/parser.h"
#include "style.h"
#include "signals.h"

Shell::Shell() : shellEnv(), executor(shellEnv, shellEnv.exitCode) {}

// Runs the shell program
int Shell::run() {

    std::string command;
    std::string promptPath;

    while (true) {
        getPromptPath(promptPath);
        
        printPrompt(promptPath);

        if (!getline(std::cin, command)) {
            // Input error or EOF
            std::cout << '\n';
            break;
        }
        
        // The signals don't work well with getline
        // Might need to use something else instead of getline
        if (Signals::signalStatus != 0) {
            break;
        } else if (Signals::childSignalStatus != 0) {
            Signals::childSignalStatus = 0;
            shellEnv.reapBackgroundJobs();
        }
        
        // Ignore if empty or only whitespace
        if (command.empty() || command.find_first_not_of(' ') == std::string::npos) {
            continue;
        }

        // Add to history
        shellEnv.history.saveEntry(command);
        
        // In the case of an error or exit, break out of the loop
        if (!executor.executeCommands(Parser::parse(command)))
            break;
    }
    return shellEnv.exitCode;
}

void Shell::getPromptPath(std::string& promptPath) {
    // Get the path and modify it to be printed in the shell
    promptPath = std::filesystem::current_path().string();
    if (promptPath.find(shellEnv.homeDir) == 0) {
        promptPath.replace(0, shellEnv.homeDir.length(), "~");
    }
}

// Prints the hard-coded default prompt
void Shell::printPrompt(const std::string& path) const {
    std::cout
        << Style::getColorCode(shellEnv.getSetting(primaryColour)) << shellEnv.user << Style::reset
        << ':' << Style::getColorCode(shellEnv.getSetting(secondaryColour)) << path << Style::reset
        << "$ ";
}
