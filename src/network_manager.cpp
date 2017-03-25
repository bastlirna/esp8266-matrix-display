#include <Arduino.h>

#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include <display.h>

static WiFiManager wifiManager;

int network_manager_init(){
  scrollText("WIFI manager up", 16);

  wifiManager.resetSettings();
  wifiManager.startConfigPortal("Dotgrid display config");
  Serial.println("connected...yeey :)");

  delay(500);
  WiFi.softAPdisconnect();
  WiFi.mode(WIFI_STA);

}
