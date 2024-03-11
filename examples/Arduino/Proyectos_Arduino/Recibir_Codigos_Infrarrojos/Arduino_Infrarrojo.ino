// A32AB931 ----- OFF
// 4EA240AE -----LAMP
// 371A3C86 ----- MODE
// E0984BB6 -----TIMER




#include <IRremote.h>
int RECV_PIN =A5;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value,HEX);
    irrecv.resume(); // Recibe el proximo valor
    delay(300);
  }
  
}
