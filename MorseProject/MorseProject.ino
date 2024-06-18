#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,8);
const int shortDuration = 200;
const int longDuration = 500;
const int buttonPin = 7;
const int backspacePin = 2;
const int buzzerPin = 9;
volatile int cursorX = 0;
volatile int cursorY = 0;


char* letters[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
};
char* numbers[] = {
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};
void setup() {
  pinMode(buttonPin,INPUT);
  pinMode(backspacePin,INPUT);
  pinMode(buzzerPin,OUTPUT);
  attachInterrupt(0,backSpace,RISING);
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
      tone(9,500,shortDuration);//Serial.println("short");//
      lcd.write('.');
      cursorX++;
      Serial.print(".");
  }
  else if(pressDuration < longDuration){
    tone(9,500,longDuration);//Serial.println("long");//
    lcd.write('-');
    cursorX++;
    Serial.print("-");
  }
  else{
    lcd.write(' ');
    cursorX++;
    Serial.print(" ");
  }

  if(Serial.available()>0 && Serial.read()=='#'){
    lcd.clear();
    lcd.home();
    cursorX=0;
    cursorY=0;
  }
  if(Serial.available()>0 && Serial.read() == '/'){
    cursorX=0;
    cursorY=1;
  }
  
}

void backSpace(){
  Serial.println("back");
  if(cursorX==0&&cursorY==1){
    lcd.write(' ');
    cursorX=15;
    cursorY=0;
    lcd.setCursor(cursorX,cursorY);
  }
  else if(cursorX>0){
    cursorX--;
    lcd.setCursor(cursorX,cursorY);
    lcd.write(' ');
    lcd.setCursor(cursorX,cursorY);
  }
}





