#include "tokenizer.h"

namespace Tokenizer {

std::vector<Token> tokenize(const std::string& command) {
    std::vector<Token> tokens;

    // Consider using a distinct enum as state
    TokenType state = TokenType::Word;
    std::string buffer;
    for (auto current: command) {

        // Implements a simple state machine based on the type of token currently in
        switch (state) {
            case TokenType::Word:
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
                break;
            case TokenType::DQuote:
                if (current == '"') {
                    tokens.emplace_back(TokenType::DQuote, buffer);
                    buffer.clear();
                    state = TokenType::Word;
                } else {
                    buffer += current;
                }
                break;
            case TokenType::SQuote:
                if (current == '\'') {
                    tokens.emplace_back(TokenType::SQuote, buffer);
                    buffer.clear();
                    state = TokenType::Word;
                } else {
                    buffer += current;
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

} // namespace Tokenizer
