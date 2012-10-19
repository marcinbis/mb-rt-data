/*
 * IO latency test module - interupt managed in upper half.
 * Data passed to userspace for processing.
 *
 * Copyright (C) 2012 BIS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
/* INPUT_PIN and OUTPUT_PIN definitions. */
#include "pins.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcin Bis");
MODULE_DESCRIPTION("IO latency test module - userspace connector without bottom halve.");

/* Struct of this type will be passed to isr */
typedef struct {
	long counter;
} inout_data;

/* Module globals: */

/* Character device numbers and control struct. */
dev_t inout_dev_num;
static struct cdev * inout_cdev;

/* Data to be passed to interrupt handler. */
inout_data * data;

/* Wait queue for read requests. */
DECLARE_WAIT_QUEUE_HEAD(inout_wait_queue);

static irqreturn_t inout_isr(int irq, void *dev_id)
{
	inout_data * data = dev_id;
	data->counter++;
	
        /* Wake up waiting processes */
        wake_up(&inout_wait_queue);

	return IRQ_HANDLED;
}

static ssize_t inout_read(struct file *file, char __user * userbuf, size_t count, loff_t * ppos)
{
	int cur = data->counter;
	char buf[2];
	int data_size;
	int ret;
	/* Process is put to sleep (S) */
	wait_event_interruptible(inout_wait_queue, (data->counter > cur));
	/* Wake - up occured */

	/* Check whenever wake-up occured, or signal was received */
	if (signal_pending(current))
		return -EINTR;

	/* Write current state of the input_pin. */
#ifdef BEAGLEBOARD_XM
	data_size = sprintf(buf, "%d", (gpio_get_value(INPUT_PIN) + 1) % 2);
#else
	data_size = sprintf(buf, "%d", gpio_get_value(INPUT_PIN));
#endif
	ret = copy_to_user(userbuf, buf, data_size);
	if (ret != 0)
		return -ENODEV;
	return data_size;
}

static ssize_t inout_write(struct file *file, const char __user * userbuf, size_t count, loff_t * ppos)
{
	char tmpbuf[2] = "0";
	int val = 0;
	int res;

	/* Get one charecter from user. */
	res = copy_from_user(tmpbuf, userbuf, 1);
	val = simple_strtol(tmpbuf, NULL, 10);

	//printk(KERN_INFO "Requested value: %d\n",val);

	gpio_set_value(OUTPUT_PIN,val);
	return count;
}

static struct file_operations inout_fops = {
	.read = inout_read,
	.write = inout_write,
};

static int __init inout_init(void)
{
	int irq, error;
	char * desc = "inout latency test";

	data = kzalloc(sizeof(inout_data), GFP_KERNEL);
	data->counter = 0;

	/* claim and configure output pin */
	error = gpio_request(OUTPUT_PIN, desc);
	if (error < 0) {
		printk(KERN_ERR "Failed to request GPIO %d, error %d\n",OUTPUT_PIN,error);
		goto err0;
	}

	error = gpio_direction_output(OUTPUT_PIN,1);

	if (error < 0) {
		printk(KERN_ERR "Failed to set direction of GPIO %d, error %d\n",OUTPUT_PIN,error);
		goto err1;
	}

	gpio_set_value(OUTPUT_PIN,0);
	
	/* claim and configure input pin */
	error = gpio_request(INPUT_PIN, desc);
	if (error < 0) {
		printk(KERN_ERR "Failed to request GPIO %d, error %d\n",INPUT_PIN,error);
		goto err1;
	}

	error = gpio_direction_input(INPUT_PIN);
	if (error < 0) {
		printk(KERN_ERR "Failed to set direction of GPIO %d, error %d\n",INPUT_PIN,error);
		goto err2;
	}

	irq = gpio_to_irq(INPUT_PIN);
	if (error < 0) {
		error = irq;
		printk(KERN_ERR "Failed to get IRQ number for GPIO %d, error %d\n",INPUT_PIN,error);
		goto err2;
	}

	/* Register character device */
	error = alloc_chrdev_region(&inout_dev_num, 0, 1, desc);
	if (error) {
		printk(KERN_ERR "Failed to allocate device number.\n");
		goto err3;
	}

	inout_cdev = cdev_alloc();
	cdev_init(inout_cdev, &inout_fops);
	error = cdev_add(inout_cdev, inout_dev_num, 1);
	if (error) {
		printk(KERN_ERR "Failed to register device\n");
		goto err3;
	}

	/* request interrupt on input pin - the fun starts from now */
	error = request_irq(irq, inout_isr,
                            IRQF_SHARED | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                            desc, data);

	if (error) {
		printk(KERN_ERR "Failed to request interrupt %d, error %d\n",irq,error);
		goto err4;
	}

	printk(KERN_INFO "Cinout module loaded, device major: %d, minor %d. (Execute: \"mknod /dev/inout%d c %d %d\")\n",
		MAJOR(inout_dev_num), MINOR(inout_dev_num), MINOR(inout_dev_num), MAJOR(inout_dev_num), MINOR(inout_dev_num));

	return 0;

err4:
	cdev_del(inout_cdev);
err3:
	unregister_chrdev_region(inout_dev_num, 1);
err2:
	gpio_free(INPUT_PIN);
err1:
	gpio_free(OUTPUT_PIN);
err0:
	return error;
}
module_init(inout_init);

static void __exit inout_exit(void)
{
	int irq;
	
	cdev_del(inout_cdev);
	unregister_chrdev_region(inout_dev_num, 1);

	irq = gpio_to_irq(INPUT_PIN);
	free_irq(irq, data);
	gpio_free(INPUT_PIN);
	gpio_free(OUTPUT_PIN);
	printk(KERN_INFO "Event was trigerred %ld times\n", data->counter);
	kfree(data);
	return;
}
module_exit(inout_exit);
