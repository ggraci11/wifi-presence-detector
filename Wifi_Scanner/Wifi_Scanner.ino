#include <WiFiS3.h>
#include <WiFiSSLClient.h>
#include <ArduinoHttpClient.h>

char ssid[] = "TMOBILE-B811";     // your Wi-Fi network name
char pass[] = "cda462a7c9";       // your Wi-Fi password

char serverAddress[] = "wifi-presence-detector.onrender.com";
int port = 443; // HTTPS port

int status = WL_IDLE_STATUS;
WiFiSSLClient wifi; // SSL client for HTTPS
HttpClient client = HttpClient(wifi, serverAddress, port);

String deviceId = "Arduino R4"; // Unique ID for device

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Connecting to WiFi");
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(2000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  printWiFiStatus();
}

void loop() {
  scanAndSend();
  delay(5000); // 5 seconds between scans
}

void scanAndSend() {
  Serial.println("Scanning WiFi networks...");
  int numNetworks = WiFi.scanNetworks();

  if (numNetworks == -1) {
    Serial.println("Scan failed");
    return;
  }

  String json = "{";
  json += "\"device_id\":\"" + deviceId + "\",";
  json += "\"timestamp\":\"" + getISOTime() + "\",";
  json += "\"networks\":[";

  for (int i = 0; i < numNetworks; i++) {
    json += "{";
    json += "\"ssid\":\"" + String(WiFi.SSID(i)) + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
    json += "\"encryption\":\"" + encryptionTypeString(WiFi.encryptionType(i)) + "\"";
    json += "}";
    if (i < numNetworks - 1) json += ",";
  }

  json += "]}";

  Serial.println("Sending JSON:");
  Serial.println(json);

  client.beginRequest();
  client.post("/upload");
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", json.length());
  client.beginBody();
  client.print(json);
  client.endRequest();

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

String getISOTime() {
  // Placeholder timestamp (you can integrate NTP if needed)
  return "2025-04-16T20:00:00";
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

String encryptionTypeString(int thisType) {
  switch (thisType) {
    case ENC_TYPE_WEP: return "WEP";
    case ENC_TYPE_TKIP: return "WPA";
    case ENC_TYPE_CCMP: return "WPA2";
    case ENC_TYPE_NONE: return "Open";
    case ENC_TYPE_AUTO: return "Auto";
    default: return "Unknown";
  }
}
