#include "nativeworld.h"
#include "steeringbehaviors.h"
#include "entityfunctiontemplates.h"

using namespace godot;

void NativeWorld::_register_methods()
{
  register_method("_init", &NativeWorld::_init);
  register_method("_ready", &NativeWorld::_ready);

  register_method("set_world_cross_hair", &NativeWorld::set_world_cross_hair);
  register_method("get_world_cross_hair", &NativeWorld::get_world_cross_hair);
}

NativeWorld::NativeWorld() {
}
NativeWorld::~NativeWorld() {
}

void NativeWorld::_init(){
  godot::ResourceLoader* resourceLoader =godot:: ResourceLoader::get_singleton();
  native_vehicle_scene = resourceLoader->load("res://SteeringBehaviors/NativeVehicle.tscn");
  native_obstacle_scene = resourceLoader->load("res://SteeringBehaviors/NativeObstacle.tscn");
  native_wall_scene = resourceLoader->load("res://SteeringBehaviors/NativeWall.tscn");
}

void NativeWorld::_ready() {
  build_world();
}

void NativeWorld::build_world() {

  // initialize world data
  game_world = new GameWorld();

  // grab parameters
  Node* parameters_node = find_node("NativeParameters");
  if (not parameters_node) {
    std::cout<<"unable to build world as the native parameters are not attached to the native world node!";
    return;
  }
  parameters = Object::cast_to<NativeParameters>(parameters_node);

  game_world->set_parameters(parameters);

  // Set toroid
  parameters->toroid_width = get_viewport()->get_visible_rect().get_size().x;
  parameters->toroid_height = get_viewport()->get_visible_rect().get_size().y;

  build_vehicles();
  build_obstacles();
  build_walls();
  build_paths();

}

void NativeWorld::build_vehicles() {

  godot::Node* vehicles = Object::cast_to<Node>(find_node("vehicles"));
  if (!vehicles) {
    vehicles = godot::Node::_new();
    vehicles->set_name("vehicles");
    add_child(vehicles);
  }
  append_vehicles(vehicles);
  create_vehicles(vehicles);
}

void NativeWorld::append_vehicles(godot::Node* vehicles) {
  // set up existing vehicles
  for (int i = 0; i < vehicles->get_child_count(); i++) {
    NativeVehicle* native_vehicle = Object::cast_to<NativeVehicle>(vehicles->get_child(i));
    if (native_vehicle) {
      native_vehicle->create_world_vehicle(Vector2D(native_vehicle->get_global_position().x,native_vehicle->get_global_position().y),
                                           native_vehicle->get_rotation(),
                                           Vector2D(native_vehicle->get_initial_velocity().x,native_vehicle->get_initial_velocity().y),
                                           native_vehicle->get_mass(),
                                           native_vehicle->get_max_steering_force(),
                                           native_vehicle->get_max_speed(),
                                           native_vehicle->get_max_turn_rate(),
                                           Vector2D(native_vehicle->get_scale().x,native_vehicle->get_scale().y),
                                           Vector2D(native_vehicle->get_scale_factor().x, native_vehicle->get_scale_factor().y),
                                           game_world,
                                           native_vehicle->get_logic());
      native_vehicle->set_automatically_generated(false);
    }
  }
}

void NativeWorld::create_vehicles(godot::Node* vehicles) {

  for (int i = 0; i < parameters->number_of_agents; i++) {

    //determine a random starting position
    Vector2D spawn_pos = Vector2D(parameters->toroid_width/2.0+RandomClamped()*parameters->toroid_width/2.0,
                                  parameters->toroid_height/2.0+RandomClamped()*parameters->toroid_height/2.0);

    spawn_pos.x += get_position().x;
    spawn_pos.y += get_position().y;

    godot::NativeVehicle* native_vehicle = Object::cast_to<godot::NativeVehicle>(native_vehicle_scene->instance());

    Vector2D scale = Vector2D(native_vehicle->get_scale().x, native_vehicle->get_scale().y);
    if (!parameters->use_scale_from_vehicle) {
      scale.x = parameters->vehicle_scale;
      scale.y = parameters->vehicle_scale;
      native_vehicle->set_scale(Vector2(scale.x,scale.y));
    }

    native_vehicle->create_world_vehicle(Vector2D(spawn_pos),
                                         RandFloat()*TwoPi,
                                         Vector2D(0,0),
                                         parameters->vehicle_mass,
                                         parameters->maximum_steering_force,
                                         parameters->maximum_speed,
                                         parameters->maximum_turn_rate_per_second,
                                         scale,
                                         Vector2D(native_vehicle->get_scale_factor().x, native_vehicle->get_scale_factor().y),
                                         game_world,
                                         native_vehicle->get_logic());
    native_vehicle->set_automatically_generated(true);

    if (parameters->vehicle_wander_on){
      native_vehicle->get_vehicle()->steering->wander_on();
    }

    if (parameters->vehicle_seek_on){
      native_vehicle->get_vehicle()->steering->seek_on();
    }

    if (parameters->vehicle_flee_on){
      native_vehicle->get_vehicle()->steering->flee_on();
    }

    if (parameters->vehicle_arrive_on){
      native_vehicle->get_vehicle()->steering->arrive_on();
    }

    if (parameters->vehicle_obstacle_avoidance_on){
      native_vehicle->get_vehicle()->steering->obstacle_avoidance_on();
    }

    if (parameters->vehicle_wall_avoidance_on){
      native_vehicle->get_vehicle()->steering->wall_avoidance_on();
    }

    vehicles->add_child(native_vehicle);
  }
}

void NativeWorld::build_obstacles() {

  // build obstacles
  godot::Node* obstacles = Object::cast_to<Node>(find_node("obstacles"));
  if (!obstacles) {
    obstacles = godot::Node::_new();
    obstacles->set_name("obstacles");
    add_child(obstacles);
  }
  append_obstacles(obstacles);
  create_obstacles(obstacles);
}

void NativeWorld::append_obstacles(godot::Node* obstacles) {

  for (int i = 0; i < obstacles->get_child_count(); i++) {
    NativeObstacle* native_obstacle = Object::cast_to<NativeObstacle>(obstacles->get_child(i));
    if (native_obstacle) {
      native_obstacle->create_world_obstacle(native_obstacle->get_global_position().x,
                                             native_obstacle->get_global_position().y,
                                             native_obstacle->get_radius());
      native_obstacle->set_automatically_generated(true);
      game_world->obstacles.push_back(native_obstacle->get_obstacle());
    }
  }
}

void NativeWorld::create_obstacles(godot::Node* obstacles) {

  double world_width = get_viewport()->get_visible_rect().get_size().x;
  double world_height = get_viewport()->get_visible_rect().get_size().y;

  //create a number of randomly sized tiddlywinks
  for (int i = 0; i < parameters->number_of_obstacles; i++)
  {
    bool overlapped = true;

    //keep creating tiddlywinks until we find one that doesn't overlap
    //any others.Sometimes this can get into an endless loop because the
    //obstacle has nowhere to fit. We test for this case and exit accordingly

    int num_tries = 0; int number_allowed_tries = 2000;

    while (overlapped)
    {
      num_tries++;

      if (num_tries > number_allowed_tries) return;

      int radius = RandInt((int)parameters->obstacle_minimum_radius, (int)parameters->obstacle_maximum_radius);

      const int border = 10;
      const int minimum_gap_between_obstacles = 20;

      Obstacle* ob = new Obstacle(RandInt(radius + border, world_width - radius - border),
                                  RandInt(radius + border, world_height - radius - border),
                                  radius);

      if (!::overlapped(ob, game_world->obstacles, minimum_gap_between_obstacles))
      {
        //its not overlapped so we can add it
        godot::NativeObstacle* native_obstacle = Object::cast_to<godot::NativeObstacle>(native_obstacle_scene->instance());

        native_obstacle->set_position(Vector2(ob->get_position().x, ob->get_position().y));
        native_obstacle->set_radius(ob->get_bounding_radius());

        native_obstacle->create_world_obstacle(ob->get_position().x, ob->get_position().y, ob->get_bounding_radius());
        native_obstacle->set_automatically_generated(true);
        obstacles->add_child(native_obstacle);
        game_world->obstacles.push_back(native_obstacle->get_obstacle());
        overlapped = false;
      }

      else
      {
        delete ob;
      }
    }
  }
}

void NativeWorld::build_walls() {

  // build walls
  godot::Node* walls = Object::cast_to<Node>(find_node("walls"));
  if (!walls) {
    walls = godot::Node::_new();
    walls->set_name("walls");
    add_child(walls);
  }
  append_walls(walls);
}

void NativeWorld::append_walls(godot::Node* walls) {
  Vector2D previous_point;
  Vector2 current_point;
  for (int i = 0; i < walls->get_child_count(); i++) {
    NativeWall* native_wall = Object::cast_to<NativeWall>(walls->get_child(i));
    if (native_wall) {
      // need to take all the wall points and create wall 2d objects
      for (int j = 0; j < native_wall->get_point_count(); j++) {
        if (j>0) {
          current_point = native_wall->get_points()[j];
          current_point += native_wall->get_global_position();
          native_wall->add_wall_edge(previous_point,Vector2D(current_point.x, current_point.y));
        }
        previous_point = Vector2D(native_wall->get_points()[j].x+native_wall->get_global_position().x,native_wall->get_points()[j].y+native_wall->get_global_position().y);
      }
      // add walls to world list
      for (unsigned int j = 0; j <native_wall->get_wall_edges().size(); j++) {
        game_world->walls.push_back(native_wall->get_wall_edges()[j]);
      }
    }
  }
}

void NativeWorld::build_paths() {

  // build paths
  godot::Node* paths = Object::cast_to<Node>(find_node("paths"));
  if (!paths) {
    paths = godot::Node::_new();
    paths->set_name("paths");
    add_child(paths);
  }
  append_paths(paths);
}

void NativeWorld::append_paths(godot::Node* paths) {
  Vector2D previous_point;
  Vector2 current_point;
  for (int i = 0; i < paths->get_child_count(); i++) {
    NativePath* native_path = Object::cast_to<NativePath>(paths->get_child(i));
    if (native_path) {
      Path* path = new Path();
      for (int j = 0; j < native_path->get_point_count(); j++) {
        current_point = native_path->get_points()[j];
        current_point += native_path->get_global_position();
        path->add_waypoint(Vector2D(current_point.x, current_point.y));
      }
      if (native_path->is_looped) {
        path->set_looped_on();
      } else {
        path->set_looped_off();
      }
      native_path->path = path;
      game_world->paths.push_back(path);
    }
  }
}

void NativeWorld::set_world_cross_hair(const Vector2 cross_hair) {
  if (game_world) {
    game_world->set_cross_hair(Vector2D(cross_hair.x,cross_hair.y));
  }
}

Vector2 NativeWorld::get_world_cross_hair() const {
  if (game_world) {
    return Vector2(game_world->get_cross_hair().x, game_world->get_cross_hair().y);
  }
  return Vector2();
}
