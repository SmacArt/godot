#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "vehicle.h"
#include "obstacle.h"
#include "nativeparameters.h"
#include "entityfunctiontemplates.h"
#include <vector>
#include "common/2d/wall2d.h"
#include "common/2d/path.h"

class GameWorld {

private:

  Vector2D cross_hair;

public:

  std::vector<Vehicle*> vehicles;
  godot::NativeParameters* parameters;
  std::vector<BaseGameEntity*> obstacles;
  std::vector<Wall2D*> walls;
  std::vector<Path*> paths;

  GameWorld();
  ~GameWorld();

  void add_vehicle(Vehicle* vehicle);
  void add_obstacle(Obstacle* obstacle);
  void add_wall(Wall2D* wall);
  void add_path(Wall2D* path);

  void set_parameters(godot::NativeParameters* _params) {parameters=_params;}

  Vector2D get_cross_hair() const {return cross_hair;}
  void set_cross_hair(const Vector2D new_cross_hair) {cross_hair=new_cross_hair;}

  void tag_obstacles_within_view_range(BaseGameEntity* vehicle, double range)
  {
    tag_neighbors(vehicle, obstacles, range);
  }

  void tag_vehicles_within_view_range(BaseGameEntity* vehicle, double range)
  {
    tag_neighbors(vehicle, vehicles, range);
  }

};




#endif
