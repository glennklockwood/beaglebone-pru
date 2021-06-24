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

    $ pructl stop       # stop PRU0
    $ pructl state      # check state of PRU0
    $ pructl start 1    # start PRU1

## config-pin

I also include a Python-based implementation of the `config-pin` tool that is a
little nicer than the [dash-based shell script][2] and [C version][3] of the
same tool.  Its options are fully compatible with the originals, but it also

- prints the current state and valid options if a pin is specified with no
  config or other parameters
- returns a meaningful error if you try to set a pin to an invalid mode
- has a handy `-h`/`--help` option

It's a bit slower since it's Python, but I think its richness makes up for
that.  For example, specifying a pin with no parameters something useful:

    $ config-pin p9_31
    Pin P9_31 is configured as: pruout
    Valid modes for P9_31 are default gpio gpio_pu gpio_pd spi_sclk pwm pruout pruin

The original command-line flags (`-q`, `-l`, `-f`) still work:

    $ config-pin -l p9_31
    Valid modes for P9_31 are default gpio gpio_pu gpio_pd spi_sclk pwm pruout pruin

Setting a new mode for a pin works too:

    $ config-pin p9_31 pruout
    Pin P9_31 is configured as: pruout

Trying to set an invalid mode for a pin tells you something helpful rather than
throwing a `No such device` error:

    $ config-pin p9_31 somethinginvalid
    Invalid mode. Valid modes for P9_31 are default gpio gpio_pu gpio_pd spi_sclk pwm pruout pruin
 
If you peek at its source code, you can see that it is implemented using a
class that you may find useful for other BeagleBone automation tasks.

[1]: https://www.glennklockwood.com/embedded/beaglebone.html
[2]: https://github.com/beagleboard/bb.org-overlays/blob/master/tools/beaglebone-universal-io/config-pin
[3]: https://github.com/pmunts/muntsos/blob/master/bootkernel/initramfs/config-pin.c
