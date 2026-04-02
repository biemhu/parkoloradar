# Intelligens Parkolóasszisztens Rendszer

Készítette: Balogh Martin
________________________________________
# 1. Bevezetés
A modern gépjárművezetés-támogató rendszerek egyik alapvető eleme a parkolóasszisztens. A projekt célja egy olyan Arduino alapú prototípus tervezése és szimulációja Tinkercad környezetben, amely egy ultrahangos távolságmérő szenzor segítségével vizuális (LCD) és hangalapú visszajelzést ad a környező akadályok távolságáról.
# 2. Rendszerarchitektúra
A rendszer három fő egységre bontható:
1.	Érzékelés: HC-SR04 ultrahangos szenzor.
2.	Feldolgozás: Arduino Uno mikrokontroller (ATMega328P).
3.	Beavatkozás/Visszacsatolás: 16x2 karakteres LCD kijelző, LED dióda és Piezo buzzer.
________________________________________
# 3. Hardveres felépítés és komponensek
3.1. HC-SR04 Ultrahangos szenzor
A szenzor működése a piezoelektromos hatáson alapul. Egy 40 kHz-es ultrahang impulzust bocsát ki, majd méri a visszaverődés idejét.
●	Mérési tartomány: 2 cm – 400 cm.
●	Működési feszültség: 5V.
3.2. Megjelenítés és jelzés
●	LCD 16x2: Hitachi HD44780 kompatibilis kijelző, amelyen a pontos távolság és szöveges figyelmeztetések jelennek meg.
●	LED: PWM-alapú vagy digitális villogtatás a figyelemfelkeltés érdekében.
●	Piezo Buzzer: Frekvencia alapú jelzés. A kód a tone(buzzerPin, 1000) parancsot használja a tiszta hangzásért.
________________________________________
# 4. Implementáció (Szoftver)
4.1. Algoritmus leírása
A program egy végtelen ciklusban (loop) végzi a következő lépéseket:
1.	Trigger jel küldése
2.	Visszaverődési idő mérése pulseIn() függvénnyel.
3.	Távolság számítása cm-ben.
4.	Logikai döntési fa futtatása:
○	Biztonságos zóna (> 50 cm): Csak távolságkiírás.
○	Figyelmeztetési zóna (5-50 cm): Távolságtól függő villogás és sípolás.
○	Vészhelyzeti zóna (< 5 cm): Folyamatos jelzés és "STOP" felirat.
4.2. Főbb kódrészletek magyarázattal
A hangjelzésért felelős modul:
C++
if (distance < 50) {
    tone(buzzerPin, 1000); // 1000 Hz-es figyelmeztető hang
    delay(distance * 10);  // A távolsággal arányos szünet
    noTone(buzzerPin);
}

________________________________________
# 5.	Tinkercad link | Kód
[Intelligens Parkolóasszisztens Rendszer] (https://www.tinkercad.com/things/152hcNtW7Aw-intelligens-parkoloradar?sharecode=hUeobfc7fTpdt_t-G1C3DV8OTxNck9J3OOOi1Shyx4Q)

#Kód: 
```
 #include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 13;
const int buzzerPin = 2;

long duration;
int tavolsag;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  //LCD
  lcd.begin(16, 2); //16s 2o
  lcd.print("Parkolo asszisztens");
  delay(1000); //keses
  lcd.clear();
  
  Serial.begin(9600);
}

void loop() {
  //Távolság mérése
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  tavolsag = duration * 0.034 / 2;

  //LCD frissites
  lcd.setCursor(0, 0); //Elso sor
  lcd.print("Tavolsag: ");
  lcd.print(tavolsag);
  lcd.print(" cm   ");

  lcd.setCursor(0, 1); //sorvaltas

  if (tavolsag > 0) {
    //NAGYON KÖZEL: 5cm alatt
    if (tavolsag < 5) {
      lcd.print("!!! ALLJ !!!    ");
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1000); 
    } 
    //KÖZEL: 5-50cm
    else if (tavolsag < 50) {
      lcd.print("VIGYAZAT!       ");
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1000);
      delay(tavolsag * 10); 
      
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
      delay(tavolsag * 10);
    } 
    //BIZTONSÁGOS: 50cm+
    else {
      lcd.print("BIZTONSAGOS     ");
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
    }
  }

  delay(20);
}
```

# 6. Összegzés
A projekt sikeresen demonstrálja a mikrokontrollerek alkalmazhatóságát a vezetéstámogató rendszerekben. A prototípus költséghatékony, moduláris és továbbfejleszthető.


