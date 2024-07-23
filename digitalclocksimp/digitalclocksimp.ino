#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "myfonts.h"


#include <DS3231.h>
#include <Wire.h>
DS3231 Clock;

bool Century = false;
bool h12;
bool PM;
byte dd, mm, yyy;
uint16_t h, m, s;

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_ZONES 2
#define ZONE_SIZE 4
#define MAX_DEVICES (MAX_ZONES * ZONE_SIZE)
#define SCROLL_SPEED 30
#define ZONE_UPPER 1
#define ZONE_LOWER 0
#define CLK_PIN 13
#define DATA_PIN 11
#define CS_PIN 10
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


char szTime[9];  // mm:ss\0
char szMesg[18] = "";

char *mon2str(uint8_t mon, char *psz, uint8_t len) {
  static const __FlashStringHelper *str[] = { F("Jan"), F("Feb"), F("Mar"), F("Apr"), F("May"), F("Jun"), F("Jul"), F("Aug"), F("Sep"), F("Oct"), F("Nov"), F("Dec") };
  strncpy_P(psz, (const char PROGMEM *)str[mon - 1], len);
  psz[len] = '\0';
  return (psz);
}

char *dow2str(uint8_t code, char *psz, uint8_t len) {
  static const __FlashStringHelper *str[] = { F("Sun"), F("Mon"), F("Tue"), F("Wed"), F("Thur"), F("Fri"), F("Sat"), F("Sun") };
  strncpy_P(psz, (const char PROGMEM *)str[code - 1], len);
  psz[len] = '\0';
  return (psz);
}
// Time Setup: Code for reading clock time
void getTime(char *psz, bool f = true) {
  s = Clock.getSecond();
  m = Clock.getMinute();
  if (Clock.getHour(h12, PM) >= 13 || Clock.getHour(h12, PM) == 0) {
    h = Clock.getHour(h12, PM) - 12;
  } else {
    h = Clock.getHour(h12, PM);
  }
  sprintf(psz, "%02d%c%02d", h, (f ? ':' : ' '), m);
}

void getDate(char *psz) {
  char szBuf[10];
  dd = Clock.getDate();
  mm = Clock.getMonth(Century);  //12
  yyy = Clock.getYear();
  sprintf(psz, "%d %s %04d", dd, mon2str(mm, szBuf, sizeof(szBuf) - 1), (yyy + 2000));
}



void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  P.begin(MAX_ZONES);
  P.setZone(ZONE_LOWER, 0, ZONE_SIZE - 1);
  P.setFont(ZONE_LOWER, BigFontLower);
  P.setZone(ZONE_UPPER, ZONE_SIZE, MAX_DEVICES - 1);
  P.setFont(ZONE_UPPER, BigFontUpper);
  P.setCharSpacing(P.getCharSpacing() * 0);  // double height --> double spacing
}

int acquisitionMode = 0;

void loop(void) {
  static uint32_t lastTime = 0;  // millis() memory
  static uint8_t display = 0;    // current display mode
  static bool flasher = false;   // seconds passing flasher
  
  
  
  static uint8_t cycle = 0;

  // Run the animation and then check if BOTH zones have
  // completed. The animations are not the same length due
  // to upper/lower effects being displayed differently.
  P.displayAnimate();

  if (P.getZoneStatus(ZONE_LOWER) && P.getZoneStatus(ZONE_UPPER)) {

    
      getTime(szTime, flasher);
        flasher = !flasher;
      P.displayClear();
      dow2str(Clock.getDoW(), szMesg, 15);

      P.setFont(ZONE_LOWER, numeric7Seg);
      P.setFont(ZONE_UPPER, nullptr);
      P.setCharSpacing(2);
      P.displayZoneText(ZONE_LOWER, szTime, PA_CENTER, SCROLL_SPEED, 1000, PA_PRINT, PA_NO_EFFECT);
      P.displayZoneText(ZONE_UPPER, szMesg, PA_CENTER, SCROLL_SPEED, 1000, PA_PRINT, PA_NO_EFFECT);
      
    
    P.displayClear();
    P.synchZoneStart();
  }
}
