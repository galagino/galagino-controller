#include "globals.h"

unsigned long nextToggle     = 0;
bool          isOn           = false;

bool          pairingEnabled = true;
unsigned long pairingStart   = 0;

bool          serialDump = false;

bool          forgetKeys = false;

ControllerPtr Controllers[BP32_MAX_GAMEPADS];
volatile uint8_t numControllers = 0;

