//
// Created by roach on 05.02.2025.
//

#include <codecvt>
#include <locale>
#include <set>

#include "./inc/XArc.h"

Settings::Settings()
: modeXArc(Err), compressionLevel(6)
, preservePaths(false), test(true)
, selfExtracting(false), deleteAfter(false), currMode(0)
{}

Settings::Settings(int argc, char* argv[]) : Settings() {
    try{
        parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error level parser: "<< e.what() << std::endl;
    }
}

void Settings::parse(int argc, char* argv[])
{
    const char* shortOptions = "hf:o:t:l:m:s:p:X:a:u:rdcxST";

    const struct option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"files", required_argument, nullptr, 'f'},
        {"output", required_argument, nullptr, 'o'},
        {"type", required_argument, nullptr, 't'},
        {"compression-level", required_argument, &currMode, 'l'},
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
    char currOption = 0;
    int indexOption = 0;
    while ((currOption = getopt_long(argc, argv, shortOptions, long_options, &indexOption)) != -1)
        argsValidator(currOption);

    if (argc == 1 )
        notSelectedMode();
}

void Settings::parseHelp() {
    if (currMode != 0 && currMode != 'h')
        throw std::invalid_argument("--help cannot be combined with other modes");
    modeXArc = Info;
    currMode = 'h';
}

std::string Settings::validateSplitSize(const std::string& size) {
    std::regex size_regex(R"(^\d+(\.\d+)?[KMGT]?B?$)", std::regex::icase);
    if (!std::regex_match(size, size_regex))
        throw std::invalid_argument("Invalid split size format. Example: 100MB, 10.5GB");
    return size;
}

int Settings::validateLevelCompress(const std::string& level) {
    std::regex level_regex(R"(^\\d+$)");
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

    return val;
}

std::string Settings::validateCompressionMethod(const std::string& method)
{
    const std::set<std::string> allowedMethods = {"store", "deflate", "bzip2", "lzma"};
    if (!allowedMethods.count(method)) {
        throw std::invalid_argument("Unsupported compression method: " + method);
    }
    return method;
}

void Settings::notSelectedMode() {
    if (currMode != 'h') {
        throw std::invalid_argument("You must specify one mod of the \'-cxauT\'.\nTry 'xArch --help' for more information.\n");
    }
}

void Settings::selectMainMode() {

    // Установка основного режима работы
    switch (currMode) {
    case 'c': modeXArc = Compress; break;
    case 'x': modeXArc = Extractc; break;
    case 'a': modeXArc = Append; break;
    case 'u': modeXArc = Update; break;
    case 'T': modeXArc = Test; break;
    case 0: notSelectedMode(); // Режим не выбран
        break;
    default:
        break;
    }

}

void Settings::conflictModes(char arg)
{
    if (currMode != '0' && currMode != arg) {
        throw std::invalid_argument("Conflicting modes: can't combine -c, extract -x,"
                     " -append -a, update -u, test -T .");
    }
    currMode = arg;
}

void Settings::unknownOption(char arg) {
    if (arg != 'h')
        throw std::invalid_argument("Unknown option: -" + arg);
}

void Settings::validatorOfModes(char arg) {

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

void Settings::validateArchiveType(const std::string& type)
{
    const std::set<std::string> allowed = {"zip", "tar", "7z", "rar"};
    if (!allowed.count(type))
        throw std::invalid_argument("Unsupported archive type: " + type);
}

/*
 * @brief Check arguments in time parsing
 */
void Settings::argsValidator(char arg)
{
    validatorOfModes(arg);
    selectMainMode();
    switch (arg)
    {
        case 'f':
            files.push_back(convertFromANSI(optarg));
            break;
#ifdef DEBUG_SETTINGS
        std::cout << "Added file: " << optarg << std::endl;
#endif
        case 'o':
            arcName = optarg;
            break;
        case 't':
            validateArchiveType(optarg);
            arcName = optarg;
            break;
        case 'l':
             validateLevelCompress(optarg);
            compressionLevel = std::stoi(optarg);
            break;
        case 'm':
            validateCompressionMethod(optarg);
            compressionMethod = optarg;
            break;
        case 's':
            validateSplitSize(optarg);
            break;
        case 'x':
            excludePattern.push_back(optarg);
            break;
        case 'r':
            preservePaths = true;
            break;
        case 'd':
            if (modeXArc != Compress && modeXArc != Append && modeXArc != Update)
                throw std::invalid_argument("--delete-after requires compress/append/update mode");
            deleteAfter = true;
            break;
        case 'S':
            if (modeXArc != Compress)
                throw std::invalid_argument("--self-extracting requires compress mode");
            selfExtracting = true;
            break;
        case 'a':
        case 'u':
            files.push_back(convertFromANSI(optarg));
            break;
        case 'T':
            test = true;
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

