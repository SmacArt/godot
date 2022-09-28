#include "nativepath.h"

void NativePath::_bind_methods() {
  ClassDB::bind_method("automatically_generated", &NativePath::automatically_generated);

  ClassDB::bind_method(D_METHOD("set_is_looped"), &NativePath::set_is_looped);
  ClassDB::bind_method(D_METHOD("get_is_looped"), &NativePath::get_is_looped);

  ClassDB::bind_method(D_METHOD("set_radius"), &NativePath::set_radius);
  ClassDB::bind_method(D_METHOD("get_radius"), &NativePath::get_radius);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_looped"),"set_is_looped","get_is_looped");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius"),"set_radius","get_radius");
}

NativePath::NativePath() {}
NativePath::~NativePath() {}

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
