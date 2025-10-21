#ifndef REGENERATING_H
#define REGENERATING_H

#include "Crop.h"

class Regenerating : public Crop {
    protected:
    int maxHarvests;
    int totalHarvests;
    public:
    Regenerating(float dt);
    ~Regenerating();

    bool Harvest(int& payout) override;
};

#endif
