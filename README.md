Black Magic Probe
=================

[![Build Status](https://travis-ci.org/blacksphere/blackmagic.svg?branch=master)](https://travis-ci.org/blacksphere/blackmagic)
[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/blacksphere/blackmagic?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![Donate](https://img.shields.io/badge/paypal-donate-blue.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=N84QYNAM2JJQG)
[![Kickstarter](https://img.shields.io/badge/kickstarter-back%20us-14e16e.svg)](https://www.kickstarter.com/projects/esden/1bitsy-and-black-magic-probe-demystifying-arm-prog)

## How to flash

Download latest binary from release. Set Core Module to boot mode and flash using `bcf`

```
bcf flash --dfu core_module_blackmagic.bin
```

## How to build
Not neccessary, but if you would like to recompile the binary:
```
make PROBE_HOST=bigclown
```

## Core-Module pinout

| Core Pin  | GPIO Pin  | Function  | 
|-----------|-----------|-----------|
| P1        | PA1       | SWD_DIO   | 
| P2        | PA2       | SWD_CLK   | 
| P11       | PA9       | UART TX   | 
| P10       | PA10      | UART RX   | 

## GDB Command

```
C:\Users\Martin\Documents\BigClown\bcf-generic-node>arm-none-eabi-gdb out\debug\firmware.elf
GNU gdb (GNU Tools for ARM Embedded Processors 6-2017-q2-update) 7.12.1.20170417-git
Copyright (C) 2017 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=i686-w64-mingw32 --target=arm-none-eabi".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from out\debug\firmware.elf...done.
(gdb) target extended-remote \\.\COM4
Remote debugging using \\.\COM4
(gdb) mon swdp_scan
Target voltage: [not implemented]
Available Targets:
No. Att Driver
 1      STM32L0x
(gdb) load
Loading section .isr_vector, size 0xc0 lma 0x8000000
Load failed
(gdb) attach 1
Attaching to program: C:\Users\Martin\Documents\BigClown\bcf-generic-node\out\debug\firmware.elf, Remote target
0x0800535c in bc_gpio_set_mode (channel=<optimized out>, mode=<optimized out>) at sdk/bcl/src/bc_gpio.c:360
360                 bc_gpio_port[channel]->AFR[1] |= ((uint8_t)mode >> _BC_GPIO_MODE_AF_POS) << (_bc_gpio_64_bit_pos(channel) - 32);
(gdb) load
Loading section .isr_vector, size 0xc0 lma 0x8000000
Loading section .text, size 0x1212c lma 0x80000c0
Loading section .rodata, size 0x4d8c lma 0x80121f0
Loading section .ARM, size 0x8 lma 0x8016f7c
Loading section .init_array, size 0x8 lma 0x8016f84
Loading section .fini_array, size 0x4 lma 0x8016f8c
Loading section .data, size 0xba0 lma 0x8016f90
Start address 0x800e188, load size 97068
Transfer rate: 5 KB/sec, 915 bytes/write.
(gdb) c
Continuing.
```

VSCode launch.json
https://sites.google.com/site/hubmartin/arm/black-magic-probe-vs-code

Pin definitions with JTAG pins
https://github.com/hubmartin/BMP_STM32L083/blob/master/src/platforms/bigclown/platform.h#L68

## Blackmagic official text

Firmware for the Black Magic Debug Probe.

The Black Magic Probe is a modern, in-application debugging tool for
embedded microprocessors. It allows you see what is going on 'inside' an
application running on an embedded microprocessor while it executes. It is
able to control and examine the state of the target microprocessor using a
JTAG or Serial Wire Debugging (SWD) port and on-chip debug logic provided
by the microprocessor. The probe connects to a host computer using a
standard USB interface. The user is able to control exactly what happens
using the GNU source level debugging software, GDB.

See online documentation at https://github.com/blacksphere/blackmagic/wiki

Binaries from the latest automated build are at http://builds.blacksphere.co.nz/blackmagic
