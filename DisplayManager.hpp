class DisplayManager {
private:
    int mode;
    int i2cFd;

public:
    DisplayManager(int i2cBus, int i2cAddr);
    ~DisplayManager();
};