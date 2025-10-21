#ifndef TOMATO_H
#define TOMATO_H

#include "Regenerating.h"

class Tomato : public Regenerating {
 public:
  Tomato(float dt);
  ~Tomato();

  int GetSellPrice() override;
};

#endif
