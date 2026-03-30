🔐 Dual Microcontroller Door Locker Security System

📌 Overview
This project implements a secure smart door locking system using dual microcontrollers (ATmega32) with password authentication. The system is designed with a layered architecture and separates responsibilities between:
- HMI_ECU → Handles user interaction
- Control_ECU → Handles processing, validation, and hardware control
The system ensures high security using EEPROM storage, motion detection, and lockout mechanisms.

🎯 Project Objective
Build a reliable and secure embedded system that:
- Authenticates users via password
- Controls door access using a motor
- Detects motion using PIR sensor
- Prevents brute-force attacks via lockout mechanism

🧠 System Architecture
🔹 HMI_ECU (User Interface)
- LCD (2x16)
- Keypad (4x4)
- UART Communication
🔹 Control_ECU (Processing Unit)
- EEPROM (I2C)
- DC Motor (H-Bridge + PWM)
- PIR Sensor
- Buzzer
- Timer + UART

⚙️ Features
- Password-based authentication (stored in EEPROM)
- Password confirmation during setup
- Motorized door control (unlock/lock)
- Motion detection using PIR sensor
- Buzzer alarm on multiple failed attempts
- Password change functionality
- Security lock after 3 incorrect attempts (1 minute lockout)
- UART communication between ECUs

🔄 System Workflow
1️⃣ Setup Password
- User enters a 5-digit password
- Re-enters for confirmation
- Saved in EEPROM if matched
2️⃣ Main Menu
- ➕ Open Door
- ➖ Change Password
3️⃣ Open Door
- Enter password
    If correct:
    Motor rotates (unlock)
    Wait until PIR detects no motion
    Motor rotates back (lock)
4️⃣ Change Password
- Verify old password
- Repeat setup process
5️⃣ Security Mechanism
- 3 wrong attempts →
    Buzzer ON for 1 minute
    System locked (no input)

🔌 Hardware Components
- ATmega32 (x2)
- LCD 16x2
- Keypad 4x4
- External EEPROM
- DC Motor + H-Bridge
- PIR Motion Sensor
- Buzzer

🧩 Drivers Used
- GPIO
- UART (Configurable using struct)
- I2C (TWI)
- Timer (Interrupt-based + Callback)
- PWM (Timer0 Fast PWM)
- LCD
- Keypad
- EEPROM
- PIR
- DC Motor
 
⏱️ System Specifications
- MCU: ATmega32
- Clock: 8 MHz
- Communication: UART
- Storage: External EEPROM (I2C)

🚀 How to Run
- Open project in Atmel Studio / AVR-GCC
- Compile both ECUs separately
- Simulate using Proteus
- Connect UART between both ECUs
- Run and interact via keypad
