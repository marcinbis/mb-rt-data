#ifndef CONTROL_H
#define CONTROL_H

#include <time.h> /* Definicja struct timespec */

/* Stan WAIT_FOR_TIGGER zawsze ma przypisany numer 0, dzięki temu wyzerowana struktura kontrolna odpowiada automatowi zatrzymanemu. */
typedef enum {
	WAIT_FOR_TRIGGER = 0, PWM_ON, PWM_OFF
} STATES;

/* Nazwa pliku pamięci współdzielonej */
#define SHM_NAME "/laser"

/* zawartość pamięci współdzielonej
   proces przechowuje w niej:
   - współczynnik nasycenia impulsu PWM (0-100%)
*/
typedef struct {
	int pwm_work;
} shm_content_t;

#define SHM_SIZE (sizeof(shm_content_t))

/* Kolejka komunikatów używana do raportowania błędów i informacji o stanie automatu*/
#define MQ_FILE "/laser"
#define MQ_MAXMSGS 1024

/* Struktura opisująca komunikat błędu wysyłany przez kolejkę */
typedef struct {
	STATES msg_id;
	struct timespec msg_timestamp;
} message_t;

/* Definicje do debugowania. Są w tym pliku, ponieważ jest on załączany praktycznie wszędzie */
#ifdef DEBUG
//#warning "DEBUG BUILD"
#define log(_fmt, ...) printf("[%s:%d:%s()]: " _fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define log(_fmt, ...)
#endif

#endif /* CONTROL_H */
