#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>
#include <unordered_set>

#include "token.h"

class Tokenizer {
public:
    Tokenizer() = default;

    // Tokenizes a command string into a list of tokens
    std::vector<Token> tokenize(const std::string& command);

private:
    std::vector<Token> tokens;
    // Consider using a distinct enum as state
    TokenType state = TokenType::Word;
    std::string buffer;

    void handleWordToken(const std::string& command, int& index);

    // Set of recognized operator characters.
    inline static const std::unordered_set<char> operators{'|', '>', '&'};
    
    // Checks if a character is an operator defined in the operators set
    bool isOperator(char current) const;
    bool isDoubleOperator(char current, char next) const;
};

#endif // TOKENIZER_H
