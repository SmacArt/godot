#include "nativepath.h"

using namespace godot;


void NativePath::_register_methods() {
  register_method("_ready", &NativePath::_ready);
  register_method("automatically_generated", &NativePath::automatically_generated);
  register_property<NativePath, bool>("is_looped",&NativePath::is_looped,false);
  register_property<NativePath, double>("radius",&NativePath::radius,5);
}

NativePath::NativePath() {
}

NativePath::~NativePath() {
}

NativePath::NativePath(Vector2D position) {
  set_position(Vector2(position.x, position.y));
  is_defined = true;
  radius=5;
}

void NativePath::_init() {
  is_looped = false;
  radius=5;
}

void NativePath::_ready() {
  if (!is_defined) {
    create_world_path(Vector2D(get_global_position().x, get_global_position().y));
  }
}

void NativePath::create_world_path(Vector2D position) {
  set_position(Vector2(position.x, position.y));
  is_defined=true;
}
