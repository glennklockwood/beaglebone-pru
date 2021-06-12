#include <stdint.h>
#include <rsc_types.h>  /* provides struct resource_table */

#define CYCLES_PER_SECOND 200000000 /* PRU has 200 MHz clock */

volatile register uint32_t __R30; /* output register for PRU */

/* R30 at 0x1 = pru1_pru0_pru_r30_0 = pin 31 = ball A13 = P9_31 */
#define P9_31 (1 << 0)
void main(void) {
    while (1) {
        __R30 |= P9_31; /* set first bit of register 30 to 1 */
        __delay_cycles(CYCLES_PER_SECOND / 4); /* wait 0.5 seconds */
        __R30 &= ~P9_31; /* set first bit of register 30 to 0 */
        __delay_cycles(CYCLES_PER_SECOND / 4); /* wait 0.5 seconds */
    }
}

#pragma DATA_SECTION(pru_remoteproc_ResourceTable, ".resource_table")
#pragma RETAIN(pru_remoteproc_ResourceTable)
struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1];
} pru_remoteproc_ResourceTable = { 1, 0, 0, 0, 0 };
