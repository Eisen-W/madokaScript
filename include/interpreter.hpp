#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "parser.hpp"

struct Value {
    enum class Type { INT, FLOAT, STRING, BOOL, NIL } type = Type::NIL;
    double num = 0;
    std::string str;
    bool boolean = false;
};

struct Environment {
    std::unordered_map<std::string, Value> vars;

    void set(const std::string& name, Value v) 
    {
        vars[name] = v;
    }

    Value get(const std::string& name)
    {
        if(vars.count(name)) return vars[name];
        printf("runtime error: undefined variable '%s'", name.c_str());
        exit(1);
    }
};

class Interpreter{
    Environment env;

    public:
    void run(std::vector<ASTNode*>& ast);

    private:
    void exec(ASTNode* node);
    Value eval(ASTNode* node);
    void print_value(const Value& val);
};