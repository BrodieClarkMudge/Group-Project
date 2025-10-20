#ifndef COW_H
#define COW_H

#include "animal.h"

class Cow : public Animal {
    private:
    Texture2D cowTex = LoadTexture("assets/cowPixel.png");

    public:
    Cow(Texture2D cowText);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    void updateDaily() override;
};

#endif