#ifndef NATIVE_WORLD_H
#define NATIVE_WORLD_H

#include "core/variant/variant.h"

#include "nativeparameters.h"
#include "nativevehicle.h"
#include "nativeobstacle.h"
#include "nativewall.h"
#include "gameworld.h"


  class NativeWorld : public Node2D {

    GODOT_CLASS(NativeWorld, Node2D);

  public:

    NativeParameters* parameters;
    GameWorld* game_world;

    static void _register_methods();

    NativeWorld();
    ~NativeWorld();

    void _init();
    void _ready();

    void build_world();

    void set_world_cross_hair(const Vector2 cross_hair);
    Vector2 get_world_cross_hair() const;

  private:
    godot::Ref<godot::PackedScene> native_vehicle_scene;
    godot::Ref<godot::PackedScene> native_obstacle_scene;
    godot::Ref<godot::PackedScene> native_wall_scene;

    void build_vehicles();
    void append_vehicles(godot::Node* vehicles);
    void create_vehicles(godot::Node* vehicles);

    void build_obstacles();
    void append_obstacles(godot::Node* obstacles);
    void create_obstacles(godot::Node* obstacles);

    void build_walls();
    void append_walls(godot::Node* walls);

    void build_paths();
    void append_paths(godot::Node* paths);
}
#endif
