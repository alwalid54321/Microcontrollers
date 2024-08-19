# I2CDisplay Project

This project demonstrates the use of an LCD display, a lightning sensor, and a speaker with an Arduino board. It utilizes the `LiquidCrystal_I2C`, `LCDGraph`, and `AS3935MI` libraries to display a sine wave on the LCD, detect lightning, and play a melody.

## Overview

- **LCD Display**: Shows a sine wave graph and alerts for lightning detection.
- **AS3935 Lightning Sensor**: Detects lightning and provides alerts.
- **Speaker**: Plays a melody.

## Components

- **Microcontroller**: Arduino or compatible board.
- **LCD Display**: I2C interface (address `0x27`).
- **AS3935 Lightning Sensor**: For lightning detection.
- **Speaker**: Connected to pin 9.

## Libraries Required

- `Wire.h`: For I2C communication.
- `LiquidCrystal_I2C.h`: For controlling the LCD via I2C.
- `LCDGraph.h`: For drawing graphs on the LCD.
- `AS3935MI.h`: For interfacing with the AS3935 lightning sensor.
- `AceWire.h`: For I2C communication with the AS3935 sensor.

## Setup

1. **Install Arduino IDE**: Download and install from [Arduino's website](https://www.arduino.cc/en/software).

2. **Add Libraries**:
   - Install the required libraries through the Arduino Library Manager or manually.
   - `LiquidCrystal_I2C`: [GitHub Link](https://github.com/johnrickman/LiquidCrystal_I2C)
   - `LCDGraph`, `AS3935MI`, and `AceWire` can be found in their respective repositories or forums.

3. **Upload Code**:
   - Open the `I2CDisplay.ino` file in the Arduino IDE.
   - Connect your Arduino board to your computer via USB.
   - Select the correct board and port from `Tools` > `Board` and `Tools` > `Port`.
   - Click the `Upload` button to transfer the code to your Arduino.

## Code Description

- **Libraries and Initialization**: Includes necessary libraries and initializes the LCD, graph, lightning sensor, and speaker.
- **Setup Function**: Configures the LCD, graph, and lightning sensor. Initializes the serial communication and sets up the speaker.
- **Loop Function**:
  - Draws a sine wave on the LCD.
  - Checks for lightning detection and updates the LCD if lightning is detected.
  - Plays a melody through the speaker.
  - Updates the display and sensor data in real-time.

## Troubleshooting

- **Code Upload Issues**:
  - Ensure correct board and port selection in the Arduino IDE.
  - Check hardware connections.

- **Display Not Updating**:
  - Verify that the LCD is correctly wired and powered.
  - Ensure that the `LiquidCrystal_I2C` library is properly installed.

- **Lightning Detection Not Working**:
  - Confirm proper connections to the AS3935 sensor.
  - Check sensor configuration and calibration.
