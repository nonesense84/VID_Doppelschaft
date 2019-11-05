//Zu wählendes Board: Diecimila ATmega168
byte i2cAdress;
void setup(){
  motorPolA = North;                          //Festlegen einer anfangspolung der Motoren
  motorPolB = North;
  pinMode(8, OUTPUT) ;                        //Festlegen der Ausgänge, an denen die Motoren angeschlossen sind
  pinMode(9, OUTPUT) ;
  pinMode(10, OUTPUT) ;
  pinMode(12, OUTPUT) ;
  pinMode(A0, OUTPUT) ;
  pinMode(A1, OUTPUT) ;
  pinMode(A2, OUTPUT) ;
  pinMode(A3, OUTPUT) ;
  getEEPromData();
  SyncPointers(false);                        //Einfache Zeigersyncronisation durchführen (Rücklauf von letzem Ist-Wert)
  i2cAdress = EEPROM.read(10);                //Auslesen der I²C-Adresse aus dem EEPROM
  if ((i2cAdress > 0) && (i2cAdress < 128)){  //Diese ist nur gültig, wenn sie zwischen einschließlich 1 und 127 liegt
    Wire.begin(i2cAdress);                    //I²C mit gelesener Adresse einschalten.
  }
  else{                                       //War die Adresse aus dem EEPROM nicht gültig, wird Adresse 5 angenommen
    Wire.begin(5);
  }
  Wire.onReceive(receiveEvent);               //In I²C-Bib festlegen, was beim Eintreffen von Daten aufgerufen werden soll
  Wire.onRequest(requestEvent);               //In I²C-Bib festlegen, was bei der Abfrage von Daten aufgerufen werden soll
  //...Nur für Debug-Zwecke
}

void getEEPromData(){                         // Aus dem EEProm holen wir die Settings für:
  if (EEPROM.read(11) == 255){                // Das Register 11 verwenden wir um zu erkennen, ob wir den Chip zum ersten mal verwenden. Falls ja,
                                              // werden einige Parameter erstmals festgelegt, die vom Benutzer später abgeändert werden können.
    EEPROM.write(4,100);                      // Standarddämpfung: 100
    EEPROM.write(5,100);                      // Standarddämpfung: 100
    EEPROM.write(6,4);                        // MaxScalValue = 1200;
    EEPROM.write(7,176);
    EEPROM.write(8,4);                        // MaxStepValue = 1080;
    EEPROM.write(9,56);
    EEPROM.write(11,0);                       // Jetzt wissen wir, dass der Chip ncht mehr neu ist.
  }

  configureAttenuation(EEPROM.read(4), EEPROM.read(5));    // -Die beiden Dämpfungen der Zeiger A und B
  MaxScalValue = (word(EEPROM.read(6),EEPROM.read(7)));    // -Den Höchsten Wert, den die Skala anzeigt (*10)
  MaxStepValue = (word(EEPROM.read(8),EEPROM.read(9)));    // -Wieviele Schritte es sind, um zum höchsten Punkt zu gehen
}







/*=============================================================================================
 Diese Funktion wird ausgeführt, um die Zeiger mit der gespeicherten Position zu Syncronisieren.
 Bei Schrittvelusten oder anderen Problemen 'hardreset' als TRUE übergeben.
 ===============================================================================================*/
void SyncPointers(boolean hardreset){
  DestPositionA = 0;                          //Die Zielpositionen sind 0
  DestPositionB = 0;
  if(hardreset){                              //Es soll ein harte Rücksetzvorgang der Zeiger vorgenommen werden
    for(int i=0; i <= 1080; i++){             //1080 Schritte (vollständige Drehung) sollen beide Zeiger nach unten laufen
      stepDownA();                            //Motor A bewegt sich einen Schritt runter
      delay(3);                               //Zwischen den Schritten sollen 3ms vergehen
      stepDownB();                            //Motor B bewegt sich einen Schritt runter
      delay(3);
    }
    AktPositionA = 0;                         //Ganz unten angekommen, kann Ist-Position als 0...
    AktPositionB = 0;                         //...(unterer Anschlag) angenommen werden
  }
  else{                                       //Normaler Zeigerreset mach Neustart.
    AktPositionA = word(EEPROM.read(0),EEPROM.read(1))+40;  //Die Aktuellen Zeigerpositionen (von vor dem Ausschalten)...
    AktPositionB = word(EEPROM.read(2),EEPROM.read(3))+40;  //...werden aus dem EEPROM geholt und wieder weiter verwendet.
    //...Zur Sicherheit werden noch 40 Schritte addiert.

    while(AktPositionA > DestPositionA){      //So lange, das Ziel für Zeiger A (unterer Anschlag, 0) nicht erreicht...
      stepDownA();                            //...wurde, machen wir Schritte nach unten...
      AktPositionA = AktPositionA - 1;        //...und zeiehn jeden schritt von der aktuellen Position ab.
      delay(3);                               //nur alle 3ms einen Schritt!
      if((AktPositionA - DestPositionA)<20){  //Im unteren Bereich sogar nur alle 6ms (3ms zusätzlich)
        delay(3);
      }
    }
    while(AktPositionB > DestPositionB){      //Das Gleiche mit Zeiger B
      stepDownB();
      AktPositionB = AktPositionB - 1;
      if ((AktPositionB - DestPositionB) < 20){
        delay(3);
      }
      delay(3);
    }
  }
  EEPROM.write(0, 0);                          //Nach der Syncronisation stehen die Zeiger bei 0,...
  EEPROM.write(1, 0);                          //...also am unteren Anschlag. Das wird im EEPROM vermerkt
  EEPROM.write(2, 0);
  EEPROM.write(3, 0);
}
