/* ##### IDLE SWITCH ##### */

// Variable Initilisations
GroveMultiSwitch mswitch[1];
const String key_names[5] = {
    "RIGHT",
    "FORWARDS",
    "LEFT",
    "BACK",
    "DOWN",
};

// Checking Switch Status
String checkIdle(bool error=false) {
  if (error) {
    if (digitalRead(IDLE_PIN) == HIGH) return "ERROR";
    return "NONE";
  }
  
  GroveMultiSwitch::ButtonEvent_t* evt;
  evt = mswitch->getEvent();

  for (int i = 0; i < 5; i++) {
    if (evt->button[i] & ~GroveMultiSwitch::BTN_EV_RAW_STATUS) {
      return key_names[i];
    }
  }

  return "NONE";
}