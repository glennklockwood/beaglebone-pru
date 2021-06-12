#include <stdint.h>
#include <rsc_types.h>  /* provides struct resource_table */

#define GPIO1 0x4804C000        /* am335x GPIO address */

#define LED_USR3 (1 << 24)      /* USR LED offset */

/* /4 to convert from byte address to word address */
#define GPIO_LOW    (0x190 / 4)
#define GPIO_HIGH   (0x194 / 4)

#define CYCLES_PER_SECOND 200000000 /* PRU has 200 MHz clock */

void main(void) {
    uint32_t *gpio1 = (uint32_t *)GPIO1;
    
    while (1) {
        gpio1[GPIO_HIGH] = LED_USR3;
        __delay_cycles(CYCLES_PER_SECOND / 2); /* wait 0.5 seconds */
        gpio1[GPIO_LOW] = LED_USR3;
        __delay_cycles(CYCLES_PER_SECOND / 2); /* wait 0.5 seconds */
    }
}

/* without the following resource table,starting the PRU will generate these
 * errors in syslog:
 *
 * kernel: remoteproc remoteproc1: header-less resource table
 * kernel: remoteproc remoteproc1: Boot failed: -22
 */
#pragma DATA_SECTION(pru_remoteproc_ResourceTable, ".resource_table")
#pragma RETAIN(pru_remoteproc_ResourceTable)
struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1];
} pru_remoteproc_ResourceTable = { 1, 0, 0, 0, 0 };;
