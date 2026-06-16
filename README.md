galagino-controller
===================

* This allows you to use a bluetooth controller over i2c with galagino. Useful for CYD builds with limited GPIO pins available.

* copy `platformio_override.empty.ini` as `platformio_override.ini` and populate WiFI credentials before building.

* Any `ESP32` with Bluetooth Classic should work, most controllers don't have BLE. An `ESP32 D1 Mini` can be glued with double sided tape inside the 3d encolusure.

* Connections

| CYD    | D1 Mini |                      |
| ---    | ---     | ---                  |
| 5v     | 5v      |                      |
| GND    | GND     |                      |
| SDA    | SDA     | Pull Up 2.2k to 3.3v |
| SCL    | SCL     | Pull Up 2.2k to 3.3v |


