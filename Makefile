.PHONY: all clean install

PRU_SWPKG = /usr/lib/ti/pru-software-support-package

CC = clpru
LD = lnkpru
CFLAGS = --include_path=$(PRU_SWPKG)/include \
         --include_path=$(PRU_SWPKG)/include/am335x
LDFLAGS = $(PRU_SWPKG)/labs/lab_2/AM335x_PRU.cmd


all: am335x-pru0-fw

hello-pru0.o: hello-pru0.c
	$(CC) $(CFLAGS) $^ --output_file $@

am335x-pru0-fw: hello-pru0.o
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -vf hello-pru0.o am335x-pru0-fw

install: am335x-pru0-fw
	cp -v am335x-pru0-fw /lib/firmware/am335x-pru0-fw
	echo "am335x-pru0-fw" > /sys/class/remoteproc/remoteproc1/firmware
