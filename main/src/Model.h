#ifndef Model_h
#define Model_h

#include <Arduino.h>
#include <Trim.h>
class Model
{
private:
    char* name;
    Trim trims[4];
    /* data */
public:
    Model(char* name);
    char* getName();
    Trim getTrim(byte index);
    byte getTrimLength();
    void setName(char* name);
    void setTrimAmount(byte id, byte amount);
    void reverseTrim(byte id);
    void incTrim(byte id, int step);
};
#endif
