#include<wiringPi.h>

#define boot_LED 19 // GPIO Number

int main (void)
{
  wiringPiSetupGpio();

  pinMode(boot_LED, OUTPUT);

  digitalWrite (boot_LED, HIGH);
  return 0;
}
