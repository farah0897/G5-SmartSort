# ArduinoBot Firmware

This package contains the Arduino firmware for controlling the ArduinoBot's servos.

## Setup Instructions

1. Install the Arduino IDE
2. Open the servo_control.ino sketch
3. Upload to your Arduino board

## Pin Configuration

- Joint 1: Pin 8
- Joint 2: Pin 9
- Joint 3: Pin 10
- Joint 4: Pin 11
- Joint 5: Pin 12

## Serial Protocol

The firmware accepts the following commands over serial (115200 baud):

- `r` - Read current positions (returns comma-separated values)
- `w90,90,90,90,90` - Write new positions (comma-separated values in degrees)

## Constraints

- Valid angle range: 0-180 degrees
- Serial timeout: 1ms
- Update rate: ~1kHz 