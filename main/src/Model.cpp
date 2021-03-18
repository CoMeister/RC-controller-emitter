#include <Model.h>

    Model::Model(char* name):
        name(name){
            for(byte i = 0; i < 4; i++){
                trims[i] = Trim();
            }

            trims[0].setName("Roll");
            trims[1].setName("Pitch");
            trims[2].setName("Throttle");
            trims[3].setName("Yaw");
        }

    char* Model::getName(){
        return name;
    }

    Trim Model::getTrim(byte index){
        return trims[index];
    }

    byte Model::getTrimLength(){
        return *(&trims + 1) - trims;
    }

    void Model::setName(char* name){
        Model::name = name;
    }

    void Model::setTrimAmount(byte id, byte amount){
        Model::trims[id].setAmount(amount);
    }

    void Model::reverseTrim(byte id){
        Model::trims[id].setRev(!trims[id].isRev());
    }

    void Model::incTrim(byte id, int step){
         Model::trims[id].setAmount(trims[id].getAmount()+step);
    }
