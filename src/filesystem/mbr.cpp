#include <filesystem/mbr.h>

MasterBootRecord::MasterBootRecord()
{
    for (int i = 0; i < 440; i++)
        bootloader[i] = 0;
    signature = 7;
    unused = 0;
    for (int i = 0; i < 64; i++)
        primary_partition[i] = 0;
    magicnumber = 0x55AA;
}
