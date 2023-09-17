/*===========================================================================
Die Schrittmotoren sollen 8 Positionen einnehmen können.
Diese sind nach Himmelsrichtungen benannt.
Diese Positionen sind NICHT die des Zeigers, sondern die des Getriebeantriebs!
=============================================================================*/
#define North 0
#define NorthEast 1
#define East 2
#define SouthEast 3
#define South 4
#define SouthWest 5
#define West 6
#define NorthWest 7

byte motorPolA = 0;          //Die aktuelle Himmelsrichtung von A
byte motorPolB = 0;          //Die aktuelle Himmelsrichtung von B

/*===========================================================================
Das Prinzip ist immer das gleiche:
Soll Zeiger rauf oder runter, wird anhand er aktuellen Himmelsrichtung
entschieden, welche er als nächstes bekommt. Diese neue Position wird für den
nächsten Schritt zwischengespeichert.
=============================================================================*/

/*===Zeiger A soll einen Schritt nach oben machen===*/
void stepUpA(){
  switch (motorPolA) {
  case North:
    motorPolA = NorthEast;
    setNorthEastA();
    break;

  case NorthEast:
    motorPolA = East;
    setEastA();
    break;

  case East:
    motorPolA = SouthEast;
    setSouthEastA();
    break;

  case SouthEast:
    motorPolA = South;
    setSouthA();
    break;

  case South:
    motorPolA = SouthWest;
    setSouthWestA();
    break;

  case SouthWest:
    motorPolA = West;
    setWestA();
    break;

  case West:
    motorPolA = NorthWest;
    setNorthWestA();
    break;

  case NorthWest:
    motorPolA = North;
    setNorthA();
    break;
    //default:
  }
}
/*===Zeiger A soll einen Schritt nach unten machen===*/
void stepDownA(){
  switch (motorPolA) {
  case East:
    motorPolA = NorthEast;
    setNorthEastA();
    break;

  case SouthEast:
    motorPolA = East;
    setEastA();
    break;

  case South:
    motorPolA = SouthEast;
    setSouthEastA();
    break;

  case SouthWest:
    motorPolA = South;
    setSouthA();
    break;

  case West:
    motorPolA = SouthWest;
    setSouthWestA();
    break;

  case NorthWest:
    motorPolA = West;
    setWestA();
    break;

  case North:
    motorPolA = NorthWest;
    setNorthWestA();
    break;

  case NorthEast:
    motorPolA = North;
    setNorthA();
    break;
    //default:
  }
}
/*===Zeiger B soll einen Schritt nach oben machen===*/
void stepUpB(){
  switch (motorPolB) {
  case North:
    motorPolB = NorthEast;
    setNorthEastB();
    break;

  case NorthEast:
    motorPolB = East;
    setEastB();
    break;

  case East:
    motorPolB = SouthEast;
    setSouthEastB();
    break;

  case SouthEast:
    motorPolB = South;
    setSouthB();
    break;

  case South:
    motorPolB = SouthWest;
    setSouthWestB();
    break;

  case SouthWest:
    motorPolB = West;
    setWestB();
    break;

  case West:
    motorPolB = NorthWest;
    setNorthWestB();
    break;

  case NorthWest:
    motorPolB = North;
    setNorthB();
    break;
    //default:
  }
}
/*===Zeiger B soll einen Schritt nach unten machen===*/
void stepDownB(){
  switch (motorPolB) {
  case East:
    motorPolB = NorthEast;
    setNorthEastB();
    break;

  case SouthEast:
    motorPolB = East;
    setEastB();
    break;

  case South:
    motorPolB = SouthEast;
    setSouthEastB();
    break;

  case SouthWest:
    motorPolB = South;
    setSouthB();
    break;

  case West:
    motorPolB = SouthWest;
    setSouthWestB();
    break;

  case NorthWest:
    motorPolB = West;
    setWestB();
    break;

  case North:
    motorPolB = NorthWest;
    setNorthWestB();
    break;

  case NorthEast:
    motorPolB = North;
    setNorthB();
    break;
    //default:
  }
}

/*===========================================================================
Die folgenden Funktionen setzen eine High/Low-Kombination an die
Spulenscnschlüsse, um eine entsprechnde Himmelsrichtung einzustellen.
=============================================================================*/
void setNorthA(){
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  digitalWrite(12,HIGH);
}
void setNorthEastA(){
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(12,HIGH);
}
void setEastA(){
  digitalWrite(8,HIGH);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(12,HIGH);
}
void setSouthEastA(){
  digitalWrite(8,HIGH);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(12,LOW);
}
void setSouthA(){
  digitalWrite(8,HIGH);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
  digitalWrite(12,LOW);
}
void setSouthWestA(){
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
  digitalWrite(12,LOW);
}
void setWestA(){
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(12,LOW);
}
void setNorthWestA(){
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  digitalWrite(12,LOW);
}
void setNorthB(){
  digitalWrite(A0,LOW);
  digitalWrite(A1,HIGH);
  digitalWrite(A2,LOW);
  digitalWrite(A3,HIGH);
}
void setNorthEastB(){
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,HIGH);
}
void setEastB(){
  digitalWrite(A0,HIGH);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,HIGH);
}
void setSouthEastB(){
  digitalWrite(A0,HIGH);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,LOW);
}
void setSouthB(){
  digitalWrite(A0,HIGH);
  digitalWrite(A1,LOW);
  digitalWrite(A2,HIGH);
  digitalWrite(A3,LOW);
}
void setSouthWestB(){
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,HIGH);
  digitalWrite(A3,LOW);
}
void setWestB(){
  digitalWrite(A0,LOW);
  digitalWrite(A1,HIGH);
  digitalWrite(A2,HIGH);
  digitalWrite(A3,LOW);
}
void setNorthWestB(){
  digitalWrite(A0,LOW);
  digitalWrite(A1,HIGH);
  digitalWrite(A2,LOW);
  digitalWrite(A3,LOW);
}
/*===Diese Funktion schaltet die Spulen vollständig ab===*/
void switchOffMotorA(){
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(12,LOW);
}
void switchOffMotorB(){
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,LOW);
}
