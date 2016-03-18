#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define NUM_LEDS 12
#define BRIGHTNESS 15


#define PINX 11
#define PINY 12
#define PINZ 13 //HIGH as on, LOW as off
//(PINX, PINY) as emotion command
//(LOW, LOW) addNote
//(LOW, HIGH) takeANoteOff
//(HIGH, LOW) noteTooLong

int condition = 0;
//1 = addNote
//2 = takeANoteOff
//3 = noteTooLong



// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800); //for 12 pixels ring
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGBW + NEO_KHZ800); //for 8 pixels stick

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


boolean onoff = false; //false as currently off.

//define color white for different LED modes
uint32_t whiteGRB = strip.Color(127, 127, 127);
uint32_t whiteRGBW = strip.Color(0, 0, 0, 255);

void setup() {
   Serial.begin(9600);
   pinMode(PINX, INPUT);
   pinMode(PINY, INPUT);
   pinMode(PINZ, INPUT);
   strip.begin();
   strip.setBrightness(BRIGHTNESS);//Set up the brightness of
   strip.show(); // Initialize all pixels to 'off'  
}

void loop() {


  Serial.print(F("current PINZ: "));
  Serial.println(digitalRead(PINZ));
  
  Serial.print("condition: ");
  Serial.println(condition);
  Serial.print("onoff: ");
  Serial.println(onoff);


   if(digitalRead(PINZ)==HIGH){
     emotion();
     colorWipe(whiteGRB, 50);//Turn the light back on once finish an action
   }
   else{
    Serial.println(F("IT IS OFF"));
     strip.show(); //Turn off the light
     ColorSet(strip.Color(0, 0, 0)); //Double check 
   }


}//end of loop();



/*
* A method for handling different condition based on input numeric command 
* and modify the LED light accordingly. 
*
*/

void emotion(){
//if(digitalRead(PINZ)==HIGH){//digitalRead(PINZ)==HIGH
  //  checkCondition();
  switch(checkCondition()){
      case 1:{//addNote (blink x2)
        long addNoteStart = millis(); 
         colorWipe(strip.Color(0, 0, 0), 0);//reset LEDs
         colorWipe(whiteGRB, 100);
         blink(whiteGRB, 250);
         blink(whiteGRB, 250);
         blink(whiteGRB, 250);
         colorWipe(whiteGRB, 100);
        long addNoteEnd = millis();  
        Serial.print(F("addNoteTotalTime: "));
        Serial.println(addNoteEnd - addNoteStart);
        
//        condition =  2;
      }
      break;
      case 2: {//takeOffTheNote (colorWipe and blink)
      long takeANoteOffStart = millis(); 
//      colorWipe(strip.Color(0, 255, 127), 0);//reset LEDs
      theaterChase(strip.Color(0,255,127), 50);
      theaterChase(strip.Color(0,255,127), 50);
      blink(strip.Color(0,255,127), 50);
      blink(strip.Color(0,255,127), 50);
      long takeANoteOffEnd = millis();  
      Serial.print(F("takeANoteOffTotalTime: "));
      Serial.println(takeANoteOffEnd - takeANoteOffStart); 
//      condition = 3;
      }
      break;
      case 3:{//noteTooLong fade/show slowly x2 with red color

      Serial.println(F("1. State: overdue;"));
      Serial.println(F("=========================================="));
      long totalTimeStart = millis();
      Serial.print(F("totalTimeStart: "));
      Serial.println(totalTimeStart);
      
      //action 0 . fade to red -- take about (1000)
      Serial.println("-------------------------");
      Serial.println(F("Motion 0: Look = fade from white to red"));
      long timeStart = millis();
      Serial.print(F("Motion 0 timeStart: "));
      Serial.println(timeStart);
      
      fade(2);
      
      long timeEnd = millis();
      Serial.print(F("Motion 0 timeEnd: "));
      Serial.println(timeEnd);
      Serial.print(F("Motion 0 timeDif: "));
      Serial.println(timeEnd - timeStart);
      Serial.println(F("-------------------------"));
      
      //2. action 1: (855)
      Serial.println(F("-------------------------"));
      Serial.println(F("Motion 1: raise head to netural position = hold red light"));
      timeStart = millis();
      Serial.print(F("Motion 1 timeStart: "));
      Serial.println(timeStart);
       
      delay(500);

      timeEnd = millis();
      Serial.print(F("Motion 1 timeEnd: "));
      Serial.println(timeEnd);
      Serial.print(F("Motion 1 timeDif: "));
      Serial.println(timeEnd - timeStart);
      Serial.println(F("-------------------------"));

        //3. action 2: 1574
      Serial.println(F("-------------------------"));
      Serial.println(F("Motion 2: neck turn back to neutral = blink twice"));
      timeStart = millis();
      Serial.print(F("Motion 2 timeStart: "));
      Serial.println(timeStart);
      
      ColorSet(strip.Color(0, 0, 0));
      delay(500); 
      ColorSet(strip.Color(255, 0, 0));   
      delay(500);
      ColorSet(strip.Color(0, 0, 0));
      delay(500); 
      ColorSet(strip.Color(255, 0, 0));   
      delay(500);
       
      timeEnd = millis();
      Serial.print(F("Motion 2 timeEnd: "));
      Serial.println(timeEnd);
      Serial.print(F("Motion 2 timeDif: "));
      Serial.println(timeEnd - timeStart);
      Serial.println(F("-------------------------"));
       
      //4. action 3: 3266
      Serial.println(F("-------------------------"));
      Serial.println(F("Motion 3: rase head to neutral position = theatre chase + fade back to white"));
      timeStart = millis();
      Serial.print(F("Motion 3 timeStart: "));
      Serial.println(timeStart);

      
      fadeFromRedtoWhite(2);    
      Serial.print(F("Motion 3 end: "));
      Serial.println(timeEnd);
      Serial.print(F("Motion 3 time difference: "));
      Serial.println(timeEnd - timeStart);
      Serial.println(F("-------------------------"));
       
    //5. total 9059
    long totalTimeEnd = millis();
    Serial.print(F("totalTimeEnd: "));
    Serial.println(totalTimeEnd);
    Serial.print(F("totalTimeUsed: "));
    Serial.println(totalTimeEnd - totalTimeStart);
    Serial.println(F("=========================================="));    
         
//    condition = 0;
    }
    break;   
  }
  condition = 0;//reset condition so that each action will only do it once
}


/*
* A method for checking the condition from the servo arduino and pass the command to 
* the emotion handler
*
*/
int checkCondition(){

	Serial.print("PINX:");
	Serial.println(digitalRead(PINX));

	Serial.print("PINY:");
	Serial.println(digitalRead(PINY));

	if((digitalRead(PINX)==HIGH)&&(digitalRead(PINY)==HIGH)){
    	condition = 1; //addNote()
  	}
  	else if((digitalRead(PINX)==LOW)&&(digitalRead(PINY)==HIGH)){
    	condition = 2; //takeANoteOff()
  	}
  	else if((digitalRead(PINX)==HIGH)&&(digitalRead(PINY)==LOW)){
    	condition = 3; //noteTooLong()
  	}
  	else{
//    condition = 0;//just turning on
}
	return condition;
}


/*
* This method fade all LEDs from white to red, only works on RBG neopixel.
*/
void fade(uint8_t wait){
    //WHITE (127, 127, 127);
    //RED (255, 0, 0);
    int r = 127;
    int g = 127;
    int b = 127;
//  long startTime = millis();
    
  for (int j=0; j < 127; j++) {   
    for (int i=0; i <= strip.numPixels() ; i++){           // i is the pixel number. This forloop selects all the pixels between 0 - 50
      strip.setPixelColor(i,r,g,b);        // the last 3 values are RGB values, red blue and green respectivly. Currently set to off, adjust these to change the color.
      strip.show();                        //Sends the comand to the pixels to Light up in the specified code above.
    }
    r++;
    g--;
    b--;
    delay(wait);
  }
}

/*
* This method fade all LEDs from red to white, only works on RBG neopixel.
*/
void fadeFromRedtoWhite(uint8_t wait){
    //WHITE (127, 127, 127);
    //RED (255, 0, 0);
    int r = 254;
    int g = 0;
    int b = 0;

    
  for (int j=0; j < 127; j++) {   
    for (int i=0; i <= strip.numPixels() ; i++){           // i is the pixel number. This forloop selects all the pixels between 0 - 50
      strip.setPixelColor(i,r,g,b);        // the last 3 values are RGB values, red blue and green respectivly. Currently set to off, adjust these to change the color.
      strip.show();                        //Sends the comand to the pixels to Light up in the specified code above.
    }
    r--;
    g++;
    b++;
    delay(wait);
  }
}


//https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms/
// Set all pixels to a color (synchronously)
 void ColorSet(uint32_t color){
   for (int i = 0; i < strip.numPixels(); i++)
   {
     strip.setPixelColor(i, color);
   }
   strip.show();
 }
 // Returns the Red component of a 32-bit color
 uint8_t Red(uint32_t color)
 {
   return (color >> 16) & 0xFF;
 }

 // Returns the Green component of a 32-bit color
 uint8_t Green(uint32_t color)
 {
   return (color >> 8) & 0xFF;
 }

 // Returns the Blue component of a 32-bit color
 uint8_t Blue(uint32_t color)
 {
   return color & 0xFF;
 }

/*  Basic Neo-Pixel Code to Blink Neo-Pixles on and off white.
 *   By Richard Clarkson
 *   Be aware that Neo-Pixel Strips require alot of power and may require alterd RGB values or external Power sources.
 *   Code works with both Ardino & Flora.
 */
 void blink(uint32_t c, uint8_t wait){  
  //blink off
  for (int i=0; i <= 50; i++){           // i is the pixel number. This forloop selects all the pixels between 0 - 50
    strip.setPixelColor(i,0,0,0);        // the last 3 values are RGB values, red blue and green respectivly. Currently set to off, adjust these to change the color.
    strip.show();                        //Sends the comand to the pixels to Light up in the specified code above.
  }
  delay (wait); 
   //blink on
  for (int i=0; i <= 50; i++){           // i is the pixel number. This forloop selects all the pixels between 0 - 50
    strip.setPixelColor(i,c);  // the last 3 values are RGB values, red blue and green respectivly. Currently set to white, adjust these to change the color.
    strip.show();                        //Sends the comand to the pixels to Light up in the specified code above.
  }
  delay (wait);     

}


//-------------THE FOLLOWING METHODS ARE FROM ADAFRUIT NEOPIXEL LIBRARY STANDTEST EXAMPLE ---------//

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void reverseColorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=strip.numPixels(); i>=0; i--){
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


void theaterChaseTwoColor(uint32_t c1, uint32_t c2, uint8_t wait) {
  for (int j=0; j<5; j++) {  //do 5 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c1);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c2);        //turn third pixel to another color
      }
    }
  }
}



//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}





// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {

  WheelPos = 255 - WheelPos;
  Serial.print("WheelPos: ");
  Serial.println(WheelPos);
  if(WheelPos < 85) {    
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

