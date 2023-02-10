

/* This Code is written by CodeBuzz (Advait Muley)
   http://codebuzz.ml is my blog/website

   Author: - Advait Muley 
   Language: - C++ (Arduino)

   Thank You!
*/

//Including Essential Libraries
  #include <LiquidCrystal_I2C.h>
  #include <Wire.h>
  #include <HCSR04.h>
  #include <Servo.h>
  #include <SoftwareSerial.h>  

// Define Ultrasonic Sensor Pin
  #define TrigPin  8
  #define EchoPin  7

//Define custom LCD characters

  //Middle Pointer
    byte Mid[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B11111, B00100};
  //Left Pointer
    byte Left[8] = {B00000, B00000, B10000, B01000, B00100, B11111, B11111, B00100};
  //Right Pointer
    byte Right[8] = {B00000, B00000, B00001, B00010, B00100, B11111, B11111, B00100}; 

  //Prediction Characters
    //Middle Prediction Character
      byte predictMid[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
    //Left Prediction Character      
      byte predictLeft[8] = {B00000, B00000, B00000, B11100, B11000, B10100, B00010, B00001};
    //Right Prediction Character
      byte predictRight[8] = {B00000, B00000, B00000, B00111, B00011, B00101, B01000, B10000};

    
LiquidCrystal_I2C lcd(0x27,16,2); //LCD Display Initialization


// Variable to store distance
  //When the servo is at 90° position (Front)
    int Front_Distance = 0;
  //When the servo is at 135° position (Left)
    int Left_Distance = 0;
  //When the servo is at 45° position (Right) 
    int Right_Distance = 0;

//Initialize the ultrasonic sensor
  UltraSonicDistanceSensor distanceSensor(TrigPin, EchoPin);  

//Creating an object for the servo motor named servo
  Servo servo;

//Creating a variable for storing the servo position
  int pos = 0;

void setup() 
{
  lcd.begin(); //Beginning the LCD display
  lcd.backlight(); //Turning on the LCD display backlight
  servo.attach(A0); //Attaching the servo motor object to the pin Analog 0 (A0)
  int pos = 90; //Changing the value of the variable pos to 90°
  servo.write(pos); //Writing the value of the variable pos (90°) to the servo motor
  delay(1000);  //Adding a delay of 1 second (1000 milliseconds) for smooth transition
}

void loop() 
{
  //Creating custom characters with the bytes we made before on lines 23, 25 and 27 respectively
  lcd.createChar(0,Mid); //Middle symbol
  lcd.createChar(1,Left); //Left symbol
  lcd.createChar(2,Right); //Right symbol

  //Creating custom characters for the prediction symbols
  lcd.createChar(3,predictMid); //Middle Prediction Character
  lcd.createChar(4,predictLeft); //Left Prediction Character
  lcd.createChar(5,predictRight); //Right Prediction Character

  //Creating a forever loop to execute the custom function MidProcess() that is created on line 74
  while(true)
  {     
    MidProcess(); 
  }     
}


void MidProcess()
{
  /* This set of instructions controls all the modules when the servo is pointing on right*/
  
  
  delay(1000);
  servo.write(45);   // Right Position 
  Right_Distance = distanceSensor.measureDistanceCm();
  lcd.clear();  //Clear the LCD screen just in case anything is alredy present
  lcd.setCursor(0,0); //Set the cursor on column 0 and row 0 to write the next character on the LCD display
  lcd.print(char(2)); //Print the custom symbol of Right Position
  lcd.setCursor(3,0); //Leave one character space 
  lcd.print("Distance"); //Print the string "Distance"
  lcd.setCursor(0,1); //Shifting to the next line
  lcd.print(Right_Distance);  //Print the distance of the object on the right in CM
  lcd.setCursor(14,1);  //Move to the end of the second line
  lcd.print("CM");  //Print the string "CM"
  delay(2000); //A two second delay to make the LCD readable
  

  /* This set of instructions controls all the modules when the servo is pointing on front*/     
  delay(1000);
  servo.write(90);  // Front Positon   
  Front_Distance = distanceSensor.measureDistanceCm();
  lcd.clear();  //Clear the LCD screen just in case anything is alredy present
  lcd.setCursor(0,0); //Set the cursor on column 0 and row 0 to write the next character on the LCD display
  lcd.print(char(0)); //Print the custom symbol of Front Position
  lcd.setCursor(3,0); //Leave one character space 
  lcd.print("Distance");  //Print the string "Distance"
  lcd.setCursor(0,1); //Shifting to the next line
  lcd.print(Front_Distance);  //Print the distance of the object on the front in CM
  lcd.setCursor(14,1);  //Move to the end of the second line
  lcd.print("CM");  //Print the string "CM"
  delay(2000);  //A two second delay to make the LCD readable


  delay(1000);          
  servo.write(135); // Left position of servo  
  Left_Distance = distanceSensor.measureDistanceCm();  
  lcd.clear();  //Clear the LCD screen just in case anything is alredy present
  lcd.setCursor(0,0); //Set the cursor on column 0 and row 0 to write the next character on the LCD display
  lcd.print(char(1)); //Print the custom symbol of Left Position
  lcd.setCursor(3,0); //Leave one character space
  lcd.print("Distance");  //Print the string "Distance"
  lcd.setCursor(0,1); //Shifting to the next line
  lcd.print(Left_Distance); //Print the distance of the object on the front in CM
  lcd.setCursor(14,1);  //Move to the end of the second line
  lcd.print("CM");  //Print the string "CM"
  delay(2000);  //A two second delay to make the LCD readable       

  //Prediction Code 
  int safeDirection;
  
  // 1 = Mid, 2 = Left, 3 = Right

  if(Front_Distance > Left_Distance && Front_Distance > Right_Distance)
  {
    safeDirection = 1;
  }
  else if(Left_Distance > Front_Distance && Left_Distance > Right_Distance)
  {
    safeDirection = 2;
  }
  else if(Right_Distance > Front_Distance && Right_Distance > Left_Distance)
  {
    safeDirection = 3;
  }

  //Printing Predicted Safe Direction
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Safest Direction");
  lcd.setCursor(10,1);

  if(safeDirection == 1)
  {
    lcd.print(char(3));
    lcd.setCursor(0,1);
    lcd.print("Middle");    
  }
  else if(safeDirection == 2)
  {
    lcd.print(char(4));    
    lcd.setCursor(0,1);
    lcd.print("Left");
  }
  else if(safeDirection == 3)
  {
    lcd.print(char(5));
    lcd.setCursor(0,1);
    lcd.print("Right");
  }
  delay(5000);
}

