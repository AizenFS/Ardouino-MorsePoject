#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,10,8);
const int shortDuration = 200;
const int longDuration = 500;
const int buttonPin = 7;
const int backspacePin = 3;
const int translationPin = 2;
const int buzzerPin = 9;
volatile int cursorX = 0;
volatile int cursorY = 0;
const int maxLength = 6;
volatile char inputSeq[maxLength] = "******";
volatile int index = 0;


char* letters[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
};
char* numbers[] = {
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

void backSpace(){//interrupt pin 3
  //Serial.println("back");
  if(cursorX==0&&cursorY==1){
    lcd.write(' ');
    cursorX=15;
    cursorY=0;
    lcd.setCursor(cursorX,cursorY);
  }
  else if (cursorX>0){
    cursorX--;
    lcd.setCursor(cursorX,cursorY);
    lcd.write(' ');
    lcd.setCursor(cursorX,cursorY);
  }
  if(index>0){
    index--; 
  }
}

void translate(){//interrupt pin 2
  char ts='?';
  char real[index];
  for(int i=0;i<index;i++){
    real[i]=inputSeq[i];
  }
  for(int i=0;i<26;i++){
    if(strcmp(letters[i],real)==0){
      ts=(char)(i+97);
      break;
    }
    if(i<=9&&strcmp(numbers[i],real)==0){
      ts=(char)(i+48);
      break;
    }
  }
  removePlaces(index);
  publish(ts);
  resetInput();

}

void setup() {
  pinMode(translationPin,INPUT);
  pinMode(buttonPin,INPUT);
  pinMode(backspacePin,INPUT);
  pinMode(buzzerPin,OUTPUT);
  attachInterrupt(1,backSpace,RISING);
  attachInterrupt(0,translate,RISING);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.home();
  lcd.leftToRight();
  lcd.cursor();
}

void loop() {
  if(cursorX==16){
    cursorY=1;
    cursorX=0;
    lcd.setCursor(cursorX,cursorY);
  }
  long pressDuration = pulseIn(7,LOW,30000000/*30 sec*/)/1000;
  if(pressDuration < shortDuration &&  pressDuration > 20){
    tone(buzzerPin,500,shortDuration);
    publish('.');
  }
  else if(pressDuration < longDuration){
    tone(buzzerPin,500,longDuration);
    publish('-');
  }
  else{
    publish(' ');
  }

  if(Serial.available()>0 && Serial.read()=='#'){
    lcd.clear();
    lcd.home();
    cursorX=0;
    cursorY=0;
  }
}

void publish(char c){
  lcd.write(c);
  Serial.print(c);
  cursorX++;
  if(c!=' '){
    addChar(c);
  }
  else{
    resetInput();
  }
}

void removePlaces(int n){
  for(int i=0;i<n;i++){
    backSpace();
  }
}

void addChar(char c){
  inputSeq[index]=c;
  index++;
}

void resetInput(){
  for(int i=0;i<maxLength;i++){
    inputSeq[i]='*';
  }
  index=0;
}








