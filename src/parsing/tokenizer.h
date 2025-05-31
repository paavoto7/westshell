#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>
#include <unordered_set>

namespace Tokenizer {
    // Represents the type of a token parsed from a command string
    enum class TokenType {
        Word, // Executable or argument
        DQuote, // Double quote
        SQuote, // Single quote
        Operator // |, & etc.
    };

    // A single token with a type and text content
    struct Token {
        const TokenType type;
        const std::string text;

        explicit Token(const TokenType& type, const std::string& text)
            : type(type), text(text) {}
    };
    
    // Tokenizes a command string into a list of tokens
    std::vector<Token> tokenize(const std::string& command);

    // Set of recognized operator characters
    inline const std::unordered_set<char> operators{'|', '>', '&'};
    
    // Checks if a character is an operator defined in the operators set
    inline bool isOperator(char current) {
        return operators.count(current);
    }

} // namespace Tokenizer

#endif // TOKENIZER_H
