.PHONY: all clean install

TARGET_OBJ = hello-pru0-p9_45

PRU_SWPKG = /usr/lib/ti/pru-software-support-package

CC = clpru
LD = lnkpru
CFLAGS = --include_path=$(PRU_SWPKG)/include \
         --include_path=$(PRU_SWPKG)/include/am335x
LDFLAGS = $(PRU_SWPKG)/labs/lab_2/AM335x_PRU.cmd


all: am335x-pru0-fw

am335x-pru0-fw: $(TARGET_OBJ).o
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ --output_file $@

clean:
	rm -vf $(TARGET_OBJ).o am335x-pru0-fw

install: am335x-pru0-fw
	cp -v am335x-pru0-fw /lib/firmware/am335x-pru0-fw
	echo "am335x-pru0-fw" > /sys/class/remoteproc/remoteproc1/firmware
