.PHONY: all clean install

TARGETS = hello-pru0.bin

PRU_SWPKG = /usr/lib/ti/pru-software-support-package

CC = clpru
LD = lnkpru
CFLAGS = --include_path=$(PRU_SWPKG)/include \
         --include_path=$(PRU_SWPKG)/include/am335x \
         --c99
LDFLAGS = $(PRU_SWPKG)/labs/lab_2/AM335x_PRU.cmd

all: $(TARGETS)

%.bin: %.o
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ --output_file $@

clean:
	rm -vf $(TARGETS) *.o
