
#include <Arduino.h>
#include <button_handler.h>
#include <mqtt_client.h>
#include <display.h>

void setup() {
  Serial.begin(115200);
  init_button();
  init_mqtt();
  display_init();

}


void loop() {
 check_button();
 mqtt_loop();
}
