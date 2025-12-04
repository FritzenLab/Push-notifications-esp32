#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";

// Your ntfy topic
String ntfyTopic = "fritzenlab_esp32_supernice";

unsigned long lastSend = 0;
const unsigned long interval = 60000;  // 60 seconds

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
}

void loop() {
  unsigned long now = millis();

  if (now - lastSend >= interval) {
    lastSend = now;

    sendNotification(now/1000);
  }
}

void sendNotification(unsigned long elapsed) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Skipping...");
    return;
  }

  HTTPClient http;
  String url = "https://ntfy.sh/" + ntfyTopic;
  http.begin(url);

  http.addHeader("Title", "ESP32");
  http.addHeader("Content-Type", "text/plain");

  String message = "ESP32 elapsed: " + String(elapsed) + " s";
  Serial.println("Sending: " + message);

  int httpResponseCode = http.POST(message);

  if (httpResponseCode > 0) {
    Serial.printf("ntfy response: %d\n", httpResponseCode);
  } else {
    Serial.printf("Error sending: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}
