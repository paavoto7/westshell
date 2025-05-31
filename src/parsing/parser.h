#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "tokenizer.h"

namespace Parser {
    // Contains executable name, arguments and flag for piping
    // Need to add support for other operators
    struct Command {
        std::string executable;
        std::vector<std::string> args;
        bool piped = false;
    };

    // A simple whitespace parser.
    std::vector<std::string> simpleParser(const std::string& command);

    // Parses the given tokens into a vector of commands to be executed
    std::vector<Command> parser(const std::vector<Tokenizer::Token>& tokens);
    
    // Calls tokenize function and passes the return value to overloaded function
    inline std::vector<Command> parser(const std::string& command) {
        return parser(Tokenizer::tokenize(command));    
    };
    
} // namespace Parser

#endif // PARSER_H
