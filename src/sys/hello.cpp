#include <sys/hello.h>

Hello::Hello() : Program("hello") { ; }

int Hello::main(int argc, char *const argv[], char *const env[])
{
    kprintf("Hello!!!\n");
    return 0;
}
