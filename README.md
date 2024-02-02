# Project Title

## Overview

This repository contains the code and documentation for a project that interfaces an Arduino with an MPU6050 sensor, implementing a Kalman filter to process the data. 
The processed data is transmitted via the MCP2515 CAN controller to a Texas Instruments F28069M Launchpad, which controls two BLDC motors through the BOOSTXL-DRV8301 motor driver.

## Components

- Arduino
- MPU6050 sensor
- MCP2515 CAN controller
- F28069M Launchpad
- BOOSTXL-DRV8301 motor driver

## Software Requirements

- Arduino IDE
- Code Composer Studio for C2000 (for F28069M)
- Relevant libraries for MPU6050, and MCP2515

## Setup

1. **Arduino Setup:**
   - Connect the MPU6050 sensor to the Arduino.
   - Install the necessary libraries for the MPU6050 sensor and MCP2515 CAN controller in the Arduino IDE.
   - Upload the Arduino code (`MPU6050_Kalman.ino`) to the Arduino board.

2. **F28069M Setup:**
   - Install Code Composer Studio for C2000.
   - Set up the F28069M Launchpad with the BOOSTXL-DRV8301 motor driver.
   - Import the F28069M code into Code Composer Studio.
   - Build and flash the code onto the F28069M Launchpad.

3. **CAN Communication:**
   - Ensure proper wiring and configuration for CAN communication between Arduino and F28069M.
   - Adjust CAN bus parameters in both Arduino and F28069M codes for compatibility.

## Running the Project

1. Power on the Arduino and F28069M Launchpad.

2. The Arduino reads data from the MPU6050 sensor, applies a Kalman filter, and transmits the processed data over the CAN bus.

3. The F28069M Launchpad receives the CAN messages, interprets the data, and controls the BLDC motors accordingly.

