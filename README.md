# Wi-Fi Presence Detector with Dashboard

> A local IoT system using an Arduino R4 WiFi board to scan and report nearby Wi-Fi networks to a Flask server. Visualizes presence data for network awareness and passive monitoring.

---

## 📡 Project Overview

This project implements a Wi-Fi-based presence detection system. Arduino R4 WiFi board periodically scans for nearby wireless networks and sends details such as SSID names and signal strength (RSSI) to a Flask server. The server logs the data and displays it on a web dashboard.

It demonstrates core networking concepts like client-server architecture, IP-based communication, HTTP data transfer, and local network visibility — all without requiring physical sensors.

---

## 🎯 Objectives

- Build a local IoT system that detects nearby Wi-Fi networks
- Implement client-server communication using HTTP and JSON
- Log and visualize scanned network data
- Understand passive network awareness (only observing network information)

---

## 🧱 Network Architecture

- **Arduino R4 WiFi Clients**: Actively scan for nearby Wi-Fi networks and reports results
- **Flask Server**: Receives JSON data and logs/displays it
- **Wi-Fi Router**: (For local server) Provides the LAN and IP routing

For the local server all components are within the same private LAN and communicate over port 5000.

---

## ⚙️ Tools & Technologies

- Arduino IDE + WiFiS3 Library
- Arduino R4 WiFi board
- Python + Flask
- JSON file for storing scan logs
- Render (Cloud Application Platform)

---

## 🔄 Communication Flow

1. Arduino performs a Wi-Fi scan every couple of seconds
2. It collects SSID names, RSSI values, and Encryption Type
3. Sends the data as a JSON POST to `/upload` on the Flask server
4. The server logs the data and displays it on the dashboard

