#include "../include/lexer.hpp"
#include <cctype>
#include <string>

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while(pos < len)
    {
        skip_whitespace();
        if(pos >= len) break;

        char c = src[pos];

        if(c == '"')        tokens.push_back(read_string());
        else if(isdigit(c)) tokens.push_back(read_number());
        else if(isalpha(c)) tokens.push_back(read_word());
        else                tokens.push_back(read_symbol());
    }
    tokens.push_back({TokenType::EOF_, ""});
    return tokens;
}

void Lexer::skip_whitespace()
{
    while(pos < len && (src[pos] == ' ' || src[pos] == '\n' || src[pos] == '\t' || src[pos] == '\r'))
        pos++;
}

Token Lexer::read_number()
{
    size_t start = pos;
    bool is_float = false;

    while (pos < len && isdigit(src[pos])) pos++;

    if(pos < len && src[pos] == '.' && isdigit(src[pos + 1]))
    {
        is_float = true;
        pos++;
        while(pos < len && isdigit(src[pos])) pos++;
    }

    std::string val(src + start, pos - start);
    return {is_float ? TokenType::NUMBER_FLOAT : TokenType::INT, val};
}

Token Lexer::read_string()
{
    pos++;
    size_t start = pos;
    while(pos < len && src[pos] != '"') pos++;
    std::string val(src + start, pos - start);
    pos++;
    return {TokenType::STRING_LIT, val};
}

Token Lexer::read_word()
{
    size_t start = pos;
    while (pos < len && (isalnum(src[pos]) || src[pos] == '_')) pos++;
    std::string word(src + start, pos - start);

    if(word == "int") return {TokenType::INT, word};
    if(word == "float") return {TokenType::FLOAT, word};
    if(word == "string") return {TokenType::STRING, word};
    if(word == "bool") return {TokenType::BOOL, word};
    if(word == "true") return {TokenType::BOOL_LIT, word};
    if(word == "false") return {TokenType::BOOL_LIT, word};
    if(word == "ADD") return {TokenType::ADD, word};
    if(word == "SUB") return {TokenType::SUB, word};
    if(word == "MULT") return {TokenType::MULT, word};
    if(word == "DIV") return {TokenType::DIV, word};
    if(word == "SAY") return {TokenType::PRINT, word};

    return {TokenType::IDENT, word};
}

Token Lexer::read_symbol()
{
    char c = src[pos++];
    switch(c) {
        case '=': return {TokenType::EQUALS, "="};
        case '(': return {TokenType::LPAREN, "("};
        case ')': return {TokenType::RPAREN, ")"};
        case ',': return {TokenType::COMMA, ","};
        case ';': return {TokenType::SEMICOLON, ";"};
        default:
            printf("unknown char : %c\n", c);
            return read_symbol();
    }
}