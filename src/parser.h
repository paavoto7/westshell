#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <sstream>
#include <string>

namespace Parser {
    // A simple whitespace parser.
    inline std::vector<std::string> parser(const std::string& command) {
        std::vector<std::string> commands;

        std::stringstream ss(command);
        
        std::string token;
        while (getline(ss, token, ' ')) {
            commands.push_back(token);
        }
        
        return commands;
    }
}

#endif // PARSER_H
