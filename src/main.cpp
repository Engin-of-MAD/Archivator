
#include <locale>
#include "./modules/inc/XArc.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    Settings conf(argc, argv);
    return 0;
}
