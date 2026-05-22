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

#endif
