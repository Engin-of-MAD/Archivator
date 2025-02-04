//
// Created by roach on 04.02.2025.
//

#include "inc/ParserCLI.h"

ParserCLI::ParserCLI()
    : parsedData({}), longOptions{
          {"help", no_argument, nullptr, 'h'}, {"input", required_argument, nullptr, 'i'},
          {"output", required_argument, nullptr, 'o'}, {"type", required_argument, nullptr, 't'},
          {"compression-level", required_argument, nullptr, 'l'},
          {"compression-method", required_argument, nullptr, 'm'}, {"password", required_argument, nullptr, 'm'},
          {"split-size", required_argument, nullptr, 's'}, {"exclude", required_argument, nullptr, 'x'},
          {"preserve-path", no_argument, nullptr, 'r'}, {"delete-after", no_argument, nullptr, 'd'},
          {"self-extracting", no_argument, nullptr, 'S'}, {"append", required_argument, nullptr, 'a'},
          {"update", required_argument, nullptr, 'u'}, {"test", no_argument, nullptr, 'T'}
      }
{
}

void ParserCLI::parse(int argc, char* argv[])
{
    int optionIndex = 0; char currOption = 0;

   while ((currOption = getopt_long(argc, argv, shortOpts, longOptions, &optionIndex)) != -1) {
    switch (currOption) {
        case 'h': std::cout << "Help" << std::endl; break;
        case 'i': std::cout << currOption << " " << optarg << std::endl;break;
        case 'o': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'l': std::cout << currOption << " " <<optarg << std::endl;break;
        case 't': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'm': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'p': std::cout << currOption << " " <<optarg << std::endl;break;
        case 's': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'x': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'r': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'd': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'S': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'a': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'u': std::cout << currOption << " " <<optarg << std::endl;break;
        case 'T': std::cout << currOption << " " <<optarg << std::endl;break;
        case '?': std::cerr << "Incorrect argument" << std::endl;break;
    default: std::cerr << "Incorrect argument null" << std::endl;break;
    }
   }

}
