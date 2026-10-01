# flxplc-firmware

**Open-source ESP32-based Soft PLC firmware with MQTT connectivity, OTA updates, and real-time PLC execution.**

`flxplc-firmware` is the embedded firmware component of the **flxplc** automation platform. It aims to transform ESP32-based devices into programmable logic controllers (PLCs) that can be configured, monitored, and controlled through a cloud-based platform.

The project is built using **C++, Arduino, PlatformIO, and FreeRTOS**, following a modular architecture with independent tasks for PLC execution, communication, and system management.

> **Development Status:** Early development. The architecture and features described below represent the project's intended design. Functionality will be implemented incrementally.

## Features

The following features are planned:

- **Soft PLC Runtime:** Cyclic execution of PLC programs.
- **MQTT Communication:** Communication with the flxplc cloud platform.
- **Remote Control:** Remote PLC start, stop, and status monitoring.
- **OTA Updates:** Remote firmware updates.
- **Digital I/O:** Hardware abstraction for digital inputs and outputs.
- **Ethernet Connectivity:** Network communication using Ethernet.
- **Program Management:** Receiving, validating, storing, and executing PLC programs.
- **FreeRTOS:** Independent tasks for PLC execution, communication, and system services.

## Supported Hardware

Initial development targets the following hardware:

**Waveshare ESP32-S3-POE-ETH-8DI-8DO**

- ESP32-S3 microcontroller
- Ethernet connectivity
- 8 digital inputs
- 8 digital outputs
- USB programming and debugging

Support for additional ESP32-based devices may be introduced in the future.

## Architecture

The firmware follows a modular architecture designed to separate PLC execution from communication and system management.

Each major subsystem is encapsulated in its own module. FreeRTOS provides task scheduling and enables independent execution of time-sensitive and background operations.

### Planned Modules

| Module | Responsibility |
|---|---|
| PLCRuntime | PLC program execution and runtime state management |
| MQTTService | MQTT communication and message handling |
| EthernetService | Ethernet initialization and connection management |
| OTAService | Firmware update management |
| IOService | Hardware input and output abstraction |
| ProgramManager | PLC program validation, storage, and loading |

### Task Architecture

The planned firmware architecture separates the following responsibilities:

- **PLC Task:** Executes PLC programs using a configurable cycle time.
- **Communication Task:** Handles MQTT messages and cloud communication.
- **System Task:** Manages system status, diagnostics, and background operations.
- **OTA Task:** Handles firmware updates independently of normal communication.

Shared resources and runtime states will be protected using appropriate FreeRTOS synchronization mechanisms.

PLC execution timing and deterministic behavior will be evaluated during development.

## Project Structure

The intended PlatformIO project structure is:

```text
flxplc-firmware/
├── platformio.ini
├── include/
│   ├── config.h
│   └── pins.h
├── lib/
│   ├── PLCRuntime/
│   ├── MQTTService/
│   ├── EthernetService/
│   ├── OTAService/
│   ├── IOService/
│   └── ProgramManager/
├── src/
│   └── main.cpp
└── test/
```

Each module will provide a clearly defined interface to minimize dependencies between firmware components.

## Getting Started

### Prerequisites

- Visual Studio Code
- PlatformIO IDE extension
- ESP32-S3 development hardware
- USB connection for programming

### Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/flxplc-firmware.git
cd flxplc-firmware
```

Replace `YOUR_USERNAME` with the repository owner's GitHub username.

### Build

Open the project in Visual Studio Code using PlatformIO.

Alternatively, build the firmware using the PlatformIO CLI:

```bash
pio run
```

### Upload

Connect your ESP32 device via USB and execute:

```bash
pio run -t upload
```

### Serial Monitor

Open the serial monitor:

```bash
pio device monitor
```

The default serial monitor baud rate is 115200.

For ESP32-S3 boards using native USB, USB CDC may need to be enabled in the PlatformIO configuration.

## MQTT Integration

MQTT will serve as the primary communication protocol between the firmware and the flxplc cloud platform.

Planned capabilities include:

- Device authentication and provisioning
- Runtime status reporting
- Input and output state synchronization
- Remote execution control
- PLC program deployment
- System diagnostics

The final MQTT topic structure and message schemas will be documented as the communication protocol is implemented.

## PLC Runtime

The PLC runtime is intended to execute programs independently of network communication.

Its planned responsibilities include:

1. Reading the current input states.
2. Executing the loaded PLC program.
3. Updating the output states.
4. Publishing runtime information asynchronously.

The runtime will support the following states:

| State | Description |
|---|---|
| STOPPED | PLC program execution is stopped |
| RUNNING | PLC program execution is active |
| ERROR | An error prevents normal execution |

A defined runtime state machine will manage transitions between these states.

## Security

The planned security architecture includes:

- Device-specific authentication
- Secure MQTT communication using TLS
- Validation of received PLC programs
- Controlled firmware updates
- Secure device provisioning

Security-related functionality will be introduced and tested incrementally.

## Roadmap

- [x] Initial PlatformIO project setup
- [x] Basic ESP32 firmware execution
- [ ] Modular firmware architecture
- [ ] Ethernet connectivity
- [ ] Digital input and output abstraction
- [ ] MQTT client integration
- [ ] Device provisioning
- [ ] PLC runtime implementation
- [ ] PLC program management
- [ ] Remote RUN/STOP control
- [ ] Runtime status reporting
- [ ] OTA firmware updates
- [ ] Secure MQTT communication
- [ ] Integration with flxplc cloud

## Disclaimer

This project is under active development and is not intended for safety-critical applications.

Do not use this firmware for safety functions or applications requiring certified industrial control systems.

## License

A license has not yet been specified. Refer to the repository's LICENSE file once a license has been selected.