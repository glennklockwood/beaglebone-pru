#define GPIO0   0x44E07000
#define GPIO1   0x4804C000
#define GPIO2   0x481AC000
#define GPIO3   0x481AE000

/* GPIO0 */
#define P9_11   (1<<30)
#define P9_13   (1<<31)
#define P9_17   (1<<5)
#define P9_18   (1<<4)
#define P9_19   (1<<13)
#define P9_20   (1<<12)
#define P9_21   (1<<3)
#define P9_22   (1<<2)
#define P9_24   (1<<15)
#define P9_26   (1<<14)

/* GPIO1 */
#define LED_USER0   (1<<21)
#define LED_USER1   (1<<22)
#define LED_USER2   (1<<23)
#define LED_USER3   (1<<24)
#define P9_12   (1<<28)
#define P9_14   (1<<18)
#define P9_15   (1<<16)
#define P9_16   (1<<19)
#define P9_23   (1<<17)

/* GPIO3 */
#define P9_25   (1<<21)
#define P9_27   (1<<19)
#define P9_28   (1<<17)
#define P9_29   (1<<15)
#define P9_30   (1<<16)
#define P9_31   (1<<14)
#define P9_41   (1<<20)

// Shared memory
#define AM33XX_DATARAM0_PHYS_BASE        0x4a300000
#define AM33XX_DATARAM1_PHYS_BASE        0x4a302000
#define AM33XX_PRUSS_SHAREDRAM_BASE        0x4a310000

// /4 to convert from byte address to word address
#define GPIO_CLEARDATAOUT    0x190/4     // Write 1 here to set a given bit    
#define GPIO_SETDATAOUT     0x194/4     // A 1 here clears the corresponding bit
#define GPIO_DATAOUT        0x138/4     // For reading the GPIO registers
