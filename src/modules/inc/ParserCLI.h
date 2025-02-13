//
// Created by roach on 05.02.2025.
//

#ifndef XARC_H
#define XARC_H

#include <vector>
#include <string>
#include <regex>
#include <codecvt>
#include <getopt.h>
#include <iostream>
#include <set>
#include <unordered_map>
#include <filesystem>
#include <functional>

///@brief All params after parsings for all program


struct Settings
{
    enum ModeXArc {Compress = 0, Extract, Append, Update, Test, Info, Err, NotSelected};
    enum TypeSize{KiB, Mib, Gib};
    std::vector<std::wstring> input;
    std::vector<std::wstring> updateArchive;

    std::vector<std::string> excludePattern;
    size_t sizeSplit;
    std::string output;
    std::string arcType;
    std::string compressionMethod;
    std::string password;
    bool preservePaths, deleteAfter, selfExtracting;
    int compressionLevel;
    ModeXArc mode = NotSelected;
};

std::wstring convertFromANSI(const char* str);
int detectSize(std::string& str);
size_t convertToBytes(int size);

void printHelp();


class Parser {
public:
    Parser(int argc, char **argv);
    const Settings& getConf() const;
private:
    void parseArgs(int argc, char **argv);
    size_t parseSizeSplit(const std::string& sizeSplite);
    void validate();
    void validateSplitSize(const std::string& size);
    void validateArchiveType(const std::string& type);
    void validateLevelCompress(const std::string& level);
    void validateCompressionMethod(const std::string& method);

    void conflictMode(Settings::ModeXArc flag);
    void registerHandlers();

    std::unordered_map<char, std::function<void(const char*)>> handlers;
    Settings conf;
};

#endif //XARC_H
