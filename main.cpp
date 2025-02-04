#include <iostream>
#include "./molules/inc/ParserCLI.h"

int main(int argc, char *argv[])
{
    ParserCLI parser;
    parser.parse(argc, argv);
    return 0;
}
