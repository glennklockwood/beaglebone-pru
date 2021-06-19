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

void uart_init(void)
{
    /* According to the AM335x and AMIC110 Sitara Technical Reference Manual Sec. 4.4.4.2.7:

    The following steps are required to initialize the UART:

    1. Perform the necessary device pin multiplexing setup (see your
       device-specific data manual).
    2. Set the desired baud rate by writing the appropriate clock divisor values
       to the divisor latch registers (DLL and DLH).
    3. If the FIFOs will be used, select the desired trigger level and enable
       the FIFOs by writing the appropriate values to the FIFO control register
       (FCR). The FIFOEN bit in FCR must be set first, before the other bits in
       FCR are configured.
    4. Choose the desired protocol settings by writing the appropriate values to
       the line control register (LCR).
    5. If autoflow control is desired, write appropriate values to the modem
       control register (MCR). Note that all UARTs do not support autoflow
       control; see your device-specific data manual for supported features.
    6. Choose the desired response to emulation suspend events by configuring
       the FREE bit, and enable the UART by setting the UTRST and URRST bits in
       the power and emulation management register (PWREMU_MGMT).
    */

    /* use 115200 baud (192MHz / 104 / 16x = 115200) */
    CT_UART.DLL = 104; /* divisor latch low */
    CT_UART.DLH = 0; /* divisor latch high - called DLM in TL16C550C data sheet */

    /* Mode Definition Register */
    CT_UART.MDR_bit.OSM_SEL = 0; /* use 16x oversampling */

    /* Interrupt Enable Register */
    CT_UART.IER_bit.ERBI = 0; /* enable received data available interrupt */
    CT_UART.IER_bit.ETBEI = 0; /* enable transmitter holding register empty interrupt */
    CT_UART.IER_bit.ELSI = 0; /* enable receiver line status interrupt */
    CT_UART.IER_bit.EDSSI = 0; /* enable modem status interrupt */

    /* FIFO Control Register */
    /* not sure this is set up correctly; the six-step initialization process
       above indicates that the trigger level must be set for the FIFO, but if
       we aren't using interrupts, does this matter?  what happens if the FIFO
       fills up?
     */
    CT_UART.FCR_bit.FIFOEN = 1; /* FIFO enable */
    CT_UART.FCR_bit.RXCLR = 1; /* receiver FIFO reset */
    CT_UART.FCR_bit.TXCLR = 1; /* transmitter FIFO reset */
    CT_UART.FCR_bit.DMAMODE1 = 0; /* DMA mode select - set to change RXRDY and TXRDY from 0 to 1 */
    CT_UART.FCR_bit.RXFIFTL = 0; /* receiver trigger - trigger at 8 bytes */

    /* Line Control Register */
    CT_UART.LCR_bit.WLS = 3; /* word length select; 0b11 = 8 bits */
    CT_UART.LCR_bit.STB = 0; /* number of stop bits; 0 = 1 stop bit */
    CT_UART.LCR_bit.PEN = 0; /* parity enable */
    CT_UART.LCR_bit.EPS = 0; /* even parity select */
    CT_UART.LCR_bit.SP = 0; /* stick parity */
    CT_UART.LCR_bit.BC = 0; /* break control */
    CT_UART.LCR_bit.DLAB = 0; /* divisor latch access */

    /* Modem Control Register */
    CT_UART.MCR_bit.RTS = 0; /* request to send */
    CT_UART.MCR_bit.OUT1 = 0; /* user-designated output signal */
    CT_UART.MCR_bit.OUT2 = 0; /* user-designated output signal */
    CT_UART.MCR_bit.LOOP = 0; /* loop back for diagnistic testing */
    CT_UART.MCR_bit.AFE = 0; /* autoflow control enable */

    /* Power and Emulation Management Register */
    CT_UART.PWREMU_MGMT_bit.FREE = 1; /* do not halt on halt/break */
    CT_UART.PWREMU_MGMT_bit.URRST = 1; /* enable transmitter */
    CT_UART.PWREMU_MGMT_bit.UTRST = 1; /* enable receiver */
}

void uart_put(char *s)
{
    uint8_t index = 0;

    do {
        uint8_t count = 0;

        while (!CT_UART.LSR_bit.TEMT); /* TEMT = THR and TSR empty? */

        while (s[index] != '\0' && count < MAX_CHARS) {
            CT_UART.THR = s[index];
            index++;
            count++;
        }
    } while (s[index] != '\0');

    while (!CT_UART.LSR_bit.TEMT); /* TEMT = THR and TSR empty? */
}

void main(void)
{
    char buf[BUF_SIZE] = { '\0' };
    uint8_t done = 0;

    uart_init();

    while (!done) {
        uart_put("Enter some characters: ");

        for (uint32_t i = 0; i < BUF_SIZE - 1; i++) {
            while (!CT_UART.LSR_bit.DR); /* data ready bit */

            buf[i] = CT_UART.RBR_bit.DATA;

            if (buf[i] == '\r') {
                buf[i+1] = '\0';
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

    CT_UART.PWREMU_MGMT_bit.URRST = 0; /* disable transmitter */
    CT_UART.PWREMU_MGMT_bit.UTRST = 0; /* disable receiver */

    __halt();
}
