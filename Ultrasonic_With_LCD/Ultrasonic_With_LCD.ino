/*Created By Advait for Tech Zoo 2022 Interschool Competition*/
#include <HCSR04.h>
#include <LiquidCrystal_I2C.h>

const int echo = 13;
const int trig = 12;

UltraSonicDistanceSensor distanceSensor(trig, echo);

int distanceCM;
int distanceINCH;



void setup()
{
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  lcd.begin();
  lcd.backlight();
  lcd.print("Please Wait...");
  delay(5000);
  lcd.clear();
}

void loop()
{
  distanceCM = distanceSensor.measureDistanceCm();
  distanceINCH = (distanceCM / 2.54);
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CM: - ");
  lcd.print(distanceCM);
  lcd.setCursor(0, 1);
  lcd.print("INCH: - ");
  lcd.print(distanceINCH);
}
