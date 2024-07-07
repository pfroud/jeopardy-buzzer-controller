# Jeopardy buzzer controller

![photo of enclosure](docs/img/enclosure/enclosure-angle.jpeg)

## About

For use with my [Jeopardy software](https://github.com/pfroud/jeopardy).

This device uses an Arduino to emulate a USB HID keyboard (like a QWERTY keyboard not a music keyboard). Instead of normal keyboard keys, the inputs come from rugged metal footswitches on long cables.

The sole purpose of this device is to convert physical button presses into USB HID keystrokes. It cannot tell which person buzzed first for a question, for example. All of that logic is done by software running on the host PC.

The PCB was designed using KiCad version 6 and fabricated by JLCPCB.

## Parts

The two most interesting parts are:
* [Sparkfun Pro Micro](https://www.sparkfun.com/products/12640) (not Mini, not Nano)
    * Informally called "Arduino Pro Micro" but it's actually a Sparkfun product which is Arduino-compatible.
    * To learn about the software running on the Arduino, see the [Arduino directory](Arduino).
* [XLR receptacle NC4MBV](https://www.neutrik.com/en/product/nc4mbv-1) to connect the buzzers.
    * To learn more, go to the "XLR connectors" section in this document.

Other parts:
* [12-pin female headers](https://www.aliexpress.us/item/2255801012106911.html) to hold the Arduino.
    * The Arduino could instead be soldered directly to the PCB if you're feeling confident.
* A toggle switch to enable/disable the Arduino software.
    * Lets you turn the buzzers off without unplugging the USB cable.
* [Panel-mount LED lights](https://www.aliexpress.com/item/3256803816108154.html) to indicate when it's powered and enabled.
* [2-pin pluggable terminal blocks](https://www.aliexpress.us/item/2251832631116561.html) to connect the toggle switch and indicator LEDs to the PCB.
* A 6mm tactile pushbutton to reset the Arduino.
   * The Pro Micro does not have a built-in reset button.


## Known bugs & future work

### PCB design

* There is no input protection on the Arduino pins.
* The trace length between the Arduino and XLR connector is different for each connector.

### Does using an HID keyboard for this make any sense??

Not really. The original scope of my [Jeopardy project](https://github.com/pfroud/jeopardy) was software only. The hardware buzzers have been gradually developed as needed. Want to keep it simple!

See docs/dont-use-a-HID-keyboard.md.

### Issues with the XLR connectors

XLR connectors are usually used in professional audio and stage lighting, so they are overkill for this application. I wanted locking connectors and found these in surplus.

#### Wasted pins

Only two out of four pins are used. In the future, the spare pins could be used for lighting or some other new feature. An overwhelming number of two-pin locking connectors are available.

#### Polarity

The polarity of the XLR connectors should be reversed.

Neutrik makes four types of 4-pin XLR connectors. Look at the table below for a visual aid.

The receptacles on the controller are male (top left, NC4MBV) which allows fingers to touch the pins, exposing the Arduino to static electricity.

Meanwhile the connectors on the buzzer cables are female (bottom right, NC4FXX). The recessed pins prevent fingers touching them, but that protection isn't needed because the buzzers contain only passive switches.

The controller should use female receptacles, and the buzzers should use male cable connectors.

<table>
<tr>
    <td></td>
    <td align="center">Male</td>
    <td align="center">Female</td>
</tr>
<tr>
    <td>Receptacle</td>
    <td align="center">
        <a href="https://www.neutrik.us/en-us/product/nc4mbv-1">
            <img src="docs/img/XLR-connectors/Neutrik-NC4MBV-XLR-receptacle.jpg" alt="Neutrik NC4MBV XLR receptacle">
            <br>
            NC4MBV
        </a>
    </td>
    <td align="center">
        <a href="https://www.neutrik.us/en-us/product/nc4fbv">
            <img src="docs/img/XLR-connectors/Neutrik-NC4FBV-XLR-receptacle.jpg" alt="Neutrik NC4FBV XLR receptacle">
            <br>
            NC4FBV
        </a>
    </td>
</tr>
<tr>
    <td>Cable connector</td>
    <td align="center">
        <a href="https://www.neutrik.us/en-us/product/nc4mxx">
            <img src="docs/img/XLR-connectors/Neutrik-NC4MXX-XLR-cable-connector.jpg" alt="Neutrik NC4MXX XLR cable connector">
            <br>
            NC4MXX
        </a>
    </td>
    <td align="center">
        <a href="https://www.neutrik.us/en-us/product/nc4fxx">
            <img src="docs/img/XLR-connectors/Neutrik-NC4fXX-XLR-cable-connector.jpg" alt="Neutrik NC4FXX XLR cable connector">
            <br>
            NC4FXX
        </a>
    </td>
</tr>
</table>