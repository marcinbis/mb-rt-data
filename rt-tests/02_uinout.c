/*
 * IO latency test module - interupt managed in upper half and bottom half.
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
#include <linux/slab.h>
/* INPUT_PIN and OUTPUT_PIN definitions. */
#include "pins.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcin Bis");
MODULE_DESCRIPTION("IO latency test module - userspace connector.");

/* Struct of this type will be passed to isr and bottom half */
typedef struct {
	struct work_struct work;
	long * count;             /* events counter (global)*/
	int value;                /* current input pin state */
} inout_data;

/* Module globals: */

/* Data to be passed to interrupt handler. */
long counter;

/* Workqueue */
static struct workqueue_struct *inout_wq;


/* BH */
static void inout_job_function(struct work_struct * work)
{
	/* Get our data from pointer to struct work_struct */
	inout_data * data = 
		container_of(work, inout_data, work);

	long * d = data->count;

	(*d)++;

	gpio_set_value(OUTPUT_PIN,data->value);

	kfree(data);
}

static irqreturn_t inout_isr(int irq, void *dev_id)
{
	inout_data * job;
	int ret;

	job = kmalloc(sizeof(inout_data), GFP_ATOMIC);
	
	INIT_WORK(&(job->work), inout_job_function);
	job->count = (long *) dev_id;
	job->value = gpio_get_value(INPUT_PIN);

	ret = queue_work(inout_wq, &(job->work));

	return IRQ_HANDLED;
}

static int __init inout_init(void)
{
	int irq, error;
	char * desc = "inout latency test";

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
	
	/* create workqueue to handle jobs */
	inout_wq = create_workqueue("inout_queue");

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

	/* request interrupt on input pin - the fun starts from now */
	error = request_irq(irq, inout_isr,
                            IRQF_SHARED | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                            desc, (void *)  &counter);

	if (error) {
		printk(KERN_ERR "Failed to request interrupt %d, error %d\n",irq,error);
		goto err2;
	}

	return 0;

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
	
	irq = gpio_to_irq(INPUT_PIN);
	free_irq(irq, (void *) &counter);
	flush_workqueue(inout_wq);
	destroy_workqueue(inout_wq);
	gpio_free(INPUT_PIN);
	gpio_free(OUTPUT_PIN);
	printk(KERN_INFO "Event was trigerred %ld times\n", counter);
	return;
}
module_exit(inout_exit);
