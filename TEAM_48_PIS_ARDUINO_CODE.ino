#include <LiquidCrystal.h>



#include <Servo.h>

#include "HX711.h"
#include <LiquidCrystal.h>


const int LOADCELL_DOUT_PIN=40;
const int LOADCELL_SCK_PIN=41;
HX711 scale;
unsigned long time=0;

const int LOADCELL_DOUT_PIN_2=42;
const int LOADCELL_SCK_PIN_2=43;
HX711 scale_2;

float average=0 ;
float temp =0;

LiquidCrystal lcd(12,11,5,4,3,2);


int pos=0;
int pos2=0;
float calibration_factor = 106.72;
float calibration_factor2 = -411.08;
// this calibration factor is adjusted according to my load cell
float units;
float units2;
float ounces;
float inr_money_iron;
float inr_money_paper;


Servo myservo_oreo;
Servo myservo_bit;  // create servo object to control a servo

int Button1 = 0;
int Button2 = 0;
int Button3 = 0;
const int Button_1=30;
const int Button_2=31;
const int Button_3=32;



void setup() {
  Serial.begin(9600);
  myservo_oreo.attach(50);
  myservo_bit.attach(51);

   
  
  lcd.begin(16,2);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale_2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale();
  scale.tare();  //Reset the scale to 0
 
  scale_2.set_scale();
  scale_2.tare(); 
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

  pinMode(Button1,INPUT);
  pinMode(Button2,INPUT);
  pinMode(Button3,INPUT);
   
}

void loop() {
   myservo_bit.write(63);
   myservo_oreo.write(90);
  Button1=digitalRead(Button_1);
  Button2=digitalRead(Button_2);
  Button3=digitalRead(Button_3);
  
  if(Serial.available()) 
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }
scale.set_scale(calibration_factor);
scale_2.set_scale(calibration_factor2); //Adjust to this calibration factor

  Serial.print("Reading: ");
  units = scale.get_units(),10;
  units2 = scale_2.get_units(),10;
  
  
  
  if (units < 0)
  {
    units = 0.00;
  }

  if (units2 < 0)
  {
    units2 = 0.00;
  }

  
  ounces = units * 0.035274;
  inr_money_iron = units /50;
  inr_money_paper = units2/100;
//  Serial.print(units);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(calibration_factor);
//  Serial.println();

  
 

  lcd.clear();
    
  lcd.setCursor(0,0);
  lcd.print("W_PR=");
  
  lcd.setCursor(5,0);
  lcd.print(units2,2);

  lcd.setCursor(0,1);
  lcd.print("W_IN=");
  
  lcd.setCursor(5,1);
  lcd.print(units,2);

 
  
  if (Button1 == HIGH){
    
    //for (int i =0 ; i<8 ;i++){

    average =inr_money_iron + inr_money_paper+temp;
    //}
    //average = ((average)/8)+ temp;
     
 
 
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Net_INR=");
  lcd.setCursor(8,0);
  lcd.print(average);
  scale.tare();
      scale_2.tare();
  
  
    if (Button2 == HIGH and average >=10){
       myservo_bit.write(118);
      delay(2000);
      myservo_bit.write(60);
      average = average -10;
      scale.tare();
      scale_2.tare();
      lcd.clear();
      }
      
    if (Button3 == HIGH and average >=20)
    {
            
 myservo_oreo.write(130);                 
      delay(2500);
myservo_oreo.write(90);  
      average = average -20;
      lcd.clear();
      
    }
    if ((Button2==HIGH and average <=10) or (Button3==HIGH and average <=20))
     {
      lcd.clear();
     lcd.setCursor(0,0);
      lcd.print("INSUFFICIENT FUND");
      delay(1000);
      lcd.clear();

  }
   temp=average; 
  }

 
  }
    
  


  
  


  
