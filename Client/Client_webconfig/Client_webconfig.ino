#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define SERVER_NAME "ESP32_server"
#define SERVER_PASSWORD "#PhVWEK8Msjvk"

#define SERVER_ACCESS "http://192.168.4.1/"

void setup(void) {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(SERVER_NAME, SERVER_PASSWORD);
    Serial.print("Connecting...");
    while (WiFi.status() != WL_CONNECTED) { 
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}

void loop(void) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, SERVER_ACCESS);
        int httpCode = http.GET();
        if (httpCode > 0) {
            String payload = http.getString();
            Serial.print("Message recieved: ");
            Serial.println(payload);
        }
        http.end();
    }
    delay(1000);
}
