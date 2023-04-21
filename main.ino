
#define PRINT_DELAY 1                            //Ticker print delay in seconds
#define TZ_INFO "EST5EDT,M3.2.0,M11.1.0"         //EST/EDT Florida UTC -5/-4 
//#define TZ_INFO "NZST-12NZDT,M9.5.0,M4.1.0/3"  //NZST/NZDT  UTC +12/+13       
#include "private.h"                             //SSID and Password
#include <WiFi.h>
#include <sys/time.h>                           //C++ time library
#include <Ticker.h>                             //One Second tick for the time stamp prints
Ticker time_print;
time_t now;                             
const int64_t DEFAULT_TIME = 1357041600;        //1 January 2013
short count = 10;

int setUnixtime(volatile int64_t unixtime) {   //Sets C++ system time
    timeval epoch = {unixtime, 0};
    return settimeofday((const timeval*)&epoch, 0);
}

void timePrint(void) {                            //Creates our Clock
  struct tm now;
  getLocalTime(&now,0);
      if(count < 10){                             //For 9 seconds this format stamp
          //Outputs Zone ST/DT and Local Time
          Serial.println(&now, "(%Z) Local Time %I:%M:%S (%p)"); 
      }else{                                      
          //Outputs Day Date & Current UTC offset //The on 10 count we do a full Stamp
          Serial.println(&now, "\n%a %d-%B-%Y %T UTC: %z\n");
          count = 0;
      }   
  count++;
}

void setup(void) {
  Serial.begin(115200);
  while(!Serial);
  WiFi.begin(ssid, password);
  Serial.println("WiFi Connected.....\nSetting TZ\nGetting current NTP epoch stamp"); 
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", TZ_INFO, 1); tzset();                      //Sets Timezone  
  volatile int64_t timeInt64 = 0;
  while(time(&now) < DEFAULT_TIME){                       //Wait for NTP Time sync
      timeInt64 = time(&now);                             //Create UNIX epoch stamp
          if(timeInt64 > 0){
              setUnixtime(timeInt64);                     //Sets the system Clock
          }
  }
  Serial.println("Clock Set\n");
  time_print.attach(PRINT_DELAY, timePrint);              //Starts time stamp prints
}
void loop(void) {}                                        //Nothing to do here

//
//
//DONATE SRAV studios
//BTC - bc1q5kmqqynratseyh7v0n8q58rn7p5xejuemmc4pxDONATE SRAV studios
//USDT(ETH) - 0x8558288490E11E7F900471E7D52F0b0A0B6b8572
//USDT(SOLANA) - 4MjmiAwiQT1cqb5fSpvdsKCabZAKxopcMsTqem9gWBqB
//USDT(POLYGON) - 0x8558288490E11E7F900471E7D52F0b0A0B6b8572
//ETH - 0x8558288490E11E7F900471E7D52F0b0A0B6b8572
