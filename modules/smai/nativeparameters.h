#ifndef NATIVE_PARAMETERS_H
#define NATIVE_PARAMETERS_H

#include "scene/main/node.h"

class NativeParameters : public Node {
  GDCLASS(NativeParameters, Node);

protected:
  static void _bind_methods();

public:

/* godot export attributes */
  int number_of_agents;

  bool vehicle_seek_on;
  bool vehicle_flee_on;
  bool vehicle_wander_on;
  bool vehicle_arrive_on;
  bool vehicle_obstacle_avoidance_on;
  bool vehicle_wall_avoidance_on;

  int number_of_obstacles;
  double obstacle_minimum_radius;
  double obstacle_maximum_radius;

  int number_of_cells_x;
  int number_of_cells_y;

  int number_of_smoothing_samples;

  double steering_force_tweaker;
  double maximum_steering_force;
  double maximum_speed;
  double vehicle_mass;
  double vehicle_scale;
  bool use_scale_from_vehicle;

  double separation_weight;
  double alignment_weight;
  double cohesion_weight;
  double obstacle_avoidance_weight;
  double wall_avoidance_weight;
  double wander_weight;
  double seek_weight;
  double flee_weight;
  double arrive_weight;
  double pursuit_weight;
  double offset_pursuit_weight;
  double interpose_weight;
  double hide_weight;
  double evade_weight;
  double follow_path_weight;

  double view_distance;

  double minimum_detection_box_length;
  double wall_detection_feeler_length;

  double probability_wall_avoidance;
  double probability_obstacle_avoidance;
  double probability_separation;
  double probability_alignment;
  double probability_cohesion;
  double probability_wander;
  double probability_seek;
  double probability_flee;
  double probability_evade;
  double probability_hide;
  double probability_arrive;
  double probability_interpose;
  double probability_follow_path;

  double wander_radius;
  double wander_distance;
  double wander_jitter_per_second;
  double wander_force_multiplier;

  double path_waypoint_seek_distance;
  double path_predict_position;
  double follow_path_force_multiplier;

  double arrive_radius;

  bool use_evade_threat_range;
  double evade_threat_range;

  double maximum_turn_rate_per_second;

  int toroid_width;
  int toroid_height;

  int logic;   // 1 - AI BY EXAMPLE, 2 - NATURE OF CODE

public:
  //void _init();

};

#endif
