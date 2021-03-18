#include <Trim.h>

    Trim::Trim():amount(0), rev(false){};

    void Trim::setName(char* name){
        Trim::name = name;
    }

    void Trim::setAmount(int amount){
        Trim::amount = amount;
    }

    void Trim::setRev(bool rev){
        Trim::rev = rev;
    }

    char* Trim::getName(){
        return Trim::name;
    }

    int Trim::getAmount(){
        return Trim::amount;
    }

    bool Trim::isRev(){
        return Trim::rev;
    }

    
