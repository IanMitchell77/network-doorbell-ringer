#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiSTA.h>
#include <WiFiClient.h>


/*
 *  This sketch sends a message to a TCP server
 *
 */

// WiFi settings
// jules's house
//const char* ssid = "VM6339503";
//const char* password = "zdhzmcFCy2vr";
//sky 4 desk
//const char* ssid = "SKYFB337";
//const char* password = "XDSUPDAU";
// in the home office
const char* ssid="mitchsoft";
const char* password="davethecat";

const int buzzer = 13;

WiFiUDP UDPTestServer;
unsigned int UDPPort = 4950;

const int packetSize = 21;
byte packetBuffer[packetSize];

WiFiUDP UDP;

void setup() {
    Serial.begin(115200);
    pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
    pinMode(LED_BUILTIN, OUTPUT);
    delay(10);

    // We start by connecting to a WiFi network
    WiFi.begin(ssid, password);

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi to connect to ");
    Serial.print(ssid);

    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(50);
    }

    delay(100);
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    UDPTestServer.begin(UDPPort);
    
    Serial.println("Waiting for button press broadcasts");
}

void loop()
{ 
  //playTone();
  handleUDPServer();
   delay(1);
}

void playTone(void){
  tone(buzzer, 2000); // Send 1KHz sound signal...
  //playTone(buzzer, 262, 35);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);        // ...for 1 sec
  tone(buzzer, 1000); // Send 1KHz sound signal...
  //playTone(buzzer, 262, 35);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  noTone(buzzer);     // Stop sound...
  //playTone(buzzer, 400, 35);
  digitalWrite(LED_BUILTIN, HIGH);
  //delay(1000);        // ...for 1sec
}

void handleUDPServer() {
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    UDPTestServer.read(packetBuffer, packetSize);
    String myData = ""; 
    for(int i = 0; i < packetSize; i++) {
      myData += (char)packetBuffer[i];
    }
    //Serial.println(myData);
    char *pbuf = (char *)packetBuffer;
//sprintf(pbuf, "%s", myData);
    if(strcmp(pbuf, "doorbell-button-press") == 0)
    {
      //Serial.println("bingo");
      playTone();
    }
  }
}
