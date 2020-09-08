#ifndef Trim_h
#define Trim_h
#include <Arduino.h>
class Trim
{
private:
    String name;
    int amount;
    bool rev;
public:
    Trim();

    void setAmount(int amount);
    void setRev(bool rev);
    void setName(String name);

    String getName();
    int getAmount();
    bool isRev();
};

#endif
