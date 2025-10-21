#ifndef POTATO_H
#define POTATO_H

#include "Stalk.h"

class Potato : public Stalk {
public:
    Potato(float dt);
    ~Potato();

    int GetSellPrice() override;
};

#endif
