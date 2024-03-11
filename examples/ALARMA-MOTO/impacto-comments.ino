// Define the pin numbers for the knock sensor and the LED.
const int knockPin = 4;
const int ledPin = 2;

// Initialize variables to manage the state of the knock sensor and LED.
int knockVal = HIGH;           // Store the current state of the knock sensor.
boolean knockAlarm = false;    // Keep track of whether a knock has been detected.
unsigned long prevKnockTime;   // Store the timestamp of the last knock.

// Set the time threshold for considering a knock as an "alarm" (in milliseconds).
int knockAlarmTime = 100;

void setup() {
  // Initialize the serial communication for debugging purposes.
  Serial.begin(9600);
  
  // Set the pinMode for the LED and knock sensor pins.
  pinMode(ledPin, OUTPUT);
  pinMode(knockPin, INPUT);
}

void loop() {
  // Read the current state of the knock sensor.
  knockVal = digitalRead(knockPin);

  // Check if a knock is detected (the sensor reads LOW).
  if (knockVal == LOW) {
    // Record the current time when a knock is detected.
    prevKnockTime = millis();
    
    // Check if an alarm hasn't been triggered yet.
    if (!knockAlarm) {
      // Indicate a knock and turn on the LED.
      Serial.println("KNOCK, KNOCK");
      digitalWrite(ledPin, HIGH);
      knockAlarm = true;
      delay(1000); // Delay for one second to prevent rapid alarms from a single knock.
    }
  } else {
    // Check if enough time has passed since the last knock to reset the alarm.
    if ((millis() - prevKnockTime) > knockAlarmTime && knockAlarm) {
      // Turn off the LED and indicate that no more knocks have been detected.
      digitalWrite(ledPin, LOW);
      Serial.println("No Knocks");
      knockAlarm = false;
    }
  }
}
