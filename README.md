# Arduino-based Substitution for Rudder Pedals Electronics
An Arduino Project for substituting the electronics of flight sim rudder pedals

For some years, I owned a yoke for flying in flight sims, but never had rudder pedals.
Upon recently spending some time in Microsoft's new Flight Simulator, I wanted to complete my equipment by a pair of rudder pedals.
Because I was too stingy to buy some new ones for ~150€, I bought a defective pair on eBay for 40€, where the description simply said "No power".
I hoped for the best, i.e., that a cable was simply soldered badly, or a plug was not corrected properly, but I was unlucky, as to the best of my knowledge microcontroller was bricked.

I therefore came up with the idea to substitute the whole internal PCB by an Arduino-based circuit after testing that the potentiometers measuring the three axes positions were still intact.

### Hardware

An Arduino Pro Micro (clone) is used, as the Atmega32U4 provides both a fully-compliant USB2.0 interface, which allows for easy adaption of the [HID library](https://github.com/NicoHood/HID) I used, and has ADC converters, which are required to measure the state of the three potentiometers (the rudder axis and the two brake axes).

The two outer pins of the potentiometers of the rudder pedals were connected to Vcc (5V) and GND, and the middle pins were connected to one of the ADC pins of the Arduino each.

The pin mapping is as follows:

- `A0`: Rudder potentiometer (centre/rotation)
- `A1`: Left brake potentiometer
- `A2`: Right brake potentiometer
- `2`: Calibration reset button (see below, "Auto calibration")

Also, the rudder pedals have a power LED, which i simply hooked up to 5V with an appropriate series resistor of 330 Ohm.

**TODO:** Upload a picture of the circuit.

### Code

#### HID

This is the main implementation of reading the analog voltages on the potentiometer pins, the auto-calibration, and the USB Gamepad HID communication.

#### ReadTest

This is a project used to test both the general functionality of reading the analog voltages from the potentiometers, and for testing the auto-calibration.

For this, three buttons are connected to pins `2`, `3`, and `4` of the Arduino Micro, which can be used to switch between monitoring the analog pins `A0`, `A1`, and `A2`.

The measured and auto-calibrated values of the given analog pin are simply repeatedly printed on the serial interface, such that they can be easily visualized using the serial plotter of the Arduino IDE.

### Auto-calibration

To avoid having to deal with manually measuring and then mapping the values which the ADC spits out to the desired value space, I implemented a simple auto-calibration algorithm, which simply adapts the minimum and maximum input values based on the given inputs.
It also centers the rotation (rudder) axis on power-up.
Therefore, upon plugging in the USB cable, one should have the pedals in neutral position, and afterwards push all three axes to their minimum and maximum positions in order to properly calibrate.

This calibration can be reset on-the-fly by using the calibration reset button (pin `2`, see above).
