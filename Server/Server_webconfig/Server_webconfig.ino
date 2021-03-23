#include <WiFi.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>

#define SERVER_NAME "ESP32_server"
#define SERVER_PASSWORD "#PhVWEK8Msjvk"

AsyncWebServer server(80);

void setup(void) {
    Serial.begin(115200);
    Serial.println("Starting ESP32 server...");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(SERVER_NAME, SERVER_PASSWORD);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "success");
    });

    server.begin();
}

void loop(void) {

}
