/*
  UDPSendReceive.pde:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.

  created 21 Aug 2010
  by Michael Margolis

  This code is in the public domain.

  adapted from Ethernet library examples
*/


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "TELLO-CBBE3C"
#define STAPSK ""
#endif

unsigned int localPort = 8889;  // local port to listen on

char COMMAND[] = "command";  // a string to send back
char TAKEOFF[] = "takeoff";  // a string to send back

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  Serial.print("Attempting Connection!");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Sending UDP on port %d\n", localPort);
  //Listen to state later
  // Udp.begin(localPort);
  
  // send a reply, to the IP address and port that sent us the packet we received
  Udp.beginPacket("192.168.10.1", localPort);
  Udp.write(COMMAND);
  Udp.endPacket();
  delay(2000);
  Udp.beginPacket("192.168.10.1", localPort);
  Udp.write(TAKEOFF);
  Udp.endPacket();
}

void loop() {

}
