.PHONY: all clean install

TARGETS = hello-uart.bin hello-uart-minimal.bin hello-uart-factored.bin

PRU_SWPKG = /usr/lib/ti/pru-software-support-package

CC = clpru
LD = lnkpru
CFLAGS = --include_path=$(PRU_SWPKG)/include \
         --include_path=$(PRU_SWPKG)/include/am335x \
         --c99
LDFLAGS = $(PRU_SWPKG)/labs/lab_2/AM335x_PRU.cmd

all: $(TARGETS)

hello-uart-factored.bin: hello-uart-factored.o uart.o

%.bin: %.o
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ --output_file $@

clean:
	rm -vf $(TARGETS) *.o
