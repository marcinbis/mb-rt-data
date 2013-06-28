#include "../common/control.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>

#include <unistd.h>
#include <string.h>

#include <stdlib.h>

#include "msg.h"

int msg_put(struct msg_t * msg, int msg_id)
{
	message_t m;
	
	m.msg_id = msg_id;
	clock_gettime(CLOCK_REALTIME, &m.msg_timestamp);
	
	/* 0 jeżeli wysłano, EAGAIN jeżeli kolejka pełna */
	return mq_send(msg->mq, (char *)&m, sizeof(message_t), 0);
}

struct msg_t * new_msg(void)
{
	struct mq_attr attr = {
		.mq_flags = O_NONBLOCK,
		.mq_maxmsg = MQ_MAXMSGS,
		.mq_msgsize = sizeof(message_t),
	};

	static struct msg_t * msg = NULL;
	if (msg == NULL)
	{
		msg = malloc(sizeof(struct msg_t));
		msg->put = &msg_put;

		/* Inicjowanie kolejki komunikatów, flaga O_NOBLOCK zapewnia, że
		 * wysyłanie komunikatu nie będzie powodowało blokowania procesu.
		 * (nadal jednak komunikat jest kopiowany do kolejki) */
		msg->mq = mq_open(MQ_FILE, O_WRONLY | O_NONBLOCK | O_CREAT, S_IRUSR | S_IWUSR, &attr);
		if (msg->mq == -1)
		{
			log("msg_t: message queue open failed, error messages will be dropped");
		}

		log("msg_t: inicialized");
	}
	return msg;
}
