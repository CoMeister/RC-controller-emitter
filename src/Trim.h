#ifndef Trim_h
#define Trim_h
class Trim
{
private:
    int amount;
    bool rev;
public:
    Trim();

    void incrementAmount(int amount);

    int getAmount();

    bool getRev();

    void setRev(bool rev);
};

#endif
