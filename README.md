# QR_Code-Item-Scanner_Using_ESP32
--QR code item scanner using an ESP32 microcontroller--
## Overview
This repository contains an implementation of a QR code item scanner using an ESP32 microcontroller. The project is designed for a supermarket scenario where items with QR codes are scanned, and the total cost of the selected items is calculated. The QR code scanner is interfaced with an ESP32 microcontroller, and the scanned data is processed to update item counts and total cost in real-time.

## Features
QR code scanning for items (e.g., Apple, Biscuit, Soap).
Real-time updating of item counts and total cost.
Utilizes the ESP32 microcontroller and the ESP32QRCodeReader library for QR code decoding.
Simple web interface to display item counts, individual prices, and the total cost.
## QR Code Scanner Setup
Connect the ESP32 microcontroller to the appropriate pins and components.
Configure the Wi-Fi credentials in the code (ssid and password variables).
Upload the code to the ESP32 using the Arduino IDE or platform of your choice.
## Items and Prices
Apple: $10.00
Biscuit: $15.00
Soap: $20.00
## How It Works
The ESP32 continuously scans for QR codes using the QR code reader.
When a valid QR code is detected, the corresponding item count is updated.
If an item is scanned twice, it is removed from the list, and the count is decremented.
The web interface displays the current item counts, individual prices, and the total cost.
## How to Run
Power on the ESP32 device.
Connect to the ESP32 Wi-Fi network.
Open a web browser and navigate to the ESP32 IP address (e.g., http://192.168.1.2).
Scan QR codes to add items, and the web interface will update in real-time.
## Dependencies
ESP32QRCodeReader library.
# Contributing
Feel free to contribute to this project by creating issues, suggesting improvements, or submitting pull requests. Your feedback and contributions are welcome!

## License
This project is licensed under the MIT License.
