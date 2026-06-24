#include <Arduino.h>

#include <Bluepad32.h>
#include "galagino_buttons.h"
#include "dump.h"
#include "i2c.h"
#include "globals.h"

#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPAsyncWebServer.h>
#include <WebOTA.h>
#include "webserver.h"
#ifndef WIFI_HOSTNAME
#define WIFI_HOSTNAME "galagino-%06x"
#endif

#include <esp_idf_version.h>
#include <esp_arduino_version.h>

WiFiMulti wifiMulti;

void onStart_cb(void) { Console.printf("onStart\n"); };
void onEnd_cb(void)   { Console.printf("onEnd\n"); };
void onError_cb(int code, const char* msg) {
  Console.printf("onError - %d %s\n", code, msg);
};
void onReboot_cb(void) {
  Console.printf("\nRebooting\n");
}

#define INTERNAL_LED_PIN 2
#define EXTERNAL_LED_PIN 27
#ifndef NEW_PAIRING_TIMEOUT
#define NEW_PAIRING_TIMEOUT 90000
#endif

void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (Controllers[i] == nullptr) {
      Console.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Console.printf("Controller addr=%02x:%02x:%02x:%02x:%02x:%02x  model=[ %s ] VID=0x%04x, PID=0x%04x\n", 
                    properties.btaddr[0], properties.btaddr[1], properties.btaddr[2],
                    properties.btaddr[3], properties.btaddr[4], properties.btaddr[5],
                    ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
      Controllers[i] = ctl;
      foundEmptySlot = true;
      numControllers++;
      break;
    }
  }

  if (!foundEmptySlot) {
    Console.println("CALLBACK: Controller connected, but could not found empty slot");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (Controllers[i] == ctl) {
      Console.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      Controllers[i] = nullptr;
      foundController = true;
      numControllers--;
      break;
    }
  }

  if (!foundController) {
    Console.println("CALLBACK: Controller disconnected, but not found in Controllers");
  }
}

void processGamepad(ControllerPtr ctl) {
    // There are different ways to query whether a button is pressed.
    // By query each button individually:
    //  a(), b(), x(), y(), l1(), etc...
    if (ctl->a()) {
        static int colorIdx = 0;
        // Some gamepads like DS4 and DualSense support changing the color LED.
        // It is possible to change it by calling:
        switch (colorIdx % 3) {
            case 0:
                // Red
                ctl->setColorLED(255, 0, 0);
                break;
            case 1:
                // Green
                ctl->setColorLED(0, 255, 0);
                break;
            case 2:
                // Blue
                ctl->setColorLED(0, 0, 255);
                break;
        }
        colorIdx++;
    }

    if (ctl->b()) {
        // Turn on the 4 LED. Each bit represents one LED.
        static int led = 0;
        led++;
        // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
        // support changing the "Player LEDs": those 4 LEDs that usually indicate
        // the "gamepad seat".
        // It is possible to change them by calling:
        ctl->setPlayerLEDs(led & 0x0f);
    }

    if (ctl->x()) {
        // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S, Stadia support rumble.
        // It is possible to set it by calling:
        // Some controllers have two motors: "strong motor", "weak motor".
        // It is possible to control them independently.
        ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
                            0x40 /* strongMagnitude */);
    }

    // Another way to query controller data is by getting the buttons() function.
    // See how the different "dump*" functions dump the Controller info.
    dumpGamepad(ctl);
}

void processControllersx() {
    for (auto Controller : Controllers) {
        if (Controller && Controller->isConnected() && Controller->hasData()) {
            if (Controller->isGamepad()) {
                processGamepad(Controller);
            } else {
                Console.println("Unsupported controller");
            }
        }
    }
}

void processControllers() {
  for (auto Controller : Controllers) {
    if (Controller && Controller->isConnected() && Controller->hasData()) {
      updateControllerInput(Controller);
      if (serialDump) {
        Console.printf("controller: %d\n", controller_state);
        dumpGamepad(Controller);
      }
    }
  }
}

void setup() {
  char hostname[33];

  Console.begin(115200);
  delay(300);

  const uint8_t* addr = BP32.localBdAddress();
  Console.printf("ESP IDF version:     %s\n", esp_get_idf_version());
  Console.printf("ESP Arduino version: %d.%d.%d\n", ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR, ESP_ARDUINO_VERSION_PATCH);
  Console.printf("Firmware:            %s\n", BP32.firmwareVersion());
  Console.printf("BD Addr:             %02x:%02x:%02x:%02x:%02x:%02x\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
  Console.printf("Free heap:           %d\n", ESP.getFreeHeap());
  Console.printf("Main core:           %d\n", xPortGetCoreID());
  Console.printf("Main priority:       %d\n", uxTaskPriorityGet(NULL));
  Console.printf("Build __DATE__:      %s\n", __DATE__);
  Console.printf("Build __TIME__:      %s\n", __TIME__);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableNewBluetoothConnections(true);
  BP32.enableBLEService(false);
  BP32.enableVirtualDevice(false);

  pinMode(INTERNAL_LED_PIN, OUTPUT);
  pinMode(EXTERNAL_LED_PIN, OUTPUT);
  analogWrite(INTERNAL_LED_PIN, 0);
  analogWrite(EXTERNAL_LED_PIN, 0);

  dumpMacros();

  setupSlave(SLAVE_ADDR, SLAVE_SDA, SLAVE_SCL);

  snprintf(hostname, sizeof(hostname)-1, WIFI_HOSTNAME, WebOTA.ESP_getChipId());
  WiFi.setHostname(hostname);
  WiFi.mode(WIFI_OFF);
  WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);
  WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWD);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWD);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWD);
  wifiMulti.run();
  Console.printf("Hostname: %s\n", hostname);

  setupWebServer();
  WebOTA.onReboot(onReboot_cb);
  WebOTA.onStart(onStart_cb);
  WebOTA.onEnd(onEnd_cb);
  WebOTA.onError(onError_cb);
  WebOTA.enable(&web);
  web.begin();

}

void loop() {

  unsigned long m = millis();

  if (!pairingEnabled && m < pairingStart + NEW_PAIRING_TIMEOUT) {
    pairingEnabled=true;
    BP32.enableNewBluetoothConnections(true);
    Console.println("pairing enabled");
  }

  // Only allow pairing for NEW_PAIRING_TIMEOUT ms
  if (pairingEnabled && m > pairingStart + NEW_PAIRING_TIMEOUT) {
    pairingEnabled=false;
    BP32.enableNewBluetoothConnections(false);
    Console.println("pairing disabled");
  }

  if (forgetKeys) {
    BP32.forgetBluetoothKeys();
    Console.println("forget keys");
    delay(500);
    ESP.restart();
  }

  if (m >= nextToggle) {
    unsigned long cycle = 1000;
    if (numControllers == 0 && pairingEnabled) cycle=250;
    isOn = !isOn;
    if (numControllers > 0) isOn = true;
    analogWrite(INTERNAL_LED_PIN, isOn ? 50 : 0);
    analogWrite(EXTERNAL_LED_PIN, isOn ? 50 : 0);
    nextToggle = m + cycle;
  }

  if (BP32.update())
    processControllers();
  vTaskDelay(1);
}
