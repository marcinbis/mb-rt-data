#include "../common/control.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>

#include <unistd.h>

#include <time.h>

#include <sys/mman.h>

#include "io.h"
#include "msg.h"
#include "controller.h"

/* Wykonaj krok automatu.
*/
void do_fsm_step()
{
	/* Pobierz istniejący obiekt sterowania */
	struct io_t * io = new_io();
	/* Obiekt do raportowania błędów */
	struct msg_t * msg = new_msg();

	/* Obiekt do odczytywania danych procesu */
	struct controller_t * c = new_controller();
	
	/* Stan - inicjowany przy pierwszym użyciu funkcji (static) */
	static int state = WAIT_FOR_TRIGGER;

	struct timespec timeout;

	switch (state)
	{
		case WAIT_FOR_TRIGGER:
			log("WAIT_FOR_TRIGGER");
			/* Proces właśnie został wystartowany oczekuje na włącznik nożny */
			msg->put(msg,WAIT_FOR_TRIGGER);
			io->wait_for_foot_switch(io);
			/* Proces został obudzony */
			state = PWM_ON;
			break;;
		case PWM_ON:
			log("PWM_ON");
			/* Proces właśnie został obudzony (przez trigger)
			   - pobierz ustawiony przez GUI czas impulsu
			   - włącz PWM
			   - idź spać na 900ms
			*/
			msg->put(msg,PWM_ON);

			/* Pobierz aktualny czas */
			clock_gettime(CLOCK_MONOTONIC, &timeout);
			/* Włącz pwm */
			io->pwm_set(io,c->get_pwm_work(c));
			#define NSEC_IN_SEC 1000000000l
			#define INTERVAL 900000000l

                        /* Zwiększ timeout o 900ms */
                        timeout.tv_nsec += INTERVAL;
                        if (timeout.tv_nsec >= NSEC_IN_SEC)
                        {
                                timeout.tv_nsec -= NSEC_IN_SEC;
                                timeout.tv_sec++;
                        }
			/* Idź spać na zadany czas */
			clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &timeout, NULL);

			state = PWM_OFF;

			break;
		case PWM_OFF:
			log("PWM_OFF");
			/* Wyłącz PWM */
			io->pwm_set(io,0);
			
			msg->put(msg,PWM_OFF);
			
			state = WAIT_FOR_TRIGGER;
			break;
	}
}
