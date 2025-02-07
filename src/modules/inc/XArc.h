//
// Created by roach on 05.02.2025.
//

#ifndef XARC_H
#define XARC_H

#include <vector>
#include <string>
#include <regex>


///@brief All params after parsings for all program

class Settings
{
public:

    enum Mode {Compress = 0, Extractc, Append, Update, Test, Info, Err};
    Settings();
    Settings(int argc, char *argv[]);
    ~Settings() = default;
    std::string getArcName() const;
    std::string getArcType() const;
    std::string getFiles() const;
    std::string getPassword() const;
    std::string getCompressionMethod() const;
    std::string getSplitSize() const;
    bool getPreservePaths() const;
    bool getDeleteAfter() const;
    bool getTets() const;
    bool getSelfExtraction() const;
    int getCompressionLevel() const;

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
    Mode modeXArc;
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
