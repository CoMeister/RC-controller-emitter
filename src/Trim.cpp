#include <Trim.h>
#include <Arduino.h>

    Trim::Trim():amount(0), rev(false){};

    void Trim::setName(String name){
        Trim::name = name;
    }

    void Trim::setAmount(int amount){
        Trim::amount = amount;
    }

    void Trim::setRev(bool rev){
        Trim::rev = rev;
    }

    String Trim::getName(){
        return Trim::name;
    }

    int Trim::getAmount(){
        return Trim::amount;
    }

    bool Trim::isRev(){
        return Trim::rev;
    }

    
