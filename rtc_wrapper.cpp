
#include "rtcClass.h"
#include "ledClass.h"
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
   ds3231.readTemp();
	
   cout << "Flashing the LEDs for 5 seconds" << endl;
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

      redled.turnOff();               // turn GPIO on
      orangeled.turnOff();           // turn GPIO off
      yellowled.turnOff();           // turn GPIO off
      blueled.turnOn();             // turn GPIO off
      
      usleep(FLASH_DELAY);           // sleep for 50ms
   }

    cout << "Finished the RTC Wrapper program" << endl;
   return 0;
}
