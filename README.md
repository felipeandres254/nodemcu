# NodeMCU Over-The-Air

Sketch used to update the NodeMCU software via web.

## Usage

1. Setup your WiFi SSID and password at the beginning of [`main.ino`](./main.ino).
   ```cpp
   #define WIFI_SSID "YOUR-SSID"
   #define WIFI_PSK  "YOUR-PASSWORD"
   ```

2. Use the [`proxy.ino`](./proxy.ino) file to write your sketch as usual, using the corresponding functions for `setup()` and `loop()`.
