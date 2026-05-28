#include "../include/interpreter.hpp"
#include <vector>

void Interpreter::run(std::vector<ASTNode*>& ast)
{
    //printf("ast size: %zu\n", ast.size());
    for(auto* node : ast)
    {
        exec(node);
    }
}

void Interpreter::exec(ASTNode* node)
{
    //printf("exec node types: %d\n", (int)node->type);
    if(node->type == NodeType::DECLARE)
    {
        Value v = eval(node->children[0]);
        env.set(node->decl_name, v);
        return;
    }
    if(node->type == NodeType::SAY)
    {
        Value v = eval(node->children[0]);
        print_value(v);
        return;
    }
}

Value Interpreter::eval(ASTNode* node)
{
    switch(node->type)
    {
        case NodeType::NUMBER_INT: 
        {
            Value v;
            v.type = Value::Type::INT;
            v.num  = node->num_val;
            return v;
        }
        case NodeType::NUMBER_FLOAT:
        {
            Value v;
            v.type = Value::Type::FLOAT;
            v.num  = node->num_val;
            return v;
        }
        case NodeType::STRING_LIT:
        {
            Value v;
            v.type = Value::Type::STRING;
            v.str  = node->str_val;
            return v;
        }
        case NodeType::BOOL_LIT:
        {
            Value v;
            v.type = Value::Type::BOOL;
            v.boolean = (node->str_val == "true");
            return v;
        }
        case NodeType::IDENT:
        {
            return env.get(node->str_val);
        }
        case NodeType::BINARY_OP:
        {
            Value l = eval(node->children[0]);
            Value r = eval(node->children[1]);
            Value result;
            result.type = (l.type == Value::Type::FLOAT || r.type == Value::Type::FLOAT)
                            ? Value::Type::FLOAT
                            : Value::Type::INT;
            if      (node->math_ops == "+") result.num = l.num + r.num;
            else if (node->math_ops == "-") result.num = l.num - r.num;
            else if (node->math_ops == "*") result.num = l.num * r.num;
            else if (node->math_ops == "/")
            {
                if(r.num == 0) { printf("runtime error: division by zero\n"); exit(1); }
                result.num = l.num / r.num;
            }
            
            return result;
        }
        default:
        printf("runtime error: unknown node\n");
        exit(1);
    }
}

void Interpreter::print_value(const Value& val)
{
    switch(val.type)
    {
        case Value::Type::INT:      printf("%d\n", (int)val.num); break;
        case Value::Type::FLOAT:    printf("%g\n", val.num);      break;
        case Value::Type::STRING:   printf("%s\n", val.str.c_str()); break;
        case Value::Type::BOOL:     printf("%s\n", val.boolean ? "true" : "false");
        default: break;
    }
}