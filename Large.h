#ifndef LARGE_H
#define LARGE_H

#include "Crop.h"

class Large : public Crop {
protected:
    int width;
    int height;

public:
    Large(float dt, int w, int h);
    ~Large();

    bool Harvest(int& payout) override;
    int GetWidth() const;
    int GetHeight() const;
};

#endif
