#!/bin/bash
HOSTNAME="NodeMCU OTA"
echo -e "\e[1;34m[$HOSTNAME] \e[0mInitializing..."

# Initialize environment variables
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null 2>&1 && pwd )"
if [[ ! -f "$DIR/.env" ]]; then
    cp .env.sample .env
    echo -e "\e[1;34m[$HOSTNAME] \e[1;33mWARN \e[0mPlease update your .env file"
    exit 1
fi
set -o allexport
source .env
set +o allexport

# Initialize arguments
BINARY="$1"

if [[ -z $BINARY ]]; then
    echo -e "\e[1;34m[$HOSTNAME] \e[1;31mERROR \e[0mMissing binary name!"
    exit 1
fi
if [[ ! -f "$DIR/.output/nodemcu-${BINARY,,}.ino.bin" ]]; then
    echo -e "\e[1;34m[$HOSTNAME] \e[1;31mERROR \e[0mBinary '$BINARY' not found!"
    exit 1
fi

# Upload sketch
file="$DIR/.output/nodemcu-${BINARY,,}.ino.bin"
echo -e "\e[1;34m[$HOSTNAME] \e[0mUploading binary..."
curl -F "image=@$file" 192.168.43.228/update >/dev/null 2>&1
rm -rf "$file"

echo -e "\e[1;34m[$HOSTNAME] \e[1;32mOK!\e[0m"
