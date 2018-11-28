#include <IRremote.h>
#include <IRremoteInt.h>
int active  = 0;
unsigned long tempoDepart = 0;
unsigned long tempo = 0;

// ----------------- ** ARduino, réception des codes de la télécommande ------------- **
int broche_reception = 6; // broche 6 utilisée
IRrecv reception_ir(broche_reception); // crée une instance
decode_results decode_ir; // stockage données reçues

// ----------------** Registre à décalage** ------------------**
//Pin connected to ST_CP of (4094) , sinon le très utilisé 74HC595
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

// --------------- ** Codes en décimal de la télécommande, 1bit alterné lors d'appui sur la même touche --------- **
//de 1 à 9[1281,3329] ... [1289,3337]
//Play/pause [1333,3381]
//carré [1334,3382]

// ----------- ** Variables du match ------------- **
int sc1 = 0;
int sc2 = 0;
int sc1a = 0;
int sc1b = 0;
//tab représente [0,1,2...9] du binaire convertit en décimal
int tab[] = {252,96,218,242,38,182,190,224,254,246};

void setup()
{
  //pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  reception_ir.enableIRIn(); // démarre la réception

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  //shiftOut(dataPin, clockPin, MSBFIRST, 0);     
  digitalWrite(latchPin, HIGH);
}

void loop()
{

  //Seulement après réception de la télécommande
  if (reception_ir.decode(&decode_ir)){   

    Serial.println("ça décode");
    Serial.println(active);
     
    if(decode_ir.value == 1281 || decode_ir.value == 3329) {

    if(active == 0){
    active = 1; 
    tempoDepart = millis();
    Serial.println(tempoDepart); 
    
    }

      else if(active == 1){ //début du test

      //test de la temporisation
      Serial.println("tempo : ");
      Serial.println(millis() - tempoDepart); 
      Serial.println(active); 
      if(millis() - tempoDepart >= 200) {
    
      //if(allume) digitalWrite(ledPin, LOW);
      //else digitalWrite(ledPin, HIGH);
      //allume = !allume;
      //if(sc1 == 9) sc1 = 0;
      sc1++;
      sc1a = sc1%10;
      sc1b = sc1/10;
      Serial.print("score = ");
      Serial.println(sc1);
      Serial.print("a = ");
      Serial.println(sc1a);
      Serial.print("b = ");
      Serial.println(sc1b);
          active = 0;
      }
      }//fin du test      
    }    

    
    
    //Serial.println(decode_ir.value, HEX);
    reception_ir.resume(); // reçoit le prochain code
    Serial.println(decode_ir.value, DEC);
  }

 //test de tempo pour relachement du bouton
 // Arduino va trop vite, même si on reste appuyé il ne prend pas en compte pas la transmission
 /* else {
  active = 0;
  Serial.println("c'est la cata");
  Serial.println(active);
 }
 */

  
  //Envoi des données à afficher aux registres
  digitalWrite(latchPin, LOW);
  //Registres en série, le premier data envoyé est envoyé au dernier regsitre de la série
  shiftOut(dataPin, clockPin, LSBFIRST, tab[sc1a]);
  shiftOut(dataPin, clockPin, LSBFIRST, tab[sc1b]);        
    //shiftOut(dataPin, clockPin, MSBFIRST, tab[score1-1]); 
          
    //shiftOut(dataPin, clockPin, MSBFIRST, tab1[j]);
    //return the latch pin high to signal chip that it
    //no longer needs to listen for information
  digitalWrite(latchPin, HIGH);

  //delay(200);
}
