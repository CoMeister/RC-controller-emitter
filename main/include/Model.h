#ifndef Model_h
#define Model_h

#include "Arduino.h"
#include "Trim.h"
class Model
{
private:
    char* name;
    Trim trims[4];
    /* data */
public:
    Model(char* name);
    char* getName() const;
    Trim getTrim(int8_t index) const;
    int8_t getTrimLength() const;
    void setName(char* name);
    void setTrimAmount(int8_t id, int8_t amount);
    void reverseTrim(int8_t id);
    void incTrim(int8_t id, int8_t step);
};
#endif
