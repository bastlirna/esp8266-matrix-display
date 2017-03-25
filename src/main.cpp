
#include <Arduino.h>
#include <button_handler.h>
#include <mqtt_client.h>
#include <display.h>
#include <ESP8266WiFi.h>

void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  init_button();
  init_mqtt();
  display_init();

}


void loop() {
 check_button();
 mqtt_loop();
}
