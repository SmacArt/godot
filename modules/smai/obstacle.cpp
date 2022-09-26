#include "obstacle.h"
#include <fstream>
#include "gameworld.h"

Obstacle::Obstacle(double x, double y, double radius) :
  BaseGameEntity (0, Vector2D(x,y), radius) {
}

void Obstacle::construct(double x, double y, double radius){
  set_position(Vector2D(x,y));
  set_bounding_radius(radius);
}
