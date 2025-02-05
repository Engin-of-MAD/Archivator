//
// Created by roach on 05.02.2025.
//

#include "./inc/XArc.h"

Settings::Settings()
: modeXArc(Compress), compressionLevel(6)
, preservePaths(false), test(true)
, selfExtracting(false), deleteAfter(false)
{}

Settings::Settings(int argc, char* argv[]) : Settings()
{
    parse(argc, argv);
}

void Settings::parse(int argc, char* argv[])
{
    const char* shortOptions = "hi:o:t:l:m:s:p:X:a:u:rdcxST";
    std::vector<int> args;
    const struct option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"input", required_argument, nullptr, 'i'},
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
        {"compress", no_argument, nullptr, 'c'},
        {"extract", no_argument, nullptr, 'x'}
    };
    int currOption = 0, indexOption = 0;
    while ((currOption = getopt_long(argc, argv, shortOptions, long_options, &indexOption)) != -1) {
       args.push_back(currOption);
    }
}

void Settings::printHelp() {
    std::cout << "Usage: ./archiver [options]\n"
                          << "Options:\n"
                          << "  -h              Show help\n"
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

