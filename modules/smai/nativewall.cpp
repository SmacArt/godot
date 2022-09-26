#include "nativewall.h"

void NativeWall::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_automatically_generated"), &NativeWall::get_is_automatically_generated);
	ClassDB::bind_method(D_METHOD("set_is_automatically_generated"), &NativeWall::set_is_automatically_generated);
}

NativeWall::NativeWall(Vector2D position) {
  set_position(Vector2(position.x, position.y));
  is_defined = true;
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
