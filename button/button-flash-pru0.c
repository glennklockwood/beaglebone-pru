/*
 * button-flash-pru0.c
 *
 * When a button connected to P8_15 is pressed, flash an LED connected to
 * P9_31 on and off every 0.1 seconds.  Hook one side of the button up to 3.3V
 * and the other to P8_15 (with a resistor to be safe), and hook P9_31 to an LED
 * through a resistor to ground.  Don't forget to `config-pin p8_15 pruin` and
 * `config-pin p9_31 pruout`.
 *
 * Glenn K. Lockwood, June 2021
 *
 */
#include <stdint.h>
#include <rsc_types.h>  /* provides struct resource_table */

#define CYCLES_PER_SECOND 200000000 /* PRU has 200 MHz clock */

#define P9_31 (1 << 0)
#define P8_15 (1 << 15)
#define SET_BIT(reg, bit) (reg) |= (bit)
#define REMOVE_BIT(reg, bit) (reg) &= ~(bit)
#define IS_SET(reg, bit) (reg) & (bit)

volatile register uint32_t __R30; /* output register for PRU */
volatile register uint32_t __R31; /* input register for PRU */

void main(void) {
    uint8_t led_state = 0;
    REMOVE_BIT(__R30, P9_31); /* initial LED state is off */

    while (1) {
        /* if button is pressed */
        if (IS_SET(__R31, P8_15)) {
            /* alternate between LED on and off */
            if (led_state) {
                REMOVE_BIT(__R30, P9_31);
                led_state = 0;
            }
            else {
                SET_BIT(__R30, P9_31);
                led_state = 1;
            }
        }
        else {
            /* if button not pressed, make sure LED is off */
            REMOVE_BIT(__R30, P9_31);
        }
        /* repeat this check every 0.1 seconds */
        __delay_cycles(CYCLES_PER_SECOND / 10);
    }
}

/* required by PRU */
#pragma DATA_SECTION(pru_remoteproc_ResourceTable, ".resource_table")
#pragma RETAIN(pru_remoteproc_ResourceTable)
struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1];
} pru_remoteproc_ResourceTable = { 1, 0, 0, 0, 0 };
