#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdio>

enum class TokenType {
    // DATATYPES
    INT, FLOAT, STRING, BOOL,

    //LITERALS
    NUMBER_INT, 
    NUMBER_FLOAT,
    STRING_LIT,
    BOOL_LIT,

    //IDENTIFIERS
    IDENT,

    // MATH OPS
    PLUS, MINUS, STAR, SLASH,

    //SYMBOLS
    EQUALS,
    LPAREN,
    RPAREN,
    COMMA,
    SEMICOLON,

    //BUILT INS
    SAY,

    EOF_
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
    const char* src;
    size_t pos;
    size_t len;

    public:
    Lexer(const char* source) : src(source), pos(0), len(strlen(source)) {}

    std::vector<Token> tokenize();

    private:
    void skip_whitespace();
    Token read_number();
    Token read_string();
    Token read_word();
    Token read_symbol();

};
