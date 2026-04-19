#include "i2c.h"

void onReceive(int i) {

  if (Wire.available()) {
    Wire.read();
  }
  while (Wire.available()) {
    Wire.read();
  }

}

void onRequest() {
  uint8_t data = controller_state;
  Wire.write(&data, 1);
}

void setupSlave(uint8_t addr, int sda, int scl) {
  Wire.onRequest(onRequest);
  Wire.onReceive(onReceive);
  Wire.begin(addr, sda, scl, 400000);
}
