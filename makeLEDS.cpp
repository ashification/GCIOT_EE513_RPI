#include "ledClass.h"
#define GPIO         "/sys/class/gpio/"
#define FLASH_DELAY  50000 // 50 milliseconds


int main(int argc, char* argv[]){  // the main function start point
   cout << "Starting the makeLEDs program" << endl;
   LED led1(6), led2(13);          // create two LED objects
   cout << "Flashing the LEDs for 5 seconds" << endl;
   for(int i=0; i<50; i++){        // LEDs will alternate
      led1.turnOn();               // turn GPIO4 on
      led2.turnOff();              // turn GPIO17 off
      usleep(FLASH_DELAY);         // sleep for 50ms
      led1.turnOff();              // turn GPIO4 off
      led2.turnOn();               // turn GPIO17 on
      usleep(FLASH_DELAY);         // sleep for 50ms
   }
   led1.displayState();            // display final GPIO4 state
   led2.displayState();            // display final GPIO17 state
   cout << "Finished the makeLEDs program" << endl;
   return 0;
}
