#ifndef NATIVE_PARAMETERS_H
#define NATIVE_PARAMETERS_H

#include "scene/main/node.h"

class NativeParameters : public Node {
  GDCLASS(NativeParameters, Node);

protected:
  static void _bind_methods();

public:

  enum AiLogic {
    AILOGIC_AI_BY_EXAMPLE,
    AILOGIC_NATURE_OF_CODE
  };

  void _init();

  int number_of_agents;
  int get_number_of_agents() {return number_of_agents;}
  void set_number_of_agents(int _number_of_agents) {number_of_agents = _number_of_agents;}

  bool vehicle_seek_on;
  int get_vehicle_seek_on() {return vehicle_seek_on;}
  void set_vehicle_seek_on(int _vehicle_seek_on) {vehicle_seek_on = _vehicle_seek_on;}

  bool vehicle_flee_on;
  bool get_vehicle_flee_on() {return vehicle_flee_on;}
  void set_vehicle_flee_on(bool _vehicle_flee_on) {vehicle_flee_on = _vehicle_flee_on;}

  bool vehicle_wander_on;
  bool get_vehicle_wander_on() {return vehicle_wander_on;}
  void set_vehicle_wander_on(bool _vehicle_wander_on) {vehicle_wander_on = _vehicle_wander_on;}

  bool vehicle_arrive_on;
  bool get_vehicle_arrive_on() {return vehicle_arrive_on;}
  void set_vehicle_arrive_on(bool _vehicle_arrive_on) {vehicle_arrive_on = _vehicle_arrive_on;}

  bool vehicle_obstacle_avoidance_on;
  bool get_vehicle_obstacle_avoidance_on() {return vehicle_obstacle_avoidance_on;}
  void set_vehicle_obstacle_avoidance_on(bool _vehicle_obstacle_avoidance_on) {vehicle_obstacle_avoidance_on = _vehicle_obstacle_avoidance_on;}

  bool vehicle_wall_avoidance_on;
  bool get_vehicle_wall_avoidance_on() {return vehicle_wall_avoidance_on;}
  void set_vehicle_wall_avoidance_on(bool _vehicle_wall_avoidance_on) {vehicle_wall_avoidance_on = _vehicle_wall_avoidance_on;}

  int number_of_obstacles;
  int get_number_of_obstacles() {return number_of_obstacles;}
  void set_number_of_obstacles(int _number_of_obstacles) {number_of_obstacles = _number_of_obstacles;}

  double obstacle_minimum_radius;
  double get_obstacle_minimum_radius() {return obstacle_minimum_radius;}
  void set_obstacle_minimum_radius(double _obstacle_minimum_radius) {obstacle_minimum_radius = _obstacle_minimum_radius;}

  double obstacle_maximum_radius;
  double get_obstacle_maximum_radius() {return obstacle_maximum_radius;}
  void set_obstacle_maximum_radius(double _obstacle_maximum_radius) {obstacle_maximum_radius = _obstacle_maximum_radius;}

  int number_of_cells_x;
  int get_number_of_cells_x() {return number_of_cells_x;}
  void set_number_of_cells_x(int _number_of_cells_x) {number_of_cells_x = _number_of_cells_x;}

  int number_of_cells_y;
  int get_number_of_cells_y() {return number_of_cells_y;}
  void set_number_of_cells_y(int _number_of_cells_y) {number_of_cells_y = _number_of_cells_y;}

  int number_of_smoothing_samples;
  int get_number_of_smoothing_samples() {return number_of_smoothing_samples;}
  void set_number_of_smoothing_samples(int _number_of_smoothing_samples) {number_of_smoothing_samples = _number_of_smoothing_samples;}

  double steering_force_tweaker;
  double get_steering_force_tweaker() {return steering_force_tweaker;}
  void set_steering_force_tweaker(double _steering_force_tweaker) {steering_force_tweaker = _steering_force_tweaker;}

  double maximum_steering_force;
  double get_maximum_steering_force() {return maximum_steering_force;}
  void set_maximum_steering_force(double _maximum_steering_force) {maximum_steering_force = _maximum_steering_force;}

  double maximum_speed;
  double get_maximum_speed() {return maximum_speed;}
  void set_maximum_speed(double _maximum_speed) {maximum_speed = _maximum_speed;}

  double vehicle_mass;
  double get_vehicle_mass() {return vehicle_mass;}
  void set_vehicle_mass(double _vehicle_mass) {vehicle_mass = _vehicle_mass;}

  double vehicle_scale;
  double get_vehicle_scale() {return vehicle_scale;}
  void set_vehicle_scale(double _vehicle_scale) {vehicle_scale = _vehicle_scale;}

  bool use_scale_from_vehicle;
  bool get_use_scale_from_vehicle() {return use_scale_from_vehicle;}
  void set_use_scale_from_vehicle(bool _use_scale_from_vehicle) {use_scale_from_vehicle = _use_scale_from_vehicle;}

  double separation_weight;
  double get_separation_weight() {return separation_weight;}
  void set_separation_weight(double _separation_weight) {separation_weight = _separation_weight;}

  double alignment_weight;
  double get_alignment_weight() {return alignment_weight;}
  void set_alignment_weight(double _alignment_weight) {alignment_weight = _alignment_weight;}

  double cohesion_weight;
  double get_cohesion_weight() {return cohesion_weight;}
  void set_cohesion_weight(double _cohesion_weight) {cohesion_weight = _cohesion_weight;}

  double obstacle_avoidance_weight;
  double get_obstacle_avoidance_weight() {return obstacle_avoidance_weight;}
  void set_obstacle_avoidance_weight(double _obstacle_avoidance_weight) {obstacle_avoidance_weight = _obstacle_avoidance_weight;}

  double wall_avoidance_weight;
  double get_wall_avoidance_weight() {return wall_avoidance_weight;}
  void set_wall_avoidance_weight(double _wall_avoidance_weight) {wall_avoidance_weight = _wall_avoidance_weight;}

  double wander_weight;
  double get_wander_weight() {return wander_weight;}
  void set_wander_weight(double _wander_weight) {wander_weight = _wander_weight;}

  double seek_weight;
  double get_seek_weight() {return seek_weight;}
  void set_seek_weight(double _seek_weight) {seek_weight = _seek_weight;}

  double flee_weight;
  double get_flee_weight() {return flee_weight;}
  void set_flee_weight(double _flee_weight) {flee_weight = _flee_weight;}

  double arrive_weight;
  double get_arrive_weight() {return arrive_weight;}
  void set_arrive_weight(double _arrive_weight) {arrive_weight = _arrive_weight;}

  double pursuit_weight;
  double get_pursuit_weight() {return pursuit_weight;}
  void set_pursuit_weight(double _pursuit_weight) {pursuit_weight = _pursuit_weight;}

  double offset_pursuit_weight;
  double get_offset_pursuit_weight() {return offset_pursuit_weight;}
  void set_offset_pursuit_weight(double _offset_pursuit_weight) {offset_pursuit_weight = _offset_pursuit_weight;}

  double interpose_weight;
  double get_interpose_weight() {return interpose_weight;}
  void set_interpose_weight(double _interpose_weight) {interpose_weight = _interpose_weight;}

  double hide_weight;
  double get_hide_weight() {return hide_weight;}
  void set_hide_weight(double _hide_weight) {hide_weight = _hide_weight;}

  double evade_weight;
  double get_evade_weight() {return evade_weight;}
  void set_evade_weight(double _evade_weight) {evade_weight = _evade_weight;}

  double follow_path_weight;
  double get_follow_path_weight() {return follow_path_weight;}
  void set_follow_path_weight(double _follow_path_weight) {follow_path_weight = _follow_path_weight;}

  double view_distance;
  double get_view_distance() {return view_distance;}
  void set_view_distance(double _view_distance) {view_distance = _view_distance;}

  double minimum_detection_box_length;
  double get_minimum_detection_box_length() {return minimum_detection_box_length;}
  void set_minimum_detection_box_length(double _minimum_detection_box_length) {minimum_detection_box_length = _minimum_detection_box_length;}

  double wall_detection_feeler_length;
  double get_wall_detection_feeler_length() {return wall_detection_feeler_length;}
  void set_wall_detection_feeler_length(double _wall_detection_feeler_length) {wall_detection_feeler_length = _wall_detection_feeler_length;}

  double probability_wall_avoidance;
  double get_probability_wall_avoidance() {return probability_wall_avoidance;}
  void set_probability_wall_avoidance(double _probability_wall_avoidance) {probability_wall_avoidance = _probability_wall_avoidance;}

  double probability_obstacle_avoidance;
  double get_probability_obstacle_avoidance() {return probability_obstacle_avoidance;}
  void set_probability_obstacle_avoidance(double _probability_obstacle_avoidance) {probability_obstacle_avoidance = _probability_obstacle_avoidance;}

  double probability_separation;
  double get_probability_separation() {return probability_separation;}
  void set_probability_separation(double _probability_separation) {probability_separation = _probability_separation;}

  double probability_alignment;
  double get_probability_alignment() {return probability_alignment;}
  void set_probability_alignment(double _probability_alignment) {probability_alignment = _probability_alignment;}

  double probability_cohesion;
  double get_probability_cohesion() {return probability_cohesion;}
  void set_probability_cohesion(double _probability_cohesion) {probability_cohesion = _probability_cohesion;}

  double probability_wander;
  double get_probability_wander() {return probability_wander;}
  void set_probability_wander(double _probability_wander) {probability_wander = _probability_wander;}

  double probability_seek;
  double get_probability_seek() {return probability_seek;}
  void set_probability_seek(double _probability_seek) {probability_seek = _probability_seek;}

  double probability_flee;
  double get_probability_flee() {return probability_flee;}
  void set_probability_flee(double _probability_flee) {probability_flee = _probability_flee;}

  double probability_evade;
  double get_probability_evade() {return probability_evade;}
  void set_probability_evade(double _probability_evade) {probability_evade = _probability_evade;}

  double probability_hide;
  double get_probability_hide() {return probability_hide;}
  void set_probability_hide(double _probability_hide) {probability_hide = _probability_hide;}

  double probability_arrive;
  double get_probability_arrive() {return probability_arrive;}
  void set_probability_arrive(double _probability_arrive) {probability_arrive = _probability_arrive;}

  double probability_interpose;
  double get_probability_interpose() {return probability_interpose;}
  void set_probability_interpose(double _probability_interpose) {probability_interpose = _probability_interpose;}

  double probability_follow_path;
  double get_probability_follow_path() {return probability_follow_path;}
  void set_probability_follow_path(double _probability_follow_path) {probability_follow_path = _probability_follow_path;}

  double wander_radius;
  double get_wander_radius() {return wander_radius;}
  void set_wander_radius(double _wander_radius) {wander_radius = _wander_radius;}

  double wander_distance;
  double get_wander_distance() {return wander_distance;}
  void set_wander_distance(double _wander_distance) {wander_distance = _wander_distance;}

  double wander_jitter_per_second;
  double get_wander_jitter_per_second() {return wander_jitter_per_second;}
  void set_wander_jitter_per_second(double _wander_jitter_per_second) {wander_jitter_per_second = _wander_jitter_per_second;}

  double wander_force_multiplier;
  double get_wander_force_multiplier() {return wander_force_multiplier;}
  void set_wander_force_multiplier(double _wander_force_multiplier) {wander_force_multiplier = _wander_force_multiplier;}

  double path_waypoint_seek_distance;
  double get_path_waypoint_seek_distance() {return path_waypoint_seek_distance;}
  void set_path_waypoint_seek_distance(double _path_waypoint_seek_distance) {path_waypoint_seek_distance = _path_waypoint_seek_distance;}

  double path_predict_position;
  double get_path_predict_position() {return path_predict_position;}
  void set_path_predict_position(double _path_predict_position) {path_predict_position = _path_predict_position;}

  double follow_path_force_multiplier;
  double get_follow_path_force_multiplier() {return follow_path_force_multiplier;}
  void set_follow_path_force_multiplier(double _follow_path_force_multiplier) {follow_path_force_multiplier = _follow_path_force_multiplier;}

  double arrive_radius;
  double get_arrive_radius() {return arrive_radius;}
  void set_arrive_radius(double _arrive_radius) {arrive_radius = _arrive_radius;}

  bool use_evade_threat_range;
  bool get_use_evade_threat_range() {return use_evade_threat_range;}
  void set_use_evade_threat_range(bool _use_evade_threat_range) {use_evade_threat_range = _use_evade_threat_range;}

  double evade_threat_range;
  double get_evade_threat_range() {return evade_threat_range;}
  void set_evade_threat_range(double _evade_threat_range) {evade_threat_range = _evade_threat_range;}

  double maximum_turn_rate_per_second;
  double get_maximum_turn_rate_per_second() {return maximum_turn_rate_per_second;}
  void set_maximum_turn_rate_per_second(double _maximum_turn_rate_per_second) {maximum_turn_rate_per_second = _maximum_turn_rate_per_second;}

  int toroid_width;
  int get_toroid_width() {return toroid_width;}
  void set_toroid_width(int _toroid_width) {toroid_width = _toroid_width;}

  int toroid_height;
  int get_toroid_height() {return toroid_height;}
  void set_toroid_height(int _toroid_height) {toroid_height = _toroid_height;}

  AiLogic ai_logic;
  AiLogic get_ai_logic() const {return ai_logic;}
  void set_ai_logic(const AiLogic _ai_logic) {ai_logic = _ai_logic;}

public:
  //void _init();

};

#endif
