#include "shell.h"

#include <iostream>
#include <filesystem>
#include <sys/wait.h>

#include "parsing/parser.h"
#include "builtins.h"
#include "style.h"
#include "signals.h"

Shell::Shell() : shellEnv(), commandLookup(shellEnv.commandLookup) {}

// Runs the shell program
int Shell::run() {

    int exitCode = 0;
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
            Signals::signalStatus = 0;
            break;
        }
        
        // Ignore if empty or only whitespace
        if (command.empty() || command.find_first_not_of(' ') == std::string::npos) {
            continue;
        }
        
        // Get the space parsed command and arguments
        // Using the simple parser before supporting the new parser
        std::vector<std::string> parsedCommand = Parser::simpleParser(command);

        // Moved this here so we avoid explicit dynamic memory allocation
        std::vector<char*> commands;
        for (auto& cmdstr: parsedCommand) {
            commands.push_back(cmdstr.data());
        }
        commands.push_back(nullptr);
        
        // Check if command is a builtin and if so, execute it
        auto control = Builtins::handleBuiltin(commands, shellEnv);
        using Builtins::Control;

        // Add to history
        shellEnv.history.saveEntry(command);

        // Go in if command was a builtin
        if (control != Control::NONE) {
            if (control == Control::BREAK) {
                // Need to add support for exit codes at some point
                break;
            }
            continue; // Continue and Empty controls
        }
        
        // Saves from forking and unnecessary execvp
        if (!commandLookup.findExecutable(commands[0], shellEnv.PATH)) {
            // Currently makes no distinction between not found and no permission
            std::cout << commands[0] << ": Command not found or no permission" << std::endl;
            continue;
        }
        
        if (!executeExternalCommand(commands, exitCode)) {
            break;
        }
    }
    return exitCode;
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

// Forks and then executes the provided command.
// Returns true if successful, false othwerise.
bool Shell::executeExternalCommand(const std::vector<char*>& args, int& exitCode) const {
    const pid_t childPid = fork();
    if (childPid == 0) { // Child process
        execvp(args[0], args.data());
        // In case of an error
        perror("execvp");
        exit(1);
    } else if (childPid == -1) { // Failed to create
        return false;
    } else { // Main process
        int status = 0;
        waitpid(childPid, &status, 0);
        // Get the childs status code
        if (WIFEXITED(status)) {
            exitCode = WEXITSTATUS(status);
        }
    }
    return true;
}
