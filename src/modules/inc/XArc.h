//
// Created by roach on 05.02.2025.
//

#ifndef XARC_H
#define XARC_H

#include <vector>
#include <string>
#include <iostream>
#include <getopt.h>
/**@brief
 *
 */
class Settings
{
public:

    enum Mode {Info = 0, Compress, Extractc, CompressWithCrypt} modeXArc;

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
    void argsValidator(int arg);

};

std::wstring convertFromANSI(const char* str);
void printHelp();


#endif //XARC_H
