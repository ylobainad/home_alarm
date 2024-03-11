#include <SoftwareSerial.h>
//GSM Module RX pin to NodeMCU D6
//GSM Module TX pin to NodeMCU D5
#define rxGSM D2
#define txGSM D4
SoftwareSerial sim800(rxGSM,txGSM);


void setup() {
  Serial.begin(9600);
  sim800.begin(9600);
  Serial.println("SIM800L serial initialize");
  delay(3000);
  send_SMS();
}

void loop() {
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    sim800.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (sim800.available())
  {
    Serial.write(sim800.read());//Forward what Software Serial received to Serial Port
  }
}

void send_SMS()
{
  sim800.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  sim800.println("AT+CMGS=\"+34642411352\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  sim800.print("Circuit Digest"); //text content
  updateSerial();
  Serial.println();
  Serial.println("Message Sent");
  sim800.write(26);
}