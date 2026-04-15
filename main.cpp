#include "include/lexer.hpp"

int main()
{
    const char* src = R"(
            int x = ADD(3, 4, SUB(8, 7));
            string name = "madoka";
            SAY(name)
            SAY(x)
            )";

    Lexer lexer(src);
    auto tokens = lexer.tokenize();

    for(auto& t : tokens)
    {
        printf("type: %d value: %s\n", (int)t.type, t.value.c_str());
    }

}