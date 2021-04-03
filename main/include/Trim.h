#ifndef Trim_h
#define Trim_h
#include <Arduino.h>
class Trim
{
private:
    char* name;
    int8_t amount;
    bool rev;
public:
    Trim();

    void setAmount(int8_t amount);
    void setRev(bool rev);
    void setName(char* name);

    char* getName() const;
    int8_t getAmount() const;
    bool isRev() const;
};

#endif
