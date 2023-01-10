/**
 * @file so1602aw.h
 * @author Keisuke Magara
 * @brief Driver program for OLED display "SO1602AW".
 * @version 1.0
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2023 Keisuke Magara
 *
 * @note This program depends on the wiringPi library.
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void get_wiringPiI2C_object(int* receiver, const char* i2c_bus, const int i2c_address);
void release_wiringPiI2C_object(int* i2c_fd);
void init_display(int i2c_obj);
void print_display(int i2c_obj, int place, const char* format, ...);
void clear_display(int i2c_obj, int row);

#ifdef __cplusplus
}
#endif