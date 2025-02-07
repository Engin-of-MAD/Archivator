//
// Created by roach on 05.02.2025.
//

#ifndef XARC_H
#define XARC_H

#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <getopt.h>
/**@brief
 *
 */
class Settings
{
public:

    enum Mode {Compress = 0, Extractc, Append, Update, Test, Info, Err} modeXArc;
    Settings();
    Settings(int argc, char *argv[]);
private:

    std::vector<std::wstring> files;
    std::vector<std::string> updateArchive;
    std::vector<std::string> excludePattern;
    std::string arcName;
    std::string arcType;
    std::string compressionMethod;
    std::string password;
    std::string splitSize;
    bool preservePaths, deleteAfter, test, selfExtracting;
    int compressionLevel;
    int currMode;
    void parse(int argc, char* argv[]);
    void parseHelp();
    void notSelectedMode();
    void selectMainMode();
    void conflictModes(char arg);
    void unknownOption(char arg);
    void validatorOfModes(char arg);
    void validateArchiveType(const std::string& type);
    int validateLevelCompress(const std::string& level);
    std::string validateCompressionMethod(const std::string& method);
    std::string validateSplitSize(const std::string& size);
    void argsValidator(char arg);
};

std::wstring convertFromANSI(const char* str);
void printHelp();


#endif //XARC_H
