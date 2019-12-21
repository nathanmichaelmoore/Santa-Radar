//https://www.reddit.com/r/FastLED/comments/dnj8o2/crgb_html_color_index_as_variable/
//https://github.com/FastLED/FastLED/wiki/Pixel-reference
#include <FastLED.h>
#include "RTClib.h"

//what time to do you wnat radar activity to start and peak
//#define DEMO

//what time to do you wnat radar activity to start and peak
const int startMonth = 12; //what is the month of the day you want the radar active
const int startDay = 24; //day radar will be active
const int peakHour = 12+8; //hour that radar will be most active

DateTime peakTime(2019,startMonth,startDay,peakHour,30,0);
DateTime startTime (2019,startMonth,startDay,peakHour-10,30,0);
DateTime endTime = (peakTime + TimeSpan(0,6,0,0));


// How many leds are in the strip?
#define NUM_LEDS 7

// Data pin that led data will be written out over
#define DATA_PIN A2

#define BRIGHTNESS 64

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int demoTime;

void setup() {
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(2000);
     
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);

    DateTime now = rtc.now();
    demoTime = now.minute()+11;
    
    bootUpSeq(CRGB::Aqua);

}

void bootUpSeq(CRGB color){
  for(int x = 0; x < NUM_LEDS; x++){
      // Turn our current led on to white, then show the leds
      leds[x] = color;
        FastLED.show();

      // Wait a little bit
      delay(250);

      if (x == (NUM_LEDS-1)){
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        delay(400);
        
        fill_solid(leds, NUM_LEDS, color); 
        FastLED.show();
        delay(400);
        
        fill_solid(leds, NUM_LEDS, CRGB::Black); 
        FastLED.show();
        delay(400);
      }
  }
}

//This is the boot up sequence of the radar.
void L(int level, CRGB color, int blinkDelay){
  
  for(int y = 0; y < random(5,10); y++){ 
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show(); 
    delay(blinkDelay);
    for(int x = 0; x < level; x++) {
    leds[x] = color;
        FastLED.show();
        }
    delay(blinkDelay);
  }
}

//funtion to display the date and time
void dateTime() {
    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}


void loop() {
  DateTime now = rtc.now();
  Serial.print(peakTime.hour());
  Serial.print("  ");
  Serial.print(startTime.hour());
  Serial.print("  ");

  #ifdef DEMO
    if(1){
  #else
    if (now.month() == peakTime.month() && (now.day() == peakTime.day()|| now.day() == (peakTime.day()+1))){
  #endif    
      #ifdef DEMO
        int nowPeakDif = abs(demoTime - now.minute());
      #else
        int nowPeakDif;
        if (now.month() == peakTime.month() && now.day() == peakTime.day())
          nowPeakDif = abs(peakTime.hour() - now.hour());
        else
          nowPeakDif = abs(peakTime.hour() - (24+now.hour()));
      #endif
      
      if (nowPeakDif >= 10)
        L(random(1,2+1),CRGB::Yellow,1000);
  
      if (nowPeakDif == 9)
        L(random(1,3+1),CRGB::Yellow,1000);
  
      if (nowPeakDif == 8)
        L(random(2,3+1),CRGB::Orange,1000);
  
      if (nowPeakDif == 7)
        L(random(2,4+1),CRGB::Orange,1000);
  
      if (nowPeakDif == 6)
        L(random(3,4+1),CRGB::Pink,1000);
        
      if (nowPeakDif == 5)
        L(random(3,5+1),CRGB::Pink,1000);
      
      if (nowPeakDif == 4)
        L(random(4,5+1),CRGB::Purple,1000);
        
      if (nowPeakDif == 3)
        L(random(4,6+1),CRGB::Purple,1000);
        
      if (nowPeakDif == 2)
        L(random(5,6+1),CRGB::Red,1000);
        
      if (nowPeakDif == 1)
        L(random(5,6+1),CRGB::Red,1000);
      
      if (nowPeakDif == 0 && now.minute() >=30)
        L(7,CRGB::Green,500);
    }
  else{
    L(1,CRGB::Pink, 1000);
  }
   
}
