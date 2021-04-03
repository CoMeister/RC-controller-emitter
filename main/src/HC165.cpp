#include "HC165.h"
#include "Arduino.h"

HC165::HC165()
{
}

HC165::HC165(char _ser, char _load, char _clk, char _nbrChip = 1)
    : serialOut(_ser), load(_load), clk(_clk), numBerOfChip(_nbrChip)
{
    pinMode(this->serialOut, INPUT);
    pinMode(this->load, OUTPUT);
    pinMode(this->clk, OUTPUT);
}

HC165::~HC165()
{
}

int HC165::readByte()
{
    int inputs = 0;

    //digitalWrite(CLK_enable, HIGH);
    digitalWrite(this->load, LOW);
    delayMicroseconds(5);
    digitalWrite(this->load, HIGH);
    //digitalWrite(CLK_enable, LOW);

    for (int i = 0; i < this->numBerOfChip * 8; i++)
    {
        inputs |= digitalRead(serialOut) << ((this->numBerOfChip * 8 - 1) - i);
        digitalWrite(this->clk, HIGH);
        delayMicroseconds(5);
        digitalWrite(this->clk, LOW);
    }

    return inputs;
}