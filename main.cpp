#include "include/lexer.hpp"
#include "include/parser.hpp"
#include <cstdio>
#include <string>

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("usage: mad <file.mad>\n");
        return 1;
    }
    FILE* f = fopen(argv[1], "r");
    std::string filename(argv[1]);
    if(filename.size() < 4 || filename.substr(filename.size() - 4) != ".mad")
    {
        printf("error: wrong extension or file not found\n");
        return 1;
    }
    if(!f)
    {
        printf("could not open file: %s\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* src = (char*)malloc(size + 1);
    fread(src, 1, size, f);
    src[size] = '\0';
    fclose(f);

    Lexer lexer(src);
    auto tokens = lexer.tokenize();

    for(auto& t : tokens)
    {
        printf("type: %d value: %s\n", (int)t.type, t.value.c_str());
    }
    Parser parser(tokens);
    auto ast = parser.parse();

    free(src);
    return 0;
}