#ifndef TOKEN_H
#define TOKEN_H

#include <string>

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

#endif // TOKEN_H
