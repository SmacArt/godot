#include "gameworld.h"

GameWorld::GameWorld() {
}

GameWorld::~GameWorld() {
  for (unsigned int i=0; i < vehicles.size(); i++) {
    delete vehicles[i];
  }
  for (unsigned int i=0; i < obstacles.size(); i++) {
    delete obstacles[i];
  }
}

void GameWorld::add_vehicle(Vehicle* vehicle) {
  vehicles.push_back(vehicle);
  vehicle->world = this;
}

void GameWorld::add_obstacle(Obstacle* obstacle) {
  obstacles.push_back(obstacle);
}

void GameWorld::add_wall(Wall2D* wall) {
  walls.push_back(wall);
}
