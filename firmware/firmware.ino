
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "TELLO-CBBE3C"
#define STAPSK ""
#endif

unsigned int commandPort = 8889;  // local port to send on
unsigned int statePort = 8890;  // local port to send on
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; 
WiFiUDP Udp;

void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  Serial.print("\n Attempting Connection!");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Sending UDP on port %d\n", commandPort);
  //Listen to state later
  Udp.begin(statePort);
  // Drone Start Up Commands
  telloCmd("192.168.10.1", commandPort,"command");
}

void telloCmd(char* address, unsigned int port, const char* command)
{
  Serial.print("Running Command: ");
  Serial.println(command);
  Udp.beginPacket(address, port);
  Udp.write(command);
  Udp.endPacket();
}

void loop() {
    // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort(), ESP.getFreeHeap());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
  String commandString = Serial.readString();
  commandString.trim();
  if(commandString.length() != 0){
    telloCmd("192.168.10.1", commandPort, commandString.c_str());
  }
}
