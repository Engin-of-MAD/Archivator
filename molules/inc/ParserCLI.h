//
// Created by roach on 04.02.2025.
//

#ifndef PARSERCLI_H
#define PARSERCLI_H

#include <getopt.h>
// #include <unistd.h>
#include <string>
#include <iostream>

struct ArchiveData
{
public:

    std::string inputPath;
    std::string outputPath;
    std::string archivePath;
    int compression_level = 6;
    std::string compressionMethod;
    std::string password;
    std::string splitSize;
    std::string excludePattern;
    bool preservePaths;
    bool deleteAfter;
    bool selfExtracting;
    bool test;

    bool appendMode;
    bool updateMode;
};

class ParserCLI {
public:
    ParserCLI();

    void parse(int argc, char *argv[]);
private:
    ArchiveData parsedData;
    const struct option longOptions[15];

    const char* shortOpts = "hi:o:t:l:m:p:s:x::u:rdSTa";
    // ArchiveData getData() const;
};




#endif //PARSERCLI_H
