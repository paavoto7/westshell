#include "tokenizer.h"

std::vector<Token> Tokenizer::tokenize(const std::string& command) {
    // Initialise
    buffer.clear();
    tokens.clear();
    state = TokenType::Word;

    for (int i = 0; i < command.length(); ++i) {
        
        // Implements a simple state machine based on the type of token currently in
        switch (state) {
            case TokenType::Word:
                handleWordToken(command, i);
                break;
            case TokenType::DQuote:
                if (command[i] == '"') {
                    tokens.emplace_back(TokenType::DQuote, buffer);
                    buffer.clear();
                    state = TokenType::Word;
                } else {
                    buffer += command[i];
                }
                break;
            case TokenType::SQuote:
                if (command[i] == '\'') {
                    tokens.emplace_back(TokenType::SQuote, buffer);
                    buffer.clear();
                    state = TokenType::Word;
                } else {
                    buffer += command[i];
                }
                break;
            default:
                break;
        }
    }
    // Add the remaining buffer
    if (!buffer.empty()) {
        tokens.emplace_back(TokenType::Word, buffer);
    }
    return tokens;
}

bool Tokenizer::isOperator(char current) const {
    return operators.count(current);
}

void Tokenizer::handleWordToken(const std::string& command, int& index) {
    auto current = command[index];
    if (current == '"') {
        state = TokenType::DQuote;
    } else if (current == '\'') {
        state = TokenType::SQuote;
    } else if (isspace(current)) {
        if (!buffer.empty()) {
            tokens.emplace_back(TokenType::Word, buffer);
            buffer.clear();
        }
    } else if (isOperator(current)) {
        // Need to add support for multi-character operators
        if (!buffer.empty()) { // Bash allows operators without spaces
            tokens.emplace_back(TokenType::Word, buffer);
            buffer.clear();
        }
        tokens.emplace_back(TokenType::Operator, std::string() + current);
    } else {
        buffer += current;
    }
}
