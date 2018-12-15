#include <Adafruit_NeoPixel.h>
#define PIN        14                              // 제어 신호핀
#define LEDNUM     30                             // 제어하고 싶은 LED 개수
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNUM, PIN, NEO_GRB + NEO_KHZ800); //라이브러리 내 변수 선언

/**The MIT License (MIT)

Copyright (c) 2018 by ThingPulse Ltd., https://thingpulse.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <JsonListener.h>
#include <time.h>
#include "OpenWeatherMapCurrent.h"


// initiate the client
OpenWeatherMapCurrent client;

// See https://docs.thingpulse.com/how-tos/openweathermap-key/
String OPEN_WEATHER_MAP_APP_ID = "8e6b6b26de10f95b858f6579e817ca25";  //키발급은 상단 howto에서 api키 발급받으세요
/*
Go to https://openweathermap.org/find?q= and search for a location. Go through the
result set and select the entry closest to the actual location you want to display 
data for. It'll be a URL like https://openweathermap.org/city/2657896. The number
at the end is what you assign to the constant below.
 */

String OPEN_WEATHER_MAP_LOCATION_ID = "1846326"; //날씨정보를 받아올 지역선택
 
//String OPEN_WEATHER_MAP_LOCATION_ID = "1846326"; //창원
//String OPEN_WEATHER_MAP_LOCATION_ID = "2657896"; //스위스
//2643743 런던


/*
Arabic - ar, Bulgarian - bg, Catalan - ca, Czech - cz, German - de, Greek - el,
English - en, Persian (Farsi) - fa, Finnish - fi, French - fr, Galician - gl,
Croatian - hr, Hungarian - hu, Italian - it, Japanese - ja, Korean - kr,
Latvian - la, Lithuanian - lt, Macedonian - mk, Dutch - nl, Polish - pl,
Portuguese - pt, Romanian - ro, Russian - ru, Swedish - se, Slovak - sk,
Slovenian - sl, Spanish - es, Turkish - tr, Ukrainian - ua, Vietnamese - vi,
Chinese Simplified - zh_cn, Chinese Traditional - zh_tw.
*/
String OPEN_WEATHER_MAP_LANGUAGE = "kr"; //언어선택 초기값 한국어
boolean IS_METRIC = true;

/**
 * WiFi Settings
 */
const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();
const char* WIFI_SSID     = "AndroidHotspot3025";      //와이파이 ssid 입력하세요
const char* WIFI_PASSWORD = "sunnydesis";    //와이파이 password 

// initiate the WifiClient
WiFiClient wifiClient;

int weather;
int temp = 0;



/**
 * Helping funtions
 */
void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());
  Serial.println();
}


/**
 * SETUP
 */
void setup() {
  Serial.begin(115200);
  delay(500);
  strip.begin(); // This initializes the NeoPixel library.
  strip.show();
  delay(100);
  
   
  connectWifi();

  Serial.println();
  Serial.println("\n\nNext Loop-Step: " + String(millis()) + ":");

  OpenWeatherMapCurrentData data;
  client.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  client.setMetric(IS_METRIC);
  client.updateCurrentById(&data, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);

  Serial.println("------------------------------------");

  // "lon": 8.54, float lon;
  Serial.printf("lon: %f\n", data.lon);
  // "lat": 47.37 float lat;
  Serial.printf("lat: %f\n", data.lat);
  // "id": 521, weatherId weatherId;
  Serial.printf("weatherId: %d\n", data.weatherId);
  // "main": "Rain", String main;
  Serial.printf("main: %s\n", data.main.c_str());
  // "description": "shower rain", String description;
  Serial.printf("description: %s\n", data.description.c_str());
  // "icon": "09d" String icon; String iconMeteoCon;
  Serial.printf("icon: %s\n", data.icon.c_str());
  Serial.printf("iconMeteoCon: %s\n", data.iconMeteoCon.c_str());
  // "temp": 290.56, float temp;
  Serial.printf("temp: %f\n", data.temp);
  // "pressure": 1013, uint16_t pressure;
  Serial.printf("pressure: %d\n", data.pressure);
  // "humidity": 87, uint8_t humidity;
  Serial.printf("humidity: %d\n", data.humidity);
  // "temp_min": 289.15, float tempMin;
  Serial.printf("tempMin: %f\n", data.tempMin);
  // "temp_max": 292.15 float tempMax;
  Serial.printf("tempMax: %f\n", data.tempMax);
  // "wind": {"speed": 1.5}, float windSpeed;
  Serial.printf("windSpeed: %f\n", data.windSpeed);
  // "wind": {"deg": 1.5}, float windDeg;
  Serial.printf("windDeg: %f\n", data.windDeg);
  // "clouds": {"all": 90}, uint8_t clouds;
  Serial.printf("clouds: %d\n", data.clouds);
  // "dt": 1527015000, uint64_t observationTime;
  time_t time = data.observationTime;
  Serial.printf("observationTime: %d, full date: %s", data.observationTime, ctime(&time));
  // "country": "CH", String country;
  Serial.printf("country: %s\n", data.country.c_str());
  // "sunrise": 1526960448, uint32_t sunrise;
  time = data.sunrise;
  Serial.printf("sunrise: %d, full date: %s", data.sunrise, ctime(&time));
  // "sunset": 1527015901 uint32_t sunset;
  time = data.sunset;
  Serial.printf("sunset: %d, full date: %s", data.sunset, ctime(&time));

  // "name": "Zurich", String cityName;
  Serial.printf("cityName: %s\n", data.cityName.c_str());
  Serial.println();
  Serial.println("---------------------------------------------------/\n");

}


/**
 * LOOP
 */
void loop() {
  

    OpenWeatherMapCurrentData data;
  client.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  client.setMetric(IS_METRIC);
  client.updateCurrentById(&data, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);

    // "main": "Rain", String main;
  Serial.printf("main: %s\n", data.main.c_str());
    // "name": "Zurich", String cityName;
  Serial.printf("cityName: %s\n", data.cityName.c_str());
  
   float rantime = random(20,50);

   //날씨에 따른 데이터 값 변경
   
   //drop
  if(data.weatherId>199 && data.weatherId<532){   
    type3();
  }
  else ;
  //smoky
  if(data.weatherId>700 && data.weatherId <782){
    type2();
  }
  else ;
  if(data.weatherId>801 && data.weatherId<805){
    type2();
  }
  else ;
  //sunny
  if(data.weatherId>799 && data.weatherId<802){   
     type1();
   }
   else ;
}



void type1(){    //날씨값으로 led 색상 및 패턴을 생성
  rainbow(15);
  }

void type2(){
   uint16_t i, j;
  for(j=0; j<256; j+=1) {
    for(i=0; i<strip.numPixels(); i++) {
      int sum = i+j;
      map(sum,0,300,120,190);
     strip.setPixelColor(i, Wheel(sum),Wheel(sum),Wheel(sum));

    }
    strip.show();
    delay(10);
  }
}

void type3(){
   uint16_t i, j,q;
 for(q=0; q<random(1,4);q++){
  for(j=0; j<256; j+=1) {
    for(i=0; i<strip.numPixels(); i++) {
      int sum = i+j;
      map(sum,0,300,120,190);
     strip.setPixelColor(i, Wheel(sum),Wheel(sum),0);

    }
    strip.show();
    delay(10);
  }
 }
    theaterChase(strip.Color(255, 255, 0), 60); //흰색 출력 
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

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) { 
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c); 
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0); 
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
