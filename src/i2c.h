#pragma once
#include <Wire.h>
#include <Bluepad32.h>
#include "galagino_buttons.h"


#if !defined SLAVE_ADDR
#define SLAVE_ADDR 0x09
#endif

#if !defined SLAVE_SDA
#define SLAVE_SDA 21
#endif

#if !defined SLAVE_SCL
#define SLAVE_SCL 22
#endif

void setupSlave(uint8_t addr, int sda, int scl);
