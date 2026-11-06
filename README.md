# ESP32 Wi-Fi Controlled 4WD Robot Car

A four-wheel-drive robot car controlled wirelessly through a mobile-friendly web interface hosted directly on an ESP32.

## Project Overview

This project uses an ESP32 microcontroller to create a Wi-Fi-controlled robotic car. The ESP32 creates its own Wi-Fi access point and hosts a control webpage. A smartphone or computer can connect to the car and control its movement through a browser.

## Features

- Wireless control using ESP32 Wi-Fi
- Four-wheel-drive movement
- Forward and backward movement
- Left and right turning
- Stop control
- Mobile-friendly web interface
- Automatic motor stop when commands time out

## Hardware Components

- ESP32 development board
- L298N dual H-bridge motor driver
- Four BO geared DC motors
- Four-wheel robot chassis
- Rechargeable battery pack
- Connecting wires

## Pin Connections

| L298N | ESP32 |
|---|---|
| ENA | GPIO 22 |
| ENB | GPIO 23 |
| IN1 | GPIO 26 |
| IN2 | GPIO 25 |
| IN3 | GPIO 33 |
| IN4 | GPIO 32 |
| GND | GND |

The ENA and ENB jumper caps are removed because these pins are controlled by the ESP32.

## How to Use

1. Upload the Arduino sketch from the `code` folder to the ESP32.
2. Power the robot car.
3. Connect your phone or computer to the Wi-Fi network `ESP32_CAR`.
4. Enter the password `12345678`.
5. Open `http://192.168.4.1` in a browser.
6. Use the on-screen controls to operate the car.

## Project Structure

```text
ESP32-WiFi-Robot-Car/
├── README.md
└── code/
    └── esp32_wifi_car.ino
```

## Future Improvements

- Obstacle detection
- Distance measurement
- Autonomous navigation
- Sensor integration
- Battery monitoring

## Project Status

**Completed:** Basic Wi-Fi-controlled 4WD movement.

## Author

Mohak Sharma

GitHub: [@themohaksharma](https://github.com/themohaksharma)
