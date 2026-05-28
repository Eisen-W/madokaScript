#include "../include/parser.hpp"
#include <vector>

//public
std::vector<ASTNode*> Parser::parse()
{
    std::vector<ASTNode*> statements;
    while(!check(TokenType::EOF_))
    {
        statements.push_back(parse_statement());
    }
    return statements;
}

//private
ASTNode* Parser::parse_statement()
{
    if (check(TokenType::INT) ||
        check(TokenType::FLOAT) ||
        check(TokenType::STRING) ||
        check(TokenType::BOOL))
    {
        return parse_declaration();
    }

    if(check(TokenType::SAY))
    {
        return parse_say();
    }

    printf("parse error: unexpected token '%s'\n", tokens[pos].value.c_str());

    exit(1);
}

ASTNode* Parser::parse_declaration()
{
    Token type_tok = advance();

    Token name_tok =    expect(TokenType::IDENT, "expected variable name");
                        expect(TokenType::EQUALS, "expected '='");

    ASTNode* value =    parse_expr();
                        expect(TokenType::SEMICOLON, "expected ';'");

    ASTNode* node = new ASTNode{};
    node->type = NodeType::DECLARE;
    node->dec1_type = type_tok.value;
    node->dec1_name = name_tok.value;
    node->children = {value};

    return node;
}

//bottom to top precedence
//parse expr -> parse additive -> parse multilicative -> parse primary
ASTNode* Parser::parse_expr()
{
    return parse_additive();
}

ASTNode* Parser::parse_additive()
{
    ASTNode* left = parse_multiplicative();

    while(check(TokenType::PLUS) || check(TokenType::MINUS))
    {
        std::string op = advance().value;
        ASTNode* right = parse_multiplicative();

        ASTNode* node = new ASTNode();
        node->type      = NodeType::BINARY_OP;
        node->math_ops  = op;
        node->children  = {left, right};
        left = node;
    }
    return left;
}

ASTNode* Parser::parse_multiplicative()
{
    ASTNode* left = parse_primary();

    while (check(TokenType::STAR) || check(TokenType::SLASH))
    {
        std::string op = advance().value;
        ASTNode* right = parse_primary();

        ASTNode* node = new ASTNode{};
        node->type      = NodeType::BINARY_OP;
        node->math_ops  = op;
        node->children  = {left, right};
        left = node;
    }
    return left;
}

ASTNode* Parser::parse_primary()
{
    //literals
    if(check(TokenType::NUMBER_INT))
    {
        Token t = advance();
        ASTNode* n = new ASTNode{};
        n->type = NodeType::NUMER_INT;
        n->num_val = std::stod(t.value);
        return n;
    }
    if(check(TokenType::NUMBER_FLOAT))
    {
        Token t = advance();
        ASTNode* n = new ASTNode{};
        n->type = NodeType::NUMBER_FLOAT;
        n->num_val = std::stod(t.value);
        return n;
    }
    if(check(TokenType::STRING_LIT))
    {
        Token t = advance();
        ASTNode* n = new ASTNode{};
        n->type = NodeType::STRING_LIT;
        n->str_val = t.value;
        return n;
    }
    if(check(TokenType::BOOL_LIT))
    {
        Token t = advance();
        ASTNode* n = new ASTNode{};
        n->type = NodeType::BOOL_LIT;
        n->str_val = t.value; //true false
        return n;
    }

    //variable reference
    if(check(TokenType::IDENT))
    {
        Token t = advance();
        ASTNode* n = new ASTNode{};
        n->type = NodeType::IDENT;
        n->str_val = t.value;
        return n;
    }

    //for grouped expression
    if(check(TokenType::LPAREN))
    {
        advance();
        ASTNode* expr = parse_expr();
        expect(TokenType::RPAREN,"expected ')'");
        return expr;
    }

    printf("parse error: unexpected token '%s'\n", tokens[pos].value.c_str());
    exit(1);

}

ASTNode* Parser::parse_say()
{
    advance();

    expect(TokenType::LPAREN, "expected '(' after SAY");

    ASTNode* node = new ASTNode{};
    node->type = NodeType::SAY;
    node->children.push_back(parse_expr());

    expect(TokenType::RPAREN, "expected ')'");
    expect(TokenType::SEMICOLON, "expected ';'");
    return node;
}