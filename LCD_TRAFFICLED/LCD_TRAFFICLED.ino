#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int irPin=2;            
const int ldrPin=A0;          
const int redLed=3;
const int yellowLed=4;
const int greenLed=5;
const int whiteOut=6;

const bool IR_ACTIVE_LOW = true;
const bool LDR_DARK_LOW  = false;
int nightThreshold = 500;

bool lastVehicle = false;

void setup(){
  pinMode(irPin, INPUT_PULLUP);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(whiteOut, OUTPUT);

  digitalWrite(greenLed, HIGH);
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed, LOW);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Initializing (:");

  lcd.setCursor(0,1);
  lcd.print("       By-Kounak");

  delay(3000);   
  lcd.clear();                  // clear screen

lcd.setCursor(0,0);           // move cursor
}

void loop(){
  int irRaw = digitalRead(irPin);
  bool vehicle = IR_ACTIVE_LOW ? (irRaw==LOW) : (irRaw==HIGH);

  int ldr = analogRead(ldrPin);
  bool dark = LDR_DARK_LOW ? (ldr < nightThreshold) : (ldr > nightThreshold);

  // Vehicle arrived
  if (vehicle && !lastVehicle){
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(redLed, LOW);

    // ✅ show YELLOW
    lcd.setCursor(0,0);
    lcd.print("Det:YES ");
    lcd.print(dark ? "NIGHT" : "DAY  ");
    lcd.setCursor(0,1);
    lcd.print("Signal: YELLOW ");

    delay(1000);

    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, HIGH);
  }

  // Vehicle left
  if (!vehicle && lastVehicle){
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, HIGH);

    // ✅ show YELLOW
    lcd.setCursor(0,0);
    lcd.print("Det:NO  ");
    lcd.print(dark ? "NIGHT" : "DAY  ");
    lcd.setCursor(0,1);
    lcd.print("Signal: YELLOW  ");

    delay(1000);

    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, HIGH);
  }

  // steady states
  if (vehicle){
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(whiteOut, dark ? HIGH : LOW);
  } else {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(whiteOut, LOW);
  }

  // -------- LCD DISPLAY (NO BLINK) --------

  lcd.setCursor(0,0);
  lcd.print("Det:");
  lcd.print(vehicle ? "YES " : "NO  ");
  lcd.print(dark ? "NIGHT" : "DAY  ");

  lcd.setCursor(0,1);
  if (vehicle){
    lcd.print("Signal: RED-STOP");
  } else {
    lcd.print("Signal: GREEN  ");
  }

  lastVehicle = vehicle;

  delay(200);
}