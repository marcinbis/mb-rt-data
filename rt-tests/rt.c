#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


/* Priorytet procesu zostanie ustawiony na 49.
   Jądro z nałożoną łatą PRREMPT_RT ustawia 50 dla zadań
   obsługujących przerwania (więcej o PREEMPT_RT w dalszej części */
#define MY_PRIORITY (99)

/* Maksymalny rozmiar stosu, dostęp do którego będzie gwarantowany
   w czasie deterministycznym (bez powodowania błędu strony) */
#define MAX_SAFE_STACK (8*1024)

/* Ilość nanosekund w sekundzie - stała używana do testu. */
#define NSEC_PER_SEC    (1000000000)

/* Prealokacja stosu. */
void stack_prefault(void) {
        unsigned char dummy[MAX_SAFE_STACK];

        memset(dummy, 0, MAX_SAFE_STACK);
        return;
}

/* Funkcja, która coś robi, na przykład generuje sygnał kwadratowy 
   na jednym z PIN-ów GPIO */
int f;
int counter = 0;

void out() {
	counter = (counter + 1) % 2;
	if (counter == 0)
        	write(f,"0",1);
	else
		write(f,"1",1);
}

int main(int argc, char* argv[])
{
        struct timespec t;
        struct sched_param param;
        int interval = 30518; /* 50000ns = 50us */

	/* Obsługa parametrów */
	if (argc > 1) 
		interval = strtol(argv[1],NULL,10);
	
	printf("Interval set to %dns\n",interval);

        /* Proces ustawia się jako zadanie czasu rzeczywistego */
        param.sched_priority = MY_PRIORITY;
        if(sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
                perror("sched_setscheduler failed");
                exit(-1);
        }

        /* Blokowanie całej pamięci wirtualnej procesu */
        if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
                perror("mlockall failed");
                exit(-2);
        }

        /* Prealokacja stosu */
        stack_prefault();

        /* Przygotowanie sterowników dla funcji out() */
        f = open("/dev/inout",O_WRONLY);

        /* Pobierz aktualny czas */
        clock_gettime(CLOCK_MONOTONIC ,&t);

        /* ustaw alarm za sekundę */
        t.tv_sec++;

        while(1) {
                /* czekaj do wystąpienia alarmu */
                clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

                /* wykonaj operację */
                out();

                /* ustaw alarm (za 50us) */
                t.tv_nsec += interval;

                /* Struktura timescpec zawiera licznik nanosekund i sekund
                   jeżeli licznik naosekund się przepełnia należy odjąć od
                   niego ilość odpowiadającą jednej sekundzie i dodać jeden
                   do licznuka sekund */
                while (t.tv_nsec >= NSEC_PER_SEC) {
                       t.tv_nsec -= NSEC_PER_SEC;
                       t.tv_sec++;
                }
   }
}

