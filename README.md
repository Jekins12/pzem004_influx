# ESP8266/ESP32 Energy Monitor with PZEM004T and InfluxDB
This project uses an ESP8266 or ESP32 microcontroller to monitor electrical parameters via a PZEM004Tv30 sensor and log the data to an InfluxDB database. Additionally, an HTTP server is hosted on the ESP device, providing endpoints for device discovery and energy reset.

## Features

- Real-time Monitoring: Reads voltage, current, power, frequency, power factor, and energy consumption from the PZEM004T sensor.
- Data Logging: Sends sensor data to InfluxDB for storage and visualization.
- HTTP API: Exposes HTTP endpoints to retrieve device info and reset energy measurements.

## Requirements

-### Hardware:
- ESP8266 or ESP32
- PZEM004Tv30 sensor
- InfluxDB server
  
-### Libraries:

- ESP8266WebServer
- ArduinoJson
- InfluxDbClient
- PZEM004Tv30
