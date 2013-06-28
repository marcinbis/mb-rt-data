#include "../common/control.h" /* Struktury kontrolne procesu */

struct controller_t {
	/* Operacje modelu procesu */
	int (* get_pwm_work) (struct controller_t *);

	/* Dane prywatne (w pamięci współdzielonej) */

	/* Pamięć współdzielona - deskryptor pliku */
	int shm_fd;
	/* Zawartość pamięci współdzielonej */
	volatile shm_content_t * shm_content;
};

struct controller_t * new_controller(void);
