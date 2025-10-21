#ifndef STALK_H
#define STALK_H

#include <vector>
#include <string>
#include <memory>
#include "raylib-cpp.hpp"
#include "menu.h"
#include "Crop.h"




class Stalk : public Crop {
    public:
        Stalk(float dt);
        ~Stalk();
    
        bool Harvest(int& payout) override;

    };
        
#endif