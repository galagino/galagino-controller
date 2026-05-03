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
  uint16_t data = controller_state;
  Wire.write(data & 0xff);
  Wire.write(data >> 8);
}

void setupSlave(uint8_t addr, int sda, int scl) {
  Wire.onRequest(onRequest);
  Wire.onReceive(onReceive);
  Wire.begin(addr, sda, scl, 400000);
}
