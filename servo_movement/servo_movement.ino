#include <math.h>
#include <Servo.h>
#include <Button.h>


//1. Set parameter for light condition
#define PINX 11
#define PINY 12
#define PINZ 13//Low as lights off, High as lights on
//digitalLy write (PINX, PINY) to define type of emotion
// (LOW, LOW) addNote
// (LOW, HIGH) takeOffTheNote
// (HIGH, LOW) overdue
// (HIGH, HIGH)..

//2. Set up IR sensor parameter
#define led 8
#define sensor 5
unsigned long sensorReading;
unsigned long timeNoteOn;
unsigned long timeCheck;




//3. Set up global condition
#define btnOnOffPin 6
Button btnOnOff (btnOnOffPin);

//#define btn2 7
//int buttonState = 0;

boolean firstTimeOn = true;
boolean onOff = false;
int condition = -1;//netural
boolean noteOn = false;
boolean noteIsOverTime = false;



//4. Set up servo parameter
boolean notTakeANoteOff = false;
int steps;//calculating the absolute value between start angle and end angle 



//1 addNote
//2 TakeANoteOff
//3 Overdue
unsigned long timeStart;
unsigned long timeEnd;
unsigned long timeDif;

Servo h;//head servo
Servo m;//middle servo
Servo b;//bottom servo

int tempArray[180];
int tempArray1[180];
int tempArray2[180];

void setup() {
  Serial.begin(9600);
//  h.attach(2);
//  m.attach(3);
//  b.attach(4);
//  resetPosition();

  pinMode(btnOnOffPin, INPUT);
  //Serial.println("first set up by turn to off position");
//  turnOff();

  //Set up pins for arduino communication
  pinMode(PINX, OUTPUT);
  pinMode(PINY, OUTPUT);
  pinMode(PINZ, OUTPUT);
  digitalWrite(PINZ, LOW);
  digitalWrite(PINX, LOW);
  digitalWrite(PINY, LOW);
  
  
  //Set up pin for sensor
  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);

  
}

void loop() {
    //Turn on the sensor
  digitalWrite(led, HIGH); //turn on IR light
  sensorReading = analogRead(sensor);//read IR 
  Serial.println("---------+++++-----------");
  Serial.print(F("sensor reading: "));
  Serial.println(sensorReading);
  Serial.println("---------+++++-----------");
  boolean testUserAttachANote = userAttachANote();
  Serial.println("userAttachANote() status:");
  Serial.println(testUserAttachANote);

  sensorReading = analogRead(sensor);//read IR sensor

//
//  //2. Special Case: Activate the System
//  Serial.println("The light is currently on: ");
////
  int btnOnAction = btnOnOff.checkButtonAction();
//  buttonState = digitalRead(btn2);
  Serial.print(F("buttonRead: "));
  Serial.println(btnOnAction);
//
//
   if(!onOff&&(btnOnAction == Button::HELD)) {////  if(buttonState==HIGH&&!onOff)
       turnOn();
    }
  else if((btnOnAction == Button::HELD)&&onOff){
    turnOff();
   }

  
  Serial.println(onOff);
  //1. Regular interaction during loop
  if(onOff){
    emotion();
  }
  else{//
    digitalWrite(PINZ, LOW);
  }
//  delay(500);

////
////    digitalWrite(PINZ, HIGH);
////    digitalWrite(PINX, HIGH);
////    digitalWrite(PINY, LOW);
   delay(500);
}//END OF LOOP


void emotion(){
  //2. Regular interaction during loop
//  if(onOff){//a button for turn on the system and turn off the system
    digitalWrite(PINZ, HIGH);//turn on the light
    Serial.println(F("---------START OF SWITCH CONDITION------"));
    //a method to check 3 buttons that will control corresponding condition 
    switch(checkCondition2()){//TODO: change to checkCondition2();   
      case 1:{
          servoStart(); 
          long addNoteStart = millis();  
          addNote();
          long addNoteEnd = millis();  
          Serial.print(F("addNoteTotalTime: "));
          Serial.println(addNoteEnd - addNoteStart);
          digitalWrite(PINX, HIGH);
          digitalWrite(PINY, HIGH);
          resetPosition();
      }
      break;
      case 2: {
          servoStart();
          long takeNoteOffStart = millis(); 
          takeANoteOff();
          long takeNoteOffEnd = millis();  
          Serial.print(F("takeNoteOffTotalTime: "));
          Serial.println(takeNoteOffEnd - takeNoteOffStart); 
          digitalWrite(PINX, LOW);
          digitalWrite(PINY, HIGH);   
          resetPosition();
      }
      break;
      case 3: {
          long noteTooLongStart = millis();   
      servoStart();
       digitalWrite(PINX, HIGH);
       digitalWrite(PINY, LOW); 
       noteTooLong();//ovedue()
       long noteTooLongEnd = millis();  
       Serial.print(F("noteTooLongTotalTime: "));
       Serial.println(noteTooLongEnd - noteTooLongStart);
       resetPosition();
      }
      break;
//      default:
//      resetPosition();
//      break;    
    }//END OF SWITCH CONDITION 
//     resetPosition();
    Serial.println(F("---------END OF SWITCH CONDITION------"));
   
    digitalWrite(PINX, LOW);
    digitalWrite(PINY, LOW);
    condition = -1;
}



 /* 
  * Reset the lamp position to initial 
  * "ON" position from current position for all servos
  *
  */
void resetPosition(){
  servoStart();
  Serial.println();
  Serial.println(F("resetPosition START"));
  Serial.println(F("-----------------------"));
  Serial.print(F("h prev angle: "));
  Serial.println(h.read());
  Serial.print(F("m prev angle: "));
  Serial.println(m.read());
  Serial.print(F("b prev angle: "));
  Serial.println(b.read());
  long resetPositionStartTime = millis(); 
     
//  if(!firstTimeOn){
  Serial.println(F("go through this if not first time turning on"));  
    printMoves(b, b.read(), 50, 20);//reset the bottom position
    printMoves(m, m.read(), 80, 20);//reset the middle position 
    printMoves(h, h.read(), 100, 20);//rest the head position
//  }
//  else{
// Serial.println(F("go through this if this is first time to turn on"));    
//    b.write(50);
//    m.write(80);
//    h.write(100);
//  }   
       
//    printMoves(b, b.read(), 100, 20);//reset the bottom position
//    printMoves(m, m.read(), 80, 20);//reset the middle position 
//    printMoves(h, h.read(), 135, 20);//rest the head position
 
  servoStop(); 
  long resetPositionEndTime = millis();  
  
  Serial.println();
  
  Serial.print(F("h after angle: "));
  Serial.println(h.read());
  Serial.print(F("m after angle: "));
  Serial.println(m.read());
  Serial.print(F("b after angle: "));
  Serial.println(b.read());
//
// 
//  Serial.print(F("resetPositionTotalTime: "));
//  Serial.println(resetPositionEndTime - resetPositionStartTime);
//  Serial.println(F("resetPosition END"));
//  Serial.println(F("-----------------------"));

}


int checkCondition(){
  Serial.println(F("Enter condition: "));
  while(Serial.available ()==0){
  }
  

  int input = Serial.parseInt();
  condition = input;
  Serial.print(F("Set condition: "));
  Serial.println(condition);
  //check add note, set condition = 1;

  //check remove note, set condition = 2;

  //check timer for overdue, set condition = 3; 
  
  return condition;
}


int checkCondition2(){
  //if user attach a note --> start recording time


  //1. a method for turning on light and return 0
  //2. a method for checking adding note and checking timer, and taking off note
  
  //1. If the light was off, awake the lamp to netural position
   if (userAttachANote()){
    Serial.println(F("-----USER ATTACHES A NOTE-------")); 
    timeNoteOn = millis();
    noteOn = true;
    //if the lights is on, lamp look at board and nods
    if(onOff){
    Serial.println(F("-----WHEN THE LAMP IS ON-------"));   
      return 1;//addANote() 
     }
  }else if(userTakeANoteOff()){//if the note was taken off within preset duration
    //if user take a note off
    Serial.println(F("-----AND IS TAKING OFF WITHIN TIME ------")); 
    noteOn = false;
    return 2;//takeANoteOff();  
  }
  
//lamp take actions when note stays on for too long
// continue doin it every 10s(60s for user test?), until note is taken off
  noteIsOverTime = overTime(); 
  Serial.print(F("Check boolean noteIsOvertime state: "));
  Serial.println(noteIsOverTime);
  if(noteOn&&noteIsOverTime&&onOff){
  Serial.println(F("-----IS TAKING MORE TIME ------")); 
//   noteIsOvertime = overTime(); 
 
    return 3;//NoteTooLong(); 
  }
}


void servoStop(){
  h.detach();
  m.detach();
  b.detach();
}
void servoStart(){
  h.attach(2);
  m.attach(3);
  b.attach(4);
}

void turnOn(){//GO TO NEUTRAL POSITION WHEN
  onOff = true;
  resetPosition(); 
  digitalWrite(PINZ, HIGH);
  firstTimeOn = false;
  Serial.println(F("~~~~turnOn method~~~~"));
   
}

void turnOff(){
  //return to OFF position from current angle
  Serial.println(F("~~~~turnOff Method~~~~"));
  servoStart();
//  h.write(175);
//  m.write(80);
//  b.write(100);
  printMoves(h, h.read(), 165, 10);
  printMoves(m, m.read(), 80, 10);
  printMoves(b, b.read(), 50, 10);
  servoStop();
  Serial.println(F("~~~~turnOff, servo stop~~~~")); 
  digitalWrite(PINZ, LOW);   
  onOff = false; //TODO: COMMENT THIS BACK
  firstTimeOn = true;
}

void nod(){ 
  printMoves(h, h.read(), 100, 10); 
  printMoves(h, h.read(), 165, 10); 
  printMoves(h, h.read(), 100, 10);
  printMoves(h, h.read(), 165, 10); 
  h.detach(); 
}




void happynod(){
  printMoves(h, h.read(), 15, 10); 
  printMoves(h, h.read(), 70, 10); 
  printMoves(h, h.read(), 15, 10);
  printMoves(h, h.read(), 70, 10); 
  h.detach(); 
}

void shakeHead(){
  printMoves(m, m.read(), 135, 7);
  printMoves(m, m.read(), 45, 7); 
  printMoves(m, m.read(), 135, 7); 
  printMoves(m, m.read(), 45, 7);
//  printMoves(m, 45, 90, 7);
}

void addNote(){

  digitalWrite(PINX, HIGH);
  digitalWrite(PINY, HIGH);
  servoStart();
  Serial.println(F("-----------------------------"));
  
  Serial.println(F("addNote(), servoStop() START"));
  printMoves(m, m.read(), 175, 15);
  nod();
  
  Serial.print(F("h current angle: "));
  Serial.println(h.read());
  Serial.print(F("m current angle: "));
  Serial.println(m.read());
  Serial.print(F("b current angle: "));
  Serial.println(b.read());

  servoStop();
  Serial.println(F("addNote(), servoStop() END"));
  Serial.println(F("-----------------------------"));

 
//  resetPosition();
  // printMoves(m, m.read(), 80, 15);
}

void takeANoteOff(){

  digitalWrite(PINX, LOW);
  digitalWrite(PINY, HIGH);
  

//  printMoves(b, b.read(), 40, 20);
////  b.write(20);
//  b.detach();
//
////  b.detach();
//  
//  happynod();
//  b.attach(4);
//  printMoves(b, b.read(), 110, 20);
////  b.write(110);
//  b.detach();
////  b.detach();
//
//  happynod();
//  m.detach();
//  servoStop();
//  delay(500);

////OLD DANCING 
//  takeANoteOffHelper(m.read(), 100, b.read(), 30, 30);//was 12
////  takeANoteOffHelper(80, 100, 50, 30, 30);//was 12
//  takeANoteOffHelper(m.read(), 40, b.read(), 70, 15);
//  takeANoteOffHelper(m.read(), 100, b.read(), 30, 15);
//  takeANoteOffHelper(m.read(), 40, b.read(), 70, 15);
//  takeANoteOffHelper(m.read(), 100, b.read(), 30, 15);
//  takeANoteOffHelper(100, 80, 30, 50, 15);//was 12


  int mInt = 80;
  int bInt = 50;
  
  for(int i = 0; i<7; i++){
    m.write(m.read()+i+1);//from 80-100
    b.write(b.read()-i-1);//from 50-30
    delay(50);
  }
//  delay(30);
    
  for(int i = 0; i<14; i++){
    m.write(m.read()-i-1);//from 100 - 40
    b.write(b.read()+i+1);//from 30 - 70
     delay(50);
  }
//   delay(30);
 for(int i = 0; i<14; i++){
    m.write(m.read()+i+1);//from 40 - 100
    b.write(b.read()-i-1);//from 70 - 30
     delay(50);
  }
//   delay(30);
    for(int i = 0; i<14; i++){
    m.write(m.read()-i-1);//from 100 - 40
    b.write(b.read()+i+1);//from 30 - 70
     delay(50);
  }
//   delay(30);
 for(int i = 0; i<14; i++){
    m.write(m.read()+i+1);//from 40 - 100
    b.write(b.read()-i-1);//from 70 - 30
     delay(50);
  }
//   delay(30);
    for(int i = 0; i<14; i++){
    m.write(m.read()-i-1);//from 100 - 40
    b.write(b.read()+i+1);//from 30 - 70
     delay(50);
  }
//   delay(30);
 for(int i = 0; i<14; i++){
    m.write(m.read()+i+1);//from 40 - 100
    b.write(b.read()-i-1);//from 70 - 30
     delay(50);
  }
//   delay(30);
   for(int i = 0; i<7; i++){
    m.write(m.read()-i-1);//from 100-80
    b.write(b.read()+i+1);//from 30-50
    delay(50);
  }
// delay(30);



  
  
//  resetPosition();
}

void takeANoteOffHelper(int m1, int m2, int b1, int b2, int rate){
  
//  notTakeANoteOff = false;
  Serial.println();
  Serial.println(F("vvvvvvvvvvv----CHECK TAKENOTEOFFHELPER----------vvvvvv")); 
  Serial.println(F("----store m first---")); 
  
  printMoves(m, m1, m2, 1);
  Serial.println();
  memcpy(tempArray1, tempArray, 180);
  Serial.println(F("tempArray1:"));
  Serial.println((int)tempArray1);
  
  Serial.println(F("----store h first---")); 
  printMoves(b, b1, b2, 1);
  memcpy(tempArray2, tempArray, 180);
  Serial.println(F("tempArray2:"));
  Serial.println((int)tempArray2);
  Serial.println();

  servoStart();
  
  for(int i = 0; i < steps; i++){
    
      m.write(tempArray1[i]);
      b.write(tempArray2[i]);
      delay(50);  
  }
//  servoStop();
  
}


void noteTooLong(){

  printMoves(m, m.read(), 170, 15); //turn to the board
  printMoves(h, h.read(),130, 20);//lower down head
  
  digitalWrite(PINX, HIGH);
  digitalWrite(PINY, LOW);
  
  delay(1000);
  printMoves(m, m.read(),80, 15);//turn back

  shakeHead(); 
  //resetPosition();
//  printMoves(m, m.read(), 90, 15); //turn back to neutral
  
}



void sweep(Servo s1) {
  for (int pos = 15; pos <= 170; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    s1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (int pos = 170; pos >= 15; pos -= 1) { // goes from 180 degrees to 0 degrees
    s1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  // test();
}


boolean overTime(){
  long currentTime = millis();
  timeCheck = currentTime-timeNoteOn;
  Serial.print("timeCheck: ");
  Serial.println(timeCheck);
  if(timeCheck > 17000){//10 sec
    Serial.println(F("overTime is true."));
    //timer reset
    timeNoteOn = millis();   
    return true;
  }  
  return false; 
}

boolean userAttachANote(){
  //sensor reading would be less than 1000 if the user attach a note
  boolean sensorOn = false;
  if(sensorReading < 1000){
    sensorOn = true;
  }
  return (sensorOn&&!noteOn);
}

boolean userTakeANoteOff(){
   //sensor reading would be more than 1000 if the user take a note off
   return ((sensorReading > 1000)&&noteOn&&onOff);
}

void printMoves(Servo whichServo, int startAngle, int endAngle, int rate) {
  servoStart();
  double pi = 3.141592;
  steps = abs(endAngle - startAngle);
//similar to frame rate, increase it only when u want to increase the speed significantly
//normally use rate for speed change  
  int increment = 1;
  
  for (int i = 0; i < steps; i+=increment) {
    int offset = round(steps / pi * ((pi * i) / steps - cos((pi * i) / steps) * sin((pi * i) / steps)));
    double result;
    if (endAngle - startAngle < 0) {
      result = startAngle - offset;
    } else {
      result = startAngle + offset;
    }

    
    tempArray[i] = result;//COMMENT OUT

    
    if((condition!=2)){ //(condition==2&&notTakeANoteOff)//COMMENT OUT
      whichServo.write(result);
//      whichServo.detach(); //COMMENT OUT
      delay(rate);
      
    }//COMMENT OUT
  }
   servoStop();
}

