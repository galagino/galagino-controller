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

#define GALAGINO_BUTTON_A    0x0100
#define GALAGINO_BUTTON_B    0x0200
#define GALAGINO_BUTTON_X    0x0400
#define GALAGINO_BUTTON_Y    0x0800

#define GALAGINO_BUTTON_MENU 0x8000

extern volatile uint16_t controller_state;

uint16_t getControllerInput();
void updateControllerInput(ControllerPtr ctl);
void dumpMacros();
