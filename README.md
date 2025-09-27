# ESP32-Security-system
This ESP32 project implements a smart security system with a PIR motion sensor, RFID authentication, and Bluetooth control. When the sensor detects movement, the system evaluates the activity and warns the user via Bluetooth. The user can trigger the alarm or ignore it, while only an authorized RFID card can deactivate the alarm.

# Requirements
## Hardware
- 1 x ESP32 DevKit v1 <br>
- 2 × LEDs<br>
- 2 × 220 Ω resistor<br>
- 1 × 1 kΩ resistor<br>
- 1 × NPN transistor<br>
- 1 × active buzzer<br>
- 1 × RFID module<br>
- 1 × PIR motion sensor<br>
- 1 x USB cable
- 1 x RFID card
- 1 x Breadboard
- Connecting wires

## Software
- Arduino IDE (or PlatformIO)
- Android phone with Serial Bluetooth Terminal app

## Notes
- No Wi-Fi or serial port configuration required
- Each RFID card has a unique UID that must be entered in the code


# Circuit Diagram
<img width="1780" height="844" alt="image" src="https://github.com/user-attachments/assets/50c453f7-b8fd-4ad9-88dd-41e19589bdd9" />

# Preparing the ESP32
1. Open the project folder in the development environment.
2. All necessary libraries (MFRC522_I2C, Wire) are already included in the folder, so no additional installation is required.
3. Connect the ESP32 to your PC via USB.
4. Compile and upload the project to the board.

# Testing:
1. Open the Bluetooth terminal app on your Android phone (Serial Bluetooth Terminal).
2. Connect to the device named ESP32Security.
3. You should receive notifications when the PIR sensor detects movement.
4. Send an alarm or ignore command.
5. Hold an authorised RFID card up to the RFID module when the alarm is active. The alarm will only stop if the card's UID matches the authorised UID. 

# Features
- Detects motion via PIR sensor
- Alerts user via Bluetooth
- Remote control via `alarm` or `ignore` commands
- Access control using authorized RFID cards only
- Visual (LED) and audio (buzzer) alarm

