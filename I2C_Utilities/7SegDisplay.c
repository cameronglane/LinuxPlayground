#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "7SegDisplay.h"

int main(){

  unsigned char *buffer = malloc(sizeof(char));
	/*
	//----- READ BYTES -----
	length = 4;			//<<< Number of bytes to read
	if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}*/

	
	//----- WRITE BYTES -----
  int addr = 0x70;

  Clear7Seg(addr);

  realloc(buffer, 5 * sizeof(char)); 

  buffer[0] = 0x76;
  buffer[1] = 0x01;
  buffer[2] = 0x02;
  buffer[3] = 0x03;
  buffer[4] = 0x04;

  if (WriteTo7Seg(addr, &buffer) != 0) {
    return(-1);
  }

  /*if (write(file_i2c, buffer, length) != length) {		//write() returns the number of bytes actually written
    //if it doesn't match then an error occurred (e.g. no response from the device)
    printf("Failed to write to the i2c bus.\n");
  }*/
 
  addr = 0x71;

/*  if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
  {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    //ERROR HANDLING; you can check errno to see what went wrong
    return(-1);
  }*/

  realloc(buffer, 3 * sizeof(char));

  buffer[0] = 0x76;
  buffer[1] = 0x0f;
  buffer[2] = 0x0f;

  if (WriteTo7Seg(addr, &buffer) != 0) {
    return(-1);
  }

  /*if (write(file_i2c, buffer, length) != length) {		//write() returns the number of bytes actually written
    //if it doesn't match then an error occurred (e.g. no response from the device)
    printf("Failed to write to the i2c bus.\n");
  }*/

  return(0);
}

int WriteTo7Seg(const int address, const char *buffer) {
  int file_i2c;

  char *filename = (char*)"/dev/i2c-1";
  if ((file_i2c = open(filename, O_RDWR)) < 0) {
    //ERROR HANDLING: you can check errno to see what went wrong
    printf("In Change7SegAddress: Failed to open the i2c bus");
    return(-1);
  }

  if (ioctl(file_i2c, I2C_SLAVE, address) < 0) {
    printf("In Change7SegAddress: Failed to acquire bus access and/or talk to slave.\n");
    //ERROR HANDLING; you can check errno to see what went wrong
    return(-1);
  }

  if (write(file_i2c, buffer, (sizeof(*buffer)/sizeof(char))) != (sizeof(*buffer)/sizeof(char))) {		//write() returns the number of bytes actually written
    //if it doesn't match then an error occurred (e.g. no response from the device)
    /* ERROR HANDLING: i2c transaction failed */
    printf("Failed to write to 7 Segment Display at address %c.\n", address);
    return(-1);
  }
  close(file_i2c);
  return(0);
}

int Clear7Seg(int address) {
  unsigned char buffer = {0x76};
  if (WriteTo7Seg(address, buffer) != 0) {
    printf("Failed to clear 7Seg at address %d.\n", address);
    return(-1);
  }
  return(0);
}

int Change7SegAddress(int old_address, const char new_address) {
  int file_i2c;
  char buffer[2] = {0x80, new_address};

  char *filename = (char*)"/dev/i2c-1";
  if ((file_i2c = open(filename, O_RDWR)) < 0) {
    //ERROR HANDLING: you can check errno to see what went wrong
    printf("In Change7SegAddress: Failed to open the i2c bus");
    return(-1);
  }

  if (ioctl(file_i2c, I2C_SLAVE, old_address) < 0) {
    printf("In Change7SegAddress: Failed to acquire bus access and/or talk to slave.\n");
    //ERROR HANDLING; you can check errno to see what went wrong
    return(-1);
  }

  if (write(file_i2c, buffer, sizeof(buffer)) != sizeof(buffer)) {		//write() returns the number of bytes actually written
    //if it doesn't match then an error occurred (e.g. no response from the device)
    /* ERROR HANDLING: i2c transaction failed */
    printf("Failed to change address of 7 Segment Display.\n");
    return(-1);
  }
  close(file_i2c);
  printf("Changed 7 Seg address.\n");
  return(0);

}
