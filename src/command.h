#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>

enum class Operator { Pipe, Background, Redirection, None };

// Contains executable name, arguments and flag for piping
// Need to add support for other operators
struct Command {
    std::string executable;
    std::vector<std::string> args;
    Operator op = Operator::None;
};

#endif // COMMAND_H
