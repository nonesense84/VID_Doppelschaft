byte subadress;
byte valueA;
byte valueB;

/*===Diese Funktion wird immer ausgeführt, wenn Daten vom Master empfangen werden und ist im setup() registriert===*/
void receiveEvent(int howMany)                                    //howMany gibt die Anzahl der ampfangenden Bytes an.
{
  if (howMany == 3){                                              //Es müssen genau 3 Bytes empfangen werden

    subadress = Wire.read();                                      //Das erste Byte stellt eine Art Verwendungszweck dar
    valueA = Wire.read();                                         //Dann folgen die beiden Nutzwerte A und B
    valueB = Wire.read();
  }
  else {                                                          //werden mehr oder weniger als 3 Bytes empfangen, sind... 
    while(0 < Wire.available()){                                  //...diese ungültig und werden in einer Schleife verschrottet.
      byte trash = Wire.read();
    }
  }
  switch (subadress){                                             //Mit der Subadress wird entschieden, was mit valueA und B passiert
  case 1:                                                         //Fall 1: MotorA Skalenwert in z.B. bar*10, km/h*10...
    DestPositionA = mapInputToPos(word(valueA,valueB));
    break;

  case 2:                                                         //Fall 2: MotorB Skalenwert in z.B. bar*10, km/h*10...
    DestPositionB = mapInputToPos(word(valueA,valueB));
    break;

  case 3:                                                         //Fall 3: Die Zeigerdämpfung soll verändert werden
    if (disableProgmode){                                         //Das geht aber nicht bei untersagter Programmierung (nach 30 Sekunden, siehe Loop)
      break;
    }
    configureAttenuation(valueA,valueB);
    EEPROM.write(4, valueA);
    EEPROM.write(5, valueB);
    break;

  case 4:                                                         //Fall 4: Der maximale Skalenwert soll festgelegt werden. Z.B. in bar*10, km/h*10...
    if (disableProgmode){                                         //Das geht aber nicht bei untersagter Programmierung (nach 30 Sekunden, siehe Loop)
      break;
    }
    MaxScalValue = (word(valueA,valueB));
    EEPROM.write(6, valueA);
    EEPROM.write(7, valueB);
    break;

  case 5:                                                         //Fall 5: Festgelegen, wieviele Schritte für Vollausschlag (MaxScalValue) gegangen werden
    if (disableProgmode){                                         //Das geht aber nicht bei untersagter Programmierung (nach 30 Sekunden, siehe Loop)
      break;
    }
    MaxStepValue = (word(valueA,valueB));
    EEPROM.write(8, valueA);
    EEPROM.write(9, valueB);
    break;
  case 6:                                                         //Fall 6: Es soll ein vollständiger Rücksetzvorgang der Zeiger vorgenommen werden...
    SyncPointers(true);                                           //Nützlich bei großen Schrittverlusten. Nur zu Not einsetzen!
    break;
  case 7:                                                         //Fall 7: I²C-Adresse ändern: A und B müssen identisch sein und zwischen 0 und 128 liegen...
    if (disableProgmode){                                         //Das geht aber nicht bei untersagter Programmierung (nach 30 Sekunden, siehe Loop)
      break;
    }
    if((valueA == valueB) && (valueA > 0)&& (valueA < 128)){     //Wer nicht weiß, was er tut, kann auf dem Bus lange nach dem Gerät suchen!
      EEPROM.write(10,valueA);                                   //Die I²C-Adresse wird im EEPROM im Register 10 abgelegt.
    }
    
    case 18:                                                     //Fall 8: Programmiermodus abschalten
      if((valueA == 128) && (valueB == 128)){                    //Wert A und B müssen 128 sein, um den Progmodus abszusachelten
        disableProgmode = false;                                       
      }
      break;
      
     case 19:                                                    //Fall 9: Programmiermodus einschalten
      if((valueA == 128) && (valueB == 128)){                    //Wert A und B müssen 128 sein, um den Progmodus abszusachelten
        disableProgmode = true;                                       
      }
      break;
      
    case 10:                                                     //Fall 9: Auslieferungszustand setzen
      if((valueA == 128) && (valueB == 128)){                    //Wert A und B müssen 128 sein
        EEPROM.write(11,255);                                      
      }
      break;
      
  default:
    //Nichts defaultmäßiges zu tun
    break;
  }
}

/*===Zu Debug-Zwecken: Bei Anfrage die Zeigerpositionen aus dem EEPROM an den Master übertragen===*/
void requestEvent()
{
  WireOutput[0] = EEPROM.read(1);
  WireOutput[1] = EEPROM.read(2);
  WireOutput[2] = EEPROM.read(3);
  WireOutput[3] = EEPROM.read(4);
  Wire.write(WireOutput,4);
}
