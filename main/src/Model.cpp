#include "Model.h"

Model::Model(char *name) : name(name)
{
    for (int8_t i = 0; i < 4; i++)
    {
        trims[i] = Trim();
    }

    trims[0].setName((char *)"Roll");
    trims[1].setName((char *)"Pitch");
    trims[2].setName((char *)"Throttle");
    trims[3].setName((char *)"Yaw");
}

char *Model::getName() const
{
    return name;
}

Trim Model::getTrim(int8_t index) const
{
    return trims[index];
}

int8_t Model::getTrimLength() const
{
    return *(&trims + 1) - trims;
}

void Model::setName(char *name)
{
    Model::name = name;
}

void Model::setTrimAmount(int8_t id, int8_t amount)
{
    Model::trims[id].setAmount(amount);
}

void Model::reverseTrim(int8_t id)
{
    Model::trims[id].setRev(!trims[id].isRev());
}

void Model::incTrim(int8_t id, int8_t step)
{
    Model::trims[id].setAmount(trims[id].getAmount() + step);
}
