#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>

enum class Operator { Pipe, Background, Redirection, LogicAnd, LogicOr, None };

// Represents a single command
struct Command {
    std::string executable;
    std::vector<std::string> args;
    Operator op = Operator::None;
    bool isBuiltin;

    std::string inFile;
    std::string outFile;
};

#endif // COMMAND_H
