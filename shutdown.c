#include<stdio.h>
#include<wiringPi.h>
#include<wiringPiI2C.h>
#include<unistd.h>
#include<stdlib.h>

#define S_BUTTON 37 //phys pin number
#define disON 0x0C
#define clear_display 0x01
#define return_home 0x02 // display from 1st
#define set_2nd 0xA0 // display from 2nd

void print_LCD (int, int, char*); // display sentence to LCD
void clear (int);

int main (void)
{
  int flag = 0, fd = wiringPiI2CSetupInterface ("/dev/i2c-5", 0x3c);
  char buffer[17];
  wiringPiSetupPhys ();
  pinMode (S_BUTTON, OUTPUT);
  pinMode (S_BUTTON, INPUT);
  
  if (digitalRead(S_BUTTON) == LOW){
    clear(fd);
    print_LCD (fd, 1, "## Shutdown >>");
    print_LCD (fd, 2, "Push Button 5sec");
    sleep(1);
    clear(fd);
    
    flag = 1;
    print_LCD (fd, 1, "Keep ~ Shutdown");
    for (int i=0;i<5;i++){
      if (digitalRead(S_BUTTON) == LOW){
	sprintf(buffer, "remain : %dsec.", 5-i);
	print_LCD (fd, 2, buffer);
	flag = 1;
      }else{
	clear(fd);
	print_LCD (fd, 1, "### Shutdown ###");
	print_LCD (fd, 2, "### Canceld. ###");
	flag = 0;
	sleep(2);
	clear(fd);
	exit(0);
      }
      sleep(1);
    }
    if (flag == 1){
      clear(fd);
      print_LCD (fd, 1, "Shutting down...");
      print_LCD (fd, 2, "  Please Wait.");
      sleep(3);
      system("shutdown -h now && sudo i2cset -y 5 0x3c 0x00 0x08");
    }
  }
  return 0;
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
