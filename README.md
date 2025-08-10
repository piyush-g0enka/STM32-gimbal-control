# Freefly API IMU Control - STM32F3 Discovery

## Overview
This project implements a control loop on the STM32F3-Discovery board to read accelerometer and gyroscope data and control a Freefly gimbal using the QX Protocol. The system computes Tilt and Roll angles from the accelerometer and sends absolute position commands to the gimbal in real-time.

[Watch on YouTube]([Link](https://www.youtube.com/watch?v=snZTMOmNK4Y))

## Project Structure
- **Src/**: Source code (main.c and protocol files)
- **Inc/**: Header files
- **QX_Protocol/**: Freefly QX protocol implementation
- **Drivers/**: STM32 HAL and board-specific drivers

## Key Features
- Reads accelerometer and gyroscope data.
- Computes Tilt and Roll angles using trigonometric relations.
- Applies a low-pass filter for noise reduction.
- Sends position commands to the gimbal via FreeflyAPI.
- Control loop frequency set to 50Hz for stable operation.

## Modifications Summary
- Implemented Tilt and Roll angle calculation functions in `main.c`.
- Added low-pass filters to smoothen accelerometer data.
- Clamped control commands to Tilt (±55°) and Roll (+110°,-90°).
- Adjusted control loop frequency from 100Hz to 50Hz for stability.
- Fixed Gyroscope initialization by correcting device ID in `l3gd20.h` (changed I_AM_L3GD20 from 0xD4 to 0xD3).

## Build Instructions
1. Open the project in **Keil uVision5**.
2. Ensure the target is set to **STM32F303VC**.
3. Build the project (Project -> Build Target).

## Flash Instructions
1. Connect the STM32F3-Discovery board via USB.
2. Click **Download** in Keil to flash the firmware.

## Test Instructions
1. Power on the Freefly gimbal and ensure UART connection (Baudrate 111111).
2. After flashing, the gimbal will respond to board tilt and roll movements.
3. Use **Keil Debugger (Watch Window)** to monitor live Tilt and Roll angles.
4. Ensure smooth gimbal response with no vibrations.

## Known Limitations
- Pan (Yaw) control is not implemented as it requires a magnetometer for drift correction.
- Long-term Tilt/Roll drift compensation using gyroscope is not included.

## Files Modified
- **main.c**: Core control loop logic, angle calculations, filtering, and API integration.
- **main.h**: Added function declarations for lowpass_filter, get_tilt_angle and get_roll_angle.
- **l3gd20.h**: Corrected gyroscope device ID.

