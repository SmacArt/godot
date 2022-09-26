#include "nativeobstacle.h"


void NativeObstacle::_bind_methods() {
  //  ClassDB::bind_method(D_METHOD("_ready", &NativeObstacle::_ready));

  ClassDB::bind_method(D_METHOD("automatically_generated"), &NativeObstacle::automatically_generated);
  ClassDB::bind_method(D_METHOD("is_tagged"), &NativeObstacle::is_tagged);

  ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"radius"), "set_radius", "get_radius");
  ClassDB::bind_method(D_METHOD("get_radius"), &NativeObstacle::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius"), &NativeObstacle::set_radius);

}

NativeObstacle::NativeObstacle(double x, double y, double radius) {
  obstacle = new Obstacle(x,y,radius);
  is_defined = true;
}

void NativeObstacle::_init() {
  radius = 1.0;
}

void NativeObstacle::_ready() {
  if (!is_defined) {
    create_world_obstacle(get_position().x, get_position().y, radius);
  }
}

void NativeObstacle::create_world_obstacle(double x, double y, double radius) {
  obstacle->construct(x, y, radius);
  is_defined=true;
}
