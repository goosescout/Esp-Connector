#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <Adafruit_NeoPixel.h>

#define SERVER_NAME "ESP32_server"
#define SERVER_PASSWORD "#PhVWEK8Msjvk"

#define SERVER_ACCESS "http://192.168.4.1/"

#define LED_PIN 2
#define LED_COUNT 4
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN);

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

    strip.begin();
    strip.setPixelColor(2, 255, 255, 255);
    strip.show();
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

            String x, y;
            bool write_to_x = true;

            for (int i = 0; i < payload.length(); i++) {
                char symbol = payload[i];
                if (symbol == ' ') {
                    write_to_x = false;
                } else if (write_to_x) {
                    x = x + symbol;
                } else {
                    y = y + symbol;
                }
            }
            x = x.toInt();
            y = y.toInt();
            Serial.print("X: ");
            Serial.print(x);
            Serial.print(" Y: ");
            Serial.println(y);
        } else {
            Serial.print("Error: ");
            Serial.println(httpCode);
        }
        http.end();
    }
    delay(500);
}
