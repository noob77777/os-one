#include <drivers/ata.h>

ATA::ATA(bool master, uint16_t port_base) : data_port(port_base),
                                            error_port(port_base + 0x1),
                                            sector_count_port(port_base + 0x2),
                                            lba_low_port(port_base + 0x3),
                                            lba_mid_port(port_base + 0x4),
                                            lba_hi_port(port_base + 0x5),
                                            device_port(port_base + 0x6),
                                            command_port(port_base + 0x7),
                                            control_port(port_base + 0x206)
{
    this->master = master;
}

uint8_t ATA::identify()
{
    device_port.write(master ? 0xA0 : 0xB0);
    control_port.write(0);

    device_port.write(0xA0);
    uint8_t status = command_port.read();
    if (status == 0xFF)
        return 0x01;

    device_port.write(master ? 0xA0 : 0xB0);
    sector_count_port.write(0);
    lba_low_port.write(0);
    lba_mid_port.write(0);
    lba_hi_port.write(0);
    command_port.write(0xEC);

    status = command_port.read();
    if (status == 0x00)
        return 0x02;

    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = command_port.read();

    if (status & 0x01)
        return 0x03;

    for (int i = 0; i < 256; i++)
    {
        uint16_t data = data_port.read();
        char text[3] = "  ";
        text[0] = (data >> 8) & 0xFF;
        text[1] = data & 0xFF;
        kprintf(text);
    }
    kprintf("\n");

    return 0;
}

uint8_t ATA::read(uint32_t sector_num, uint8_t *data, int count)
{
    if (sector_num > 0x0FFFFFFF)
        return 0x01;
    if (count > 512)
        return 0x02;

    device_port.write((master ? 0xE0 : 0xF0) | ((sector_num & 0x0F000000) >> 24));
    error_port.write(0);
    sector_count_port.write(1);
    lba_low_port.write(sector_num & 0x000000FF);
    lba_mid_port.write((sector_num & 0x0000FF00) >> 8);
    lba_hi_port.write((sector_num & 0x00FF0000) >> 16);
    command_port.write(0x20);

    uint8_t status = command_port.read();
    if (status == 0x00)
        return 0x03;

    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = command_port.read();

    if (status & 0x01)
        return 0x04;

    for (int i = 0; i < count; i += 2)
    {
        uint16_t rdata = data_port.read();
        uint16_t *dptr = (uint16_t *)&data[i];
        *dptr = rdata;
    }

    for (int i = count + (count % 2); i < 512; i += 2)
        data_port.read();

    return 0;
}

uint8_t ATA::write(uint32_t sector_num, uint8_t *data, uint32_t count)
{
    if (sector_num > 0x0FFFFFFF)
        return 0x01;
    if (count > 512)
        return 0x02;

    device_port.write((master ? 0xE0 : 0xF0) | ((sector_num & 0x0F000000) >> 24));
    error_port.write(0);
    sector_count_port.write(1);
    lba_low_port.write(sector_num & 0x000000FF);
    lba_mid_port.write((sector_num & 0x0000FF00) >> 8);
    lba_hi_port.write((sector_num & 0x00FF0000) >> 16);
    command_port.write(0x30);

    uint8_t status = command_port.read();
    if (status == 0x00)
        return 0x03;

    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = command_port.read();

    if (status & 0x01)
        return 0x04;

    for (int i = 0; i < count; i += 2)
    {
        uint16_t wdata = data[i];
        if (i + 1 < count)
            wdata |= ((uint16_t)data[i + 1]) << 8;
        data_port.write(wdata);
    }

    for (int i = count + (count % 2); i < 512; i += 2)
        data_port.write(0x0000);

    return 0;
}

uint8_t ATA::flush()
{
    device_port.write(master ? 0xE0 : 0xF0);
    command_port.write(0xE7);

    uint8_t status = command_port.read();
    if (status == 0x00)
        return 0x01;

    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = command_port.read();

    if (status & 0x01)
        return 0x02;

    return 0;
}

void ATA::ata_check()
{
    char test[512] = "Disk I/O Test [PASSED]";
    char read[512];

    ATA ataDisk(true, 0x01F0);
    ataDisk.identify();

    uint8_t status = ataDisk.write(8, (uint8_t *)test, 512);
    if (status)
    {
        kprintf_hex8(status);
        kprintf("\n");
    }
    status = ataDisk.flush();
    if (status)
    {
        kprintf_hex8(status);
        kprintf("\n");
    }
    status = ataDisk.read(8, (uint8_t *)read, 512);
    if (status)
    {
        kprintf_hex8(status);
        kprintf("\n");
    }
    kprintf(read);
    kprintf("\n");
}
