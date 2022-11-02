//Include the essential libraries
#include <LiquidCrystal_I2C.h> //LCD Library
#include <Servo.h> //Servo Motor Library
#include <HCSR04.h> //Ultrasonic Sensor Library

//Define variables and create objects.
//Left Sensor
const int trig_L = 12;
const int echo_L = 11;

//Right Sensor
const int trig_R = 10;
const int echo_R = 9;

//Middle Sensor
const int trig_M = 8;
const int echo_M = 7;

//Servo 
const int ServoPin = 6;
Servo servo;

//Indicator LEDs (Left)
const int red_L = 5;
const int green_L = 4;

//Indicator LEDs (Right)
const int red_R = 3;
const int green_R = 2;

//Initialize the U.S. sensors

UltraSonicDistanceSensor Sensor_L(trig_L, echo_L);
UltraSonicDistanceSensor Sensor_R(trig_R, echo_R);
UltraSonicDistanceSensor Sensor_M(trig_M, echo_M);


//Variables for storing the distance from all sensors
int Dist_L;
int Dist_R;
int Dist_M;


//Initialize the dispay
LiquidCrystal_I2C lcd(0x27,16,2);

//Define custom LCD characters

//Middle Pointer
byte Mid[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B11111, B00100};
//Left Pointer
byte Left[8] = {B00000, B00000, B10000, B01000, B00100, B11111, B11111, B00100};
//Right Pointer
byte Right[8] = {B00000, B00000, B00001, B00010, B00100, B11111, B11111, B00100};

void setup()
{ 
  LiquidCrystal_I2C lcd(0x27,16,2);
  lcd.backlight();  
  lcd.begin();
  servo.attach(ServoPin);

  //Define all indicator LEDs as outputs

  //Left
  pinMode(red_L, OUTPUT);
  pinMode(green_L, OUTPUT);

  //Right
  pinMode(red_R, OUTPUT);
  pinMode(green_R,OUTPUT); 

  //Clear the LCD Display

  lcd.clear();
  lcd.print("Calibrating");
  delay(1000);
  lcd.clear();


  
}

void loop()
{
  LiquidCrystal_I2C lcd(0x27,16,2);
  //Create the custom characters
  lcd.createChar(0,Left);
  lcd.createChar(1,Right);
  lcd.createChar(2,Mid);

  //Starting the system
  ProcessMid();
  ProcessLeft();
  ProcessRight();
    
}
void ProcessMid()
{ 
  Dist_M = Sensor_M.measureDistanceCm();
  //Printing the values on the lcd.
    //90Deg Angle
  servo.write(90);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(char(2));
  lcd.setCursor(0,3);
  lcd.print("Distance cm");
  lcd.setCursor(1,0);
  lcd.print(Dist_M);
  delay(3000);

    //45Deg Angle
  servo.write(45);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(char(1));
  lcd.setCursor(0,3);
  lcd.print("Distance cm");
  lcd.setCursor(1,0);
  lcd.print(Dist_M);
  delay(3000);

    //135Deg Angle
  servo.write(135);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(char(3));
  lcd.setCursor(0,3);
  lcd.print("Distance cm");
  lcd.setCursor(1,0);
  lcd.print(Dist_M);
  delay(3000);
}

void ProcessLeft()
{
  Dist_L = Sensor_L.measureDistanceCm();
  if(Dist_L >= 30)
  {
    digitalWrite(red_L, LOW);
    digitalWrite(green_L, LOW);    
  }
  if(Dist_L <= 29 && Dist_L > 10)
  {
    digitalWrite(red_L, LOW);
    digitalWrite(green_L, HIGH);
  }
  if(Dist_L <= 10)
  {
    digitalWrite(red_L, HIGH);
    digitalWrite(green_L, LOW);
  }
}

void ProcessRight()
{
  Dist_R = Sensor_R.measureDistanceCm();
  if(Dist_R >= 30)
  {
    digitalWrite(red_R, LOW);
    digitalWrite(green_R, LOW);    
  }
  if(Dist_R <= 29 && Dist_R > 10)
  {
    digitalWrite(red_R, LOW);
    digitalWrite(green_R, HIGH);
  }
  if(Dist_R <= 10)
  {
    digitalWrite(red_R, HIGH);
    digitalWrite(green_R, LOW);
  }
}
