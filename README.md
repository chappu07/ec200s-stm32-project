STM32 + EC200S MQTT Temperature & Humidity Publisher

This project demonstrates publishing DHT11 temperature and humidity data to an MQTT broker using an STM32 microcontroller and the Quectel EC200S LTE Cat-1 module via AT commands.

It uses raw AT command control, not any high-level MQTT library.
If you’re scared of AT commands, stop reading now.

Hardware Used
4
Component	Purpose
STM32 (Blue Pill / compatible)	Main controller
Quectel EC200S	LTE Cat-1 cellular modem
DHT11	Temperature & humidity sensor
FTDI USB-TTL	Debug output
SIM card (Airtel tested)	Cellular connectivity
System Overview

Data flow (simple and deterministic):

STM32 reads temperature & humidity from DHT11

STM32 communicates with EC200S over UART

EC200S establishes cellular data session

MQTT connection is opened to a public broker

Sensor data is published as JSON every 5 seconds

No cloud SDKs. No abstraction layers. Full control.

Pin Configuration
DHT11
Signal	STM32 Pin
DATA	PA5
UART Connections
Purpose	STM32 Pins	Device
UART3	PB11 (RX), PB10 (TX)	EC200S
UART2	PA3 (TX), PA2 (RX)	FTDI (Debug)
MQTT Configuration
Parameter	Value
Broker	broker.hivemq.com
Port	1883
Topic	test/safwan
Client ID	stm32client
Payload format	JSON

Example payload:

{"temp":29.4,"hum":63.0}

Cellular APN

This project is tested with Airtel India.

APN: airtelgprs.com


If you’re using a different carrier and it doesn’t work, that’s not a bug—update the APN.

Software Requirements

Arduino IDE

STM32 Arduino Core (STM32duino)

DHT Sensor Library

Install STM32 core via Boards Manager:

https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json

How It Works (AT Command Flow)

The EC200S is controlled entirely via AT commands:

SIM & network checks

AT
AT+CPIN?
AT+CREG?
AT+CGATT?


PDP context configuration

AT+QICSGP=1,1,"airtelgprs.com","","",1
AT+QIACT=1


MQTT setup and connection

AT+QMTOPEN
AT+QMTCONN


Data publish

AT+QMTPUB


If you don’t understand these commands, learn them—don’t blindly copy code.

Expected Serial Output

On successful operation, you should see:

MQTT CONNECTED!
Published: {"temp":28.7,"hum":61.2}


If you don’t see >, your broker or network is blocking publishes.
This is not an Arduino issue.

Common Failure Causes (Read This Before Asking)
Problem	Cause
No MQTT publish	Broker policy or blocked port
No network	Wrong APN or weak signal
DHT FAILED	Bad wiring or fake DHT11
Random hangs	Power supply instability

Power the EC200S properly. USB power is not enough.

Why This Project Matters

Teaches real embedded networking

Uses industrial LTE module.

Shows direct MQTT control without libraries

Suitable base for IoT products, not demos

If you think this is “just another Arduino project,” you missed the point.

Future Improvements (Do These Instead of Bragging)

QoS 1 publish

Last Will & Testament

Dynamic topic naming

Sensor fault reporting

Power-fail recovery

Secure MQTT (TLS)

Author

Mahammad Safwan T
Electronics & Communication Engineering
Embedded Systems | IoT | Cellular Networking
