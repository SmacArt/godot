#include "nativewall.h"

using namespace godot;


void NativeWall::_register_methods() {
  register_method("_ready", &NativeWall::_ready);
  register_method("automatically_generated", &NativeWall::automatically_generated);
}

NativeWall::NativeWall() {
}

NativeWall::~NativeWall() {
}

NativeWall::NativeWall(Vector2D position) {
  set_position(Vector2(position.x, position.y));
  is_defined = true;
}

void NativeWall::_init() {
}

void NativeWall::_ready() {
  if (!is_defined) {
    create_world_wall(Vector2D(get_global_position().x, get_global_position().y));
  }
}

void NativeWall::create_world_wall(Vector2D position) {
  set_position(Vector2(position.x, position.y));
  is_defined=true;
}

void NativeWall::add_wall_edge(const Vector2D from, const Vector2D to) {
  wall_edges.push_back(new Wall2D(from, to));
}
