LED Blink Project
Welcome to the LED Blink project for Week 1! This guide will walk you through setting up a foundational hardware project: making a Light Emitting Diode (LED) blink using a microcontroller.

Hardware Required
To build this project, you will need the following components:

Microcontroller: 1x Arduino Uno (or compatible board)

Light Source: 1x 5mm or 3mm LED (any color)

Resistor: 1x 220Ω (or 330Ω) Resistor

Prototyping: 1x Breadboard

Wiring: 2x Jumper Wires

Circuit Diagram Description
Follow these text-based instructions to wire your circuit correctly:

Place the LED: Insert the LED into the breadboard. Take note of the longer leg (anode/positive) and the shorter leg (cathode/negative).

Connect the Anode: Use a jumper wire to connect the longer leg of the LED to Digital Pin 13 on the Arduino.

Add the Resistor: Connect one end of the 220Ω resistor to the shorter leg of the LED.

Connect to Ground: Connect the other end of the resistor to any GND (Ground) pin on the Arduino.

(Note: The resistor can technically be placed on either the anode or cathode side of the LED, as long as it is in series to limit the current.)

How to Upload Code
Follow these step-by-step instructions to compile and upload the code to your board:

Download and install the Arduino IDE from the official Arduino website if you haven't already.

Connect your Arduino board to your computer using a USB cable.

Open the Arduino IDE and load the led_blink.ino file from this repository.

Navigate to Tools > Board and select your specific board model (e.g., "Arduino Uno").

Navigate to Tools > Port and select the COM port that your Arduino is connected to.

Click the Verify button (check mark icon) in the top-left corner to ensure there are no syntax errors in the code.

Click the Upload button (right-pointing arrow icon) right next to the Verify button. Wait for the "Done uploading" message to appear at the bottom of the IDE.

Expected Output
Once the code is successfully uploaded, the LED connected to Digital Pin 13 should turn on (illuminate) for exactly one second, and then turn off for exactly one second. This blinking cycle will repeat continuously as long as the board receives power.

Troubleshooting Tips
If your circuit isn't working as expected, check the following common issues:

The LED isn't turning on: Check the LED's polarity. LEDs are directional; if it is plugged in backward, it will not light up. Ensure the long leg is connected to Pin 13 and the short leg goes to Ground.

"Port not found" or Upload errors in the IDE: Ensure your USB cable supports data transfer (some cables are charge-only). Double-check that you have selected the correct Board and Port under the Tools menu.

The LED is extremely dim or burned out: Verify you are using the correct resistor value. Using no resistor can burn out the LED instantly, while using a resistor with too high of a value (e.g., 10kΩ) will make the LED too dim to see
