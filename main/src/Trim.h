#ifndef Trim_h
#define Trim_h
#include <Arduino.h>
class Trim
{
private:
    char* name;
    int amount;
    bool rev;
public:
    Trim();

    void setAmount(int amount);
    void setRev(bool rev);
    void setName(char* name);

    char* getName();
    int getAmount();
    bool isRev();
};

#endif
