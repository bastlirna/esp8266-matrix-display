#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <button_handler.h>
#include <display.h>

char* mqtt_server = "iot.siliconhill.cz";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  //  scrollText((char *)payload, length);

  if(strcmp(topic, "/dotgrid/text") == 0){
    printText(0, 9, (char *)payload, length);
  }else if(strcmp(topic, "/dotgrid/intensity") == 0){
    char buff[8];
    memset(buff, 0, sizeof(buff));
    memcpy(buff, payload, max(length, sizeof(buff)));
    
    setIntensity(atoi(buff));
  }


}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
     check_button();
    Serial.print("Attempting MQTT connection...");
    scrollText("WIFI disconect", 16);
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("/jirkuv_pokusnej", "EHLO world");
      // ... and resubscribe
    //  client.subscribe("/arduinodays/webmessage");

      client.subscribe("/dotgrid/text");
      client.subscribe("/dotgrid/intensity");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      for(int i=0; i<50; i++){
        check_button();
        delay(100);
      }
    }
  }
}


  int init_mqtt(){
    pinMode(3, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
  }

int mqtt_loop(){

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "%u", millis());
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("/dotgrid/uptime", msg);
  }
}
