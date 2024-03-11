#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//GSM Module RX pin to NodeMCU D2
#define rxGSM D2
//GSM Module TX pin to NodeMCU D5
#define txGSM D4
SoftwareSerial sim800(rxGSM,txGSM);

// Define the RX and TX pins for the GPS module
const int RXPin = D7;  // Connect GPS TX to this pin
const int TXPin = D6;  // Connect GPS RX to this pin

// Create a SoftwareSerial object for the GPS module
SoftwareSerial gpsSerial(RXPin, TXPin);

// Create a TinyGPS++ object
TinyGPSPlus gps;


void setup() {
  // Initialize serial communication for debugging
  Serial.begin(921600);

  // Initialize serial communication with the GPS module
  gpsSerial.begin(9600);
  
  // SIM800L serial initialize
  sim800.begin(9600);
}

void loop() {
  // Read data from the GPS module
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      // If valid GPS data is received, print it
      if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);

        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        String googleMapsLink = "http://maps.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);
        //String googleMapsLink = "http://maps.google.com/maps?q=" + String(40.408314, 6) + "," + String(3.701850, 6);
        send_SMS(googleMapsLink);
        delay(30000);
      }
    }
  }
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

void send_SMS(String googleMapsLink)
{
  sim800.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  sim800.println("AT+CMGS=\"+34642411352\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  //sim800.println("AT+CMGS=\"+34643865371\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  sim800.print(googleMapsLink); //text content
  updateSerial();
  Serial.println();
  Serial.println("Message Sent");
  sim800.write(26);
}

