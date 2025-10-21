#ifndef PIG_H
#define PIG_H

#include "animal.h"

class Pig : public Animal {
 private:
  Texture2D pigTex = LoadTexture("assets/pigPixel.png");
  Sound pigSound = LoadSound("assets/pigSound.wav");

 public:
  Pig(Texture2D pigText, Sound pigSound);
  void updateDaily() override;
  void makeSound() override;
};

#endif