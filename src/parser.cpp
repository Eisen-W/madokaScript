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

    if(check(TokenType::PRINT))
    {
        return parse_say();
    }

    printf("parse error: unexpected token '%s'\n", tokens[pos].value.c_str());

    exit(1);
}

ASTNode* Parser::parse_declaration()
{
    Token type_tok = advance();

    Token name_tok = expect(TokenType::IDENT, "expected variable name");

    expect(TokenType::EQUALS, "expected '='");

    ASTNode* value = parse_expr();

    expect(TokenType::SEMICOLON, "expected ';'");

    ASTNode* node = new ASTNode{};
    node->type = NodeType::DECLARE;
    node->dec1_type = type_tok.value;
    node->dec1_name = name_tok.value;
    node->children = {value};

    return node;
}

ASTNode* Parser::parse_expr()
{
    //math ops
    if(check(TokenType::ADD)) return parse_op(NodeType::ADD);
    if(check(TokenType::SUB)) return parse_op(NodeType::SUBTRACT);
    if(check(TokenType::MULT)) return parse_op(NodeType::MULTIPLY);
    if(check(TokenType::DIV)) return parse_op(NodeType::DIVIDE);

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

    printf("parse error: unexpected token '%s'\n", tokens[pos].value.c_str());
    exit(1);
}

ASTNode* Parser::parse_op(NodeType op_type)
{
    advance();

    expect(TokenType::LPAREN, "expected '(' after op");

    ASTNode* node = new ASTNode{};
    node->type = op_type;

    while(!check(TokenType::RPAREN) && !check(TokenType::EOF_))
    {
        node->children.push_back(parse_expr());
        if(check(TokenType::COMMA)) advance();
    }

    expect(TokenType::RPAREN, "expected ')'");
    return node;
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