#include <stdio.h>
#include <time.h>  /* clock_nanosleep */
#include <sched.h> /* sched_setscheduler */
#include <string.h> /* memset */
#include <sys/mman.h> /* mlockall */
#include <fcntl.h> /* stałe: O_* */
#include <sys/stat.h>  /* sałe S_* */
#include <unistd.h> /* ftruncate */
#include <errno.h>  /* EINTR */
#include <stdlib.h> /* malloc */

#include "../common/control.h" /* Struktury kontrolne procesu */
#include "io.h" /* Operacje sterowania */
#include "msg.h" /* Raportowanie błędów przez kolejkę komunikatów */
#include "controller.h" /* Dostęp do parametrów procesu w pamięci współdzielonej */

/* Priorytet procesu w klasie SCHED_FIFO */
#define MY_PRIORITY 52

extern void do_fsm_step();

int main(int argc, char ** argv)
{
	struct sched_param sp; /* Priorytet procesu */

	/* Zainicjuj obiekty. Ponieważ jest to pierwsze użycie - wykonają sie wszystkie funcje malloc i pliki zostaną otwarte */
	struct controller_t * c = new_controller();
	struct msg_t * msg = new_msg();
	struct io_t * io = new_io();

	/* Ustawianie parametrów planisty */
	sp.sched_priority = MY_PRIORITY;
	if (sched_setscheduler(0, SCHED_FIFO, &sp) == -1)
        {
                log("sched_setscheduler() failed");
        }

	/* Blokowanie pamięci procesu */
	if (mlockall( MCL_CURRENT|MCL_FUTURE ) == -1)
	{
		log("mlockall() failed");
	}

	while (1)
	{
		/* Wykonaj krok automatu */
		do_fsm_step();
	}
	return 0;
}
