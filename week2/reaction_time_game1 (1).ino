#include <LiquidCrystal.h>

// Initialize LCD on pins 8 through 13
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// Define our Start Button and Buzzer on the RX/TX pins
const int startButtonPin = 0;
const int buzzerPin = 1;

// Arrays for our LEDs and Player Buttons
int ledPins[] = {2, 3, 4, 5, 6, 7};
int buttonPins[] = {A0, A1, A2, A3, A4, A5};

int score = 0;
bool gameRunning = false;

void setup() {
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Press Start (D0)");

  // Use internal pullup resistors for buttons (defaults to HIGH, goes LOW when pressed)
  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP); 
  }
}

void loop() {
  if (!gameRunning) {
    // Wait for the Start button to be pressed
    if (digitalRead(startButtonPin) == LOW) {
      startGame();
    }
  } else {
    playGame();
  }
}

void startGame() {
  score = 0;
  gameRunning = true;
  lcd.clear();
  lcd.print("Get Ready...");
  delay(2000);
  lcd.clear();
  lcd.print("GO!");
  delay(500);
}

void playGame() {
  // Pick a random LED (Mole)
  int activeMole = random(0, 6);
  digitalWrite(ledPins[activeMole], HIGH);
  
  long startTime = millis();
  // Time limit gets faster as your score increases!
  long timeLimit = max(400, 2000 - (score * 150)); 
  bool hit = false;
  
  // Wait for player input while within the time limit
  while (millis() - startTime < timeLimit) {
    // Check if correct button is pressed
    if (digitalRead(buttonPins[activeMole]) == LOW) {
      hit = true;
      break;
    }
    
    // Instant game over if they press the WRONG button
    for(int i = 0; i < 6; i++) {
       if(i != activeMole && digitalRead(buttonPins[i]) == LOW) {
          digitalWrite(ledPins[activeMole], LOW);
          gameOver();
          return;
       }
    }
  }
  
  // Turn off the mole
  digitalWrite(ledPins[activeMole], LOW);
  
  if (hit) {
    score++;
    tone(buzzerPin, 1000, 100); // High ding for a point
    lcd.clear();
    lcd.print("Score: ");
    lcd.print(score);
    delay(250); // Short pause before the next mole pops up
  } else {
    gameOver(); // They ran out of time
  }
}

void gameOver() {
  gameRunning = false;
  tone(buzzerPin, 150, 800); // Low, sad buzz for game over
  lcd.clear();
  lcd.print("GAME OVER!");
  lcd.setCursor(0, 1);
  lcd.print("Final Score: ");
  lcd.print(score);
  
  delay(3000); // Show score for 3 seconds
  lcd.clear();
  lcd.print("Press Start (D0)");
}