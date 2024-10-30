#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>
#include "config.h"


// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Declare Data points
Point pzemData("pzem_data");

// PZEM004Tv30 setup
PZEM004Tv30 pzem(14, 12);

ESP8266WebServer server(80);

unsigned long previousMillis = 0;

void setup() {

Serial.begin(115200);


  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  // Define HTTP endpoints
  server.on("/discover", HTTP_GET, handleDiscover);
  server.on("/resetEnergy", HTTP_GET, handleResetEnergy);

  // Start server
  server.begin();
  Serial.println("HTTP server started. IP: "+ WiFi.localIP().toString());
}

void loop() {

  server.handleClient();
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= INTERVAL){

    previousMillis = currentMillis;
  
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float frequency = pzem.frequency();
    float pf = pzem.pf();
    float energy = pzem.energy();
    //Serial.println(power);
    //Serial.println(energy,5);
  
    if (!isnan(voltage) && !isnan(current) && !isnan(power) && !isnan(energy) && !isnan(frequency) && !isnan(pf)) {
      
      pzemData.addField("voltage", voltage);
      pzemData.addField("current", current);
      pzemData.addField("power", power);
      pzemData.addField("energy", energy);
      pzemData.addField("frequency", frequency);
      pzemData.addField("pf", pf);
  
      // Send data to InfluxDB
      if (!client.writePoint(pzemData)) {
        Serial.print("InfluxDB write failed: ");
        Serial.println(client.getLastErrorMessage());
      } else {
        Serial.println("Data sent to InfluxDB");
      }
    } else {
      Serial.println("Error reading data from PZEM004");
    }
  }
  
}

void handleDiscover() {
  StaticJsonDocument<200> doc;
  doc["name"] = "ESP8266 Energy";
  doc["ip_address"] = WiFi.localIP().toString(); // Add the IP address
  doc["mac_address"] = WiFi.macAddress(); // Add the MAC address
  JsonArray endpoints = doc.createNestedArray("endpoints");
  //endpoints.add("/discover");
  endpoints.add("/resetEnergy");

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleResetEnergy() {
  pzem.resetEnergy();
  server.send(200, "text/plain", "The energy value has been set to 0");
}
