obj-m := 01_inout.o 02_uinout.o 03_cinout.o 04_real_cinout.o

CROSS_COMPILE = arm-none-linux-gnueabi-
#KDIR = /home/marcin/DATA/WORK/RT/buildroot-2012.05/output/build/linux-3.4.4
#KDIR = /home/marcin/DATA/WORK/RT/buildroot-2012.05/output/build/linux-3.2.22
KDIR = /home/marcin/DATA/WORK/RT/linux-3.2.22

default:
	make -C $(KDIR) SUBDIRS=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) modules
	$(CROSS_COMPILE)gcc -s -o user_worker user_worker.c

clean:
	make -C $(KDIR) SUBDIRS=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) clean
	rm -f user_worker

dist:
	tar -cf /srv/tftp/tests.tar *ko user_worker
