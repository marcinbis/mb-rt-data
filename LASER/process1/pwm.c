#include "../common/control.h"
#include "pwm.h"

#include <stdio.h>

void init_hardware(void)
{
	log("Hardware initialized.\n");
}

void set_pulse(int val)
{
	log("Pulse set to: %d\n", val);
}
