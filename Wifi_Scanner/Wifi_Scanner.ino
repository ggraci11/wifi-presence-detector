#include <WiFiS3.h>
#include <WiFiClient.h>
#include <WiFiSSLClient.h>
#include <ArduinoHttpClient.h>

char ssid[] = "TMOBILE-B811";     // network SSID (name)
char pass[] = "**********"; // network password 

char serverAddress[] = "192.168.12.197"; // Flask server IP
int port = 5000;

int status = WL_IDLE_STATUS;
WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

String deviceId = "esp-r4-node-01"; // Unique device ID

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Connecting to WiFi...");
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
  delay(15000); // Wait 15 seconds between scans
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
    json += "\"rssi\":" + String(WiFi.RSSI(i));
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
  // This is a dummy time since Arduino R4 WiFi has no RTC or NTP by default.
  return "2025-04-14T16:00:00";
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}
