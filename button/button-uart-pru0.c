/*
 * button-uart-pru0.c
 *
 * Reports whether a button hooked up to P8_15 is pressed or not every half
 * second via the UART (connect at 115200 baud).  Hook one side of the button
 * up to 3.3V and the other to P8_15 (with a resistor to be safe), and don't
 * forget to `config-pin P8_15 pruin`.
 *
 * Glenn K. Lockwood, June 2021
 *
 */
#include <stdint.h>
#include <rsc_types.h>  /* provides struct resource_table */
#include <pru_uart.h> /* provides CT_UART */

#define CYCLES_PER_SECOND 200000000 /* PRU has 200 MHz clock */

/* handy for manipulating individual bits */
#define SET_BIT(reg, bit) (reg) |= (bit)
#define REMOVE_BIT(reg, bit) (reg) &= ~(bit)
#define IS_SET(reg, bit) (reg) & (bit)

/* function prototypes for UART I/O functions */
void uart_init(void);
void uart_tx(char *s);

/* define input and output GPIO bits within __R30 and __R31 */
#define P9_31 (1 << 0)
#define P8_15 (1 << 15)

volatile register uint32_t __R30; /* output register for PRU */
volatile register uint32_t __R31; /* input register for PRU */

void main(void) {
    uart_tx("PRU has started!\n\r");
    while (1) {
        if (IS_SET(__R31, P8_15)) /* if button is pressed */
            uart_tx("Button is currently pressed!\n\r");
        else
            uart_tx("Button is not pressed.\n\r"); 
        __delay_cycles(CYCLES_PER_SECOND / 2);
    }
}

/*
 * Code to write to the UART
 */
void uart_init(void)
{
    /* use 115200 baud (192 MHz / 104 / 16 = 115200) */
    CT_UART.DLL = 104; /* divisor latch low */
    CT_UART.DLH = 0;   /* divisor latch high - aka DLM*/
    CT_UART.MDR_bit.OSM_SEL = 0; /* use 16x oversampling */
    CT_UART.FCR_bit.FIFOEN = 1; /* FIFO enable */
    CT_UART.FCR_bit.RXCLR = 1; /* receiver FIFO reset */
    CT_UART.FCR_bit.TXCLR = 1; /* transmitter FIFO reset */
    CT_UART.LCR_bit.WLS = 3; /* word length select; 0b11 = 8 bits */
    CT_UART.PWREMU_MGMT_bit.URRST = 1; /* enable transmitter */
    CT_UART.PWREMU_MGMT_bit.UTRST = 1; /* enable receiver */
}

#define UART_FIFO_SIZE 16
void uart_tx(char *s)
{
    uint8_t index = 0;
    do {
        uint8_t count = 0;

        /* while transmit registers aren't empty, do nothing */
        while (!CT_UART.LSR_bit.TEMT)
            ;

        while (s[index] != '\0' && count < UART_FIFO_SIZE) {
            CT_UART.THR = s[index];
            index++;
            count++;
        }
    } while (s[index] != '\0');
}

/* 
 * Resource table - required by the PRU loader
 */
#pragma DATA_SECTION(pru_remoteproc_ResourceTable, ".resource_table")
#pragma RETAIN(pru_remoteproc_ResourceTable)
struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1];
} pru_remoteproc_ResourceTable = { 1, 0, 0, 0, 0 };


