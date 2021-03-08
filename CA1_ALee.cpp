/*
* Code taken and adapted from
* https://github.com/derekmolloy/exploringrpi/blob/master/chp08/i2c/cpp/I2CDevice.cpp
*/


#include<sstream>
#include<fcntl.h>
#include<stdio.h>
#include<iomanip>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<iostream>
#include"I2CDevice.h"
#include"ledClass.h"

using namespace std;

#define GPIO         "/sys/class/gpio/"
#define FLASH_DELAY  50000 // 50 milliseconds
#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)
#define BUFFER_SIZE 19      //0x00 to 0x12

// the time is in the registers in encoded decimal form
int bcdToDec(char b) { return (b/16)*10 + (b%16); }

//Converts Decimal value to BCD taken from http://danceswithferrets.org/geekblog/?p=504
char decTobcd(int d) { return ((d/10) << 4 | (d%10));}

namespace exploringRPi {
	I2CDevice::I2CDevice(unsigned int bus, unsigned int device) {
	this->file=-1;
	this->bus = bus;
	this->device = device;
	this->open();
}

// Open a connection to an I2C device
int I2CDevice::open(){
   string name;
   if(this->bus==0) name = I2C_0;
   else name = I2C_1;

   if((this->file=::open(name.c_str(), O_RDWR)) < 0){
      perror("I2C: failed to open the bus\n");
	  return 1;
   }
   if(ioctl(this->file, I2C_SLAVE, this->device) < 0){
      perror("I2C: Failed to connect to the device\n");
	  return 1;
   }
   return 0;
}


// Write a single byte value to a single register.
int I2CDevice::writeRegister(unsigned int registerAddress, unsigned char value){
   unsigned char buffer[2];
   buffer[0] = registerAddress;
   buffer[1] = value;
   if(::write(this->file, buffer, 2)!=2){
      perror("I2C: Failed write to the device\n");
      return 1;
   }
   return 0;
}


//Write a single value to the I2C device.
int I2CDevice::write(unsigned char value){
   unsigned char buffer[1];
   buffer[0]=value;
   if (::write(this->file, buffer, 1)!=1){
      perror("I2C: Failed to write to the device\n");
      return 1;
   }
   return 0;
}

// Read a single register value from the address on the device.
unsigned char I2CDevice::readRegister(unsigned int registerAddress){
   this->write(registerAddress);
   unsigned char buffer[1];
   if(::read(this->file, buffer, 1)!=1){
      perror("I2C: Failed to read in the value.\n");
      return 1;
   }
   return buffer[0];
}


// Method to read a number of registers from a single device.
unsigned char* I2CDevice::readRegisters(unsigned int number, unsigned int fromAddress){
	this->write(fromAddress);
	unsigned char* data = new unsigned char[number];
    if(::read(this->file, data, number)!=(int)number){
       perror("IC2: Failed to read in the full buffer.\n");
	   return NULL;
    }
	return data;
}


// Method to dump the registers to the standard output. 
void I2CDevice::debugDumpRegisters(unsigned int number){
	cout << "Dumping Registers for Debug Purposes:" << endl;
	unsigned char *registers = this->readRegisters(number);
	for(int i=0; i<(int)number; i++){
		cout << HEX(*(registers+i)) << " ";
		if (i%16==15) cout << endl;
	}
	cout << dec;
}


// Close the file handles and sets a temporary state to -1.

void I2CDevice::close(){
	::close(this->file);
	this->file = -1;
}

// Closes the file on destruction, provided that it has not already been closed.
I2CDevice::~I2CDevice() {
	if(file!=-1) this->close();
}

}

int main(int argc, char* argv[] ){

   int file;
   printf("Starting the DS3231 test application\n");

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
   printf("The current temperature is %.2fÂ°C\n", temperature);


   //char system_datetime = std::chrono::system_clock::now();
   //printf("The System Time is", system_datetime);

   // Test decTobcd Convertor
   printf("Buffer 2 = %d, Converted val = %d, Converted back = %d\n", buf[0], bcdToDec(buf[0]), decTobcd(bcdToDec(buf[0])));

   printf(" Write to RTC Clock (minutes reg)\n");
   writeBuffer[0x01] ={decTobcd(23)};
   //write(file, writeBuffer[2], decTobcd(23));

   printf(" Read Current Values\n");
   printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]), bcdToDec(buf[1]), bcdToDec(buf[0]));
   //printf("The RTC Date is %02d-%02d-%02d\n", bcdToDec(buf[4]), bcdToDec(buf[5]), bcdToDec(buf[6]));

   close(file);
   printf("\n\nScript Complete\n");
   return 0;
}
