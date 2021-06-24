# Hello World - UART Edition

The documentation for this code is located here:

<https://www.glennklockwood.com/sysadmin-howtos/beaglebone.html#pru-uart>

The Makefile should work on BeagleBone Black using a sufficiently new version
of Debian.  You can compile and install by hand using

    make hello-uart-minimal.bin
    cp hello-uart-minimal.bin /lib/firmware/am335x-pru0-fw

or you can let the install-n-start script do it:

    ./install-n-start hello-uart-minimal

Don't forget to config-pin appropriately first:

    config-pin p9_17 pru_uart
    config-pin p9_18 pru_uart

## Contents

This directory contains the source for the following hello world
implementations:

Binary                  | Description
------------------------|-----------------------
hello-uart.bin          | Verbose implementation of hello-uart
hello-uart-minimal.bin  | More compact version of hello-uart
hello-uart-factored.bin | hello-uart with UART functions factored out into uart.c to facilitate reuse
