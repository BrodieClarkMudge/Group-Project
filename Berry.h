#ifndef BERRY_H
#define BERRY_H

#include "Regenerating.h"

class Berry : public Regenerating {
public:
    Berry(float dt);
    ~Berry();

    int GetSellPrice() override;
};

#endif
