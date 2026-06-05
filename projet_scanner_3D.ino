//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//sips_5
//mattis cordier 6e A
//hardware:
//  micro-controleur: M5 stack core basic
//  driver moteur pap : TB6600
//  moteur pap: moons 17HD4001-28N
//  moteur DC: SR5840-395B
//  capteur hall: KY-003
//09/04/2025
//°°°°°°°°°°°°°°°°°°°°°°°librairie°°°°°°°°°°°°°°°°°°°°°°°°°°
#include "BluetoothSerial.h"
#include <M5Stack.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
//°°°°°°°°°°°°°°°°°°°°°°°°°objet°°°°°°°°°°°°°°°°°°°°°°°°°°°°
BluetoothSerial SerialBT;
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
char BT_data;
const char *pin = "1234";  //creation du code pin
unsigned long temps_1;
unsigned long temps_2;

//°°°°°°°°°°°°°°°°°°°°°°initialisation°°°°°°°°°°°°°°°°°°°°°°
void setup() {
  M5.begin();        // allume la M5
  M5.Power.begin();  //active l'alimentation de la M5
  Wire.begin(21, 22);
  Wire.setClock(400000);
  Serial.begin(115200);
  SerialBT.begin("louis");  // activation du bluetooth et definition du nom affiche dans bluetooth electronics
  SerialBT.setPin(pin);     // definition du code pin necesaire dans bluetooth electronics avec la variable cree plus haute
  Serial.println("Bluetooth actif !");
  Serial.println("Connecte-toi à : louis");
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT_OPEN_DRAIN);
  digitalWrite(DIR, HIGH);  //descend
  pinMode(MOTEUR, OUTPUT);
  digitalWrite(MOTEUR, HIGH);
  pinMode(SWITCH0, INPUT);
  pinMode(ENCODEUR, INPUT);
  pinMode(CAPTEUR_HALL, INPUT);
  M5.Lcd.setTextColor(YELLOW);  //deffinition de la couleur en jaune
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

    Serial.println(tension);
    if (tension > 2.0 || tension < 1.4) {
      Serial.println("sortie while 1");
      break;
    }
  }
  digitalWrite(MOTEUR, HIGH);
  temps_2 = millis();
  unsigned long intervalle = temps_2 - temps_1;
  temps_1 = temps_2;  // mémorisation pour le prochain tour
  Serial.println(intervalle);

  Serial.println("fin setup");
}


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°loop°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void loop() {
  M5.update();  // ne pas oublier
  int valeur = analogRead(CAPTEUR_HALL);
  float tension = valeur * (3.3 / 4095.0);  // convertion anlog -> volts

  if (M5.BtnA.wasPressed()) {
    Serial.println("debut du scan");

    VL53L0X_RangingMeasurementData_t measure;  // déclaration EN PREMIER
    lox.rangingTest(&measure, false);

    if (measure.RangeStatus == 0 && measure.RangeMilliMeter <= 240) {
      Serial.print("Distance (mm): ");
      Serial.println(measure.RangeMilliMeter);
    } else {
      Serial.println("aucun objet detecter");
    }
    for (int hauteur = 0; hauteur < 55; hauteur++) {
      for (int i = 0; i < 100; i++) {
        digitalWrite(PUL, LOW);
        delay(1);
        digitalWrite(PUL, HIGH);
        delay(1);
        Serial.println(hauteur);
      }
      for (int mesure = 0; mesure < 10; mesure++) {

        VL53L0X_RangingMeasurementData_t measure;  // déclaration EN PREMIER
        lox.rangingTest(&measure, false);

        if (measure.RangeStatus == 0 && measure.RangeMilliMeter <= 240) {
          Serial.print("Distance (mm): ");
          Serial.println(measure.RangeMilliMeter);
        } else {
          Serial.println("aucun objet detecter");
        }
        Serial.println(mesure);
        delay(interval / 10);
      }
    }
  }
}

/*  
int valeur = analogRead(CAPTEUR_HALL);    // 0 à 4095
  float tension = valeur * (3.3 / 4095.0);  // conversion en volts
  if (tension > 2.0 || tension < 1.4) {
    digitalWrite(MOTEUR, HIGH);
  } else {
    digitalWrite(MOTEUR, LOW);
  }

  if (tension > 2.0 || tension < 1.4) {
    digitalWrite(MOTEUR, HIGH);
    temps_2 = millis();
    unsigned long intervalle = temps_2 - temps_1;
    temps_1 = temps_2;  // mémorisation pour le prochain tour
    Serial.println(intervalle);
  }
*/
