#ifndef NATIVE_WORLD_H
#define NATIVE_WORLD_H

#include "core/variant/variant.h"
#include "scene/resources/packed_scene.h"

#include "nativeparameters.h"
#include "nativevehicle.h"
#include "nativeobstacle.h"
#include "nativewall.h"
#include "gameworld.h"


class NativeWorld : public Node2D {

  GDCLASS(NativeWorld, Node2D);

protected:
  static void _bind_methods();

  void _init();
  void _ready();

public:

  NativeParameters* parameters;
  GameWorld* game_world;

  void build_world();
  void set_world_cross_hair(const Vector2 cross_hair);
  Vector2 get_world_cross_hair() const;

private:
  Ref<PackedScene> native_vehicle_scene;
  Ref<PackedScene> native_obstacle_scene;
  Ref<PackedScene> native_wall_scene;

  void build_vehicles();
  void append_vehicles(Node* vehicles);
  void create_vehicles(Node* vehicles);

  void build_obstacles();
  void append_obstacles(Node* obstacles);
  void create_obstacles(Node* obstacles);

  void build_walls();
  void append_walls(Node* walls);

  void build_paths();
  void append_paths(Node* paths);
};
#endif
