#ifndef NATIVE_OBSTACLE_H
#define NATIVE_OBSTACLE_H

#include "obstacle.h"
#include "scene/2d/node_2d.h"


  class NativeObstacle : public Node2D {
    GDCLASS(NativeObstacle, Node2D);

  private:
    Obstacle* obstacle;
    double radius;
    bool is_automatically_generated;

  protected:
    static void _bind_methods();

  public:

    NativeObstacle(double x, double y, double radius);

    void create_world_obstacle(double x, double y, double radius);

    void _init();
    void _ready();

    bool is_defined = false;

    Obstacle* get_obstacle() const { return obstacle;}
    void set_obstacle(Obstacle* _obstacle) {obstacle = _obstacle;}

    bool automatically_generated() {return is_automatically_generated;}
    void set_automatically_generated(bool is_auto) {is_automatically_generated = is_auto;}

    void set_radius(const double _radius) {radius=_radius;}
    double get_radius() const {return radius;}

    bool is_tagged() const {if (obstacle) return obstacle->is_tagged(); else return false;}
  };
#endif
