# ODROID U3 GameStatiob Hacks
My hacks for ODROID [GameStation Linux distro](https://forum.odroid.com/viewtopic.php?f=11&t=2684)

## Button
One can connect external GPIO button to stop games emulation.
Button should be connected to PIN1 in INPUT mode and PIN2.

`Caution! Input voltage is limited to 1.8V!`

On button click custom app will send `Escape` keypress to active window

## Button processing
To receive data from GPIO button we should perform some configuration steps:
```bash
echo 199 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio199/direction
echo falling > /sys/class/gpio/gpio199/edge
# get button status; 1 - pressed, 0 - not pressed
cat /sys/class/gpio/gpio199/value
```
`gpio_button_logic.c` contains source code for monitoring value changes. If button pressed app will send `Escape` keypress to active window.

To compile app just run
```
arm-linux-gnueabi-gcc gpio_button_logic.c -o button
```

## GPIO PINs
J4 - 2×4 pins(IO Port #1)
| Pin Number | Net Name | GPIO & Export No | Pin Number | Net Name | GPIO & Export No |
|------------|----------|------------------|------------|----------|------------------|
| 1          | XEINT_8  | GPX1.0 (#199)    | 2          | 1.8V     |                  |
| 3          | XEINT_9  | GPX1.1 (#200)    | 4          | XURXD_0  | /dev/ttySAC0     |
| 5          | XEINT_13 | GPX1.5 (#204)    | 6          | XUTXD_0  | /dev/ttySAC0     |
| 7          | GND      |                  | 8          | 5V0      |                  |

## PIN layout
| 1 | 3 | 5 | 7 |
|---|---|---|---|
| 2 | 4 | 6 | 8 |

## UART
_____UART____  
|Pin 4 - GND|  
|Pin 3 - RXD|  
|Pin 2 - TXD|  
|Pin 1 - VCC|  
\___________|  
1.8V LVTTL


## Links
* [ODROID U3 Block Diagram and Schematic](https://wiki.odroid.com/old_product/odroid-x_u_q/odroid_u3/u3_hardware)
* [Using a push button with Raspberry Pi GPIO](https://raspberrypihq.com/use-a-push-button-with-raspberry-pi-gpio/)
* [ODROID U3 Shield scematics](https://drive.google.com/file/d/0B4UPrML8Nk9lQzBRNzBuOTBwMjQ/edit)
* [C++ GPIO example](https://github.com/wirenboard/wiegand-linux-sysfs)
* [xdotool manual](http://manpages.ubuntu.com/manpages/trusty/man1/xdotool.1.html)