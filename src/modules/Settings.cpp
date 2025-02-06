//
// Created by roach on 05.02.2025.
//

#include <codecvt>
#include <locale>

#include "./inc/XArc.h"

Settings::Settings()
: modeXArc(Compress), compressionLevel(6)
, preservePaths(false), test(true)
, selfExtracting(false), deleteAfter(false), currMode(0)
{}

Settings::Settings(int argc, char* argv[]) : Settings() {
    parse(argc, argv);
}

void Settings::parse(int argc, char* argv[])
{
    const char* shortOptions = "hf:o:t:l:m:s:p:X:a:u:rdcxST";

    const struct option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"files", required_argument, nullptr, 'f'},
        {"output", required_argument, nullptr, 'o'},
        {"type", required_argument, nullptr, 't'},
        {"compression-level", required_argument, nullptr, 'l'},
        {"compression-method", required_argument, nullptr, 'm'},
        {"password", required_argument, nullptr, 'p'},
        {"split-size", required_argument, nullptr, 's'},
        {"exclude", required_argument, nullptr, 'X'},
        {"preserve-path", no_argument, nullptr, 'r'},
        {"delete-after", no_argument, nullptr, 'd'},
        {"self-extracting", no_argument, nullptr, 'S'},
        {"append", required_argument, nullptr, 'a'},
        {"update", required_argument, nullptr, 'u'},
        {"test-archive", no_argument, nullptr, 'T'},
        {"compress", no_argument, &currMode, 'c'},
        {"extract", no_argument, &currMode, 'x'}
    };
    int currOption = 0, indexOption = 0;
    while ((currOption = getopt_long(argc, argv, shortOptions, long_options, &indexOption)) != -1) {
        argsValidator(currOption);
    }
}

/*
 * @brief Check arguments in time parsing
 */
void Settings::argsValidator(int arg)
{
    if (currMode == 'h') modeXArc = Info;
    else if (currMode == 'c') modeXArc = Compress;
    else if (currMode == 'x') modeXArc = Extractc;
    else
    {
        switch (arg)
        {
        case 'h':
            printHelp();
            break;
        case 'f':
            files.push_back(convertFromANSI(optarg));
            break;
        case 'o':
            arcName = optarg;
            break;
        case 't':
            arcType = optarg;
            break;
        case 'l':
            compressionLevel = atoi(optarg) <= 10 ? atoi(optarg) : -1;
            break;
        case 'm':
            compressionMethod = optarg;
            break;
        case 's':
            splitSize = optarg;
            break;
        case 'X':
            excludePattern.push_back(optarg);
            break;
        case 'r':
            preservePaths = true;
            break;
        case 'd':
            deleteAfter = true;
            break;
        case 'S':
            selfExtracting = true;
            break;
        case 'a':
            files.push_back(convertFromANSI(optarg));
        break;
        case 'u':
            files.push_back(convertFromANSI(optarg));
        break;
        default:
            std::cerr << "Select correct mode please" << std::endl;
        break;
        }
    }
}

/*
 * @brief Convert string c-style to wstring (Wide String)
 * @param str path from optarg
 * @return return correct path
 */
std::wstring convertFromANSI(const char* str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.from_bytes(str);
}


/*
 * @brief Print Info
 */
void printHelp() {
    std::cout << "Usage: ./archiver [options]\n"
                          << "Options:\n"
                          << "  -h              Show help\n"
                          << "  -c              Compress to archive\n"
                          << "  -x              Extract from archive\n"
                          << "  -i <input>      Input file/directory\n"
                          << "  -o <output>     Output archive\n"
                          << "  -t <type>       Archive type (tar, zip, 7z)\n"
                          << "  -l <level>      Compression level (0-9)\n"
                          << "  -m <method>     Compression method (gzip, bzip2, xz)\n"
                          << "  -p <password>   Password for encryption\n"
                          << "  -s <size>       Split archive into parts (e.g., 100M)\n"
                          << "  -X <pattern>    Exclude files matching pattern\n"
                          << "  -r              Preserve directory structure\n"
                          << "  -d              Delete files after archiving\n"
                          << "  -S              Create self-extracting archive\n"
                          << "  -a <archive>    Append files to existing archive\n"
                          << "  -u <archive>    Update existing archive\n"
                          << "  -T              Test archive integrity\n";
}

