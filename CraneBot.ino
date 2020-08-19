//www.elegoo.com
//2016.12.12
#include "IRremote.h"
#include "Stepper.h"
#include <Servo.h>
#include "SR04.h"
int rf=2;
int grapmotor=3;
int dataPin = 4;
int motor2speed=5;
int musicpin=6;
int clockPin = 7;
int led1=8; 
int motor1speed=9;
int ECHO_PIN=10;
int TRIG_PIN=11;
int led2=12;
int led3=13;
int stepper[4]={14,16,15,17};
int lightpin=18;
int latchPin = 19;      
int pressedButton=0,lastButton=0,modeButton=0;
int pos = 100;
byte mainbyte =  B00000000;
int movecount=0,turntime=0,turncheck=0;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
IRrecv irrecv(rf);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
Servo myservo; 
long distance;
const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
const int rolePerMinute = 300;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
Stepper myStepper(stepsPerRevolution, stepper[0], stepper[1], stepper[2], stepper[3]);
bool pressSafe[21];

void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{

  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER");       pressedButton=10;modeButton=0;pressSafe[pressedButton]=0;break;
  case 0xFFE21D: Serial.println("FUNC/STOP");   pressedButton=12;pressSafe[pressedButton]=0;break;
  case 0xFF629D: Serial.println("VOL+");        pressedButton=11;pressSafe[pressedButton]=0; break;
  case 0xFF22DD: Serial.println("FAST BACK");   pressedButton=13;pressSafe[pressedButton]=0;    break;
  case 0xFF02FD: Serial.println("PAUSE");       pressedButton=14;pressSafe[pressedButton]=0;   break;
  case 0xFFC23D: Serial.println("FAST FORWARD");pressedButton=15;pressSafe[pressedButton]=0;  break;
  case 0xFFE01F: Serial.println("DOWN");        pressedButton=16;pressSafe[pressedButton]=0;    break;
  case 0xFFA857: Serial.println("VOL-");        pressedButton=17;pressSafe[pressedButton]=0;   break;
  case 0xFF906F: Serial.println("UP");          pressedButton=18;pressSafe[pressedButton]=0;   break;
  case 0xFF9867: Serial.println("EQ");          pressedButton=19;pressSafe[pressedButton]=0;   break;
  case 0xFFB04F: Serial.println("ST/REPT");     pressedButton=20;pressSafe[pressedButton]=0;    break;
  case 0xFF6897: Serial.println("0");           modeButton=0;pressSafe[modeButton]=0;    break;
  case 0xFF30CF: Serial.println("1");           modeButton=1;pressSafe[modeButton]=0;    break;
  case 0xFF18E7: Serial.println("2");           modeButton=2;pressSafe[modeButton]=0;    break;
  case 0xFF7A85: Serial.println("3");           modeButton=3;pressSafe[modeButton]=0;    break;
  case 0xFF10EF: Serial.println("4");           modeButton=4;pressSafe[modeButton]=0;    break;
  case 0xFF38C7: Serial.println("5");           modeButton=5;pressSafe[modeButton]=0;    break;
  case 0xFF5AA5: Serial.println("6");           modeButton=6;pressSafe[modeButton]=0;         break;
  case 0xFF42BD: Serial.println("7");           modeButton=7;pressSafe[modeButton]=0;          break;
  case 0xFF4AB5: Serial.println("8");           modeButton=8;pressSafe[modeButton]=0;          break;
  case 0xFF52AD: Serial.println("9");           modeButton=9;pressSafe[modeButton]=0;         break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");   pressedButton=lastButton;pressSafe[pressedButton]=0;break;  

  default: 
    Serial.println(" other button : ");
    Serial.println(results.value);
    pressedButton=0;
  }// End Case

  delay(500); // Do not get immediate repeat


} //END translateIR
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, mainbyte);
   digitalWrite(latchPin, HIGH);
    
}
void setup()
{ 

  pinMode(motor1speed,OUTPUT);
  pinMode(motor2speed,OUTPUT);
    pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

  Serial.begin(9600);
Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
  myStepper.setSpeed(rolePerMinute);
   myservo.attach(grapmotor);
    updateShiftRegister();  
}
  
void movechange(){
  updateShiftRegister();  
digitalWrite( motor1speed, HIGH);
digitalWrite( motor2speed, HIGH);

 
}
void move(int dir){

switch(dir){
case 0:bitWrite(mainbyte, 2, 0);bitWrite(mainbyte, 1, 0);bitWrite(mainbyte, 3, 0);bitWrite(mainbyte, 4, 0);movechange();break;
case 1:bitWrite(mainbyte, 2, 1);bitWrite(mainbyte, 1, 0);bitWrite(mainbyte, 3, 0);bitWrite(mainbyte, 4, 1);movechange();break;  
case 2:bitWrite(mainbyte, 2, 1);bitWrite(mainbyte, 1, 0);bitWrite(mainbyte, 3, 1);bitWrite(mainbyte, 4, 0);movechange();break;  
case 3:bitWrite(mainbyte, 2, 0);bitWrite(mainbyte, 1, 1);bitWrite(mainbyte, 3, 1);bitWrite(mainbyte, 4, 0);movechange();break;  
case 4:bitWrite(mainbyte, 2, 0);bitWrite(mainbyte, 1, 1);bitWrite(mainbyte, 3, 0);bitWrite(mainbyte, 4, 1);movechange();break;  
  }
delay(100);
digitalWrite( motor1speed, LOW);
digitalWrite( motor2speed, LOW);
}

void cranepull(bool dir)
{
if (dir==true)myStepper.step(stepsPerRevolution);
else myStepper.step(-stepsPerRevolution); 
}

void craneclose(bool dir)
{if(dir){pos++;if(pos>100)pos=100;}
else {pos--;if(pos<18)pos=18;}
myservo.write(pos);
delay(100);
}

void distanceLed()
{

if (distance>40){bitWrite(mainbyte, 5, 1);bitWrite(mainbyte, 6, 0);bitWrite(mainbyte, 7, 0);}
else if (distance<40&&distance>20){bitWrite(mainbyte, 5, 0);bitWrite(mainbyte, 6, 1);bitWrite(mainbyte, 7, 0);}  
else {bitWrite(mainbyte, 5, 0);bitWrite(mainbyte, 6, 0);bitWrite(mainbyte, 7, 1);}
updateShiftRegister();  
}

 //bug if start near wall
void AutoDrive(){
if(distance>10 && turntime==0 )move(1);
else {
if (turntime==0)turntime=80;
if (turntime>40) move(3);
if (turntime<40 && turntime>0)move(2);
turntime--;
}
}

void turnOffLed()
{
digitalWrite( led1, LOW);
digitalWrite( led2, LOW);
digitalWrite( led3, LOW);
bitWrite(mainbyte, 5, 0);
bitWrite(mainbyte, 6, 0);
bitWrite(mainbyte, 7, 0);
updateShiftRegister();  
}
  bool ledlight1=0;
  bool ledlight2=0;
  bool ledlight3=0;
  bool ledlight4=0;
  bool ledlight5=0;
  bool ledlight6=0;
  
void ledControll(int led)
{ switch(led){
case 1:if(ledlight1==0)ledlight1=1;else ledlight1=0;digitalWrite( led1,ledlight1);break;
case 2:if(ledlight2==0)ledlight2=1;else ledlight2=0;digitalWrite( led2,ledlight2);break;
case 3:if(ledlight3==0)ledlight3=1;else ledlight3=0;digitalWrite( led3,ledlight3);break;
case 4:if(ledlight4==0)ledlight4=1;else ledlight4=0;bitWrite(mainbyte, 5, ledlight4);break;
case 5:if(ledlight5==0)ledlight5=1;else ledlight5=0;bitWrite(mainbyte, 6, ledlight5);break;
case 6:if(ledlight6==0)ledlight6=1;else ledlight6=0;bitWrite(mainbyte, 7, ledlight6);break;    
}
updateShiftRegister(); 
}

void pressRelease(int button)
{

for(int i=10;i<21;i++)
{if(i==button)continue;
  pressSafe[i]=0; 
}
}

void lightDetektor(){
 //max 600 
Serial.println(analogRead(lightpin));
lightShow(600,analogRead(lightpin)) ;
}

void lightShow(int maximum,int value)
{
int part=maximum/7;
int trueValue=value/part;
 switch(trueValue)
 {
case 0:digitalWrite( led1,LOW);digitalWrite( led2,LOW);digitalWrite( led3,HIGH);break;
case 1:digitalWrite( led1,LOW);digitalWrite( led2,HIGH);digitalWrite( led3,HIGH);break;
case 2:digitalWrite( led1,LOW);digitalWrite( led2,HIGH);digitalWrite( led3,LOW);break;
case 3:digitalWrite( led1,HIGH);digitalWrite( led2,LOW);digitalWrite( led3,LOW);break;
case 4:digitalWrite( led1,HIGH);digitalWrite( led2,HIGH);digitalWrite( led3,LOW);break;
case 5:digitalWrite( led1,HIGH);digitalWrite( led2,LOW);digitalWrite( led3,HIGH);break;
default: 
   digitalWrite( led1,HIGH);digitalWrite( led2,HIGH);digitalWrite( led3,HIGH);
 }
}

void loop()
{lastButton=pressedButton;
distance=sr04.Distance();
 if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); // receive the next value
    
  }

  switch(pressedButton)
  {
 case 10:break;
 case 11:move(1);break;//forward
 case 12:craneclose(true);break;
 case 13:move(4);break;//left
 case 14:move(0);break;//stop
 case 15:move(2);break;//right
 case 16:cranepull(false);break;  
 case 17:move(3);break;//back
 case 18:cranepull(true);break;
 case 19:AutoDrive();break;
 case 20:craneclose(false);break;     
  }
  switch(modeButton)
   {
 case 0:if(pressSafe[modeButton]==0){pressRelease(modeButton);pressSafe[modeButton]=1;turnOffLed();}break;   
 case 1:if(pressSafe[modeButton]==0){pressRelease(modeButton);pressSafe[modeButton]=1;distanceLed();}break;
 case 2:if(pressSafe[modeButton]==0){pressRelease(modeButton);pressSafe[modeButton]=1;ledControll(1);}break;
 case 3:if(pressSafe[modeButton]==0){pressRelease(modeButton);pressSafe[modeButton]=1;ledControll(2);}break;
 case 4:if(pressSafe[modeButton]==0){pressRelease(modeButton);pressSafe[modeButton]=1;ledControll(3);}break;
 case 5:if(pressSafe[modeButton]==0){pressRelease(modeButton);pressSafe[modeButton]=1;ledControll(4);}break;
 case 6:if(pressSafe[modeButton]==0){pressRelease(modeButton);pressSafe[modeButton]=1;ledControll(5);}break;
 case 7:if(pressSafe[modeButton]==0){pressRelease(modeButton);pressSafe[modeButton]=1;ledControll(6);}break;
 case 8:lightDetektor();break;
 case 9:break;
   }  
}/* --end main loop -- */
