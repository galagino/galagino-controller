#pragma once
#include <Bluepad32.h>

#define GALAGINO_BUTTON_LEFT  0x01
#define GALAGINO_BUTTON_RIGHT 0x02
#define GALAGINO_BUTTON_UP    0x04
#define GALAGINO_BUTTON_DOWN  0x08
#define GALAGINO_BUTTON_FIRE  0x10
#define GALAGINO_BUTTON_START 0x20
#define GALAGINO_BUTTON_COIN  0x40
#define GALAGINO_BUTTON_EXTRA 0x80

extern volatile uint8_t controller_state;

uint8_t getControllerInput();
void updateControllerInput(ControllerPtr ctl);
void dumpMacros();
