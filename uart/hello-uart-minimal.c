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
    CT_UART.PWREMU_MGMT_bit.URRST = 1; /* enable transmitter */
    CT_UART.PWREMU_MGMT_bit.UTRST = 1; /* enable receiver */
}

void uart_tx(char *s)
{
    uint8_t index = 0;
    do {
        uint8_t count = 0;

        /* while transmit registers aren't empty, do nothing */
        while (!CT_UART.LSR_bit.TEMT)
            ;

        while (s[index] != '\0' && count < FIFO_SIZE) {
            CT_UART.THR = s[index];
            index++;
            count++;
        }
    } while (s[index] != '\0');
}

void uart_rx(char *buf, uint32_t size) {
    uint32_t i;
    for (i = 0; i < size - 1; i++) {
        /* while data is not ready, do nothing */
        while (!CT_UART.LSR_bit.DR)
            ;

        buf[i] = CT_UART.RBR_bit.DATA;

        if (buf[i] == '\r')
            break;
    }
    buf[i] = '\0';
}

void main(void)
{
    char buf[BUF_SIZE] = { '\0' };
    uint8_t done = 0;

    uart_init();

    while (!done) {
        uart_tx("\n\rEnter some characters: ");
        uart_rx(buf, BUF_SIZE);
        uart_tx("\n\rYou entered: ");
        uart_tx(buf);
    }
}

/* required by PRU */
#pragma DATA_SECTION(pru_remoteproc_ResourceTable, ".resource_table")
#pragma RETAIN(pru_remoteproc_ResourceTable)
struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1];
} pru_remoteproc_ResourceTable = { 1, 0, 0, 0, 0 };
