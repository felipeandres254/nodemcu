#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#ifndef STASSID
#define STASSID "YOUR-SSID"
#define STAPSK  "YOUR-PASSWORD"
#endif

const char* host = "nodemcu";
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Booting Sketch...");
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        Serial.println("WiFi failed, retrying.");
    }

    MDNS.begin(host);

    httpUpdater.setup(&httpServer);
    httpServer.begin();

    MDNS.addService("http", "tcp", 80);
    Serial.printf("NodeMCU ready! Open http://%s.local/update in your browser\n", host);
}

void loop() {
    httpServer.handleClient();
    MDNS.update();
}
