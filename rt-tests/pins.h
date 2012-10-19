#define IMX53 1
//#define BEAGLEBOARD_XM 1
//#define KT_SBC_SAM9 1
//#define ATMEL 1
//#define DEVKIT 1

#ifdef ATMEL
#define INPUT_PIN AT91_PIN_PB19
#define OUTPUT_PIN AT91_PIN_PB20
/* led */
//#define OUTPUT_PIN AT91_PIN_PE17
#endif

#ifdef DEVKIT
#define INPUT_PIN 149
#define OUTPUT_PIN 150
#endif

#ifdef BEAGLEBOARD_XM
/* This pind needs to be configured in kernel: omap_mux_init_gpio(139, OMAP_PIN_INPUT_PULLUP); */
#define INPUT_PIN 139
#define OUTPUT_PIN 138
#endif

#ifdef KT_SBC_SAM9
#define INPUT_PIN AT91_PIN_PC9
#define OUTPUT_PIN AT91_PIN_PC10
#endif

#ifdef IMX53
#include <mach/hardware.h>
#define INPUT_PIN IMX_GPIO_NR(3, 31)
#define OUTPUT_PIN IMX_GPIO_NR(1, 0)
#endif
