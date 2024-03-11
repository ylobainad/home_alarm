#define BUZZER_PIN D8  // Define the pin to which the buzzer is connected

const int knockPin = 4; 
const int ledPin = 2; 

int knockVal = HIGH;
boolean knockAlarm = false;
unsigned long prevKnockTime;

int knockAlarmTime = 100;

void setup() {
  Serial.begin(115200);
  pinMode (ledPin, OUTPUT) ;
  pinMode (knockPin, INPUT) ;

  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {

  knockVal = digitalRead (knockPin) ;
  if (knockVal == LOW){
    prevKnockTime = millis();
    if (!knockAlarm){
      Serial.println("KNOCK, KNOCK");
      //digitalWrite(ledPin,HIGH);
      beep(1000, 500);
      knockAlarm = true;
      delay(1000);
    }
  }
  else{
    if( (millis()-prevKnockTime) > knockAlarmTime && knockAlarm){
      digitalWrite(ledPin,LOW);
      Serial.println("No Knocks");
      knockAlarm = false;
    }
  }

  // Beep a few times
  for (int i = 0; i < 3; i++) {
    beep(1000, 500);  // Beep at 1000Hz for 500ms
    delay(500);       // Delay for 500ms between beeps
  }
  delay(2000);  // Wait for 2 seconds before repeating the pattern


}

void beep(unsigned int frequency, unsigned long duration) {
  // Calculate the delay between each half-period of the wave (in microseconds)
  unsigned long halfPeriod = 1000000 / (2 * frequency);

  // Calculate the number of cycles
  unsigned long numCycles = (duration * 1000) / (halfPeriod * 2);

  // Generate the tone
  for (unsigned long i = 0; i < numCycles; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(halfPeriod);
  }
}