Classic 8-bit era joysticks interfaced to USB with Arduino Micro Pro
====================================================================

This is the source code repository for Arduino Micro Pro (ATmega32u)
with firmware that allows to connect two digital joysticks compatible with C64/128/Atari/Amiga/...
as USB HID game controllers with optional keyboard input emulation.

Features
--------

### Game controllers

Four directions and fire button are translated to USB HID game controller commands.
On the USB host side there will be two 2-axis, 1-button devices available.

### Keyboard emulation

There is also optional keyboard emulation. It will translate joystick directions into keypresses.

Joystick 1 is translated to W, S, A, D and SPACE.

Joystick 2 is translated to arrow keys and left control.

The keyboard emulation is disabled by default.

It can be toggled by opening serial port console (Arduino IDE serial monitor or [Putty](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html) or anything else)
and sending a line with single 'k' character.

Hardware
--------

Just Arduino Micro Pro connected through 18 wires with two DB9 male ports.

### Connector pinouts

DB9 joystick cable (front, looking at holes)
````
 5 4 3 2 1
  9 8 7 6
````

DB9 joystick port (front, looking at pins)
````
 1 2 3 4 5
  6 7 8 9
````

### Connections

| Arduino Mini Pro pin | DB9 pin | function |
|---------------------:|---------|----------|
| 15 | 1 | joystick 1 up |
| 14 | 2 | joystick 1 down |
| 16 | 3 | joystick 1 left |
| 10 | 4 | joystick 1 right |
| A1 | 5 | joystick 1 POTY |
| 9  | 6 | joystick 1 fire |
| VCC | 7 | joystick 1,2 +5V |
| GND | 8 | joystick 1,2 GND |
| A0 | 9 | joystick 1 POTX |
| 4 | 1 | joystick 2 up |
| 5 | 2 | joystick 2 down |
| 6 | 3 | joystick 2 left |
| 7 | 4 | joystick 2 right |
| A3 | 5 | joystick 2 POTY |
| 8 | 6 | joystick 2 fire |
| A2 | 9 | joystick 2 POTX |


More buttons & more joystick systems
------------------------------------

With little effort the code can be reconfigured to support multisystem joysticks with up to 3 buttons.

[Here is a very helpful table with possible configurations.](http://wiki.icomp.de/wiki/DB9-Joystick)

This could be even done during runtime through serial interface text menu.

Paddles POTX/POTY are not supported
------------------------------------

Currently lines POTX/POTY are connected to analog inputs of ATmega, but they are not used in any way.

The internal pullup resistors are turned on so the potentiometers in attached controllers (paddles, spinners) would work as voltage dividers.

Reading raw analog values from these lines will give huge positional error due to non-linearity of measurement.

The proper way to do it would be to use one capacitor on each line and measure the time it takes to charge it. This method is used by SID chip in C64/128.

[This project entry might be helpful](https://hackaday.io/project/7941-avercade/log/26877-rewiring-for-dual-atari-controller).

[According to this table](http://wiki.icomp.de/wikti/DB9-Joystick)) POTX/POTY can be used in the same way as FIRE button and act as button 2 and 3 lines if connected to GND.
It would work without any hardware changes, but I don't know about any C64/128 game that would use this feature.

At the moment I don't have any controllers that would use these lines, so I'm not interested in implementing it.

Project page
------------

Please visit also [project page on hackaday.io](https://hackaday.io/project/9552-digital-joystick-8-bit-to-usb-adapter)

