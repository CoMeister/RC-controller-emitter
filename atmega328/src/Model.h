#ifndef Model_h
#define Model_h

#include <Arduino.h>
#include <Trim.h>
class Model
{
private:
    String name;
    Trim trims[4];
    /* data */
public:
    Model(String name);
    String getName();
    Trim getTrim(byte index);
    byte getTrimLength();
    void setName(String name);
    void setTrimAmount(byte id, byte amount);
    void reverseTrim(byte id);
    void incTrim(byte id, int step);
};
#endif
