#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <unordered_map>

#include "tokenizer.h"
#include "token.h"
#include "command.h"

namespace Parser {

    // Set of recognized operator characters
    inline const std::unordered_map<std::string, Operator> operators {
        {"|", Operator::Pipe},
        {">", Operator::Redirection},
        {"&", Operator::Background}
    };

    // A simple whitespace parser.
    std::vector<std::string> simpleParser(const std::string& command);

    // Parses the given tokens into a vector of commands to be executed
    std::vector<Command> parse(const std::vector<Token>& tokens);
    
    // Calls tokenize function and passes the return value to overloaded function
    inline std::vector<Command> parse(const std::string& command) {
        // Note: Could be handled in other ways, but this integrates seamlessly
        static Tokenizer tokenizer; // Not safe if multithreaded
        return parse(tokenizer.tokenize(command)); 
    };
    
} // namespace Parser

#endif // PARSER_H
