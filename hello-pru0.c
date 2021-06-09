#include <stdint.h>
#include <stddef.h>
#include <rsc_types.h>
#include <pru_cfg.h>

#define GPIO1 0x4804C000    /* am335x GPIO address */

/* USR LED bit positions */
#define USR0 (1 << 21)
#define USR1 (1 << 22)
#define USR2 (1 << 23)
#define USR3 (1 << 24)

/* /4 to convert from byte address to word address */
#define GPIO_LOW    (0x190 / 4)
#define GPIO_HIGH   (0x194 / 4)

struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1];
};

volatile register unsigned int __R30;
volatile register unsigned int __R31;

void main(void) {
    uint32_t *gpio1 = (uint32_t *)GPIO1;
    
    /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    while (1) {
        gpio1[GPIO_HIGH] = USR3;    // The the USR3 LED on
        __delay_cycles(500000000/5);        // Wait 1/2 second
        gpio1[GPIO_LOW] = USR3;
        __delay_cycles(500000000/5); 
    }
}

#pragma DATA_SECTION(pru_remoteproc_ResourceTable, ".resource_table")
#pragma RETAIN(pru_remoteproc_ResourceTable)
struct my_resource_table pru_remoteproc_ResourceTable = {
    1,      /* we're the first version that implements this */
    0,      /* number of entries in the table */
    0, 0,   /* reserved, must be zero */
    0,      /* offset[0] */
};
