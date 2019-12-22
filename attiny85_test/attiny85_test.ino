// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(0, OUTPUT); //LED on Model B
  pinMode(1, OUTPUT); //LED on Model A   
}


// the loop routine runs over and over again forever:
void loop() {
  pulse(3, 100);
  delay(2000);
}

void pulse(int quantity, long blinkDurationInMillisecons) {
  for (int i = 0; i < quantity; i++) {
    blink(blinkDurationInMillisecons);
  }
}

void blink(long durationInMilliseconds) {
  lighton();
  delay(durationInMilliseconds);
  lightoff();
  delay(durationInMilliseconds);
}

void lighton() {
  digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(1, HIGH);
}

void lightoff() {
  digitalWrite(0, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(1, LOW); 
}

