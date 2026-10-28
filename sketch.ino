#include <LiquidCrystal.h>

// LCD pin assignment: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int gasSensorPin = A0;
const int greenLedPin  = 6;
const int redLedPin    = 7;
const int buzzerPin    = 8;
const int relayPin     = 9;

// Trigger alarm when gas rises above 100 PPM
const int THRESHOLD_PPM = 100;

void setup() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  // Initialize in safe condition
  noTone(buzzerPin);
  digitalWrite(relayPin, LOW);
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, HIGH);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1000);
}

void loop() {
  int rawValue = analogRead(gasSensorPin);

  // Map raw idle reading (~350 to 1023) directly to PPM (0 to 1000)
  int gasPPM = map(rawValue, 350, 1023, 0, 1000);
  if (gasPPM < 0) gasPPM = 0;

  // Row 1: Current PPM value
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(gasPPM);
  lcd.print(" PPM     ");

  if (gasPPM > THRESHOLD_PPM) {
    // DANGER: Gas leak detected
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    tone(buzzerPin, 1000);
    digitalWrite(relayPin, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("ALERT: GAS LEAK!");
  } else {
    // SAFE: Gas is zero or low
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
    noTone(buzzerPin);
    digitalWrite(relayPin, LOW);

    lcd.setCursor(0, 1);
    lcd.print("Status: SAFE    ");
  }

  delay(200);
}