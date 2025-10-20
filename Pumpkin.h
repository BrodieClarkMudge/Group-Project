#ifndef PUMPKIN_H
#define PUMPKIN_H

#include "Large.h"

class Pumpkin : public Large {
public:
    Pumpkin(float dt);
    ~Pumpkin();

    int GetSellPrice() override;
};

#endif
