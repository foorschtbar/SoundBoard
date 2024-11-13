# SoundBoard

A simple pcb with two audio drivers, a sd card reader and a microcontroller to play sounds.

## Features
- On factory settings or no known WiFi SSID is found, device starts in AP mode
  - To stay in AP mode, leave the SSID and Password empty
- The LED show the status of the device
  - `Orange`: Booting
  - `Blue`: Connecting to WiFi
  - `Green`: Connected to existing WiFi
  - `Violet`: Started WiFi AP
  - `Red`: No connection to WiFi
  - `White`: Web or MQTT activity
- The device can be controlled via MQTT
  - Subscripte to `<prefix>/status` to get the status of the device
  - Publish to `<prefix>/cmd` to play a sound
    - `{"play": "filename.wav", "volume": 50, "balance": 0}` (volume and balance are optional)

## Frontend development

- Start the frontend development server with `python .\webserver.py` in the `html` directory.
- Open the browser at `http://localhost:8000/` to see the frontend.
- Set manual a cookie with `overwrite-host` to the hostname or ip of the device and reload the page.

## ToDos

- [ ] Refactor Code in more separat files
- [ ] sync volume and balance across all clients
- [ ] If we had valid settings, but in Fallback AP mode, try to connect to the last known wifi periodically
- [ ] Download audiofiles from the webinterface
- [ ] Replace Pay/Delete/Download wit icons?
- [ ] Add tailing slash to the topic prefix
- [x] Save authentication in cookie/webstore
- [x] Add credentials to post requests
- [x] Improve MQTT reconnection and resubscription
- [x] Improve MQTT reconnection after boot when no connection is available
- [x] Make MQTT status topic more "chatty"

## Links
- https://circuitdigest.com/microcontroller-projects/esp32-based-internet-radio-using-max98357a-i2s-amplifier-board
- https://diyi0t.com/i2s-sound-tutorial-for-esp32/
- https://github.com/schreibfaul1/ESP32-audioI2S
- https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
- https://esp32.jgarrettcorbin.com/