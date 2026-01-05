# STM32–EC200S MQTT Temperature and Humidity Publisher

## Overview

This project implements an IoT data publishing system using an STM32
microcontroller, a Quectel EC200S LTE Cat-1 cellular module, and a DHT11
temperature and humidity sensor. Sensor data is published periodically
to a public MQTT broker using AT commands over UART communication.

The implementation directly controls the cellular modem without using
high-level MQTT libraries, making it suitable for embedded and cellular
IoT applications.

## Features

- Temperature and humidity measurement using DHT11
- Cellular connectivity via EC200S LTE Cat-1 module
- MQTT communication using AT commands
- JSON-formatted sensor data publishing
- UART-based serial debugging

## Hardware Requirements

- STM32 microcontroller (STM32F103 or compatible)
- Quectel EC200S LTE Cat-1 module
- DHT11 temperature and humidity sensor
- FTDI USB to TTL converter
- SIM card with data plan
- Stable external power supply

## Pin Configuration

DHT11 Connection:
- DATA pin connected to PA5

UART Connections:
- UART3 (PB11 RX, PB10 TX) connected to EC200S
- UART2 (PA3 TX, PA2 RX) connected to FTDI for debugging

## MQTT Configuration

Broker   : broker.hivemq.com  
Port     : 1883  
Topic    : test/safwan  
Client ID: stm32client  

Payload format (JSON example):
{"temp":28.6,"hum":62.0}

## Cellular Network Configuration

The project is tested with Airtel (India).

APN: airtelgprs.com

For other network providers, update the APN accordingly in the source code.

## Software Requirements

- Arduino IDE
- STM32 Arduino Core (STM32duino)
- DHT Sensor Library

STM32 Board Manager URL:
https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json

## Working Principle

1. STM32 initializes the DHT sensor and UART interfaces
2. EC200S registers to the cellular network
3. PDP context is activated using APN configuration
4. MQTT connection is established with the broker
5. Sensor data is read and published every 5 seconds

All modem interactions are handled using AT commands.

## Expected Serial Output

=== EC200S MQTT (PUBLIC BROKER) BEGIN ===
MQTT CONNECTED!
Published: {"temp":29.1,"hum":60.4}

## Notes and Limitations

- EC200S requires a stable power supply with sufficient current
- Incorrect APN settings will prevent network attachment
- DHT11 sensor readings may fail due to wiring issues
- MQTT publish may fail if the broker restricts the connection

## Future Enhancements

- Support for MQTT QoS levels
- Secure MQTT communication (TLS)
- Automatic reconnection handling
- Additional sensor integration
- Power optimization

## Author

Safwan T  
Electronics and Communication Engineering  
Embedded Systems and IoT
