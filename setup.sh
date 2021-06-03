#Please type "chmod 700 setup.sh" before run this script.
sudo raspi-config nonint do_i2c 0
sudo echo -e "dtoverlay=i2c5,pins_12_13" | sudo tee -a /boot/config.txt
sudo echo -e "dtparam=i2c_vc_baudrate=40000" | sudo tee -a /boot/config.txt
sudo apt install wiringpi
gcc -o boot_LED.out boot_LED.c -lwiringPi
gcc -o main.out display.c -lwiringPi
gcc -o scheduled_reboot.out scheduled_reboot.c -lwiringPi
gcc -o shutdown.out shutdown.c -lwiringPi
sudo mkdir /opt/LCD_Programs
sudo mv ./*.out /opt/LCD_Programs
sudo cp ./*.service /etc/systemd/system/
sudo cp ./*.timer /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable LCD_Programs.service
sudo systemctl active LCD_Ptograms.service
sudo systemctl enable scheduled_reboot.timer
printf "\n=====Please Reboot to start LCD Programs.====="
