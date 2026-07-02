/**
 * Author: [Your Name]
 * Date: July 2, 2026
 * Description: An advanced LED blink program that uses a potentiometer 
 *              connected to pin A0 to control the blinking speed of an LED 
 *              on pin 13. It also logs the total blink count to the Serial Monitor.
 */

const int ledPin = 13;
const int potPin = A0;
int blinkCount = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int blinkDelay = analogRead(potPin);
  
  digitalWrite(ledPin, HIGH);
  delay(blinkDelay);
  digitalWrite(ledPin, LOW);
  delay(blinkDelay);
  
  blinkCount++;
  Serial.print("Blink count: ");
  Serial.println(blinkCount);
}
