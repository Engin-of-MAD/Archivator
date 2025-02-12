//
// Created by roach on 05.02.2025.
//

#include "./inc/ParserCLI.h"

Parser::Parser(int argc, char** argv)
{
    registerHandlers();
    try{
        parseArgs(argc, argv);
        validate();
    } catch (const std::exception& e) {
        std::cerr << "Error level parser:\n"<< e.what() << std::endl;
    }
}

const Settings& Parser::getConf() const
{
    return conf;
}


void Parser::parseArgs(int argc, char** argv)
{
    std::string shortOptions = "hf:o:t:l:m:s:p:X:a:u:rdcxST";
    char currOption = 0;
    int indexOption = 0;
    const option long_options[] = {
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
        {"compress", no_argument, nullptr, 'c'},
        {"extract", no_argument, nullptr, 'x'},
        {nullptr, 0, nullptr, 0}
    };
    if (argc == 1)
        std::cerr << "You must specify one mod of the \'-cxauT\'.\n"
                     "Try 'xArch --help' for more information." << std::endl;

    while ((currOption = getopt_long(argc, argv, shortOptions.data(),
                                     long_options, &indexOption)) != -1) {
//        argsValidator(currOption);
        handlers[currOption](optarg);
    }
}

std::pair<size_t, Settings::TypeSize> Parser::parseSizeSplit(const std::string& sizeSplite) {
    int sizeMultiplier = std::stof(sizeSplite);
    std::pair<size_t, Settings::TypeSize> sizeSector;
    std::string qualificator = "10MB";
    for (auto iter: qualificator) {
        if (isdigit(iter))
            std::cout << "Digit is a found"<< std::endl;
    }

    sizeSector.first = sizeMultiplier;
    return sizeSector;
}

void Parser::validateCompressionMethod(const std::string& method) {
    const std::set<std::string> allowedMethods = {"store", "deflate", "bzip2", "lzma"};
    if (!allowedMethods.count(method)) {
        throw std::invalid_argument("Unsupported compression method: " + method);
    }
}

void Parser::validateArchiveType(const std::string& type) {
    const std::set<std::string> allowed = {"zip", "tar", "7z", "rar"};
    if (!allowed.count(type))
        throw std::invalid_argument("Unsupported archive type: " + type);
}

void Parser::validateLevelCompress(const std::string& level)
{
    std::regex level_regex(R"(^\d+$)");
    int val = 0;
    if (!std::regex_match(level, level_regex))
    {
        throw std::invalid_argument("Compression level must be an integer");
    }
    try
    {
        val = std::stoi(level);
        if ( val < 0 || val > 9) {
            throw std::invalid_argument("Error level: Argument must be between 0-9!");
        }
    } catch (std::exception&)
    {
        throw std::invalid_argument("Error level: Invalid compression level format.");
    }

}

void Parser::validateSplitSize(const std::string& size) {
    std::regex size_regex(R"(^\d+(\.\d+)?\s*[KMGT]B?$)", std::regex::icase);
    if (!std::regex_match(size, size_regex))
        throw std::invalid_argument("Invalid split size format. Example: 100MB, 10.5GB");
}



void Parser::registerHandlers() {
    handlers = {
            {'h', [&](const char*) { // --help
                conflictMode(Settings::Info);
                conf.mode = Settings::Info;
                printHelp();
            }},
             {'c', [&](const char*) { // --compress
                 conflictMode(Settings::Compress);
                 conf.mode = Settings::Compress;
             }},
             {'x', [&](const char*) { // --extract
                 conflictMode(Settings::Extract);
                 conf.mode = Settings::Extract;
             }},
            {'u', [&](const char*) {
                conflictMode(Settings::Update);
                conf.mode = Settings::Update;
            }},
            {'T', [&](const char*) {
                conflictMode(Settings::Test);
                conf.mode = Settings::Test;
            }},
            {'f', [&](const char* val) {
                conf.files.push_back(convertFromANSI(val));
            }},
            {'o', [&](const char* val){
                conf.arcName = val;
            }},
            {'t', [&](const char* type) {
                validateArchiveType(type);
                conf.compressionMethod = type;
            }},
            {'l', [&](const char* level){
                validateLevelCompress(level);
                conf.compressionLevel = std::stoi(level);
            }},
            {'s', [&](const char* splitSize){
                validateSplitSize(splitSize);
                conf.sizeSplit = parseSizeSplit(splitSize);
            }},
            {'m', [&](const char* method){
                validateCompressionMethod(method);
                conf.compressionMethod= method;
            }},
            {'X', [&](const char* pattern) {
                conf.excludePattern.push_back(pattern);
            }}
    };

}

void Parser::validate() {
    if (conf.mode == Settings::Err)
        throw std::invalid_argument("No mode specified");
    if (conf.mode == Settings::Compress && conf.files.empty())
        throw std::invalid_argument("Input files not required");
    if (conf.mode == Settings::Compress && conf.arcName.empty())
        throw std::invalid_argument("Not defined name archive");
    if (conf.mode == Settings::Update)
        throw std::exception
}

void Parser::conflictMode(Settings::ModeXArc flag) {
    if (conf.mode != Settings::NotSelected && conf.mode != Settings::Err && conf.mode != flag) {
        conf.mode = Settings::Err;
        throw std::invalid_argument("Mode conflict. Please choose only one main mode -cxuaT.");
    }
}

/*
 * @brief Convert string c-style to wstring (Wide String)
 * @param str path from optarg
 * @return return correct string
 */
std::wstring convertFromANSI(const char* str)
{
#ifdef _WIN32
    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &result[0], size);
    result.pop_back(); // Remove null terminator
    return result;
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
#endif
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

