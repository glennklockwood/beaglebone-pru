/*
 * This is a version of hello-uart-minimal with the uart functions factored out.
 */
#include <stdint.h>
#include <rsc_types.h> /* provides struct resource_table */
#include "uart.h" /* provides uart_init/uart_rx/uart_tx */

#define BUF_SIZE 40

void main(void)
{
    char buf[BUF_SIZE] = { '\0' };

    uart_init();

    while (1) {
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
