
#include <locale>
#include "./modules/inc/XArc.h"

int main(int argc, char* argv[]) {
    try {
        Settings conf(argc, argv);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}