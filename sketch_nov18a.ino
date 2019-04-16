#include <IRremote.h>
#include <IRremoteInt.h>
int active  = 0;
int inverse = 0;
int chrono = 0;
int affichageTotaux = 0;
unsigned long tempoDepart = 0;
unsigned long tempo = 0;

// ----------------- ** ARduino, réception des codes de la télécommande ------------- **
int broche_reception = 6; // broche 6 utilisée
IRrecv reception_ir(broche_reception); // crée une instance
decode_results decode_ir; // stockage données reçues

// ----------------** Registre à décalage** ------------------**
//Pin connected to ST_CP of (4094) , sinon le très utilisé 74HC595
//int latchPin = 8;
//int clockPin = 12;
//int dataPin = 11;

int latchPin = 11;
int dataPin = 12;
int clockPin = 13;


int dataPinScoreDroite = 2;

// --------------- ** Codes en décimal de la télécommande, 1bit alterné lors d'appui sur la même touche --------- **
//de 1 à 9[1281,3329] ... [1289,3337]
//Play/pause [1333,3381] : chrono
//carré [1334,3382] : totaux
//On/Off [1356, 3404] : changement de set
//Forward [1312, 3360] : inversion de l'affichage


// ----------- ** Variables du match ------------- **
int sc1 = 0;
int sc2 = 0;

int sc1a = 0;
int sc1b = 0;

int sc2a = 0;
int sc2b = 0;

int totalSc1 = 0;
int totalSc2 = 0;

//tab représente [0,1,2...9] du binaire convertit en décimal
int tab[] = {252,96,218,242,102,182,190,224,254,246};

void setup()
{
  //pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  reception_ir.enableIRIn(); // démarre la réception

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(dataPinScoreDroite, OUTPUT);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);

  //tests pour le score droite, sur pin 2
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  
  digitalWrite(latchPin, HIGH);
}

void loop()
{

  //Seulement après réception de la télécommande
  if (reception_ir.decode(&decode_ir)){   

    Serial.println("ça décode");
    Serial.println(active);

    //Score de gauche + 1 : 
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
          
      sc1++;
      //enregistrement des totaux
      if(inverse) totalSc2 ++;
      else totalSc1 ++;
      
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

    //Score de Droite + 1 :
    else if(decode_ir.value == 1283 || decode_ir.value == 3331) {

      if(active == 0){
      active = 1; 
      tempoDepart = millis();
      Serial.println(tempoDepart);
      }

      else if(active == 1){ 
        
        if(millis() - tempoDepart >= 200) {
            
        sc2++;
        //enregistrement des totaux
        if(inverse) totalSc1 ++;
        else totalSc2 ++;
        active = 0;
        }
      }      
    }

      //Score de Gauche - 1 :
      else if(decode_ir.value == 1284 || decode_ir.value == 3332) {

        if(active == 0){
        active = 1; 
        tempoDepart = millis();
        Serial.println(tempoDepart);
        }
    
        else if(active == 1){ 
          
          if(millis() - tempoDepart >= 200) {
    
            if(sc1 > 0) {
              sc1--;
              //enregistrement des totaux
              if(inverse) totalSc2 --;
              else totalSc1 --;      
            }
            active = 0;
          }
        }      
      }

      //Score de Droite - 1 :
      else if(decode_ir.value == 1286 || decode_ir.value == 3334) {

        if(active == 0){
        active = 1; 
        tempoDepart = millis();
        Serial.println(tempoDepart);
        }
    
        else if(active == 1){ 
          
          if(millis() - tempoDepart >= 200) {
    
            if(sc2 > 0) {
              sc2--;
              //enregistrement des totaux
              if(inverse) totalSc1 --;
              else totalSc2 --;
            }
            active = 0;
          }
        }      
      }

      // //Appui sur la touche Forward [1312, 3360] - inversion des scores
      else if(decode_ir.value == 1312 || decode_ir.value == 3360) {

        if(active == 0){
        active = 1; 
        tempoDepart = millis();
        Serial.println(tempoDepart);
        }

        else if(active == 1){ 
          
          if(millis() - tempoDepart >= 200) {

            int tmp = sc1;
            sc1 = sc2;
            sc2 = tmp;
            inverse = !inverse;
            active = 0;
          }
        }
      }

      // //Appui sur la touche Play/pause [1333,3381] - Mise en route du CHRONO
      else if(decode_ir.value == 1333 || decode_ir.value == 3381) {

        if(active == 0){
        active = 1; 
        tempoDepart = millis();
        Serial.println(tempoDepart);
        }

        else if(active == 1){ 
          
          if(millis() - tempoDepart >= 200) {
    
            chrono = 1;
            active = 0;
          }
        }
      }

      //Appui prolongé sur On/Off [1356, 3404] - Changement de Set
      else if(decode_ir.value == 1356 || decode_ir.value == 3404) {

        if(active == 0){
        active = 1; 
        tempoDepart = millis();
        Serial.println(tempoDepart);
        }

        else if(active == 1){ 
          
          if(millis() - tempoDepart >= 2000) { 

            //remise à zéro et inversion de l'affichage
            sc1 = 0;
            sc2 = 0;
            inverse = !inverse;            
            active = 0;
            
          }
        }
      }

      //Appui prolongé sur //carré [1334,3382] - affichage des TOTAUX
      else if(decode_ir.value == 1334 || decode_ir.value == 3382) {

        if(active == 0){
        active = 1; 
        tempoDepart = millis();
        Serial.println(tempoDepart);
        }

        else if(active == 1){ 
          
          if(millis() - tempoDepart >= 500) { 
            
            affichageTotaux = 1;           
            active = 0;
            
          }
        }
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
  sc1a = sc1/10;
  sc1b = sc1%10;
  
  sc2a = sc2/10;
  sc2b = sc2%10;
  
  

  //CHRONO de 30 secondes, arduino bloqué dans la boucle
  //pendant le décompte
  if(chrono){
    for(int i = 30; i>=0; i--) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 0);
      shiftOut(dataPin, clockPin, LSBFIRST, 0);
    
      //Score de droite DataPin 2
      shiftOut(dataPin, clockPin, LSBFIRST, tab[i/10]);
      shiftOut(dataPin, clockPin, LSBFIRST, tab[i%10]);
      digitalWrite(latchPin, HIGH);
      delay(1000);
    }
    chrono = 0;
  }

  //affiche le total des scores sc1 à gauche, sc2 à droite
  //affiche pendant 10 secondes
   else if(affichageTotaux){
    if(totalSc1 > 99) totalSc1 %= 100;
    if(totalSc2 > 99) totalSc2 %= 100;
    //On garde l'affichage des totaux suivant la position des équipes à l'instant T
    if(!inverse) {
      for(int i = 10; i>=0; i--) {
        digitalWrite(latchPin, LOW);
        //score de gauche = total sc1
        //Score de droite = total sc2

        //affichage de droite
        shiftOut(dataPin, clockPin, LSBFIRST, tab[totalSc2/10]);
        shiftOut(dataPin, clockPin, LSBFIRST, tab[totalSc2%10]);      

        //affichage de gauche
        shiftOut(dataPin, clockPin, LSBFIRST, tab[totalSc1/10]);
        shiftOut(dataPin, clockPin, LSBFIRST, tab[totalSc1%10]);
        digitalWrite(latchPin, HIGH);
        delay(1000);
      }
    }

    //affichage TOTAUX
    //Les shiftOut se font en ordre inversé
    else if(inverse){
      for(int i = 10; i>=0; i--) {
        digitalWrite(latchPin, LOW);
        //score de gauche devient total de sc2
        //Score de droite devient total de sc1

        //affichage de droite
        shiftOut(dataPin, clockPin, LSBFIRST, tab[totalSc1/10]);
        shiftOut(dataPin, clockPin, LSBFIRST, tab[totalSc1%10]);      

        //affichage de gauche
        shiftOut(dataPin, clockPin, LSBFIRST, tab[totalSc2/10]);
        shiftOut(dataPin, clockPin, LSBFIRST, tab[totalSc2%10]);
        digitalWrite(latchPin, HIGH);
        delay(1000);
      }
    }
    affichageTotaux = 0;
  }

  //Envoi des données à afficher aux registres
  digitalWrite(latchPin, LOW);
  //Registres en série, le premier data envoyé est envoyé au dernier registre de la série
  //Le dernier registre correspond au score de Droite
  shiftOut(dataPin, clockPin, LSBFIRST, tab[sc2a]);
  shiftOut(dataPin, clockPin, LSBFIRST, tab[sc2b]);

  //Score de droite DataPin 2
  shiftOut(dataPin, clockPin, LSBFIRST, tab[sc1a]);
  shiftOut(dataPin, clockPin, LSBFIRST, tab[sc1b]);  
  
    //return the latch pin high to signal chip that it
    //no longer needs to listen for information
  digitalWrite(latchPin, HIGH);

  //delay(200);
}
