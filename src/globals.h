#ifndef GLOBALS_H
#define GLOBALS_H

#include <Bluepad32.h>
extern ControllerPtr    Controllers[BP32_MAX_GAMEPADS];;
extern volatile uint8_t numControllers;

extern unsigned long nextToggle;
extern bool          isOn;
extern bool          pairingEnabled;
extern bool          serialDump;
extern bool          forgetKeys;

extern unsigned long pairingStart;

// 8bitdo
// addr=e4:17:d8:ca:09:02  model=[ 8BitDo ] VID=0x2dc8, PID=0x3230
//
// Shanwan Q36
// addr=82:25:06:36:25:d1  model=[ Switch Pro ] VID=0x057e, PID=0x2009
// addr=84:25:06:36:25:d1  model=[ DualShock 4 ] VID=0x054c, PID=0x09cc
//

#define CONTROLLER_8BITDO_ZERO2_VIDPID 0x2dc83230u
#define CONTROLLER_8BITDO_MAC          0xe417d8u
#define CONTROLLER_SHANWAN_MAC1        0x822506u
#define CONTROLLER_SHANWAN_MAC2        0x842506u

#endif
