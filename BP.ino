//Pin connected to ST_CP of 74HC595
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

//Sorties du register 4094
int tab[] = {1,2,4};

//Bouton Poussoir
int bpPin = 2;
int bpInverse = 3;

int score1 = 0;
int score2 = 1;     

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //set BP
  pinMode(bpPin, INPUT);
  pinMode(bpInverse, INPUT);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);     
  digitalWrite(latchPin, HIGH);
}

     

void loop() {

  //affichage debug
  Serial.begin(9600);

  //lecture du bouton poussoir
  boolean bp = digitalRead(bpPin);
  boolean inverser = digitalRead(bpInverse);

    if(bp) {
      while(digitalRead(bpPin)){
        delay(10);
      }
      score1++;
      score2++;
      if(score1 == 4) score1 = 1;
      if(score2 == 4) score2 = 1;
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, tab[score2-1]);       
      shiftOut(dataPin, clockPin, MSBFIRST, tab[score1-1]); 
          
      //shiftOut(dataPin, clockPin, MSBFIRST, tab1[j]);
      //return the latch pin high to signal chip that it
      //no longer needs to listen for information
      digitalWrite(latchPin, HIGH);
      Serial.print("score 1 = ");
      Serial.print(score1);
      Serial.println();
      Serial.print("score 2 = ");
      Serial.print(score2);
     
    }   
    

    if(inverser) {
      while(digitalRead(bpInverse)){
        delay(10);}
      int tmp = score1;
      score1 = score2;
      score2 = tmp;
      //inverser(score1, score2);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, tab[score2-1]);
      shiftOut(dataPin, clockPin, MSBFIRST, tab[score1-1]);      
      //shiftOut(dataPin, clockPin, MSBFIRST, tab1[j]);
      //return the latch pin high to signal chip that it
      //no longer needs to listen for information
      digitalWrite(latchPin, HIGH);
       Serial.print("score 1 = ");
      Serial.print(score1);
      Serial.println();
      Serial.print("score 2 = ");
      Serial.print(score2);
    }

      

  
  delay(100);
}

//void inverser(int* score1, int* score2) {
//  int tmp = *score1;
//  *score1 = *score2;
//  *score2 = tmp;
//}

