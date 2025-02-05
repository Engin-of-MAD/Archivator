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

    enum Mode { Compress = 1, Extractc = 2, CompressWithCrypt = 3, Err = 0} modeXArc;

    Settings();
    Settings(int argc, char *argv[]);
private:

    std::vector<std::string> files;
    std::vector<std::string> addToArchive;
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

    void printHelp();

};


#endif //XARC_H
