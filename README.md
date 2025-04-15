# Wi-Fi Presence Detector with Local Dashboard

> A local IoT system using an Arduino R4 WiFi board to scan and report nearby Wi-Fi networks to a Flask server. Visualizes presence data for network awareness and passive monitoring.

---

## 📡 Project Overview

This project implements a Wi-Fi-based presence detection system over a private LAN. Arduino R4 WiFi boards periodically scan for nearby wireless networks and send details such as SSID names and signal strength (RSSI) to a local Flask server. The server logs the data and optionally displays it on a web dashboard.

It demonstrates core networking concepts like client-server architecture, IP-based communication, HTTP data transfer, and local network visibility — all without requiring physical sensors.

---

## 🎯 Objectives

- Build a local IoT system that detects nearby Wi-Fi networks
- Implement client-server communication using HTTP and JSON
- Log and visualize scanned network data for each Arduino node
- Explore passive network awareness in a secure LAN setup

---

## 🧱 Network Architecture

- **Arduino R4 WiFi Clients**: Actively scan for nearby Wi-Fi networks and report results
- **Flask Server**: Receives JSON data and logs or displays it
- **Wi-Fi Router**: Provides the LAN and IP routing

All components are within the same private LAN and communicate over port 5000.

---

## ⚙️ Tools & Technologies

- Arduino IDE + WiFiS3 Library
- Arduino R4 WiFi board
- Python + Flask
- Wireshark for network traffic analysis
- (Maybe): SQLite or JSON file for storing scan logs

---

## 🔄 Communication Flow

1. Arduino performs a Wi-Fi scan every 10–30 seconds
2. It collects SSID names and RSSI values
3. Sends the data as a JSON POST to `/upload` on the Flask server
4. The server logs the data (and optionally displays it on a dashboard)

