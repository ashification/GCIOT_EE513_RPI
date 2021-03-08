
#include"I2CDevice.h"
#include<iostream>
#include<sstream>
#include<fcntl.h>
#include<stdio.h>
#include<iomanip>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include <time.h>
 
using namespace std;

#define BUFFER_SIZE 19      //0x00 to 0x12

int file;

// the time is in the registers in encoded decimal form
int bcdToDec(char b) { return (b/16)*10 + (b%16); }

//Converts Decimal value to BCD taken from http://danceswithferrets.org/geekblog/?p=504
char decTobcd(int d) { return ((d/10) << 4 | (d%10));}

int main(){
    	
     
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      perror("failed to open the bus\n");
      return 1;
   }
   
   if(ioctl(file, I2C_SLAVE, 0x68) < 0){
      perror("Failed to connect to the sensor\n");
      return 1;
   }
   
   char writeBuffer[1] = {0x00};
   
   if(write(file, writeBuffer, 1)!=1){
      perror("Failed to reset the read address\n");
      return 1;
   }
   
   char buf[BUFFER_SIZE];
   
   if(read(file, buf, BUFFER_SIZE)!=BUFFER_SIZE){
      perror("Failed to read in the buffer\n");
      return 1;
   }
   
   printf("Read RTC Current Unset Values\n");   
   printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]), bcdToDec(buf[1]), bcdToDec(buf[0]));
   printf("The RTC Date is %02d-%02d-%02d\n", bcdToDec(buf[4]), bcdToDec(buf[5]), bcdToDec(buf[6]));
      
   //The following is modified from https://www.includehelp.com/c-programs/system-date-time-linux.aspx
   time_t T= time(NULL);
   struct  tm tm = *localtime(&T);
   
   printf("\nRead System Date and Time Values\n");   
   printf("System Time is: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);   
   printf("System Date is: %02d/%02d/%02d\n",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
   	
   buf[0x02] = decTobcd(tm.tm_hour);
   buf[0x01] = decTobcd(tm.tm_min);
   buf[0x00] = decTobcd(tm.tm_sec);
	
   buf[0x04] = decTobcd(tm.tm_mday);
   buf[0x05] = decTobcd(tm.tm_mon+1);
   buf[0x06] = decTobcd(tm.tm_year-100);
	
   printf("\nWrite System time to RTC\n");	
   printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]), bcdToDec(buf[1]), bcdToDec(buf[0]));
   printf("The RTC Date is %02d-%02d-%02d\n", bcdToDec(buf[4]), bcdToDec(buf[5]), bcdToDec(buf[6]));
  
   float temperature = buf[0x11] + ((buf[0x12]>>6)*0.25);
   printf("\n\nCurrent RTC temperature reading is = %.2f°C\n", temperature);

   close(file);
   printf("\n\nScript Complete\n");  
   return 0;
}
