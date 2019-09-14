#!/bin/bash

# Initialize environment variables
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null 2>&1 && pwd )"
if [[ ! -f "$DIR/.env" ]]; then
    cp .env.sample .env
    echo -e "\e[1;33mWARN \e[0mPlease update your .env file"
    exit 1
fi
set -o allexport
source .env
set +o allexport

# Initialize arguments
SKETCH="$1"
UPLOAD="$2"

if [[ -z $SKETCH ]]; then
    echo -e "\e[1;31mERROR \e[0mMissing sketch name!"
    exit 1
fi
if [[ ! -d "$DIR/examples/$SKETCH" ]]; then
    echo -e "\e[1;31mERROR \e[0mSketch '$SKETCH' not found!"
    exit 1
fi
if [[ -z $WIFI_SSID ]]; then
    echo -e "\e[1;31mERROR \e[0mMissing WiFi SSID!"
    exit 1
fi
if [[ -z $WIFI_PASSWORD ]]; then
    echo -e "\e[1;31mERROR \e[0mMissing WiFi password!"
    exit 1
fi

# Create sketch files
name="nodemcu-${SKETCH,,}"
rm -rf "$DIR/.output/$name" "$DIR/.output/$name.ino.bin"
mkdir -p "$DIR/.output/$name"
cp "$DIR/src/main.ino" "$DIR/.output/$name/$name.ino"
cp "$DIR/examples/$SKETCH/sketch.ino" "$DIR/.output/$name/proxy.ino"

# Replace WiFi settings
sed -i -E "s/#define WIFI_SSID.*/#define WIFI_SSID \"$WIFI_SSID\"/" "$DIR/.output/$name/$name.ino"
sed -i -E "s/#define WIFI_PSK.*/#define WIFI_PSK \"$WIFI_PASSWORD\"/" "$DIR/.output/$name/$name.ino"

# Initialize Arduino CLI
shopt -s expand_aliases
alias arduino-cli='cmd //C "$ARDUINO_PATH/arduino-cli/arduino-cli.exe"'
arduino-cli config init
cd "$LOCALAPPDATA"
echo "proxy_type: auto
sketchbook_path: $SKETCHBOOK_PATH
arduino_data: $ARDUINO_PATH/arduino-cli\data
board_manager:
  additional_urls:
    - http://arduino.esp8266.com/stable/package_esp8266com_index.json
" > "Arduino15/arduino-cli.yaml"

# Compile sketch
arduino-cli compile --fqbn esp8266:esp8266:nodemcu "$DIR/.output/$name"

# Upload sketch
cd "$LOCALAPPDATA"
file=$(find Temp/arduino-sketch-*/$name.ino.bin)
if [ "$UPLOAD" == "--upload" ]; then
    curl -F "image=@$file" 192.168.43.228/update
    rm -rf "$DIR/.output/$name"
else
    mv $file "$DIR/.output"
fi

# Cleanup
cd "$LOCALAPPDATA"
rm -rf Arduino15/ Temp/arduino*/
