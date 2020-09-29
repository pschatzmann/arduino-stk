#include "WiFi.h" 
#include "ArdUdp.h" 
#include "ArdMidiStreamOut.h"

using namespace stk;

IPAddress ip(192, 168, 1, 35);
ArdUdp udp(ip, 7000);
ArdMidiStreamOut out(udp);
const char *SSID = "your ssid";
const char *PWD = "your password";

StkFloat note = 64; // 0 to 128
StkFloat amplitude = 100; // 0 to 128

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  Serial.print("Connected to IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
    Serial.print("playing ");
    Serial.println(++note);

    out.noteOn( note, amplitude );
    delay(900);
    out.noteOff( note, 20 );
    delay(200);
    if (note>=90) {
      note = 30;
    }
}
