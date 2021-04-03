#include <Trim.h>

    Trim::Trim():amount(0), rev(false){};

    void Trim::setName(char* name){
        Trim::name = name;
    }

    void Trim::setAmount(int8_t amount){
        Trim::amount = amount;
    }

    void Trim::setRev(bool rev){
        Trim::rev = rev;
    }

    char* Trim::getName() const{
        return Trim::name;
    }

    int8_t Trim::getAmount() const{
        return Trim::amount;
    }

    bool Trim::isRev() const{
        return Trim::rev;
    }

    
