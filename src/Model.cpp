#include <Arduino.h>
#include <Trim.h>
#include <Model.h>

    Model::Model(String name):
        name(name){
            for(byte i = 0; i < 4; i++){
                trims[i] = Trim();
            }
        }

    String Model::getName(){
        return name;
    }

    Trim Model::getTrim(byte index){
        return trims[index];
    }

    void Model::setName(String name){
        Model::name = name;
    }

    void Model::incrementTrim(byte index, int amount){
        Model::trims[index].incrementAmount(amount);
    }
