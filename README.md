# Flood Early Warning System

An IoT-based flood early warning system using ESP32 and wireless communication to monitor real-time water levels, generate alerts, and provide live dashboard visualization.

---

## Abstract
This project implements a real-time Flood Early Warning System using ESP32-based sensor nodes and a centralized controller unit. Water level data is measured using ultrasonic sensors and transmitted wirelessly using ESP-NOW. The controller unit evaluates risk levels, generates visual and audible alerts, and hosts a real-time web dashboard for monitoring.

---

## Problem Statement
Flood disasters often occur due to the absence of real-time, localized monitoring and early warning mechanisms. A low-cost, scalable, and wireless solution is required to provide timely flood alerts based on actual water level measurements.

---

## Objectives
- To design a real-time flood monitoring system using IoT
- To acquire water level data from remote locations
- To transmit data wirelessly with low latency
- To evaluate flood risk levels centrally
- To provide real-time alerts and dashboard visualization

---

## Features
- Real-time water level sensing
- ESP-NOW based low-latency wireless communication
- Centralized controller with alert logic
- LED and buzzer based warning system
- Live web-based monitoring dashboard
- Modular design scalable to multiple sensor nodes
- Custom-designed PCBs and enclosures

---

## System Overview
The system consists of:
- **Data Unit (Transmitter):** Measures water level and sends data wirelessly  
- **Controller Unit (Receiver):** Receives data, evaluates risk, triggers alerts, and hosts the dashboard  

---

## Hardware Used

### Controller Unit
- ESP32-C3 microcontroller
- LED indicators (Safe / Warning / Danger)
- Buzzer for audible alerts
- Power management and battery charging circuit
- Custom PCB and enclosure

### Data Unit (Sub-Unit)
- ESP32 microcontroller
- Ultrasonic water level sensor (HC-SR04 type)
- Power management and battery charging circuit
- Custom PCB and enclosure

---

## Firmware

### Communication Protocol
- **ESP-NOW** for sensor data transmission
- Fixed Wi-Fi channel for reliable communication
- One-to-one communication (extendable to multi-node)

### Transmitter (Data Unit)
- Measures water level using ultrasonic sensor
- Sends distance data periodically via ESP-NOW

**Libraries Used:**
- `WiFi.h`
- `esp_now.h`
- `esp_wifi.h`

### Receiver (Controller Unit)
- Receives sensor data via ESP-NOW
- Determines flood risk level
- Controls LEDs and buzzer
- Hosts a real-time web dashboard

**Libraries Used:**
- `WiFi.h`
- `esp_now.h`
- `esp_wifi.h`
- `WebServer.h`

---

## How It Works
1. The data unit measures water level using an ultrasonic sensor  
2. Measured distance is transmitted wirelessly via ESP-NOW  
3. The controller unit receives the data and evaluates risk levels  
4. LEDs and buzzer indicate Safe, Warning, or Danger conditions  
5. A web dashboard displays real-time distance and alert status  

---

## Real-Time Dashboard
The controller unit hosts a built-in web server that provides:
- Live water level display
- Flood status indication (Safe / Warning / Danger)
- Real-time distance trend graph



---

## Getting Started

### Prerequisites
- Arduino IDE
- ESP32 board package installed
- Two ESP32 boards (Data Unit & Controller Unit)

### Steps
1. Clone this repository
2. Upload `data_unit.ino` to the data unit ESP32
3. Upload `controller_unit.ino` to the controller unit ESP32
4. Configure Wi-Fi credentials in the controller firmware
5. Power both units
6. Access the dashboard using the controller’s IP address

---

## Demonstration
The system was successfully demonstrated by sensing real-time water level changes, transmitting data wirelessly, visualizing data on a live dashboard, and generating alerts based on predefined thresholds.



---

## Applications
- Flood early warning systems
- Disaster management
- Smart water monitoring
- Environmental sensing networks

---

## License
This project is licensed under the **MIT License**.
