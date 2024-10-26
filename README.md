# SoundBoard

A simple pcb with two audio drivers, a sd card reader and a microcontroller to play sounds.

## Frontend development

- Start the frontend development server with `python .\webserver.py` in the `html` directory.
- Open the browser at `http://localhost:8000/` to see the frontend.
- Set manual a cookie with `overwrite-host` to the hostname or ip of the device and reload the page.

## ToDos

- [ ] Save authentication in cookie/webstore
- [ ] Add credentials to post requests
- [x] Improve MQTT reconnection and resubscription
- [x] Improve MQTT reconnection after boot when no connection is available
- [x] Make MQTT status topic more "chatty"

## Links
- https://circuitdigest.com/microcontroller-projects/esp32-based-internet-radio-using-max98357a-i2s-amplifier-board
- https://diyi0t.com/i2s-sound-tutorial-for-esp32/
- https://github.com/schreibfaul1/ESP32-audioI2S
- https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
- https://esp32.jgarrettcorbin.com/