# ESP8266/ESP32 Energy Monitor with PZEM004T and InfluxDB
This project uses an ESP8266 or ESP32 microcontroller to monitor electrical parameters via a PZEM004Tv30 sensor and log the data to an InfluxDB database. Additionally, an HTTP server is hosted on the ESP device, providing endpoints for device discovery and energy reset.

## Features

- Real-time Monitoring: Reads voltage, current, power, frequency, power factor, and energy consumption from the PZEM004T sensor.
- Data Logging: Sends sensor data to InfluxDB for storage and visualization.
- HTTP API: Exposes HTTP endpoints to retrieve device info and reset energy measurements.

## Requirements

### Hardware:
- ESP8266 or ESP32
- PZEM004Tv30 sensor
- InfluxDB server
  
### Libraries:

- ESP8266WebServer
- ArduinoJson
- InfluxDbClient
- PZEM004Tv30

## Configuration

`#define WIFI_SSID "your_wifi_ssid"`
`#define WIFI_PASSWORD "your_wifi_password"`
`#define INFLUXDB_URL "your_influxdb_url"`
`#define INFLUXDB_ORG "your_influxdb_org"`
`#define INFLUXDB_BUCKET "your_influxdb_bucket"`
`#define INFLUXDB_TOKEN "your_influxdb_token"`
`#define TZ_INFO "your_timezone_info"`

## Instalation
1. Clone this repository to your local machine.
2. Install the required libraries listed above in your Arduino IDE.
3. Upload the code to your ESP8266 or ESP32.

## API Endpoints
- `GET /discover`: Returns device name, IP address, MAC address, and available endpoints.
- `GET /resetEnergy`: Resets the energy meater to 0.

