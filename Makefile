.PHONY: all clean install

CC = clpru
LD = lnkpru
CFLAGS = --include_path=/home/glock/src/cloud9-examples/common \
	 --include_path=/usr/lib/ti/pru-software-support-package/include \
	 --include_path=/usr/lib/ti/pru-software-support-package/include/am335x \
	 --include_path=/usr/share/ti/cgt-pru/include

all: am335x-pru0-fw

hello-pru0.o: hello-pru0.c
	$(CC) $(CFLAGS) $^ -fe $@

am335x-pru0-fw: hello-pru0.o /home/glock/src/cloud9-examples/common/am335x_pru.cmd
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -vf hello.pru0.o am335x-pru0-fw

install: am335x-pru0-fw
	cp -v am335x-pru0-fw /lib/firmware/am335x-pru0-fw
	echo "am335x-pru0-fw" > /sys/class/remoteproc/remoteproc1/firmware
