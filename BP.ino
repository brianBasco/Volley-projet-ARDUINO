//Pin connected to ST_CP of 74HC595
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

//Sorties du register 4094
int tab[] = {0,8,4,2,1};
//int tab1[] = {2,2,2,2,2};

//Bouton Poussoir
int bpPin = 2;

int score = 0;

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //set BP
  pinMode(bpPin, INPUT);
}

void loop() {

  //affichage debug
  Serial.begin(9600);

  //lecture du bouton poussoir
  boolean bp = digitalRead(bpPin);

    if(bp) {
      while(digitalRead(bpPin)){
        delay(10);
      }
      score++;
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, tab[score]);
      //shiftOut(dataPin, clockPin, MSBFIRST, tab1[j]);
      //return the latch pin high to signal chip that it
      //no longer needs to listen for information
      digitalWrite(latchPin, HIGH);
      Serial.println(score);
      if(score == 4) score = 0;
    }

  
  delay(100);
}
