
#include "rtcClass.h"
#include "ledClass.h"
#include <ctime>
#define GPIO         "/sys/class/gpio/"
#define FLASH_DELAY  50000 // 50 milliseconds

using namespace std;

int main(int argc, char* argv[]){  // the main function start point
   cout << "Starting the RTC Wrapper" << endl;
   
   RTC ds3231;
   LED redled(6), orangeled(13), yellowled(19), blueled(26);          // create two LED objects
   
   ds3231.readTime();
   ds3231.readDate();
   ds3231.readSystime();
   ds3231.writeSystime();
   ds3231.readTime();
   ds3231.readDate();
   float checkTemp = ds3231.readTemp();
   
   if (checkTemp < 17){  // Too cold room temp
    for(int i=0; i<10; i++){
      redled.turnOff();               // turn GPIO off
      orangeled.turnOff();           // turn GPIO off
      yellowled.turnOff();           // turn GPIO off
      blueled.turnOn();             // turn GPIO  on
     }
   }

   if (checkTemp > 17 && checkTemp < 21 ){  // ideal room temp
    for(int i=0; i<10; i++){
      redled.turnOff();               // turn GPIO on
      orangeled.turnOff();           // turn GPIO off
      yellowled.turnOn();           // turn GPIO off
      blueled.turnOff();             // turn GPIO off
     }
   }

   if (checkTemp > 21 && checkTemp < 23 ){  // very warm room temp
    for(int i=0; i<10; i++){
      redled.turnOff();               // turn GPIO on
      orangeled.turnOn();           // turn GPIO off
      yellowled.turnOff();           // turn GPIO off
      blueled.turnOff();             // turn GPIO off
     }
   }

   if (checkTemp > 23 ){  // too hot room temp
    for(int i=0; i<10; i++){
      redled.turnOn();               // turn GPIO on
      orangeled.turnOff();           // turn GPIO off
      yellowled.turnOff();           // turn GPIO off
      blueled.turnOff();             // turn GPIO off
     }
   }


   //ds3231.writeToBuffer(0x07,20);
   
   //while (ds3231.buf[0x07] < 1){
   // redled.turnOn();
   //}   
   
  // clock_t starttime = ds3231.readTime();
  // while(double(ds3231.readTime()- starttime)/CLOCKS_PER_SEC < 10){
  //   redled.turnOn();
  //   blueled.turnOn();
  // }
   
   cout << "LED Fanfare" << endl;
   for(int i=0; i<10; i++){          // LEDs will alternate

      redled.turnOn();               // turn GPIO on
      orangeled.turnOff();           // turn GPIO off
      yellowled.turnOff();           // turn GPIO off
      blueled.turnOff();             // turn GPIO off
      
      usleep(FLASH_DELAY);           // sleep for 50ms

      redled.turnOff();               // turn GPIO on
      orangeled.turnOn();           // turn GPIO off
      yellowled.turnOff();           // turn GPIO off
      blueled.turnOff();             // turn GPIO off
      
      usleep(FLASH_DELAY);           // sleep for 50ms

      redled.turnOff();               // turn GPIO on
      orangeled.turnOff();           // turn GPIO off
      yellowled.turnOn();           // turn GPIO off
      blueled.turnOff();             // turn GPIO off
      
      usleep(FLASH_DELAY);           // sleep for 50ms

      redled.turnOff();               // turn GPIO off
      orangeled.turnOff();           // turn GPIO off
      yellowled.turnOff();           // turn GPIO off
      blueled.turnOn();             // turn GPIO  on
      
      usleep(FLASH_DELAY);           // sleep for 50ms
   }

    cout << "Finished the RTC Wrapper program" << endl;
   return 0;
}
