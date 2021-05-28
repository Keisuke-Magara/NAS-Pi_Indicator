#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<wiringPiI2C.h>
#include<unistd.h>
#include<time.h>

// Scheduled Reboot TIME : 2:00 am.

#define S_BUTTON 37 //phys pin number
#define disON 0x0C
#define clear_display 0x01
#define return_home 0x02 // display from 1st
#define set_2nd 0xA0 // display from 2nd


void init (int);
void print_LCD (int, int, char*); // display sentence to LCD
void clear (int);

int system_check1 (void); // OpenMediaVault
int system_check2 (void); // SSH
int get_cpu_temp(void);
int get_cpu_rate(void);
int get_ram_rate(void);
void get_p_ip (char *);
int get_hdd_used (void);
void clear_buffer (char *, int);

int main (void)
{
  int fd = wiringPiI2CSetupInterface("/dev/i2c-5", 0x3c);
  init (fd);
  printf("Program Start\n");
  clear(fd);
  print_LCD (fd, 1, ">>>> Hello! <<<<");
  print_LCD (fd, 2, "Initializing.");
  sleep(1);
  char p_ip[13], buffer1[17], buffer2[17];
  int hdd_rate=-1, flag=0;
  wiringPiSetupPhys ();
  pinMode (S_BUTTON, OUTPUT);
  pinMode (S_BUTTON, INPUT);
  wiringPiI2CWriteReg8 (fd, 0x40, '.');
  sleep(2);
  time_t t = 0;
  wiringPiI2CWriteReg8 (fd, 0x40, '.');
  struct tm *nt;
  sleep(2);
  wiringPiI2CWriteReg8 (fd, 0x40, '.');
  sleep(2);
  clear(fd);
  printf("initializing done.\n");
  print_LCD (fd, 1, "OMV: ");
  print_LCD (fd, 2, "SSH: ");
  wiringPiI2CWriteReg8 (fd, 0x00, 0x85);
  if (system_check1() == 1){
    print_LCD (fd, 0, "ACTIVE");
    printf("Open Media Vault = \"ACTIVE\"\n");
  }else{
    print_LCD (fd, 0, "INACTIVE");
    printf("Open Media Vault = \"INACTIVE\"\n");
    flag += 1;
  }
  wiringPiI2CWriteReg8 (fd, 0x00, 0xA5);
  if (system_check2() == 1){
    print_LCD (fd, 0, "ACTIVE");
    printf("Secure Shell = \"ACTIVE\"\n");
  }else{
    print_LCD (fd, 0, "INACTIVE");
    printf("Secure Shell = \"INACTIVE\"\n");
    flag += 2;
  }
  //sleep(10);  //if program doesn't work well, enable this.
 
  if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
  //get now time
  t = time(NULL);
  strftime(buffer1, sizeof(buffer1), "%Y/%m/%d %H:%M", localtime(&t));
  get_p_ip(p_ip);
  sprintf(buffer2, "IP:%s", p_ip);
  sleep(3);
  clear(fd);
  
  printf("START for loop.\n");
  for (;;){
    if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
    print_LCD (fd, 1, buffer1); //TIME
    print_LCD (fd, 2, buffer2); //IP
    if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
    clear_buffer (buffer1, 17);
    clear_buffer (buffer2, 17);
    sprintf(buffer1, "CPU:%3d%% / %2d", get_cpu_rate(), get_cpu_temp());
    hdd_rate = get_hdd_used();
    if (hdd_rate == -1){
    sprintf(buffer2, "RAM:%3d%% HDD:n/a", get_ram_rate());
    }else{
    sprintf(buffer2, "RAM:%3d%% HDD:%2d%%", get_ram_rate(), hdd_rate);
    }
    clear(fd);

    if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
    print_LCD (fd, 1, buffer1); //CPU: X% / X*C
    wiringPiI2CWriteReg8 (fd, 0x40, 0xF2);
    wiringPiI2CWriteReg8 (fd, 0x40, 'C');
    print_LCD (fd, 2, buffer2); //RAM: X% HDD:X%
    clear_buffer (buffer1, 17);
    clear_buffer (buffer2, 17);
    if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
    sleep(1);
    if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
    if (hdd_rate >= 90){
      clear(fd);
      print_LCD (fd, 1, ">>> Warning! <<<");
      print_LCD (fd, 2, "HDD almost FULL.");
      sleep(1);
    }
    if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
    if (flag > 0){
      clear(fd);
      print_LCD (fd, 1, ">>> Warning! <<<");
      switch (flag){
      case 1:
	print_LCD (fd, 2, "OMV : INACTIVE");
	break;
      case 2:
	print_LCD (fd, 2, "SSH : INACTIVE");
	break;
      case 3:
	print_LCD (fd, 2, "OMV,SSH:INACTIVE");
	break;
      }
      sleep(1);
    }
    if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
    //get now time
    t = time(NULL);
    nt = localtime(&t);
    if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
    if (nt->tm_hour == 1){
      clear(fd);
      print_LCD (fd, 1, ">>>> Notify <<<<");
      sprintf(buffer1, "Reboot in %dmin.", 60 - nt->tm_min);
      print_LCD (fd, 2, buffer1);
      clear_buffer (buffer1, 17);
      sleep(1);
    }
    if (digitalRead(S_BUTTON) == LOW){
      system("sudo /opt/LCD_Programs/shutdown.out");
    }
    //get now time
    t = time(NULL);
    strftime(buffer1, sizeof(buffer1), "%Y/%m/%d %H:%M", localtime(&t));
    get_p_ip(p_ip);
    sprintf(buffer2, "IP:%s", p_ip);
    clear(fd);
  }
  return 0;
}

void init (int fd)
{
  wiringPiI2CWriteReg8 (fd, 0x00, return_home);
  wiringPiI2CWriteReg8 (fd, 0x00, clear_display);
  wiringPiI2CWriteReg8 (fd, 0x00, disON);
  return;
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


int system_check1(void)
{
  FILE *f=NULL;
  char buffer[128];
  f = popen("systemctl status openmediavault-engined.service", "r");
  if (f == NULL){
    return -1;
  }else{
    while (1) {
      if (fscanf(f, " %s", buffer) == EOF){
	return -1;
      }else{
	if (buffer[0]=='A' && buffer[1]=='c' && buffer[2]=='t' && buffer[3]=='i' && buffer[4]=='v' && buffer[5]=='e' && buffer[6]==':'){
	  fscanf(f, "%s", buffer);
	  break;
	}
      }
    }
    if (buffer[0] == 'a'){
      return 1;
    }else if (buffer[0] == 'i'){
      return -1;
    }else{
      return -1;
    }
  }
  return -1;
}



int system_check2(void)
{
  FILE *f=NULL;
  char buffer[128];
  f = popen("systemctl status ssh", "r");
  if (f == NULL){
    return -1;
  }else{
    while (1) {
      if (fscanf(f, " %s", buffer) == EOF){
	return -1;
      }else{
	if (buffer[0]=='A' && buffer[1]=='c' && buffer[2]=='t' && buffer[3]=='i' && buffer[4]=='v' && buffer[5]=='e' && buffer[6]==':'){
	  fscanf(f, "%s", buffer);
	  break;
	}
      }
    }
    if (buffer[0] == 'a'){
      return 1;
    }else if (buffer[0] == 'i'){
      return -1;
    }else{
      return -1;
    }
  }
  return -1;
}


int get_cpu_temp(void)
{
  FILE *f = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
  char buffer[128];
  fgets (buffer, sizeof(buffer), f);
  fclose(f);
  return atoi(buffer) / 1000.0;
}


int get_cpu_rate(void)
{
  char buffer[128];
  int user1, user2, nice1, nice2, system1, system2,  idle1, idle2, busy, rate;
  FILE *f=NULL;
  f = fopen ("/proc/stat", "r");

  fscanf(f, "%s", buffer); //cpu の切り捨て
  clear_buffer(buffer, 128);
  fscanf(f, "  %s", buffer);
  user1 = atoi(buffer);
  clear_buffer(buffer, 128);
  fscanf(f, " %s", buffer);
  nice1 = atoi(buffer);
  clear_buffer(buffer, 128);
  fscanf(f, " %s", buffer);
  system1 = atoi(buffer);
  clear_buffer(buffer, 128);
  fscanf(f, " %s", buffer);
  idle1 = atoi(buffer);

  fclose(f);

  sleep(1);
  
  f = fopen ("/proc/stat", "r");

  fscanf(f, "%s", buffer); //cpu の切り捨て
  clear_buffer(buffer, 128);
  fscanf(f, "  %s", buffer);
  user2 = atoi(buffer);
  clear_buffer(buffer, 128);
  fscanf(f, " %s", buffer);
  nice2 = atoi(buffer);
  clear_buffer(buffer, 128);
  fscanf(f, " %s", buffer);
  system2 = atoi(buffer);
  clear_buffer(buffer, 128);
  fscanf(f, " %s", buffer);
  idle2 = atoi(buffer);

  fclose(f);
  //printf("[%d, %d, %d, %d, %d, %d, %d, %d]\n",user1, user2, nice1, nice2, system1, system2, idle1, idle2);
  busy = (user2+nice2+system2)-(user1+nice1+system1);
  rate = (double)busy * 100 / (busy+(idle2-idle1));
  return rate;
}

int get_ram_rate (void)
{
  int memtotal, memavailable;
  FILE *f = fopen ("/proc/meminfo", "r");
  char buffer[128];
  fscanf(f, "%s", buffer);
  //printf("%s\n", buffer);
  clear_buffer(buffer, 128);
  fscanf(f, "        %s", buffer);
  //printf("%s\n", buffer);
  memtotal = atoi(buffer);
  fgets(buffer, sizeof(buffer), f);
  fgets(buffer, sizeof(buffer), f);
  fscanf(f, "%s", buffer);
  //printf("%s\n", buffer);
  clear_buffer(buffer, 128);
  fscanf(f, "       %s", buffer);
  //printf("%s\n", buffer);
  memavailable = atoi(buffer);
  //printf("[%d, %d]\n", memtotal, memavailable);
  fclose(f);
  return ((double)(memtotal - memavailable) / memtotal) * 100;
}

void get_p_ip (char *p_ip)
{
  FILE *f;
  char input[128];
  clear_buffer(p_ip, 13);
  clear_buffer(input, 128);
  f = popen ("hostname -I", "r");
  if (f == NULL || fscanf(f, "%s", input) == EOF){
    sprintf(p_ip, "Not Detected.");
    return;
  }
  if (!(input[0]=='1' && input[1]=='9' && input[2]=='2' && input[3]=='.') && p_ip[0]!='N' && input[1]=='0'){
    sprintf(p_ip, "Connecting...");
    return;
  }
  sprintf(p_ip, " %s", input);
  pclose(f);
  return;
}

int  get_hdd_used (void)
{
  char buffer[128];
  FILE *f;
  f = popen ("df /dev/sda1",  "r"); // <=====Change to your DISK name.=====
  fgets(buffer, sizeof(buffer), f); //一行目飛ばし
  //printf("%s", buffer);
  fscanf(f, "%s", buffer); //Filesystem
  //printf("%s", buffer);
  fscanf(f, " %s", buffer); //1K-blocks
  //printf("%s", buffer);
  fscanf(f, " %s", buffer); //Used
  //printf("%s", buffer);
  fscanf(f, " %s", buffer); //Available
  //printf("%s", buffer);
  clear_buffer(buffer, 128);
  if (fscanf(f, " %s", buffer)==EOF){
    pclose(f);
    return -1;
  }else{
  //printf("%s", buffer);
  pclose(f);
  return atoi(buffer);
  }
}


 void clear_buffer (char *buffer, int size)
{
  for (int i=0;i<size;i++){
    buffer[i] = 0;
  }
  return;
}
