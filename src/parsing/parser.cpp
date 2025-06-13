#include "parser.h"

#include <sstream>

namespace Parser {
    // A simple whitespace parser.
    std::vector<std::string> simpleParser(const std::string& command) {
        std::vector<std::string> commands;

        std::stringstream ss(command);
        
        std::string token;
        while (getline(ss, token, ' ')) {
            commands.push_back(token);
        }
        
        return commands;
    }

    std::vector<Command> parse(const std::vector<Tokenizer::Token>& tokens) {
        std::vector<Command> commands;
        commands.emplace_back(); // Create first command
        Command* current = &commands.back();
        
        using TokenType = Tokenizer::TokenType;
        const size_t len = tokens.size();
        for (size_t i = 0; i < len; ++i) {
            const auto& token = tokens[i];
            switch (token.type) {
                case TokenType::Word:
                    if (current->executable.empty()) {
                        current->executable = token.text;
                    } else {
                        current->args.push_back(token.text);
                    }
                    break;
                case TokenType::SQuote: case TokenType::DQuote:
                    current->args.push_back(token.text);
                    break;
                case TokenType::Operator:
                    current->op = operators.at(token.text);
                    if (current->op == Operator::Redirection) {
                        if (i + 1 < len) {
                            current->outFile = tokens[i + 1].text;
                            ++i; // Jump over the next token as it is only a filename
                        }
                        if (i + 1 == len) break;
                    }
                    // In case of an operator, add another command
                    commands.emplace_back();
                    current = &commands.back();
                    break;
                default:
                    break;
            }
        }
        return commands;
    }
    
} // namespace Parser

