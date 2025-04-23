#include <WiFiS3.h>
#include <WiFiClient.h>
#include <WiFiSSLClient.h>
#include <ArduinoHttpClient.h>

char ssid[] = "TMOBILE-B811";     // Your network SSID
char pass[] = "cda462a7c9";        // Your network password

char serverAddress[] = "192.168.12.197"; // Flask server IP
int port = 5000;

int status = WL_IDLE_STATUS;
WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

String deviceId = "esp-r4-node-01"; // Unique device ID

void setup() {
  Serial.begin(115200);
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
  delay(2000); // Scan every 2 seconds
}

void scanAndSend() {
  Serial.println("Scanning WiFi networks...");
  int numNetworks = WiFi.scanNetworks();

  if (numNetworks <= 0) {
    Serial.println("Scan failed or no networks found.");
    return;
  }

  String json = buildJson(numNetworks);

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

  if (statusCode != 200) {
    Serial.println("⚠️ Warning: Failed to upload scan data!");
  }
}

String buildJson(int numNetworks) {
  String json = "{";
  json += "\"device_id\":\"" + deviceId + "\",";
  json += "\"timestamp\":\"" + getISOTime() + "\",";
  json += "\"networks\":[";

  for (int i = 0; i < numNetworks; i++) {
    json += "{";
    json += "\"ssid\":\"" + escapeJson(WiFi.SSID(i)) + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
    json += "\"encryption\":\"" + encryptionTypeString(WiFi.encryptionType(i)) + "\"";
    json += "}";
    if (i < numNetworks - 1) json += ",";
  }

  json += "]}";
  return json;
}

// Dummy timestamp (or replace with real if you add NTP later)
String getISOTime() {
  return "2025-04-16T00:00:00Z";
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

// Escape quotes inside SSIDs
String escapeJson(String input) {
  input.replace("\"", "\\\"");
  return input;
}

// Helper function to translate encryption type
String encryptionTypeString(int type) {
  switch (type) {
    case ENC_TYPE_WEP: return "WEP";
    case ENC_TYPE_TKIP: return "WPA";
    case ENC_TYPE_CCMP: return "WPA2";
    case ENC_TYPE_NONE: return "Open";
    case ENC_TYPE_AUTO: return "Auto";
    default: return "Unknown";
  }
}
