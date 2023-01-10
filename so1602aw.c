/**
 * @file so1602aw.c
 * @headerfile so1602aw.h
 * @author Keisuke Magara
 * @brief Driver program for OLED display "SO1602AW".
 * @version 1.0
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2023 Keisuke Magara
 *
 * @note This program depends on the wiringPi library.
 *
 * @cite https://akizukidenshi.com/download/ds/akizuki/so1602awwb-uc-wb-u_akizuki_manu.pdf
 *
 */
#include "so1602aw.h"
#include <stdarg.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

// SO1602AW command
#ifdef DEBUG
#define Display_ON 0x0F // enable cursor & blink
#else
#define Display_ON 0x0C // disable cursor & blink
#endif
#define Display_OFF 0x08 // disable cursor & blink (POR)
#define Clear_Display 0x01
#define Return_Home 0x02

/**
 * @brief Get the wiringPi I2C Interface object.
 * @note This method is for C++ user.
 *
 * @param receiver(int*) Pointer to receive File Descriptor
 * @param i2c_bus(char*) I2C bus name
 * @param i2c_address(int) I2C address
 */
void get_wiringPiI2C_object(int* receiver, const char* i2c_bus, const int i2c_address)
{
    *receiver = wiringPiI2CSetupInterface(i2c_bus, i2c_address);
    return;
}

/**
 * @brief Release the wiringPi I2C Interface object.
 * @note This method is for C++ user.
 *
 * @param i2c_fd(int*) File Descriptor provided by get_wiringPiI2C_object function.
 */
void release_wiringPiI2C_object(int* i2c_fd)
{
    fclose(i2c_fd);
}

/**
 * @brief Initialise and turn on the SO1602AW display.
 *
 * @param i2c_obj(int) File Descriptor provided by wiringPiI2CSetupInterface function.
 */
void init_display(int i2c_obj)
{
    wiringPiI2CWriteReg8(i2c_obj, 0x00, Display_OFF);
    wiringPiI2CWriteReg8(i2c_obj, 0x00, Return_Home);
    wiringPiI2CWriteReg8(i2c_obj, 0x00, Clear_Display);
    wiringPiI2CWriteReg8(i2c_obj, 0x00, Display_ON);
    return;
}

void turnon_display(int i2c_obj)
{
    wiringPiI2CWriteReg8(i2c_obj, 0x00, Display_ON);
}

/**
 * @brief Print text on the SO1602AW display.
 *
 * @param i2c_obj(int) File Descriptor provided by wiringPiI2CSetupInterface function.
 * @param place(int)
 * @param format(char*) Specify format as with the printf function.
 * @param ...(va) Specify the value to be replaced as with the printf function.
 * @return int Returns the number of characters written to display if the execution succeeds; returns negative value if it fails.
 *
 * @note [example] print_display(i2c_fd, 0x20, "Hello %d!", 2023);
 */
int print_display(int i2c_obj, int place, const char* format, ...)
{
    char outbuf[17];
    int ret = 0;
    va_list va;
    ret = vsprintf(outbuf, format, va);
    printf("%s", outbuf);
    fflush(stdout);
    return ret;
}

/**
 * @brief Erase characters written on SO1602AW display.
 *
 * @param i2c_obj(int) File Descriptor provided by wiringPiI2CSetupInterface function.
 * @param row(int) If 1 or 2 is specified, erase characters written on the specified line; otherwise, erase all characters.
 */
void clear_display(int i2c_obj, int row)
{
    if (row == 1 || row == 2) {
        /* do something*/
    } else {
        wiringPiI2CWriteReg8(fd, 0x00, clear_display);
        wiringPiI2CWriteReg8(fd, 0x00, return_home);
    }
    return;
}