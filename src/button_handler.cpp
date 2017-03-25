#include <Arduino.h>
#include <DebouncedInput.h>
#include <network_manager.h>

// We want to keep 10 entries.  That will allocate 80 extra bytes in the object -
// 2 arrays * 10 entries * 4 bytes per entry (unsigned long)
const uint8_t numToKeep = 10;

// We want the button on pin 3.
const uint8_t buttonPin = 0;

// 20ms debounce time, with the internal pull-up enabled
DebouncedInput button(buttonPin, 20, true, numToKeep);


int init_button(){
    button.begin();
}

int check_button(){
  if (button.changedTo(LOW)){
    //  Serial.println(F(" button pressed"));
    network_manager_init();
    Serial.println("connected...yeey :)");

  }
}
