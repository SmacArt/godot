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

  private:

    bool is_automatically_generated;

  public:

    Path* path;
    bool is_looped;
    double radius;

    static void _register_methods();

    NativePath(Vector2D position);

    NativePath();
    ~NativePath();

    void create_world_path(Vector2D position);

    void _init();
    void _ready();

    bool is_defined = false;

    bool automatically_generated() {return is_automatically_generated;}
    void set_automatically_generated(bool is_auto) {is_automatically_generated = is_auto;}

  };
#endif
