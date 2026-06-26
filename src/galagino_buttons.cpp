#include <Bluepad32.h>
#include "galagino_buttons.h"

#define DEAD_ZONE 30

uint32_t controller_state = 0; // for Galagino
uint32_t flipButtons = 0;      // some controllers report A/B/X/Y as B/A/Y/X ???
uint32_t miniController = 0;   // 8BitDO mapping to avoid triggering controller combos with Galagino Combos

void setMapping(uint32_t vid, uint32_t pid, uint32_t manufacturer) {

  Console.printf("setMapping: vid=0x%04x pid=0x%04x, mac=0x%06x\n", vid, pid, manufacturer);

  // 8BitDO Zero 2
  if (vid == 0 && pid == 0) {
    flipButtons = 0;
    miniController = 1;
    return;
  }

  flipButtons = 0;
  miniController = 0;

}

uint16_t getControllerInput() {
  return controller_state && 0x0000ffffUL;
}

void updateControllerInput(ControllerPtr ctl) {

  uint8_t  dpad  = ctl->dpad();
  int32_t  axisX = ctl->axisX();
  int32_t  axisY = ctl->axisY();
  uint16_t buttons = ctl->buttons();
  uint16_t misc    = ctl->miscButtons();

  uint32_t state = 0;

  // Left Joystick
  state |= ((axisX < -DEAD_ZONE)            ? GALAGINO_BUTTON_LEFT  : 0) |
           ((axisX >  DEAD_ZONE)            ? GALAGINO_BUTTON_RIGHT : 0) |
           ((axisY < -DEAD_ZONE)            ? GALAGINO_BUTTON_UP    : 0) |
           ((axisY >  DEAD_ZONE)            ? GALAGINO_BUTTON_DOWN  : 0);

  // DPAD
  state |= ((dpad & DPAD_LEFT)              ? GALAGINO_BUTTON_LEFT  : 0) |
           ((dpad & DPAD_RIGHT)             ? GALAGINO_BUTTON_RIGHT : 0) |
           ((dpad & DPAD_UP)                ? GALAGINO_BUTTON_UP    : 0) |
           ((dpad & DPAD_DOWN)              ? GALAGINO_BUTTON_DOWN  : 0);

  // Combined fire buttons
  state |= ((buttons & (BUTTON_A|BUTTON_B)) ? GALAGINO_BUTTON_FIRE  : 0) | // A or B -> FIRE
           ((buttons & (BUTTON_X|BUTTON_Y)) ? GALAGINO_BUTTON_FIRE  : 0);  // X or Y -> FIRE

  // Individual Fire buttons
  if (flipButtons)
    state |= ((buttons & BUTTON_A)            ? GALAGINO_BUTTON_B     : 0) |
             ((buttons & BUTTON_B)            ? GALAGINO_BUTTON_A     : 0) |
             ((buttons & BUTTON_X)            ? GALAGINO_BUTTON_Y     : 0) |
             ((buttons & BUTTON_Y)            ? GALAGINO_BUTTON_X     : 0);
  else
    state |= ((buttons & BUTTON_A)            ? GALAGINO_BUTTON_A     : 0) |
             ((buttons & BUTTON_B)            ? GALAGINO_BUTTON_B     : 0) |
             ((buttons & BUTTON_X)            ? GALAGINO_BUTTON_X     : 0) |
             ((buttons & BUTTON_Y)            ? GALAGINO_BUTTON_Y     : 0);

  // mini controller - 8BitDO Zero2
  // reduced buttons
  if (miniController)
    state |= ((misc & MISC_BUTTON_START)      ? GALAGINO_BUTTON_START : 0) |    // START  -> START
             ((misc & MISC_BUTTON_SELECT)     ? GALAGINO_BUTTON_MENU  : 0) |    // SELECT -> MENU
             ((buttons & BUTTON_SHOULDER_L)   ? GALAGINO_BUTTON_EXTRA : 0) |    // L1     -> Extra
             ((buttons & BUTTON_SHOULDER_R)   ? GALAGINO_BUTTON_COIN  : 0) ;    // R1     -> Coin
  else 
    state |= ((misc & MISC_BUTTON_START)      ? GALAGINO_BUTTON_START : 0) |    // START  -> START
             ((misc & MISC_BUTTON_SELECT)     ? GALAGINO_BUTTON_MENU  : 0) |    // SELECT -> MENU
             ((misc & MISC_BUTTON_SYSTEM)     ? GALAGINO_BUTTON_MENU  : 0) |    // HOME   -> MENU
             ((buttons & BUTTON_SHOULDER_L)   ? GALAGINO_BUTTON_L1    : 0) |    // L1     -> L1
             ((buttons & BUTTON_SHOULDER_L)   ? GALAGINO_BUTTON_FIRE  : 0) |    // L1     -> Fire
             ((buttons & BUTTON_SHOULDER_R)   ? GALAGINO_BUTTON_FIRE  : 0) |    // R1     -> R1
             ((buttons & BUTTON_SHOULDER_R)   ? GALAGINO_BUTTON_R1    : 0) |    // R1     -> Fire
             ((buttons & BUTTON_TRIGGER_L)    ? GALAGINO_BUTTON_EXTRA : 0) |    // L2     -> Extra
             ((buttons & BUTTON_TRIGGER_R)    ? GALAGINO_BUTTON_COIN  : 0);     // R2     -> Coin

  controller_state = state;

/*
  controller_state = ((axisX < -DEAD_ZONE)            ? GALAGINO_BUTTON_LEFT  : 0) |
                     ((axisX >  DEAD_ZONE)            ? GALAGINO_BUTTON_RIGHT : 0) |
                     ((axisY < -DEAD_ZONE)            ? GALAGINO_BUTTON_UP    : 0) |
                     ((axisY >  DEAD_ZONE)            ? GALAGINO_BUTTON_DOWN  : 0) |
                     ((dpad & DPAD_LEFT)              ? GALAGINO_BUTTON_LEFT  : 0) |
                     ((dpad & DPAD_RIGHT)             ? GALAGINO_BUTTON_RIGHT : 0) |
                     ((dpad & DPAD_UP)                ? GALAGINO_BUTTON_UP    : 0) |
                     ((dpad & DPAD_DOWN)              ? GALAGINO_BUTTON_DOWN  : 0) |
                     ((buttons & (BUTTON_A|BUTTON_B)) ? GALAGINO_BUTTON_FIRE  : 0) |    // A or B -> FIRE
                     ((buttons & (BUTTON_X|BUTTON_Y)) ? GALAGINO_BUTTON_FIRE  : 0) |    // X or Y -> FIRE
                     ((buttons & BUTTON_A)            ? GALAGINO_BUTTON_A     : 0) |
                     ((buttons & BUTTON_B)            ? GALAGINO_BUTTON_B     : 0) |
                     ((buttons & BUTTON_X)            ? GALAGINO_BUTTON_X     : 0) |
                     ((buttons & BUTTON_Y)            ? GALAGINO_BUTTON_Y     : 0) |
                     ((misc & MISC_BUTTON_START)      ? GALAGINO_BUTTON_START : 0) |    // START  -> START 
                     ((misc & MISC_BUTTON_SELECT)     ? GALAGINO_BUTTON_MENU  : 0) |    // SELECT -> MENU
                     ((buttons & BUTTON_SHOULDER_L)   ? GALAGINO_BUTTON_EXTRA : 0) |    // L1     -> Extra
                     ((buttons & BUTTON_SHOULDER_R)   ? GALAGINO_BUTTON_COIN  : 0) ;    // L2     -> Coin
*/         
}

void dumpMacros() {

  Console.printf("DPAD_LEFT  = 0x%02x\n", DPAD_LEFT);  // 0x08
  Console.printf("DPAD_RIGHT = 0x%02x\n", DPAD_RIGHT); // 0x04
  Console.printf("DPAD_UP    = 0x%02x\n", DPAD_UP);    // 0x01
  Console.printf("DPAD_DOWN  = 0x%02x\n", DPAD_DOWN);  // 0x02

  Console.printf("BUTTON_A  = 0x%02x\n", BUTTON_A); // 0x01
  Console.printf("BUTTON_B  = 0x%02x\n", BUTTON_B); // 0x02
  Console.printf("BUTTON_X  = 0x%02x\n", BUTTON_X); // 0x04
  Console.printf("BUTTON_Y  = 0x%02x\n", BUTTON_Y); // 0x08

  Console.printf("BUTTON_THUMB_L    = 0x%02x\n", BUTTON_THUMB_L);    // 0x0100 
  Console.printf("BUTTON_THUMB_R    = 0x%02x\n", BUTTON_THUMB_R);    // 0x0200 
  Console.printf("BUTTON_SHOULDER_L = 0x%02x\n", BUTTON_SHOULDER_L); // 0x0010
  Console.printf("BUTTON_SHOULDER_R = 0x%02x\n", BUTTON_SHOULDER_R); // 0x0020
  Console.printf("BUTTON_TRIGGER_L  = 0x%02x\n", BUTTON_TRIGGER_L);  // 0x0040
  Console.printf("BUTTON_TRIGGER_R  = 0x%02x\n", BUTTON_TRIGGER_R);  // 0x0080

  Console.printf("MISC_BUTTON_START   = 0x%02x\n", MISC_BUTTON_START);  // 0x04
  Console.printf("MISC_BUTTON_SELECT  = 0x%02x\n", MISC_BUTTON_SELECT); // 0x02
  Console.printf("MISC_BUTTON_SYSTEM  = 0x%02x\n", MISC_BUTTON_SYSTEM); // 0x01

}
