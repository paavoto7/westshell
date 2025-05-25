#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <sstream>
#include <string>
#include <string.h>

namespace Parser {
    /* A simple whitespace parser.
       Will need to rethink this later as it is not that safe.
    */
    inline std::vector<char *> parser(const std::string& command) {
        std::vector<char *> commands;

        std::stringstream ss(command);
        
        std::string token;
        while (getline(ss, token, ' ')) {
            commands.push_back(strdup(token.c_str()));
        }
        commands.push_back(nullptr);
        
        return commands;
    }
}

#endif // PARSER_H