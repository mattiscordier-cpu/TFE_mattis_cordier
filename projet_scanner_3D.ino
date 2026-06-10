//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//sips_6
//mattis cordier 6e A
//hardware:
//  micro-controleur: M5 stack core basic
//  driver moteur pap : TB6600
//  moteur pap: moons 17HD4001-28N
//  moteur DC: SR5840-395B
//  capteur hall: KY-003
//09/04/2025
//°°°°°°°°°°°°°°°°°°°°°°°librairie°°°°°°°°°°°°°°°°°°°°°°°°°°
#include <M5Stack.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
//°°°°°°°°°°°°°°°°°°°°°°°°°objet°°°°°°°°°°°°°°°°°°°°°°°°°°°°
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
//°°°°°°°°°°°°°°°°definition des consatantes°°°°°°°°°°°°°°°°
#define PUL 26  //define Pulse pin
#define DIR 5   //define Direction pin
#define MOTEUR 16
#define CAPTEUR_HALL 35
#define ENCODEUR 36
#define SWITCH0 17

//°°°°°°°°°°°°°°°°°°°°°°°°variable°°°°°°°°°°°°°°°°°°°°°°°°°°
int nb_tour = -1;
int x = 0;
unsigned long temps_1;
unsigned long temps_2;
unsigned long interval;
//°°°°°°°°°°°°°°°°°°°°°°initialisation°°°°°°°°°°°°°°°°°°°°°°
void setup() {
  M5.begin();        // allume la M5
  M5.Power.begin();  //active l'alimentation de la M5
  Wire.begin(21, 22);
  Wire.setClock(400000); // definit la vitesse de l'I2C a 400000 hz
  Serial.begin(115200); // definit le baude rate a 115200 baud
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT_OPEN_DRAIN);
  digitalWrite(DIR, HIGH);  //descend
  pinMode(MOTEUR, OUTPUT);
  digitalWrite(MOTEUR, HIGH);
  pinMode(SWITCH0, INPUT);
  pinMode(ENCODEUR, INPUT);
  pinMode(CAPTEUR_HALL, INPUT);
  M5.Lcd.setTextColor(YELLOW);

  //verifie que le capteur est bien present
  Serial.println("verrif capteur");
  if (!lox.begin(0x29, false, &Wire)) {
    Serial.println("Erreur capteur !");
    while (1) {
      //Serial.println("while 1");
    }
  } else {
    Serial.println("Capteur OK !");
  }
  // descend le capteur
  while (digitalRead(SWITCH0) == HIGH) {
    digitalWrite(PUL, LOW);
    delay(1);
    digitalWrite(PUL, HIGH);
    delay(1);
  }

  //stop le capteur
  delay(1000);
  //monte 100 pas
  digitalWrite(DIR, LOW);
  for (int i = 0; i < 100; i++) {
    digitalWrite(PUL, LOW);
    delay(1);
    digitalWrite(PUL, HIGH);
    delay(1);
  }
  temps_1 = millis();
  while (1) {
    int valeur = analogRead(CAPTEUR_HALL);  // 0 à 4095
    float tension = valeur * (3.3 / 4095.0);
    //Serial.println("while 1");
    digitalWrite(MOTEUR, LOW);

    if (tension > 2.0 || tension < 1.4) {
      Serial.println("sortie while 1");
      break;
    }
  }
  digitalWrite(MOTEUR, HIGH);
  temps_2 = millis();
  interval = (temps_2 - temps_1) / 10;
  Serial.print("intervale / 10 : ");
  Serial.println(interval);

  /Serial.println("fin setup");
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°loop°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void loop() {

  M5.update();  // ne pas oublier
  if (M5.BtnA.wasPressed()) {
    Serial.println("debut du scan");

    while (1) {
      VL53L0X_RangingMeasurementData_t measure;  // déclaration de la structure
      lox.rangingTest(&measure, false); // mesure
      if (measure.RangeStatus == 0 && measure.RangeMilliMeter <= 240) {
        Serial.print("Distance (mm): ");
        Serial.println(measure.RangeMilliMeter);
        break; 
      } else {
        Serial.println("veillez posse un objet sur le plateau");
      }
    }
    temps_1 = millis();

    for (int hauteur = 0; hauteur < 55; hauteur++) {
      for (int i = 0; i < 100; i++) {
        digitalWrite(PUL, LOW);
        delay(1);
        digitalWrite(PUL, HIGH);
        delay(1);
        Serial.println(hauteur);
      }
      for (int mesure = 0; mesure < 10; mesure++) {

        VL53L0X_RangingMeasurementData_t measure; 
        digitalWrite(MOTEUR, LOW);
        lox.rangingTest(&measure, false);
        if (measure.RangeStatus == 0 && measure.RangeMilliMeter <= 240) {
          Serial.print("mesure : ");
          Serial.println(mesure);
          Serial.print("Distance (mm): ");
          Serial.println(measure.RangeMilliMeter);
        } else {
          Serial.print("mesure : ");
          Serial.println(mesure);
          Serial.println("aucun objet detecter");
        }
        Serial.print("intervale / 10 : ");
        Serial.println(interval);

        delay(interval);
      }
      while (1) {
        int valeur = analogRead(CAPTEUR_HALL);  // 0 à 4095
        float tension = valeur * (3.3 / 4095.0);
        if (tension > 2.0 || tension < 1.4) {
          digitalWrite(MOTEUR, HIGH);
          break;
        }
      }
    }
  }
}
