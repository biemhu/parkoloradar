#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 13;
const int buzzerPin = 2;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // LCD beállítása: 16 oszlop, 2 sor
  lcd.begin(16, 2);
  lcd.print("Parkolo asszisztens");
  delay(1000);
  lcd.clear();
  
  Serial.begin(9600);
}

void loop() {
  // Távolság mérése
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // LCD Frissítése
  lcd.setCursor(0, 0); // Első sor eleje
  lcd.print("Tavolsag: ");
  lcd.print(distance);
  lcd.print(" cm   "); // Szóközök a régi számok letörléséhez

  lcd.setCursor(0, 1); // Második sor

  if (distance > 0) {
    // NAGYON KÖZEL: 5 cm alatt
    if (distance < 5) {
      lcd.print("!!! STOP !!!    ");
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1000); 
    } 
    // KÖZEL: 5-50 cm között
    else if (distance < 50) {
      lcd.print("VIGYAZAT!       ");
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1000);
      delay(distance * 10); 
      
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
      delay(distance * 10);
    } 
    // BIZTONSÁGOS: 50 cm felett
    else {
      lcd.print("BIZTONSAGOS     ");
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
    }
  }

  delay(50);
}