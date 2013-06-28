#include <mqueue.h>

struct msg_t {
        int (* put) (struct msg_t *, int);

	/* private data */
	mqd_t mq;
};

struct msg_t * new_msg(void);
