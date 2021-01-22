#include <sys/system_software.h>

Clear::Clear() : Program("clear") { ; }

int Clear::main(int argc, char *const argv[], char *const env[])
{
    display::clear();
    return 0;
}
