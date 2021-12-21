
#include "StkAll.h"

SineWave input;
IPAddress ip(192, 168, 1, 35);
ArdUdp udp(ip, 7000);
ArdStreamBinaryOut output(udp);
const char *SSID = "your ssid";
const char *PWD = "your password";


void setup() {
  Serial.begin(115200);
  
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
  }

  Serial.print("Connected to IP address: ");
  Serial.println(WiFi.localIP());

  Stk::setSampleRate( 44100.0 );
  input.setFrequency( 440.0 );
}

void loop() {
  output.tick( input.tick() );
}
