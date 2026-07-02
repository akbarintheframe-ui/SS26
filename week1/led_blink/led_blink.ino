const int ledPin = 13;
const int potPin = A0;
int blinkCount = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Read the potentiometer value (0 to 1023) to use as the delay in milliseconds
  int blinkDelay = analogRead(potPin);
  
  digitalWrite(ledPin, HIGH);
  delay(blinkDelay);
  digitalWrite(ledPin, LOW);
  delay(blinkDelay);
  
  blinkCount++;
  Serial.print("Blink count: ");
  Serial.println(blinkCount);
}
