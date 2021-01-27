# IoT projects with NodeMCU

Base IoT framework fot NodeMCU.
Uses [MongooseOS](https://mongoose-os.com) and [AWS IoT](https://aws.amazon.com/iot).

## Build and installation

1. Setup your WiFi SSID and password in [mos.yml](./mos.yml).
   ```yaml
   config_schema:
     - ["wifi.sta.ssid", "WIFI_SSID"]
     - ["wifi.sta.pass", "WIFI_PASS"]
   ```

2. Run the following commands in a WSL environment
   ```sh
   $ mos build
   $ powershell.exe -File "scripts/install.ps1"
   ```

## Over-The-Air

To execute an OTA action make sure the device is connected to WiFi and it's shadow is reporting correctly.
Also, the configuration and certificates must be available in [`devices`](./devices) directory.

### Mongoose and custom RPC's

Run `scripts/ota <name> [rpc]` to execute a Remote Procedure Call.
This is equivalent to run `mos [rpc]` with a phyisically connected device.

```sh
$ scripts/ota MY_DEVICE console
Connecting to tcps://xxxxxxxxxxxxxxxxx.iot.us-east-1.amazonaws.com:8883...
Subscribed to MY_DEVICE/log
```

### Application updates

1. Edit the filesystem within `fs`.
   Notice that the first file to run is [`init.js`](./fs/init.js) which loads [`main.js`](./fs/main.js).

2. When you are ready, run `scripts/ota <name> --update`.
   You can also provide a directory name within the examples folder to this command:
   ```sh
   $ ./scripts/ota <name> --update <example>
   ```
