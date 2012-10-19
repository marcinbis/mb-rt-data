/*
 * IO latency test - userspace helper for 03_cinout and 04_real_cinout modules.
 * Process responds to trigerred events.
 *
 * Copyright (C) 2012 BIS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char ** argv)
{
	int handle;
	char buf[2];
	int ret;

	if (argc != 2) {
		printf("Usage: %s /dev/inoutX\n", argv[0]);
		return 1;
	}

	handle = open(argv[1], O_RDWR);
	if (handle == -1) {
		printf("Cannot open device file: %s\n", argv[1]);
		return 1;
	}

	while (1) {
		/* Read current counter state - this puts process to sleep. */
		ret = read(handle, buf, 1);
		/* Process was woken up. Check if it was a signal (EINTR is set in errno it this case). */
		if (ret == -1) break;
		/* Respond sending the same value */
		ret = write(handle, buf, 1);
	}
	return 0;
}
