#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <Adafruit_NeoPixel.h>

#define SERVER_NAME "ESP32_server"
#define SERVER_PASSWORD "#PhVWEK8Msjvk"

#define SERVER_ACCESS "http://192.168.4.1/"

#define LED_PIN 2 // TODO: change to 9
#define LED_COUNT 4
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN);

#define ENGINE_PIN 4 // TODO: change to 10
#define SOLAR_ENABLE_PIN 15
#define ANT_PIN 14

#define SOLAR_ANALOG_PIN A0

bool wifi_initialized = false;
bool LED_initialized = false;

void updateLED(int x, int y) {
    Serial.println(x);
    Serial.println(y);
    if (x > 2000) {  // back
        double norm = (x - 2000.0) / 2095.0 * 255.0;
        int int_norm = static_cast<int>(norm);
        strip.setPixelColor(3, int_norm, int_norm, int_norm);
    } else {
        strip.setPixelColor(3, 0, 0, 0);
    }
    if (x < 1500) {  // forward
        double norm = (x - 1500.0) / -1500.0 * 255.0;
        int int_norm = static_cast<int>(norm);
        strip.setPixelColor(2, int_norm, int_norm, int_norm);
    } else {
        strip.setPixelColor(2, 0, 0, 0);
    }
    if (y > 2000) {  // right
        double norm = (y - 2000.0) / 2095.0 * 255.0;
        int int_norm = static_cast<int>(norm);
        strip.setPixelColor(1, int_norm, int_norm, int_norm);
    } else {                
        strip.setPixelColor(1, 0, 0, 0);
    }
    if (y < 1500) { // left
        double norm = (y - 1500.0) / -1500.0 * 255.0;
        int int_norm = static_cast<int>(norm);
        strip.setPixelColor(0, int_norm, int_norm, int_norm);
    } else {
        strip.setPixelColor(0, 0, 0, 0);
    }  
    strip.show();
}

bool checkEngine() {
    bool engine_plugged = false;
    if (digitalRead(ENGINE_PIN) == LOW) {
        // инициализация светодиодов
        if (!LED_initialized) {
            strip.setPixelColor(0, 0, 0, 0);
            strip.setPixelColor(1, 0, 0, 0);
            strip.setPixelColor(2, 0, 0, 0);
            strip.setPixelColor(3, 0, 0, 0);
            strip.show();
            LED_initialized = true;
            Serial.println("LED initialized");
        }

        engine_plugged = true;
    } else {
        LED_initialized = false;
    }
    return engine_plugged;
}

bool checkSolar() {
    bool solar_plugged = false;
    // проверка наличия панелей и проверка освещенности
    if (digitalRead(SOLAR_ENABLE_PIN) == LOW && analogRead(SOLAR_ANALOG_PIN) > 500) {
        solar_plugged = true;
    }
    return solar_plugged;
}

bool checkAnt() {
    return true;
    bool ant_plugged = false;
    if (digitalRead(ANT_PIN) == LOW) {
        ant_plugged = true;
    }
    return ant_plugged;
}

void wifiStart() {
    if (!wifi_initialized) {
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
        wifi_initialized = true;
    }
}

void wifiStop() {
    if (wifi_initialized) {
        WiFi.disconnect();
        wifi_initialized = false;
        strip.setPixelColor(0, 0, 0, 0);
        strip.setPixelColor(1, 0, 0, 0);
        strip.setPixelColor(2, 0, 0, 0);
        strip.setPixelColor(3, 0, 0, 0);
        strip.show();
        LED_initialized = false;
        Serial.println("Wifi and LED disabled");
    }
}

void setup(void) {
    Serial.begin(115200);

    // инициализация пинов
    pinMode(ENGINE_PIN, INPUT_PULLUP);
    pinMode(SOLAR_ENABLE_PIN, INPUT_PULLUP);
    pinMode(ANT_PIN, INPUT_PULLUP);
    pinMode(SOLAR_ANALOG_PIN, INPUT_PULLUP);

    strip.begin();
}

void loop(void) {
    if (checkEngine() && checkAnt() && checkSolar()) {
        wifiStart();
        if (WiFi.status() == WL_CONNECTED) {
            WiFiClient client;
            HTTPClient http;
            http.begin(client, SERVER_ACCESS);
            int httpCode = http.GET();
            if (httpCode > 0) {
                String payload = http.getString();

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
                updateLED(x.toInt(), y.toInt());
            } else {
                Serial.print("Error: ");
                Serial.println(httpCode);
            }
            http.end();
            delay(50);
        }
    } else {
        wifiStop();
    }
}
