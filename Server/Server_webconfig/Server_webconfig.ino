#include <WiFi.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>

#define SERVER_NAME "ESP32_server"
#define SERVER_PASSWORD "#PhVWEK8Msjvk"

AsyncWebServer server(80);

const int SW_pin = 17;
const int X_pin = 35;
const int Y_pin = 34;

void getResponce(AsyncWebServerRequest *request) {
    String responce;
    responce = responce + analogRead(X_pin) + " " + analogRead(Y_pin);
    request->send(200, "text/plain", responce);
}

void setup(void) {
    Serial.begin(115200);
    Serial.println("Starting ESP32 server...");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(SERVER_NAME, SERVER_PASSWORD);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/", HTTP_GET, getResponce);

    server.begin();

    pinMode(X_pin, INPUT);
    pinMode(Y_pin, INPUT);
    pinMode(SW_pin, INPUT_PULLUP);
    digitalWrite(SW_pin, HIGH);
}

void loop(void) {

}
