#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// PCF8574 I2C address is set to 0x20, A pins are not connected to anything
LiquidCrystal_I2C lcd(0x20, 16, 2);

// MQ sensors connected to ESP32 ADC1 pins
const int MQ7_PIN = 34; // CO
const int MQ5_PIN = 35; // LPG
const int MQ4_PIN = 32; // CH4

// Counter to track the upward scrolling animation
int scrollSequence = 0;

void setup() {


  // Initialize LCD and turn on backlight
  lcd.init();
  lcd.backlight();

  // Set analog reading pins as input
  pinMode(MQ7_PIN, INPUT);
  pinMode(MQ5_PIN, INPUT);
  pinMode(MQ4_PIN, INPUT);

  // Startup screen 
  lcd.setCursor(0, 0);
  lcd.print("System");
  lcd.setCursor(0, 1);
  lcd.print("Starting");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read raw analog values from sensors
  int mq7_value = analogRead(MQ7_PIN);
  int mq5_value = analogRead(MQ5_PIN);
  int mq4_value = analogRead(MQ4_PIN);

  // Format the data as "Formula: Value" to fit only the left 8 characters
  String lineCO  = "CO :" + String(mq7_value);
  String lineLPG = "LPG:" + String(mq5_value);
  String lineCH4 = "CH4:" + String(mq4_value);

  // Clear the screen
  lcd.clear();

  // Upward Scrolling Logic:
  // The bottom data moves to the top row in the next loop, new data comes to the bottom.
  if (scrollSequence == 0) {
    lcd.setCursor(0, 0); lcd.print(lineCH4); // CH4 on top
    lcd.setCursor(0, 1); lcd.print(lineCO);  // CO at the bottom 
    scrollSequence = 1;
  } 
  else if (scrollSequence == 1) {
    lcd.setCursor(0, 0); lcd.print(lineCO);  // CO scrolled up
    lcd.setCursor(0, 1); lcd.print(lineLPG); // LPG at the bottom 
    scrollSequence = 2;
  } 
  else if (scrollSequence == 2) {
    lcd.setCursor(0, 0); lcd.print(lineLPG); // LPG scrolled up
    lcd.setCursor(0, 1); lcd.print(lineCH4); // CH4 at the bottom 
    scrollSequence = 0;
  }

  // Delay time determining the animation speed (scrolls up every 1.5 seconds)
  delay(1500);
}