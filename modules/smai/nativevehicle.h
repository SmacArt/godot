#ifndef NATIVE_VEHICLE_H
#define NATIVE_VEHICLE_H

#include "core/variant//variant.h"

#include "vehicle.h"
#include "nativepath.h"
#include "nativeparameters.h"

enum VehicleType {SPACE_SHIP};

class NativeVehicle : public Node2D {
  GDCLASS(NativeVehicle, Node2D);

private:

  Vehicle* vehicle;

  bool is_automatically_generated;

  /* godot export attributes */
  double mass;
  double max_steering_force;
  double max_speed;
  double max_turn_rate;
  Vector2 initial_velocity;
  Vector2 scale_factor; // used to indicate the size of the vehicle based on this and the scale coming from godot
  int ai_logic;

protected:
  static void _bind_methods();

  void _init();
  void _ready();
  void _process(float delta);

public:

  NativeVehicle(Vector2D position,
                double rotation,
                Vector2D velocity,
                double mass,
                double max_steering_force,
                double max_speed,
                double max_turn_rate,
                Vector2D scale,
                Vector2D scale_factor,
                GameWorld* world,
                int ai_logic);

  NativeVehicle();
  ~NativeVehicle();

  void create_world_vehicle(Vector2D position,
                            double rotation,
                            Vector2D velocity,
                            double mass,
                            double max_steering_force,
                            double max_speed,
                            double max_turn_rate,
                            Vector2D scale,
                            Vector2D scale_factor,
                            GameWorld* world,
                            int ai_logic);

  bool is_defined = false;

  Vehicle* get_vehicle() const { return vehicle;}
  void set_vehicle(Vehicle* _vehicle) {vehicle = _vehicle;}

  void set_mass(const double new_mass) {mass = new_mass; if (vehicle) {vehicle->set_mass(new_mass);} }
  double get_mass() const { return mass;}

  void set_max_steering_force(const double new_max_steering_force) {max_steering_force = new_max_steering_force; if (vehicle) {vehicle->set_max_steering_force(new_max_steering_force);} }
  double get_max_steering_force() const { return max_steering_force;}

  void set_max_speed(const double new_max_speed) {max_speed = new_max_speed; if (vehicle) {vehicle->set_max_speed(new_max_speed);} }
  double get_max_speed() const { return max_speed;}

  void set_max_turn_rate(const double new_max_turn_rate) {max_turn_rate = new_max_turn_rate; if (vehicle) {vehicle->set_max_turn_rate(new_max_turn_rate);} }
  double get_max_turn_rate() const { return max_turn_rate;}

  void set_initial_velocity(const Vector2 new_initial_velocity) {initial_velocity = new_initial_velocity;  if (vehicle) {vehicle->set_velocity(Vector2D(new_initial_velocity.x,new_initial_velocity.y));}}
  Vector2 get_initial_velocity()const{return initial_velocity;}

  void set_scale_factor(const Vector2 new_scale_factor) {scale_factor = new_scale_factor;  if (vehicle) {vehicle->set_scale(Vector2D(new_scale_factor.x * get_scale().x,new_scale_factor.y * get_scale().y));}}
  Vector2 get_scale_factor()const{return scale_factor;}

  // methods for godot to switch on behaviors
  void set_seek_on();
  void set_flee_on();
  void set_wander_on();
  void set_arrive_on();
  void set_pursuit_on(const NativeVehicle* target);
  void set_evade_on(const NativeVehicle* target);
  void set_interpose_on(const NativeVehicle* target_a, const NativeVehicle* target_b);
  void set_hide_on(const NativeVehicle* hunter);
  void set_obstacle_avoidance_on();
  void set_wall_avoidance_on();
  void set_follow_path_on(const NativePath* path);

  void set_weight_wander(const double weight);
  void set_weight_arrive(const double weight);
  void set_weight_seek(const double weight);
  void set_weight_flee(const double weight);
  void set_weight_pursuit(const double weight);
  void set_weight_evade(const double weight);
  void set_weight_interpose(const double weight);
  void set_weight_hide(const double weight);
  void set_weight_obstacle_avoidance(const double weight);
  void set_weight_wall_avoidance(const double weight);
  void set_weight_follow_path(const double weight);

  void set_probability_wander(const double probability);
  void set_probability_arrive(const double probability);
  void set_probability_seek(const double probability);
  void set_probability_flee(const double probability);
  void set_probability_evade(const double probability);
  void set_probability_interpose(const double probability);
  void set_probability_hide(const double probability);
  void set_probability_obstacle_avoidance(const double weight);
  void set_probability_wall_avoidance(const double weight);
  void set_probability_follow_path(const double weight);

  void set_wander_jitter(const double jitter);
  void set_wander_radius(const double radius);
  void set_wander_distance(const double distance);
  void set_wander_force_multiplier(const double force_multiplier);

  void set_arrive_radius(const double radius);

  void set_deceleration(const int deceleration);
  void set_deceleration_tweaker(const double deceleration_tweaker);

  void set_use_evade_threat_range(const bool use);
  void set_evade_threat_range(const double range);

  void set_path_waypoint_seek_distance(const double distance);
  void set_path_predict_position(const double predict_position);
  void set_follow_path_force_multiplier(const double force_multiplier);

  // Methods for Godot Rendering
  bool is_wander_on();
  double get_wander_radius();
  Vector2 get_wander_target_position();
  Vector2 get_wander_circle_position();
  Vector2 get_steering_force();
  bool is_obstacle_avoidance_on();
  bool is_wall_avoidance_on();
  Vector2 get_heading();
  Vector2 get_side();

  //determines if created on the godot scene manullay or added automatically on the world
  bool automatically_generated() {return is_automatically_generated;}
  void set_automatically_generated(bool is_auto) {is_automatically_generated = is_auto;}

  void set_minimum_detection_box_length(const double length);
  double get_minimum_detection_box_length() const;
  double get_detection_box_length() const;

  double get_bounding_radius() const;

  int get_number_of_feelers() const;
  Vector2 get_feeler(const int position) const;

  void set_ai_logic(const int _ai_logic) {ai_logic = _ai_logic;}
  int get_ai_logic() const {return ai_logic;}

  //temp
  Vector2 get_temp_final_target();
  Vector2 get_temp_returned_wander();

};

#endif
