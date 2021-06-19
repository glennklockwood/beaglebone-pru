#include <stdint.h>
#include <rsc_types.h> /* provides struct resource_table */
#include <pru_uart.h> /* provides CT_UART */

#define FIFO_SIZE 16
#define BUF_SIZE 40

void uart_init(void)
{
    /* use 115200 baud (192 MHz / 104 / 16 = 115200) */
    CT_UART.DLL = 104; /* divisor latch low */
    CT_UART.DLH = 0;   /* divisor latch high - aka DLM*/
    CT_UART.MDR_bit.OSM_SEL = 0; /* 16x oversampling */
    CT_UART.FCR_bit.FIFOEN = 1; /* FIFO enable */
    CT_UART.FCR_bit.RXCLR = 1; /* receiver FIFO reset */
    CT_UART.FCR_bit.TXCLR = 1; /* transmitter FIFO reset */
    CT_UART.LCR_bit.WLS = 3; /* word length select; 0b11 = 8 bits */
//  CT_UART.PWREMU_MGMT_bit.FREE = 1; /* do not halt on halt/break */
    CT_UART.PWREMU_MGMT_bit.URRST = 1; /* enable transmitter */
    CT_UART.PWREMU_MGMT_bit.UTRST = 1; /* enable receiver */
}

void uart_put(char *s)
{
    uint8_t index = 0;
    do {
        uint8_t count = 0;

        while (!CT_UART.LSR_bit.TEMT); /* TEMT = THR and TSR empty? */

        while (s[index] != '\0' && count < FIFO_SIZE) {
            CT_UART.THR = s[index];
            index++;
            count++;
        }
    } while (s[index] != '\0');
}

void main(void)
{
    char buf[BUF_SIZE] = { '\0' };
    uint8_t done = 0;

    uart_init();

    while (!done) {
        uart_put("\n\rEnter some characters: ");

        for (uint32_t i = 0; i < BUF_SIZE - 1; i++) {
            while (!CT_UART.LSR_bit.DR);

            buf[i] = CT_UART.RBR_bit.DATA;

            if (buf[i] == '\r') {
                buf[i+1] = '\0';
                break;
            }
        }

        uart_put("\n\rYou entered: ");
        uart_put(buf);
    }
}

/* required by PRU */
#pragma DATA_SECTION(pru_remoteproc_ResourceTable, ".resource_table")
#pragma RETAIN(pru_remoteproc_ResourceTable)
struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1];
} pru_remoteproc_ResourceTable = { 1, 0, 0, 0, 0 };
