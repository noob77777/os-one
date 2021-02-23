#ifndef __ETHERNET_H
#define __ETHERNET_H

#include <types.h>
#include <lib/inet.h>
#include <drivers/nic_driver.h>
#include <drivers/display.h>

namespace ethernet
{
    void receive_handler(uint8_t *buffer, int size);
} // namespace ethernet

class Ethernet
{
    static NICDriver *nic;
    const static uint16_t TYPE_OFFSET = 0x05dc;
    const static int HEADER_LEN = 14;
    const static int FOOTER_LEN = 4;
    const static int MAX_SZ = 1518;
    // TODO: add an static array of handlers for internet layer protocols

    /*
     * returns the protocol index adjusted by TYPE_OFFSET [little endian]
     */
    static uint16_t protocol(uint8_t *buffer, int size);

    /*
     * returns the source physical address [little endian]
     */
    static uint64_t src_addr(uint8_t *buffer, int size);

    /*
     * returns the destination physcal address [little endian]
     */
    static uint64_t dest_addr(uint8_t *buffer, int size);

    /*
     * returns checksum field from incoming packet
     */
    static uint32_t checksum(uint8_t *buffer, int size);

    /*
     * Calculates the checksum in place and updates last 4 bytes
     */
    static void add_checksum(uint8_t *buffer, int size);

    /*
     * params: dest => [little endian]
     * generates part of the Ethernet II header inplace
     */
    static void add_dest_addr(uint8_t *buffer, int size, uint64_t dest);

    /*
     * params: [none]. Retrieves mac address from nic
     * generates part of the Ethernet II header inplace
     */
    static void add_src_addr(uint8_t *buffer, int size);

    /*
     * params: protocol => [little endian]
     * generates part of the Ethernet II header inplace
     */
    static void add_protocol(uint8_t *buffer, int size, uint16_t protocol);

public:
    /*
     * params: NICDriver *
     * Sets which network interface card to use. Useful in case of multiple drivers.
     */
    static void set_nic(NICDriver *nic);

    /*
     * Send Ethernet II packet over the network
     * params: dest [little endian]. Destination MAC address.
     *         protocol [little endian]. Internet layer protocol.
     */
    static void send(uint64_t dest, uint16_t protocol, uint8_t *buffer, int size);

    /*
     * Receive interrupt handler. Calls appropriate internet layer handler.
     */
    static void receive(uint8_t *buffer, int size);
};

#endif
