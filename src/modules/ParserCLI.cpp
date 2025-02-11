//
// Created by roach on 05.02.2025.
//

#include "./inc/ParserCLI.h"

Parser::Parser(int argc, char** argv)
{
    try{
        parseArgs(argc, argv);
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
    char currOption = 0, prevOption = 0;
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
/*    std::unordered_map<char, std::function<void(const char*)>> handlers = {
            {'h', [&](const char*) {printHelp();exit(0);}},
            {'f', [&](const char*) {conf.files.push_back(convertFromANSI(optarg));}},
            {'o', [&](const char*) {conf.arcName = optarg;}},
            {'t', [&](const char*) {conf.modeXArc = Settings::Test;
                validateArchiveType(optarg); conf.test = true;}},
            {'l', [&](const char*) {}}
    };*/
    while ((currOption = getopt_long(argc, argv, shortOptions.data(),
                                     long_options, &indexOption)) != -1) {
        switch (currOption) {
            case 'h':
                if (prevOption != 0 && prevOption != 'h')
                    throw std::invalid_argument("--help cannot be combined with other modes");
                conf.modeXArc = Settings::Info;
                prevOption = 'h';
                exit(0);
            case 'c':
                conf.modeXArc = Settings::Compress;
                prevOption = currOption;
                break;
            case 'x':
                conf.modeXArc = Settings::Extract;
                prevOption = currOption;
            case 'a':
                conf.modeXArc = Settings::Append;
                prevOption = currOption;
                break;
            case 'u':
                conf.modeXArc = Settings::Update;
                conf.files.push_back(convertFromANSI(optarg));
                prevOption = currOption;
                break;
            case 'T':
                if (prevOption != 0 && currOption != prevOption)
                    throw std::invalid_argument("Conflicting modes: can't combine -c, extract -x,"
                                                " -append -a, update -u, test -T .");
                conf.modeXArc = Settings::Test;
                conf.test = true;
                prevOption = currOption;
                break;
            case 'f':
                conf.files.push_back(convertFromANSI(optarg));
                break;
            case 'o':
                conf.arcName = optarg;
                break;
            case 't':
                validateArchiveType(optarg);
                conf.arcName = optarg;
                break;
            case 'l':
                validateLevelCompress(optarg);
                conf.compressionLevel = std::stoi(optarg);
                break;
            case 'm':
                validateCompressionMethod(optarg);
                conf.compressionMethod = optarg;
                break;
            case 's':
                validateSplitSize(optarg);
                parseSizeSplit(optarg);
                break;
            case 'X':
                conf.excludePattern.push_back(optarg);
                break;
            case 'r':
                conf.preservePaths = true;
                break;
            case 'd':
                if (conf.modeXArc != Settings::Compress && conf.modeXArc != Settings::Append && conf.modeXArc != Settings::Update)
                    throw std::invalid_argument("--delete-after requires compress/append/update mode");
                conf.deleteAfter = true;
                break;
            case 'S':
                if (conf.modeXArc != Settings::Compress)
                    throw std::invalid_argument("--self-extracting requires compress mode");
                conf.selfExtracting = true;
                break;
        }

        std::cout <<indexOption << std::endl;
    }
}

std::pair<size_t, Settings::TypeSize> Parser::parseSizeSplit(const std::string& sizeSplite) {
//    float sizeMultiplier = std::stof(sizeSplite);
    std::pair<size_t, Settings::TypeSize> sizeSector;
    std::string qualificator = "10MB";
    for (auto iter: qualificator) {
        if (isdigit(iter))
            std::cout << "Digit is a found"<< std::endl;
    }
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

void Parser::parseHelp() {
    if (currMode != 0 && currMode != 'h')
        throw std::invalid_argument("--help cannot be combined with other modes");
    conf.modeXArc = Settings::Info;
    currMode = 'h';
}

void Parser::notSelectedMode() {
    if (currMode != 'h') {
        throw std::invalid_argument("You must specify one mod of the \'-cxauT\'.\nTry 'xArch --help' for more information.\n");
    }
}
void Parser::selectMainMode() {
    // Установка основного режима работы
    switch (currMode) {
        case 'c': conf.modeXArc = Settings::Compress; break;
        case 'x': conf.modeXArc = Settings::Extract; break;
        case 'a': conf.modeXArc = Settings::Append; break;
        case 'u': conf.modeXArc = Settings::Update; break;
        case 'T': conf.modeXArc = Settings::Test; break;
        case 0: notSelectedMode(); // Режим не выбран
            break;
        default:
            break;
    }
}
void Parser::conflictModes(char arg)
{
    if (currMode != '0' && currMode != arg) {
        throw std::invalid_argument("Conflicting modes: can't combine -c, extract -x,"
                                    " -append -a, update -u, test -T .");
    }
    currMode = arg;
}
void Parser::unknownOption(char arg) {
    if (arg != 'h')
        throw std::invalid_argument("Unknown option: -" + arg);
}
void Parser::validatorOfModes(char arg) {
    switch (arg) {
        case 'h': // Помощь - высший приоритет
            parseHelp(); // Нет необходимости обрабатывать другие флаги
            return;
            break;
        case 'c': // Режим сжатия
        case 'x': // Режим распаковки
        case 'a': // Добавление в архив
        case 'u': // Обновление архива
        case 'T': // Тестирование архива
            conflictModes(arg); // Проверка конфликта режимов
            break;
        default:
            break;
    }
}

/*
 * @brief Check arguments in time parsing
 */
void Parser::argsValidator(char arg) {
    validatorOfModes(arg);
    selectMainMode();
    switch (arg) {
        case 'f':
            conf.files.push_back(convertFromANSI(optarg));
            break;
#ifdef DEBUG_SETTINGS
            std::cout << "Added file: " << optarg << std::endl;
#endif
        case 'o':
            conf.arcName = optarg;
            break;
        case 't':
            validateArchiveType(optarg);
            conf.arcName = optarg;
            break;
        case 'l':
            validateLevelCompress(optarg);
            conf.compressionLevel = std::stoi(optarg);
            break;
        case 'm':
            validateCompressionMethod(optarg);
            conf.compressionMethod = optarg;
            break;
        case 's':
            validateSplitSize(optarg);
            break;
        case 'x':
            conf.excludePattern.push_back(optarg);
            break;
        case 'r':
            conf.preservePaths = true;
            break;
        case 'd':
            if (conf.modeXArc != Settings::Compress && conf.modeXArc != Settings::Append &&
                conf.modeXArc != Settings::Update)
                throw std::invalid_argument("--delete-after requires compress/append/update mode");
            conf.deleteAfter = true;
            break;
        case 'S':
            if (conf.modeXArc != Settings::Compress)
                throw std::invalid_argument("--self-extracting requires compress mode");
            conf.selfExtracting = true;
            break;
        case 'a':
        case 'u':
            conf.files.push_back(convertFromANSI(optarg));
            break;
        case 'T':
            conf.test = true;
            break;
        default:
            if (arg != 'h')
                throw std::invalid_argument("Unknown option: -" + arg);
            break;
    }
}
/*
 * @brief Convert string c-style to wstring (Wide String)
 * @param str path from optarg
 * @return return correct path
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

