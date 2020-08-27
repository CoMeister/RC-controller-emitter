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
    void setName(String name);
    void incrementTrim(byte index, int amount);
};
#endif
