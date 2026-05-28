#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include "lexer.hpp"

enum class NodeType {
    DECLARE,
    BINARY_OP,
    NUMBER_INT,
    NUMBER_FLOAT,
    STRING_LIT,
    BOOL_LIT,
    IDENT,
    SAY,
};

struct ASTNode {
    NodeType type;
    std::string str_val;
    double num_val;
    std::string decl_type;
    std::string decl_name;
    std::string math_ops;

    std::vector<ASTNode*> children;
};

class Parser {
    std::vector<Token> tokens;
    size_t pos;

    Token peek() { return tokens[pos]; }
    Token advance() { return tokens[pos++]; }

    bool check(TokenType t) { return tokens[pos].type == t; }

    Token expect(TokenType t, const char* err)
    {
        if(!check(t))
        {
            printf("parse error: %s (got '%s')\n", err, tokens[pos].value.c_str());
            exit(1);
        }
        return advance();
    }

    public:
    Parser(std::vector<Token> tok) : tokens(std::move(tok)), pos(0) {}

    std::vector<ASTNode*> parse();

    private:
    ASTNode* parse_statement();
    ASTNode* parse_declaration();
    ASTNode* parse_expr();
    ASTNode* parse_additive();
    ASTNode* parse_multiplicative();
    ASTNode* parse_primary();
    //ASTNode* parse_op(NodeType op_type);
    ASTNode* parse_say();

};