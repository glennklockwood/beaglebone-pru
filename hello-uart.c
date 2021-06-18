/* Notes from gkl on June 16, 2021:
 *
 * Code taken from :
 *
 *     https://beagleboard.org/static/prucookbook/#debug_uart2
 *
 * Use USB to TTL cable (Adafruit part 954) and connect
 *
 * 1. Green to P9_17
 * 2. White to P9_18
 * 3. Black to ground
 *
 * Then use `screen /dev/tty.usbserial-0001 $((12*9600))` on macOS.
 *
 * Full UART reference is in "AM335x and AMIC110 Sitara(tm) Processors
 * Technical Reference Manual v2.2," Section 4.4.4 (Universal Asynchronous
 * Receiver/Transmitter (UART)) pp241-253.
 *
 */
#include <stdint.h>
#include <pru_uart.h>
#include "resource_table_empty.h"

#define MAX_CHARS 16
#define BUF_SIZE 40

void uart_put(char *s)
{
    uint8_t index = 0;

    do {
        uint8_t count = 0;

        /* Wait until the TX FIFO and the TX SR are completely empty */
        while (!CT_UART.LSR_bit.TEMT);

        while (s[index] != '\0' && count < MAX_CHARS) {
            CT_UART.THR = s[index];
            index++;
            count++;
        }
    } while (s[index] != '\0');

    /* Wait until the TX FIFO and the TX SR are completely empty */
    while (!CT_UART.LSR_bit.TEMT);

}

void uart_init(void)
{
    /* Set up UART to function at 115200 baud - DLL divisor is 104 at 16x oversample
     * 192MHz / 104 / 16 = ~115200 */
    CT_UART.DLL = 104;
    CT_UART.DLH = 0;
    CT_UART.MDR_bit.OSM_SEL = 0x0;

    /* Enable Interrupts in UART module. This allows the main thread to poll for
     * Receive Data Available and Transmit Holding Register Empty */
    CT_UART.IER = 0x7;

    /* If FIFOs are to be used, select desired trigger level and enable
     * FIFOs by writing to FCR. FIFOEN bit in FCR must be set first before
     * other bits are configured */
    /* Enable FIFOs for now at 1-byte, and flush them */
    CT_UART.FCR = (0x80) | (0x8) | (0x4) | (0x2) | (0x01); // 8-byte RX FIFO trigger

    /* Choose desired protocol settings by writing to LCR */
    /* 8-bit word, 1 stop bit, no parity, no break control and no divisor latch */
    CT_UART.LCR = 3;

    /* If flow control is desired write appropriate values to MCR. */
    /* No flow control for now, but enable loopback for test */
    CT_UART.MCR = 0x00;

    /* Choose desired response to emulation suspend events by configuring
     * FREE bit and enable UART by setting UTRST and URRST in PWREMU_MGMT */
    /* Allow UART to run free, enable UART TX/RX */
    CT_UART.PWREMU_MGMT_bit.FREE = 0x1;
    CT_UART.PWREMU_MGMT_bit.URRST = 0x1;
    CT_UART.PWREMU_MGMT_bit.UTRST = 0x1;

    /* Turn off RTS and CTS functionality */
    CT_UART.MCR_bit.AFE = 0x0;
    CT_UART.MCR_bit.RTS = 0x0;


}

void main(void)
{
    char buf[BUF_SIZE] = { NULL };
    uint8_t done = 0;

    uart_init();

    while (!done) {
        uart_put("Enter some characters: ");

        for (uint32_t i = 0; i < BUF_SIZE - 1; i++) {

            while (!CT_UART.LSR_bit.DR);

            buf[i] = CT_UART.RBR_bit.DATA;

            if (buf[i] == '\r') {
                buf[i+1] = NULL;
                break;
            }
            else if (buf[i] == 'Z') {
                done = 1;
                break;
            }
        }

        uart_put("\n\rYou entered: ");
        uart_put(buf);
        uart_put("\n\r");
    }

    uart_put("\n\rHalting\n\r");
    CT_UART.PWREMU_MGMT = 0x0;
    __halt();
}
