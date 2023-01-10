#include "DisplayManager.hpp"
#include "so1602aw.h"
#include <string>

using std::string;

DisplayManager::DisplayManager(int i2cBus, int i2cAddr)
{
    string i2cBusName = "/dev/i2c-";
    i2cBusName += std::to_string(i2cBus);
    get_wiringPiI2C_object(&i2cFd, i2cBusName.c_str(), i2cAddr);
}

DisplayManager::~DisplayManager()
{
    release_wiringPiI2C_object(&i2cFd);
}
