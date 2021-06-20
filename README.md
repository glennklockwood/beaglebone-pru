# BeagleBone PRU Examples

This code accompanies my notes on [Getting started with the BeagleBone Black][1]
on my website.

## Compiling Examples

After you configure your pin multiplexing correctly, you should be able to

    $ cd hello
    $ make
    $ ../bin/install-n-start hello-pru0

## pructl

I included a simple script, `bin/pructl`, which starts and stops a PRU.

    $ pructl stop # stop PRU0
    $ pructl state # check state of PRU0
    $ pructl start 1 # start PRU1

[1]: https://www.glennklockwood.com/embedded/beaglebone.html
