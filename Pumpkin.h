#ifndef PUMPKIN_H
#define PUMPKIN_H

#include "Withering.h"

class Pumpkin : public Withering {
public:
    Pumpkin(float dt);
    ~Pumpkin() override;

    int GetSellPrice() override;
};

#endif
