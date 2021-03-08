
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
using namespace std;

#define BUFFER_SIZE 19      //0x00 to 0x12

#define RED 6
#define ORANGE 13
#define YELLOW 19
#define BLUE 26

int file;

// the time is in the registers in encoded decimal form
int bcdToDec(char b) { return (b/16)*10 + (b%16); }

//Converts Decimal value to BCD taken from http://danceswithferrets.org/geekblog/?p=504
char decTobcd(int d) { return ((d/10) << 4 | (d%10));}


// Write a single byte value to a single register.
int writeRegister(unsigned int registerAddress, unsigned char value){
   unsigned char buf[2];
   buf[0] = registerAddress;
   buf[1] = value;
   if(write(file, buf, 2)!=2){
      perror("I2C: Failed write to the device\n");
      return 1;
   }

   return 0;
}


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
   
   printf(" Read RTC Current Values\n");   
   printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]), bcdToDec(buf[1]), bcdToDec(buf[0]));
   printf("The RTC Date is %02d-%02d-%02d\n", bcdToDec(buf[4]), bcdToDec(buf[5]), bcdToDec(buf[6]));
   float temperature = buf[0x11] + ((buf[0x12]>>6)*0.25);
   printf("The current temperature is %.2f°C\n", temperature);

   // Test decTobcd Convertor
   //printf("Buffer 0 = %d, Converted val = %d, Converted back = %d\n", buf[0], bcdToDec(buf[0]), decTobcd(bcdToDec(buf[0])));

   // Test Write to Registers
   printf("Buffer 0 = %d, Converted val = %d\n", buf[1], bcdToDec(buf[1]));
   buf[0x01] = decTobcd(48);
   printf("Buffer 0 = %d, Converted val = %d\n", buf[1], bcdToDec(buf[1]));
   printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]), bcdToDec(buf[1]), bcdToDec(buf[0]));

   close(file);
   printf("\n\nScript Complete\n");  
   return 0;
}
