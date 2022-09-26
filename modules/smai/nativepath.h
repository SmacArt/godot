#ifndef NATIVE_PATH_H
#define NATIVE_PATH_H

#include "core/io/resource.h"
#include "core/variant/variant.h"
#include "scene/2d/line_2d.h"
#include "common/2d/path.h"
#include "common/2d/vector2d.h"

class GameWorld;


class NativePath : public Line2D {
  GDCLASS(NativePath, Line2D);

protected:
  static void _bind_methods();
  void _init();
  void _ready();

private:

  bool is_automatically_generated;

public:

  NativePath(Vector2D position);

  Path* path;

  bool is_looped;
  bool get_is_looped() const {return is_looped;}
  void set_is_looped(const bool _is_looped) {is_looped = _is_looped;}

  double radius;
  bool get_radius() const {return radius;}
  void set_radius(const bool _radius) {radius = _radius;}


  bool is_defined = false;

  bool automatically_generated() {return is_automatically_generated;}
  void create_world_path(Vector2D position);
  void set_automatically_generated(bool is_auto) {is_automatically_generated = is_auto;}

};
#endif
