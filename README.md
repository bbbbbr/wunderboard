wunderboard
===========

Wunderboard wunder.2 2013 code samples and docs



## Setting up the Toolchain on Linux (Ubuntu/Mint/etc)

1. Open a termninal Window

2. Make sure apt has current versions (type into terminal) :
  * `sudo apt-get update`

3. Install toolchain (type into terminal) :
  * `sudo apt-get install avr-libc binutils-avr gcc-avr avrdude`

  If it works, the last four lines should resemble :
  `Setting up binutils-avr (version stuff)`
  `Setting up gcc-avr (version stuff)`
  `Setting up avr-libc (version stuff)`
  `Setting up avrdude (version stuff)`

4. Install the DFU programmer for flashing the board  (type into terminal) :
    `sudo apt-get install dfu-programmer`

## Flashing / Updating firmware on the board


1. Connect device in DFU programming mode
  1. Plug board in
  2. Press and continue to hold "HWR" button
  3. Press and continue to hold button
  4. Release "RST" button
  5. Release "HWR" button
  6. The board should re-enumerate on the USB bus using it's built-in boot loader
  7. Enumeration can be checked by running : `lsusb | grep -i "atmel"`
  8. It should show something similar to : `Bus 002 Device 003: ID 03eb:2ffb Atmel Corp. at90usb AVR DFU bootloader`

2. Erase the device
  * `sudo dfu-programmer  at90usb1287 erase`

3. Flash the flash device (using "main.hex" as an example)
  * `sudo dfu-programmer at90usb1287 flash main.hex`

4. Reset the device and start executing the updated firmware
  * `Press and release the "RST" button`

