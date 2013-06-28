#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

#include <string.h>

#include "../common/control.h"
#include "io.h"
#include "msg.h"

#include "pwm.h"

/* Funkcja jest prywatna, dlatego nie trafiła do io.h */
void new_io_tail(struct io_t * io);

/* Alokacja - singleton */
struct io_t * new_io(void)
{
	static struct io_t * io = NULL;
	if (io == NULL)
	{
		io = malloc(sizeof(struct io_t));
		new_io_tail(io);
		log("io_t: inicialized");
	}
	return io;
}

/* Podstawowe funkcje modelu */

/* Oczekuje na sygnał z włącznika nożnego
 * Ta funkcja powinna być blokująca, tj. zawiesza wykonanie procesu
 * Dla testów - poczekaj 5s
 */
void wait_for_foot_switch(struct io_t * io)
{
	log("wait_for_foot_switch");
#ifdef MODEL
	sleep(5);
#endif
	return;
}

void pwm_set(struct io_t * io, int pwm_work)
{
	/* Tu dodaj właściwe włączanie PWM-a */
	set_pulse(pwm_work);
#ifdef MODEL
	log("pwm_work set to: %d", pwm_work);
#endif
}

/* Ciąg dalszy inicjalizacji - zależny od zastosowania
 * W tej fukcji otwierane są wszystkie niezbędne pliki, a ich deskryptory muszą być przypisane do atrybutów obiektu io */
void new_io_tail(struct io_t * io)
{
	io->wait_for_foot_switch = &wait_for_foot_switch;
	io->pwm_set = &pwm_set;

	init_hardware();
}

