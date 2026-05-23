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
//°°°°°°°°°°°°°°°°°°°°°°°°°objet°°°°°°°°°°°°°°°°°°°°°°°°°°°°
BluetoothSerial SerialBT;
//°°°°°°°°°°°°°°°°definition des consatantes°°°°°°°°°°°°°°°°
#define PUL 26  //define Pulse pin
#define DIR 5   //define Direction pin
#define ENA 3
#define MOTEUR 16
#define CAPTEUR_HALL 30
#define SWITCH0 17
//°°°°°°°°°°°°°°°°°°°°°°°°variable°°°°°°°°°°°°°°°°°°°°°°°°°°
int nb_tour = -1;
int x = 0;
char BT_data;
const char *pin = "1234";  //creation du code pin
//°°°°°°°°°°°°°°°°°°°°°°initialisation°°°°°°°°°°°°°°°°°°°°°°
void setup() {
  M5.begin();        // allume la M5
  M5.Power.begin();  //active l'alimentation de la M5
  Serial.begin(115200);
  SerialBT.begin("louis");  // activation du bluetooth et definition du nom affiche dans bluetooth electronics
  SerialBT.setPin(pin);     // definition du code pin necesaire dans bluetooth electronics avec la variable cree plus haute

  Serial.println("Bluetooth actif !");
  Serial.println("Connecte-toi à : louis");
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT_OPEN_DRAIN);
  pinMode(MOTEUR, OUTPUT);
  pinMode(CAPTEUR_HALL, INPUT);
  digitalWrite(MOTEUR, HIGH);
  pinMode(SWITCH0, INPUT);

  M5.Lcd.setTextColor(YELLOW);  //deffinition de la couleur en jaune
  M5.Lcd.setTextSize(2);        //deffinition de la taille du texte a 2
  M5.Lcd.setCursor(3, 20);      //deffinition de la position du curseur
  M5.Lcd.println("bp A debut scan");
  attachInterrupt(digitalPinToInterrupt(39), scan, FALLING);
}
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°loop°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void loop() {
  void scan(void) {  //verifie si le bouton est relache ou presse assez longtemp
    {
      Serial.println("entre interuption");
      digitalWrite(DIR, HIGH);
      ledcSetup(0, 400, 16);  // creation d'un signal PWM sur le canal 0 d'une frequence de 400 hz et d'une resolution de 16 bit
      ledcAttachPin(PUL, 0);  // attachement du PWM du canal 0 sur la pin PUL(26)
      ledcWrite(0, 128);
      if (digitalRead(SWITCH0) == LOW) {
        digitalWrite(DIR, LOW);
        delay(500);
        ledcWrite(0, 0);
        Serial.println("niveau 0");
      }
    }
  }
}
