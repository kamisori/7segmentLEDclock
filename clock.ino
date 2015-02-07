#include <Wire.h> 
#include <Time.h>  
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#define BTN_NEXT_DEC 0
#define BTN_SET_LEFT 1
#define BTN_SET_RIGHT 2
#define BTN_PREVIOUS_INC 3

#define DIGIT_FIRST 0
#define DIGIT_SECON 1
#define DIGIT_THIRD 3
#define DIGIT_FOURT 4

#define ADJ_BY_SECOND 1
#define ADJ_BY_MINUTE 60
#define ADJ_BY_HOUR 3600
#define ADJ_BY_DAY 86400
#define ADJ_BY_THIRTY_DAYS 2592000
#define ADJ_BY_YEAR 31536000

#define SCREEN_HH_MM 1
#define SCREEN_MM_SS 2
#define SCREEN_DD_MM 3
#define SCREEN_YYYY 4
#define SCREEN_BLANK 5

Adafruit_7segment matrix = Adafruit_7segment();

boolean nextDec;
boolean setLeft;
boolean setRight;
boolean prevInc;

boolean nextDecOld;
boolean setLeftOld;
boolean setRightOld;
boolean prevIncOld;

boolean nextDecProcessed;
boolean setLeftProcessed;
boolean setRightProcessed;
boolean prevIncProcessed;

byte screen;
boolean setHours;
boolean setMinutes;
boolean setSeconds;
boolean setDay;
boolean setMonth;
boolean setYear;

void setup() {
  matrix.begin(0x70);

  pinMode(BTN_NEXT_DEC, INPUT);
  digitalWrite(BTN_NEXT_DEC, HIGH);
  pinMode(BTN_SET_LEFT, INPUT);
  digitalWrite(BTN_SET_LEFT, HIGH);
  pinMode(BTN_SET_RIGHT, INPUT);
  digitalWrite(BTN_SET_RIGHT, HIGH);
  pinMode(BTN_PREVIOUS_INC, INPUT);
  digitalWrite(BTN_PREVIOUS_INC, HIGH);
  
  nextDec = false;
  setLeft = false;
  setRight = false;
  prevInc = false;

  nextDecOld = false;
  setLeftOld = false;
  setRightOld = false;
  prevIncOld = false;

  nextDecProcessed = false;
  setLeftProcessed = false;
  setRightProcessed = false;
  prevIncProcessed = false;
  
  setHours = false;
  setMinutes = false;
  setSeconds = false;
  setDay = false;
  setMonth = false;
  setYear = false;

  screen = SCREEN_HH_MM;
  
  matrix.print(0xDEAD, HEX);
  matrix.writeDisplay();
  delay(500);

  matrix.print(0xBEEF, HEX);
  matrix.writeDisplay();
  delay(500);
  
  clearScreen();

  delay(500);
  setTime(1262347200);
}

void loop(){
  handleButtons();
  printTimeOn7SegDisplay();
}

void clearScreen() {
  matrix.writeDigitRaw(DIGIT_FIRST, 0);
  matrix.writeDigitRaw(DIGIT_SECON, 0);
  matrix.drawColon(false);
  matrix.writeDigitRaw(DIGIT_THIRD, 0);
  matrix.writeDigitRaw(DIGIT_FOURT, 0);
  matrix.writeDisplay();
}

void handleButtons() {
  nextDec = !digitalRead(BTN_NEXT_DEC);
  if (nextDec != nextDecOld)
    nextDecProcessed = false;
  if (nextDec && !nextDecProcessed){
    handleDecrement();
  }
  
  setLeft = !digitalRead(BTN_SET_LEFT);
  if (setLeft != setLeftOld)
    setLeftProcessed = false;
  if (setLeft && !setLeftProcessed){
    adjust(BTN_SET_LEFT);
    setLeftProcessed = true;
    setLeftOld = true;
  }
   
  setRight = !digitalRead(BTN_SET_RIGHT);
  if (setRight != setRightOld)
    setRightProcessed = false;
  if (setRight && !setRightProcessed){
    adjust(BTN_SET_RIGHT);
    setRightProcessed = true;
    setRightOld = true;
  }
 
  prevInc = !digitalRead(BTN_PREVIOUS_INC);
  if (prevInc != prevIncOld)
    prevIncProcessed = false;
  if (prevInc && !prevIncProcessed){
    handleIncrement();
  }
}

long getAdjustmentValue() {
  long adjustmentValue = 0;
  if(setHours){
    return ADJ_BY_HOUR;
  }
  if(setMinutes){
    return ADJ_BY_MINUTE;
  }
  if(setSeconds){
    return ADJ_BY_SECOND;
  }
  if(setDay){
    return ADJ_BY_DAY;
  }
  if(setMonth){
    return ADJ_BY_THIRTY_DAYS;
  }
  if(setYear){
    return ADJ_BY_YEAR;
  }
}

void handleDecrement() {
  if( !setHours && !setMinutes && !setSeconds && !setDay && !setMonth && !setYear ) {
    screen--;
    if (screen < SCREEN_HH_MM)
      screen = SCREEN_BLANK;
  }
  else
  {
    long adjustmentValue = getAdjustmentValue();
    adjustTime(-adjustmentValue);
  }
  nextDecProcessed = true;
  nextDecOld = true;
}

void handleIncrement() {
  if( !setHours && !setMinutes && !setSeconds && !setDay && !setMonth && !setYear ) {
    screen++;
    if (screen > SCREEN_BLANK)
      screen = SCREEN_HH_MM;
  }
  else
  {
    long adjustmentValue = getAdjustmentValue();
    adjustTime(adjustmentValue);
  }
  prevIncProcessed = true;
  prevIncOld = true;
}

void adjust(int button){
  if( setHours || setMinutes || setSeconds || setDay || setMonth || setYear ) {
    setHours = false;
    setMinutes = false;
    setSeconds = false;
    setDay = false;
    setMonth = false;
    setYear = false;
  }
  else
  {
    if(screen == SCREEN_HH_MM) {
      if(button == BTN_SET_LEFT)
        setHours = true;
      else
        setMinutes = true;
    }
    if(screen == SCREEN_MM_SS) {
      if(button == BTN_SET_LEFT)
        setMinutes = true;
      else
        setSeconds = true;
    }
    if(screen == SCREEN_DD_MM) {
      if(button == BTN_SET_LEFT)
        setDay = true;
      else
        setMonth = true;
    }
    if(screen == SCREEN_YYYY) {
      setYear = true;
    }
  }
}

void printTimeOn7SegDisplay() {
  matrix.writeDigitRaw(DIGIT_FIRST, 0);
  matrix.writeDigitRaw(DIGIT_SECON, 0);
  matrix.writeDigitRaw(DIGIT_THIRD, 0);
  matrix.writeDigitRaw(DIGIT_FOURT, 0);
  if(screen == SCREEN_HH_MM) {
    int h = hour();
    int m = minute();
    boolean showColon = second()%2 == 0;
    if(h < 10)
      matrix.writeDigitNum(DIGIT_FIRST, 0, setHours);
    else
      matrix.writeDigitNum(DIGIT_FIRST, (h / 10) % 10, setHours);
    
    matrix.writeDigitNum(DIGIT_SECON, h % 10, setHours);
    matrix.drawColon(showColon);
    
    if(m < 10)
      matrix.writeDigitNum(DIGIT_THIRD, 0, setMinutes);
    else
      matrix.writeDigitNum(DIGIT_THIRD, (m / 10) % 10, setMinutes);
    
    matrix.writeDigitNum(DIGIT_FOURT, m % 10, setMinutes);
  }
  if(screen == SCREEN_MM_SS) {
    int m = minute();
    int s = second();
    if(m < 10)
      matrix.writeDigitNum(DIGIT_FIRST, 0, setMinutes);
    else
      matrix.writeDigitNum(DIGIT_FIRST, (m / 10) % 10, setMinutes);
    
    matrix.writeDigitNum(DIGIT_SECON, m % 10, setMinutes);
    matrix.drawColon(true);
    
    if(s < 10)
      matrix.writeDigitNum(DIGIT_THIRD, 0, setSeconds);
    else
      matrix.writeDigitNum(DIGIT_THIRD, (s / 10) % 10, setSeconds);
    
    matrix.writeDigitNum(DIGIT_FOURT, s % 10, setSeconds);
  }
  if(screen == SCREEN_DD_MM) {
    int d = day();
    int m = month();

    matrix.writeDigitNum(DIGIT_FIRST, (d / 10) % 10, setDay);    
    matrix.writeDigitNum(DIGIT_SECON, d % 10, true);
    matrix.drawColon(false);
    
    matrix.writeDigitNum(DIGIT_THIRD, (m / 10) % 10, setMonth);
    matrix.writeDigitNum(DIGIT_FOURT, m % 10, true);
  }
  if(screen == SCREEN_YYYY) {
    int y = year();
    matrix.writeDigitNum(DIGIT_FIRST, (y / 1000), setYear);
    matrix.writeDigitNum(DIGIT_SECON, (y / 100) % 10, setYear);
    matrix.drawColon(false);
    matrix.writeDigitNum(DIGIT_THIRD, (y / 10) % 10, setYear);
    matrix.writeDigitNum(DIGIT_FOURT, y % 10, setYear);
  }
  if(screen == SCREEN_BLANK)
    matrix.drawColon(false);
  matrix.writeDisplay();
}
