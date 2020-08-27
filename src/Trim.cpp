#include <Trim.h>

    Trim::Trim();

    void Trim::incrementAmount(int amount){
        Trim::amount += amount;
    }

    int Trim::getAmount(){
        return Trim::amount;
    }

    bool Trim::getRev(){
        return Trim::rev;
    }

    void Trim::setRev(bool rev){
        Trim::rev = rev;
    }
