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
    enum ErrorsTypes{AbsenceInputsFiles = 0, ConflictsModesCXAUT,
        SpecificMode, IncorrectCompressionLevel, AbsenceMods,
        ErrCombinateMods, UnsupportedType, InvalidFormat,UnsupportedMethod, Ok};
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
    ErrorsTypes parseHelp();
    ErrorsTypes notSelectedMode();
    ErrorsTypes selectMainMode();
    ErrorsTypes conflictModes(char arg);

    ErrorsTypes validatorOfModes(char arg);
    ErrorsTypes validateArchiveType(const std::string& type);
    ErrorsTypes validateLevelCompress(const std::string& level);
    ErrorsTypes validateCompressionMethod(const std::string& method);
    ErrorsTypes validateSplitSize(const std::string& size);
    ErrorsTypes argsValidator(char arg);
};

std::wstring convertFromANSI(const char* str);
void printHelp();


#endif //XARC_H
