#include<stdio.h>
#include<stdlib.h>
#include<wiringPiI2C.h>
#include<unistd.h>

#define disON 0x0C
#define clear_display 0x01
#define return_home 0x02 // display from 1st
#define set_2nd 0xA0 // display from 2nd

void print_LCD (int, int, char*); // display sentence to LCD
void clear (int);

int main (void)
{
  int fd = wiringPiI2CSetupInterface ("/dev/i2c-5", 0x3c);
  system ("sudo systemctl stop LCD_Programs.service");
  clear(fd);
  print_LCD (fd, 1, "Scheduled Reboot");
  print_LCD (fd, 2, " Please Wait");
  sleep(1);
  print_LCD (fd, 0, ".");
  sleep(1);
  print_LCD (fd, 0, ".");
  sleep(1);
  print_LCD (fd, 0, ".");
  system ("reboot");
}

void print_LCD (int fd, int PLACE, char *contents)
{
  char c=0;
  if (PLACE == 1){
    wiringPiI2CWriteReg8 (fd, 0x00, return_home);
  }else if (PLACE == 2){
    wiringPiI2CWriteReg8 (fd, 0x00, set_2nd);
  }
  for (int i=0;i<16;i++){
    if (contents[i] == 0 || contents[i] == '\n'){
      break;
    }else{
      if (contents[i] == ' '){
	wiringPiI2CWriteReg8 (fd, 0x40, 0x20);
      }else{
	c = contents[i];
	wiringPiI2CWriteReg8 (fd, 0x40, c);
      }
    }
  }
  return;
}


void clear (int fd)
{
  wiringPiI2CWriteReg8 (fd, 0x00, clear_display);
  wiringPiI2CWriteReg8 (fd, 0x00, return_home);
  return;
}
