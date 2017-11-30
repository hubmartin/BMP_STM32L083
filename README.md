BigClown port of Black Magic Probe to Core Module
=================


[![Twitter](https://img.shields.io/twitter/follow/BigClownLabs.svg?style=social&label=Follow)](https://twitter.com/BigClownLabs)

## Two released versions

#### SWD on P1 & P2

In releases there are two versions which have different pins for SWD signals (the reset signals stays). The "P1_P2" release has SWD signals on these two pins, please see the pinout section.

#### SWD on 10pin JTAG connector

If you like living on the edge, then use the "SWD_ON_JTAG" release where just the SWD CLK and DIO is routed to the 10 pin JTAG connector on Core Module. This way you can connect both modules by 1mm pitch 10 pin programming connector.

**----------- IMPORTANT ----------**
The target you are programming takes down the RESET signal for a while and also resets the programmer. Workaround is to cut the RESET wire on the programming cable or put at least 2k7 or bigger value resistor.

 Also there could be some power supply issues in case both modules are powered by USB or batteries because both regulated power supplies are connected together but it seems like it's working for now ;) You can also cut the Vcc wire and supply voltage to the target by other USB or Battery Modules.
**----------- IMPORTANT ----------**

## How to flash

Download latest binary from release. Set Core Module to boot mode and flash using `bcf`

```
bcf flash --dfu core_module_blackmagic.bin
```

## Core-Module pinout

This table is true only in "P1_P2" releases. Otherwise SWD signals are on the Core Module JTAG connector.

| Core Pin  | GPIO Pin  | Function  | 
|-----------|-----------|-----------|
| P0        | PA0       | JTAG_TDI  | 
| **P1**    | **PA1**   |**SWD_DIO**  / JTAG_TMS | 
| **P2**    | **PA2**   |**SWD_CLK** / JTAG_TCK | 
| P3        | PA3       | JTAG_TDO  | 
| P4        | PA4       | JTAG_TRST | 
| P5        | PA5       | JTAG_SRST | 
| P11       | PA9       | UART TX   | 
| P10       | PA10      | UART RX   | 

## VSCode launch.json

https://sites.google.com/site/hubmartin/arm/black-magic-probe-vs-code


## How to build
Not neccessary, but if you would like to recompile the binary:
```
make PROBE_HOST=bigclown
```
The SWD output is possible to change by **SWD_ON_JTAG** define which is in the bigclown/platform.h.


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
