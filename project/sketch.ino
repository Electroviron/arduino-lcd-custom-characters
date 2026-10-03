#include <LiquidCrystal.h>

// lcd pins
#define RS 8
#define EN 9

#define D4 4
#define D5 5
#define D6 6
#define D7 7

// create the lcd object
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define custom character
byte heart[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);

  // create custom character
  lcd.createChar(0, heart);

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.print("elctronics");

}

void loop() {
  // put your main code here, to run repeatedly:

}
