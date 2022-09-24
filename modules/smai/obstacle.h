#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "basegameentity.h"

class Obstacle : public BaseGameEntity {

public:

  Obstacle(const Obstacle&);
  Obstacle& operator=(const Obstacle&);

  Obstacle(double x, double y, double radius);

  void construct(double x, double y, double radius);

  //this is defined as a pure virtual function in BasegameEntity so
  //it must be implemented
  void update(double time_elapsed){}

};

#endif
