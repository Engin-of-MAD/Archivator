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
    while ((currOption = getopt_long(argc, argv, shortOptions, long_options, &indexOption)) != -1) {
        ErrorsTypes err_code = argsValidator(currOption);
        if (err_code != Ok) break;
    }
    if (argc == 1 )
        notSelectedMode();
}

Settings::ErrorsTypes Settings::parseHelp() {
    ErrorsTypes err_code = Ok;
    if (currMode != 0 && currMode != 'h')
    {
        std::cerr << "--help cannot be combined with other modes" << std::endl;
        err_code = ErrCombinateMods;
    } else
    {
        currMode = 'h';
    }
    return err_code;
}

Settings::ErrorsTypes Settings::validateSplitSize(const std::string& size) {
    std::regex size_regex(R"(^\d+(\.\d+)?[KMGT]?B?$)", std::regex::icase);
    ErrorsTypes err_code = Ok;
    if (!std::regex_match(size, size_regex)) {
        std::cerr << "Invalid split size format. Example: 100MB, 10.5GB" << std::endl;
        err_code = InvalidFormat;
    } else
    {
        splitSize = size;
    }
    return err_code;
}

Settings::ErrorsTypes Settings::validateLevelCompress(const std::string& level) {
    ErrorsTypes err_code = Ok;

    std::regex level_regex(R"(^/d$)");
    if (!std::regex_match(level, level_regex))
    {
        std::cerr << "Compression level must be an integer" << std::endl;
        err_code = InvalidFormat;
    }
    try
    {
        int val = std::stoi(level);
        if ( val < 0 || val > 9)
        {
            std::cerr << "Error level: Argument must be between 0-9!" << std::endl;
            err_code = IncorrectCompressionLevel;
        }
    } catch (std::exception&)
    {
        std::cerr << "Error level: Invalid compression level format." << std::endl;
        err_code = InvalidFormat;
    }

    return err_code;
}

Settings::ErrorsTypes Settings::validateCompressionMethod(const std::string& method)
{
    ErrorsTypes err_code = Ok;
    const std::set<std::string> allowedMethods = {"store", "deflate", "bzip2", "lzma"};
    if (!allowedMethods.count(method))
    {
        std::cerr << "Unsupported compression method: " + method << std::endl;
        err_code = UnsupportedMethod;
    }
    return err_code;
}

Settings::ErrorsTypes Settings::notSelectedMode() {
    ErrorsTypes err_code = Ok;
    if (currMode != 'h')
    {
        std::cerr << "You must specify one mod of the \'-cxauT\'.\nTry 'xArch --help' for more information.\n";
        err_code = AbsenceMods;
    }
    return err_code;
}

Settings::ErrorsTypes Settings::selectMainMode() {
    ErrorsTypes err_code = Ok;
    // Установка основного режима работы
    switch (currMode) {
    case 'c': modeXArc = Compress; break;
    case 'x': modeXArc = Extractc; break;
    case 'a': modeXArc = Append; break;
    case 'u': modeXArc = Update; break;
    case 'T': modeXArc = Test; break;
    case 0:   // Режим не выбран
        err_code = notSelectedMode();
        break;
    default:
        break;
    }
    return err_code;
}

Settings::ErrorsTypes Settings::conflictModes(char arg)
{
    ErrorsTypes err_code = Ok;
    if (currMode != '0' && currMode != arg)
    {
        std::cerr << "Conflicting modes: can't combine -c, extract -x,"
                     " -append -a, update -u, test -T ." << std::endl;
        err_code = ConflictsModesCXAUT;
    } else
    {
        modeXArc = Test;
        currMode = arg;
    }

    return err_code;
}

Settings::ErrorsTypes Settings::validatorOfModes(char arg) {
    ErrorsTypes err_code = Ok;
    switch (arg) {
    case 'h': // Помощь - высший приоритет
        return parseHelp(); // Нет необходимости обрабатывать другие флаги
        break;
    case 'c': // Режим сжатия
    case 'x': // Режим распаковки
    case 'a': // Добавление в архив
    case 'u': // Обновление архива
    case 'T': // Тестирование архива
        // Проверка конфликта режимов
       return conflictModes(arg);
        break;
    default:
        break;
    }
    return err_code;
}

Settings::ErrorsTypes Settings::validateArchiveType(const std::string& type)
{
    const std::set<std::string> allowed = {"zip", "tar", "7z", "rar"};
    if (!allowed.count(type))
        std::cerr << "Unsupported archive type: " + type << std::endl;
        return UnsupportedType;
}

/*
 * @brief Check arguments in time parsing
 */
Settings::ErrorsTypes Settings::argsValidator(char arg)
{
    ErrorsTypes err_code = validatorOfModes(arg);
    if (err_code != Ok)
        return err_code;
    err_code = selectMainMode();
    if (err_code != Ok)
        return err_code;

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
            err_code = validateArchiveType(optarg);
            arcName = optarg;
            break;
        case 'l':
            err_code = validateLevelCompress(optarg);
            compressionLevel = std::stoi(optarg);
            break;
        case 'm':
            err_code = validateCompressionMethod(optarg);
            compressionMethod = optarg;
            break;
        case 's':
            err_code = validateSplitSize(optarg);
            break;
        case 'x':
            excludePattern.push_back(optarg);
            break;
        case 'r':
            preservePaths = true;
            break;
        case 'd':
            //Написать валидацию
            deleteAfter = true;
            break;
        case 'S':
            if (modeXArc != Compress)
            {
                std::cerr << "--self-extracting requires compress mode" << std::endl;
                err_code = UnsupportedType;
            }
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
            if (arg != 'h') {
                throw std::invalid_argument(std::string("Unknown option: -") +
                    static_cast<char>(arg));
            }
            break;
    }

    return err_code;
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

