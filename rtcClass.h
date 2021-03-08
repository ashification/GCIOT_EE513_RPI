#ifndef RTC_H_
#define RTC_H_

#include<iostream>
#include<sstream>
#include<fcntl.h>
#include<stdio.h>
#include<iomanip>
#include<unistd.h>         // for the microsecond sleep function 
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include <time.h>

using namespace std;

#define BUFFER_SIZE 19      //0x00 to 0x12
// the time is in the registers in encoded decimal form
int bcdToDec(char b) { return (b/16)*10 + (b%16); }

//Converts Decimal value to BCD taken from http://danceswithferrets.org/geekblog/?p=504
char decTobcd(int d) { return ((d/10) << 4 | (d%10));}

class RTC{
   private:                // the following is part of the implementation
	  int file;
	  float temperature;
	  char writeBuffer[BUFFER_SIZE];
	  time_t T;
	  struct tm tm;
	  
   public:                 // part of the public interface
      RTC(); // the constructor -- create the object
      
      char buf[BUFFER_SIZE];

      virtual void readTime();
      virtual void readDate();
      virtual void readSystime();
      float readTemp();
      virtual void writeSystime();
      void writeToBuffer(char buf_reg, int write_value);
      virtual ~RTC();      // the destructor -- called automatically
};

RTC::RTC(){  // constructor implementation
   writeBuffer[1] = {0x00};
   
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      perror("failed to open the bus\n");
   }
   
   if(ioctl(file, I2C_SLAVE, 0x68) < 0){
      perror("Failed to connect to the sensor\n");
   }
   
   if(write(file, writeBuffer, 1)!=1){
      perror("Failed to reset the read address\n");
   }
      
   if(read(file, buf, BUFFER_SIZE)!=BUFFER_SIZE){
      perror("Failed to read in the buffer\n");
   }
}

// This implementation function is "hidden" outside of the class
void RTC::readSystime(){
   //The following is modified from https://www.includehelp.com/c-programs/system-date-time-linux.aspx
   T= time(NULL);
   tm = *localtime(&T);
   // Structure calls refernced from library https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
   printf("\nCurrent System Date and Time Values\n");   
   printf("System Time is: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);   
   printf("System Date is: %02d/%02d/%02d\n",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
}


void RTC::readTime(){
   cout << "The Current RTC time is  " << bcdToDec(buf[2]) << ":" << bcdToDec(buf[1])<< ":" << bcdToDec(buf[0])<< endl;
}

void RTC::readDate(){
	cout << "The Current RTC Date is  " << bcdToDec(buf[4]) << "/" << bcdToDec(buf[5])<< "/" << bcdToDec(buf[6])<< endl;
}

float RTC::readTemp(){
   temperature = buf[0x11] + ((buf[0x12]>>6)*0.25);
   cout << "\nThe Current RTC temperature reading is = "<< temperature << " Celsius\n\n " << endl;
   return temperature;
}

void RTC::writeToBuffer(char buf_reg, int write_value){
        char value = decTobcd(write_value);
	buf[buf_reg] = value;
}

void RTC::writeSystime(){
   T= time(NULL);
   tm = *localtime(&T); 
   cout << "\nWrite System Time to RTC\n " << endl;
   
   buf[0x02] = decTobcd(tm.tm_hour);
   buf[0x01] = decTobcd(tm.tm_min);
   buf[0x00] = decTobcd(tm.tm_sec);
	
   buf[0x04] = decTobcd(tm.tm_mday);
   buf[0x05] = decTobcd(tm.tm_mon+1);
   buf[0x06] = decTobcd(tm.tm_year-100);
}



RTC::~RTC(){  // The destructor unexports the sysfs GPIO entries
   close(file);
   cout << "\n\nRTC Object Destroyed\n " << endl;

}

#endif /* RTC_H_ */ 
