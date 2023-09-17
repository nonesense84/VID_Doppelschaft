//Doppelschaftmotoren mit Anschlag SW1401C
#include <Wire.h>                 //Unsere Baugruppe benötigt I²C
#include <EEPROM.h>               //I²C-Adresse (nach Änderung) und Letzte Zeigerposition liegen im EEPROM
int AktPositionA = 0;            //Die Aktuellen und die Zielpositionen unserer Zeiger
int AktPositionB = 0;
int DestPositionA = 0;
int DestPositionB = 0;

long previousStepMillisA = 0;    //Die Zeitpunkte, an denen zuletzt ein Zeiger bewegt wurde
long previousStepMillisB = 0;
int attenuationA;                //Die beiden Dämpfungen der Zeiger A und B
int attenuationB;
byte stepIntervallA;             //Der zeitliche Abstand zwischen den Schritten
byte stepIntervallB;

boolean storeA;                  //Wenn 'true', sollen die Zeigerpositionen in den EEPROM (Nach jeder Bewegung)
boolean storeB;
byte WireOutput[4];              //Nur zum Debuggen. Eine Möglichkeit Daten auszugeben
unsigned long currentMillis;     //Die Zeit in Millisekunden, seit dem Einschalten
int MaxScalValue;                //Der größte Skalenwert * 100
int MaxStepValue;                //Die Schrittzahl beim größen Skalenwert
boolean disableProgmode = true; //Wenn 'true', dürfen nur noch die Zeigerpositionen geändert werden


void loop(){
  currentMillis = millis();                                                     //Merken, wann loop zuletzt ausgeführt wurde
  /*=========Der Vorgang Auf-/Abwärtsschritte festzustellen und auszuführen für Zeiger B=========*/
  stepIntervallB = (int)(1+ attenuationB*3/abs(DestPositionB - AktPositionB));  //Schrittabstand aus Ist-/Zieldifferenz und Dämpfung berechenen
  if(currentMillis - previousStepMillisB >= stepIntervallB) {                   //Wenn genügen Zeit zwischen den Schritten vegangen ist, schreiten wir
    previousStepMillisB = currentMillis;		                                    //Merken, wann wir zuletzt geschritten sind
    if(AktPositionB > DestPositionB){                                           //Wenn Ziel- kleiner als Istposition, dann...
      stepDownB();                                                              //...machen wir einen Schritt runter
      AktPositionB = AktPositionB - 1;                                          //Istposition entsprechend erniedrigen
      if (AktPositionB == DestPositionB){                                       //Wenn an Zielposition angekommen, dann soll diese ins EEPROM.
        storeB = true;                                                          //EEPROM-Schreibvorgang vormerken
      }
    }	
    if(AktPositionB < DestPositionB){                                           //Wenn Ziel- größer als Istposition, dann...
      stepUpB();                                                                //...machen wir einen Schritt rauf
      AktPositionB = AktPositionB +1;                                           //Istposition entsprechend erhöhen
      if (AktPositionB == DestPositionB){                                       //Wenn an Zielposition angekommen, dann soll diese ins EEPROM.
        storeB = true;                                                          //EEPROM-Schreibvorgang vormerken
      }
    }
  }
  /*=========Der Vorgang Auf-/Abwärtsschritte festzustellen und auszuführen für Zeiger A=========*/
  stepIntervallA = (int)(1+ attenuationA*3/abs(DestPositionA - AktPositionA));  //Alles wie bei B
  if(currentMillis - previousStepMillisA >= stepIntervallA) {
    previousStepMillisA = currentMillis;
    if(AktPositionA > DestPositionA){
      stepDownA();
      AktPositionA = AktPositionA - 1;
      if (AktPositionA == DestPositionA){
        storeA = true;
      }
    }	
    if(AktPositionA < DestPositionA){
      stepUpA();
      AktPositionA = AktPositionA +1;
      if (AktPositionA == DestPositionA){
        storeA = true;
      }
    }
  }
  /*=========Speichervorgang für die Aktuellen Zeigerpositionen=========*/
  if (storeA){                                                                    //Wenn Speichervorgang für A vorgemerkt, dann...
    EEPROM.write(0, ((AktPositionA >> 8) & 0xFF));                                //...schreiben wir das Highbyte ins EEPROM auf Adresse 1...
    EEPROM.write(1, ((AktPositionA >> 0) & 0xFF));                                //...und das Lowbyte nach 2
    storeA = false;                                                               //Speichervormerkung deaktivieren, da ja nun gespeichert
    switchOffMotorA();
  }
  if (storeB){                                                                    //Wenn Speichervorgang für B vorgemerkt, dann...
    EEPROM.write(2, ((AktPositionB >> 8) & 0xFF));                                //...schreiben wir das Highbyte ins EEPROM auf Adresse 3...
    EEPROM.write(3, ((AktPositionB >> 0) & 0xFF));                                //...und das Lowbyte nach 4
    storeB = false;                                                               //Speichervormerkung deaktivieren, da ja nun gespeichert
    switchOffMotorB();
  }
}
  /*=========Der Eingabewert kommt als Skalenwert. Hier wird dieser einem Schrittwert zugeordnet=========*/
word mapInputToPos(word input){
  input = constrain(input, 0, MaxScalValue);                                     //Begrenzt Werte außerhalb des zulässigen Skalenbereichs. 
  return map(input, 0 , MaxScalValue, 0, MaxStepValue);                          //Mapt die Skalenwerte auf die Schrittwerte.
}
  /*=========Die Dämpfung der Zeiger bewirkt eine Verlamsamung vor Erreichen des Ziels=========*/
void configureAttenuation(byte Lowbyte, byte Highbyte){                          //Dämpfung einstellen
  attenuationA = Lowbyte;                                                        //Das Lowbyte dämpft Zeiger A
  attenuationB = Highbyte;                                                       //Das Highbyte dämpft Zeiger B
}
