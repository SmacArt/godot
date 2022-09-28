#ifndef NATIVE_WALL_H
#define NATIVE_WALL_H

#include "scene/2d/line_2d.h"
#include "common/2d/wall2d.h"

class GameWorld;

class NativeWall : public Line2D {
  GDCLASS(NativeWall, Line2D);


protected:
  static void _bind_methods();
  void _ready();

private:

  std::vector<Wall2D*> wall_edges;
  bool is_automatically_generated;

public:

  NativeWall();
  ~NativeWall();
  NativeWall(Vector2D position);

  void create_world_wall(Vector2D position);

  bool is_defined = false;

  bool get_is_automatically_generated() {return is_automatically_generated;}
  void set_is_automatically_generated(const bool is_auto) {is_automatically_generated = is_auto;}

  void add_wall_edge(const Vector2D from, const Vector2D to);
  std::vector<Wall2D*> get_wall_edges() const {return wall_edges;}

};
#endif
