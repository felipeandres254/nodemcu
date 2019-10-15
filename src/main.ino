#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#ifndef WIFI_SSID
#define WIFI_SSID "YOUR-SSID"
#define WIFI_PSK  "YOUR-PASSWORD"
#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PSK;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup() {
    NodeMCU_setup();

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
    }

    httpUpdater.setup(&httpServer);
    httpServer.begin();
}

void loop() {
    httpServer.handleClient();

    NodeMCU_loop();
}
