#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h> /* mmap */

#include <stdio.h>

#include <unistd.h>
#include <string.h>

#include <stdlib.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "controller.h"

int controller_get_pwm_work(struct controller_t * c)
{
	return c->shm_content->pwm_work;
}

struct controller_t * new_controller(void)
{
	static struct controller_t * c = NULL;
	if (c == NULL)
	{
		c = malloc(sizeof(struct controller_t));
		
		c->get_pwm_work = &controller_get_pwm_work;

		/* Otwarcie segmentu pamięci współdzielonej */
		c->shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		ftruncate(c->shm_fd, SHM_SIZE);
		c->shm_content = (shm_content_t *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, c->shm_fd, 0);
		if (c->shm_content == MAP_FAILED)
	        {
                	log("mmap() failed");
        	}
	}
	return c;
}
