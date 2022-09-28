#include "nativeparameters.h"
#include "common/misc/utils.h"

using namespace godot;

void NativeParameters::_bind_methods(){

  ClassDB::bind_method(D_METHOD("_init"), &NativeParameters::_init);

  ClassDB::bind_method(D_METHOD("get_number_of_agents"), &NativeParameters::get_number_of_agents);
  ClassDB::bind_method(D_METHOD("set_number_of_agents"), &NativeParameters::set_number_of_agents);
  ClassDB::bind_method(D_METHOD("get_vehicle_wander_on"), &NativeParameters::get_vehicle_wander_on);
  ClassDB::bind_method(D_METHOD("set_vehicle_wander_on"), &NativeParameters::set_vehicle_wander_on);
  ClassDB::bind_method(D_METHOD("get_vehicle_seek_on"), &NativeParameters::get_vehicle_seek_on);
  ClassDB::bind_method(D_METHOD("set_vehicle_seek_on"), &NativeParameters::set_vehicle_seek_on);
  ClassDB::bind_method(D_METHOD("get_vehicle_flee_on"), &NativeParameters::get_vehicle_flee_on);
  ClassDB::bind_method(D_METHOD("set_vehicle_flee_on"), &NativeParameters::set_vehicle_flee_on);
  ClassDB::bind_method(D_METHOD("get_vehicle_arrive_on"), &NativeParameters::get_vehicle_arrive_on);
  ClassDB::bind_method(D_METHOD("set_vehicle_arrive_on"), &NativeParameters::set_vehicle_arrive_on);
  ClassDB::bind_method(D_METHOD("get_vehicle_obstacle_avoidance_on"), &NativeParameters::get_vehicle_obstacle_avoidance_on);
  ClassDB::bind_method(D_METHOD("set_vehicle_obstacle_avoidance_on"), &NativeParameters::set_vehicle_obstacle_avoidance_on);
  ClassDB::bind_method(D_METHOD("get_vehicle_wall_avoidance_on"), &NativeParameters::get_vehicle_wall_avoidance_on);
  ClassDB::bind_method(D_METHOD("set_vehicle_wall_avoidance_on"), &NativeParameters::set_vehicle_wall_avoidance_on);
  ClassDB::bind_method(D_METHOD("get_number_of_obstacles"), &NativeParameters::get_number_of_obstacles);
  ClassDB::bind_method(D_METHOD("set_number_of_obstacles"), &NativeParameters::set_number_of_obstacles);
  ClassDB::bind_method(D_METHOD("get_obstacle_minimum_radius"), &NativeParameters::get_obstacle_minimum_radius);
  ClassDB::bind_method(D_METHOD("set_obstacle_minimum_radius"), &NativeParameters::set_obstacle_minimum_radius);
  ClassDB::bind_method(D_METHOD("get_obstacle_maximum_radius"), &NativeParameters::get_obstacle_maximum_radius);
  ClassDB::bind_method(D_METHOD("set_obstacle_maximum_radius"), &NativeParameters::set_obstacle_maximum_radius);

  ClassDB::bind_method(D_METHOD("get_number_of_cells_x"), &NativeParameters::get_number_of_cells_x);
  ClassDB::bind_method(D_METHOD("set_number_of_cells_x"), &NativeParameters::set_number_of_cells_x);
  ClassDB::bind_method(D_METHOD("get_number_of_cells_y"), &NativeParameters::get_number_of_cells_y);
  ClassDB::bind_method(D_METHOD("set_number_of_cells_y"), &NativeParameters::set_number_of_cells_y);

  ClassDB::bind_method(D_METHOD("get_number_of_smoothing_samples"), &NativeParameters::get_number_of_smoothing_samples);
  ClassDB::bind_method(D_METHOD("set_number_of_smoothing_samples"), &NativeParameters::set_number_of_smoothing_samples);

  ClassDB::bind_method(D_METHOD("get_steering_force_tweaker"), &NativeParameters::get_steering_force_tweaker);
  ClassDB::bind_method(D_METHOD("set_steering_force_tweaker"), &NativeParameters::set_steering_force_tweaker);
  ClassDB::bind_method(D_METHOD("get_maximum_steering_force"), &NativeParameters::get_maximum_steering_force);
  ClassDB::bind_method(D_METHOD("set_maximum_steering_force"), &NativeParameters::set_maximum_steering_force);
  ClassDB::bind_method(D_METHOD("get_maximum_speed"), &NativeParameters::get_maximum_speed);
  ClassDB::bind_method(D_METHOD("set_maximum_speed"), &NativeParameters::set_maximum_speed);
  ClassDB::bind_method(D_METHOD("get_vehicle_mass"), &NativeParameters::get_vehicle_mass);
  ClassDB::bind_method(D_METHOD("set_vehicle_mass"), &NativeParameters::set_vehicle_mass);
  ClassDB::bind_method(D_METHOD("get_vehicle_scale"), &NativeParameters::get_vehicle_scale);
  ClassDB::bind_method(D_METHOD("set_vehicle_scale"), &NativeParameters::set_vehicle_scale);
  ClassDB::bind_method(D_METHOD("get_use_scale_from_vehicle"), &NativeParameters::get_use_scale_from_vehicle);
  ClassDB::bind_method(D_METHOD("set_use_scale_from_vehicle"), &NativeParameters::set_use_scale_from_vehicle);

  ClassDB::bind_method(D_METHOD("get_separation_weight"), &NativeParameters::get_separation_weight);
  ClassDB::bind_method(D_METHOD("set_separation_weight"), &NativeParameters::set_separation_weight);
  ClassDB::bind_method(D_METHOD("get_alignment_weight"), &NativeParameters::get_alignment_weight);
  ClassDB::bind_method(D_METHOD("set_alignment_weight"), &NativeParameters::set_alignment_weight);
  ClassDB::bind_method(D_METHOD("get_cohesion_weight"), &NativeParameters::get_cohesion_weight);
  ClassDB::bind_method(D_METHOD("set_cohesion_weight"), &NativeParameters::set_cohesion_weight);
  ClassDB::bind_method(D_METHOD("get_obstacle_avoidance_weight"), &NativeParameters::get_obstacle_avoidance_weight);
  ClassDB::bind_method(D_METHOD("set_obstacle_avoidance_weight"), &NativeParameters::set_obstacle_avoidance_weight);
  ClassDB::bind_method(D_METHOD("get_wall_avoidance_weight"), &NativeParameters::get_wall_avoidance_weight);
  ClassDB::bind_method(D_METHOD("set_wall_avoidance_weight"), &NativeParameters::set_wall_avoidance_weight);
  ClassDB::bind_method(D_METHOD("get_wander_weight"), &NativeParameters::get_wander_weight);
  ClassDB::bind_method(D_METHOD("set_wander_weight"), &NativeParameters::set_wander_weight);
  ClassDB::bind_method(D_METHOD("get_seek_weight"), &NativeParameters::get_seek_weight);
  ClassDB::bind_method(D_METHOD("set_seek_weight"), &NativeParameters::set_seek_weight);
  ClassDB::bind_method(D_METHOD("get_flee_weight"), &NativeParameters::get_flee_weight);
  ClassDB::bind_method(D_METHOD("set_flee_weight"), &NativeParameters::set_flee_weight);
  ClassDB::bind_method(D_METHOD("get_arrive_weight"), &NativeParameters::get_arrive_weight);
  ClassDB::bind_method(D_METHOD("set_arrive_weight"), &NativeParameters::set_arrive_weight);
  ClassDB::bind_method(D_METHOD("get_pursuit_weight"), &NativeParameters::get_pursuit_weight);
  ClassDB::bind_method(D_METHOD("set_pursuit_weight"), &NativeParameters::set_pursuit_weight);
  ClassDB::bind_method(D_METHOD("get_offset_pursuit_weight"), &NativeParameters::get_offset_pursuit_weight);
  ClassDB::bind_method(D_METHOD("set_offset_pursuit_weight"), &NativeParameters::set_offset_pursuit_weight);
  ClassDB::bind_method(D_METHOD("get_interpose_weight"), &NativeParameters::get_interpose_weight);
  ClassDB::bind_method(D_METHOD("set_interpose_weight"), &NativeParameters::set_interpose_weight);
  ClassDB::bind_method(D_METHOD("get_hide_weight"), &NativeParameters::get_hide_weight);
  ClassDB::bind_method(D_METHOD("set_hide_weight"), &NativeParameters::set_hide_weight);
  ClassDB::bind_method(D_METHOD("get_evade_weight"), &NativeParameters::get_evade_weight);
  ClassDB::bind_method(D_METHOD("set_evade_weight"), &NativeParameters::set_evade_weight);
  ClassDB::bind_method(D_METHOD("get_follow_path_weight"), &NativeParameters::get_follow_path_weight);
  ClassDB::bind_method(D_METHOD("set_follow_path_weight"), &NativeParameters::set_follow_path_weight);

  ClassDB::bind_method(D_METHOD("get_view_distance"), &NativeParameters::get_view_distance);
  ClassDB::bind_method(D_METHOD("set_view_distance"), &NativeParameters::set_view_distance);

  ClassDB::bind_method(D_METHOD("get_minimum_detection_box_length"), &NativeParameters::get_minimum_detection_box_length);
  ClassDB::bind_method(D_METHOD("set_minimum_detection_box_length"), &NativeParameters::set_minimum_detection_box_length);
  ClassDB::bind_method(D_METHOD("get_wall_detection_feeler_length"), &NativeParameters::get_wall_detection_feeler_length);
  ClassDB::bind_method(D_METHOD("set_wall_detection_feeler_length"), &NativeParameters::set_wall_detection_feeler_length);

  ClassDB::bind_method(D_METHOD("get_probability_wall_avoidance"), &NativeParameters::get_probability_wall_avoidance);
  ClassDB::bind_method(D_METHOD("set_probability_wall_avoidance"), &NativeParameters::set_probability_wall_avoidance);
  ClassDB::bind_method(D_METHOD("get_probability_obstacle_avoidance"), &NativeParameters::get_probability_obstacle_avoidance);
  ClassDB::bind_method(D_METHOD("set_probability_obstacle_avoidance"), &NativeParameters::set_probability_obstacle_avoidance);
  ClassDB::bind_method(D_METHOD("get_probability_separation"), &NativeParameters::get_probability_separation);
  ClassDB::bind_method(D_METHOD("set_probability_separation"), &NativeParameters::set_probability_separation);
  ClassDB::bind_method(D_METHOD("get_probability_alignment"), &NativeParameters::get_probability_alignment);
  ClassDB::bind_method(D_METHOD("set_probability_alignment"), &NativeParameters::set_probability_alignment);
  ClassDB::bind_method(D_METHOD("get_probability_cohesion"), &NativeParameters::get_probability_cohesion);
  ClassDB::bind_method(D_METHOD("set_probability_cohesion"), &NativeParameters::set_probability_cohesion);
  ClassDB::bind_method(D_METHOD("get_probability_wander"), &NativeParameters::get_probability_wander);
  ClassDB::bind_method(D_METHOD("set_probability_wander"), &NativeParameters::set_probability_wander);
  ClassDB::bind_method(D_METHOD("get_probability_seek"), &NativeParameters::get_probability_seek);
  ClassDB::bind_method(D_METHOD("set_probability_seek"), &NativeParameters::set_probability_seek);
  ClassDB::bind_method(D_METHOD("get_probability_flee"), &NativeParameters::get_probability_flee);
  ClassDB::bind_method(D_METHOD("set_probability_flee"), &NativeParameters::set_probability_flee);
  ClassDB::bind_method(D_METHOD("get_probability_evade"), &NativeParameters::get_probability_evade);
  ClassDB::bind_method(D_METHOD("set_probability_evade"), &NativeParameters::set_probability_evade);
  ClassDB::bind_method(D_METHOD("get_probability_hide"), &NativeParameters::get_probability_hide);
  ClassDB::bind_method(D_METHOD("set_probability_hide"), &NativeParameters::set_probability_hide);
  ClassDB::bind_method(D_METHOD("get_probability_arrive"), &NativeParameters::get_probability_arrive);
  ClassDB::bind_method(D_METHOD("set_probability_arrive"), &NativeParameters::set_probability_arrive);
  ClassDB::bind_method(D_METHOD("get_probability_interpose"), &NativeParameters::get_probability_interpose);
  ClassDB::bind_method(D_METHOD("set_probability_interpose"), &NativeParameters::set_probability_interpose);

  ClassDB::bind_method(D_METHOD("get_maximum_turn_rate_per_second"), &NativeParameters::get_maximum_turn_rate_per_second);
  ClassDB::bind_method(D_METHOD("set_maximum_turn_rate_per_second"), &NativeParameters::set_maximum_turn_rate_per_second);

  ClassDB::bind_method(D_METHOD("get_wander_radius"), &NativeParameters::get_wander_radius);
  ClassDB::bind_method(D_METHOD("set_wander_radius"), &NativeParameters::set_wander_radius);
  ClassDB::bind_method(D_METHOD("get_wander_distance"), &NativeParameters::get_wander_distance);
  ClassDB::bind_method(D_METHOD("set_wander_distance"), &NativeParameters::set_wander_distance);
  ClassDB::bind_method(D_METHOD("get_wander_jitter_per_second"), &NativeParameters::get_wander_jitter_per_second);
  ClassDB::bind_method(D_METHOD("set_wander_jitter_per_second"), &NativeParameters::set_wander_jitter_per_second);
  ClassDB::bind_method(D_METHOD("get_wander_force_multiplier"), &NativeParameters::get_wander_force_multiplier);
  ClassDB::bind_method(D_METHOD("set_wander_force_multiplier"), &NativeParameters::set_wander_force_multiplier);

  ClassDB::bind_method(D_METHOD("get_arrive_radius"), &NativeParameters::get_arrive_radius);
  ClassDB::bind_method(D_METHOD("set_arrive_radius"), &NativeParameters::set_arrive_radius);

  ClassDB::bind_method(D_METHOD("get_use_evade_threat_range"), &NativeParameters::get_use_evade_threat_range);
  ClassDB::bind_method(D_METHOD("set_use_evade_threat_range"), &NativeParameters::set_use_evade_threat_range);
  ClassDB::bind_method(D_METHOD("get_evade_threat_range"), &NativeParameters::get_evade_threat_range);
  ClassDB::bind_method(D_METHOD("set_evade_threat_range"), &NativeParameters::set_evade_threat_range);

  ClassDB::bind_method(D_METHOD("get_path_waypoint_seek_distance"), &NativeParameters::get_path_waypoint_seek_distance);
  ClassDB::bind_method(D_METHOD("set_path_waypoint_seek_distance"), &NativeParameters::set_path_waypoint_seek_distance);
  ClassDB::bind_method(D_METHOD("get_path_predict_position"), &NativeParameters::get_path_predict_position);
  ClassDB::bind_method(D_METHOD("set_path_predict_position"), &NativeParameters::set_path_predict_position);
  ClassDB::bind_method(D_METHOD("get_follow_path_force_multiplier"), &NativeParameters::get_follow_path_force_multiplier);
  ClassDB::bind_method(D_METHOD("set_follow_path_force_multiplier"), &NativeParameters::set_follow_path_force_multiplier);

  ClassDB::bind_method(D_METHOD("get_toroid_width"), &NativeParameters::get_toroid_width);
  ClassDB::bind_method(D_METHOD("set_toroid_width"), &NativeParameters::set_toroid_width);
  ClassDB::bind_method(D_METHOD("get_toroid_height"), &NativeParameters::get_toroid_height);
  ClassDB::bind_method(D_METHOD("set_toroid_height"), &NativeParameters::set_toroid_height);

  ClassDB::bind_method(D_METHOD("get_ai_logic"), &NativeParameters::get_ai_logic);
  ClassDB::bind_method(D_METHOD("set_ai_logic","ai_logic"), &NativeParameters::set_ai_logic);

  ADD_PROPERTY(PropertyInfo(Variant::INT, "number_of_agents"),"set_number_of_agents","get_number_of_agents");
  ADD_PROPERTY_DEFAULT("number_of_agents", 300);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "vehicle_wander_on"),"set_vehicle_wander_on","get_vehicle_wander_on");
  ADD_PROPERTY_DEFAULT("vehicle_wander_on", true);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "vehicle_seek_on"),"set_vehicle_seek_on","get_vehicle_seek_on");
  ADD_PROPERTY_DEFAULT("vehicle_seek_on", false);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "vehicle_arrive_on"),"set_vehicle_arrive_on","get_vehicle_arrive_on");
  ADD_PROPERTY_DEFAULT("vehicle_arrive_on", false);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "vehicle_flee_on"), "set_vehicle_flee_on", "get_vehicle_flee_on");
  ADD_PROPERTY_DEFAULT("vehicle_flee_on", false);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "vehicle_obstacle_avoidance_on"),"set_vehicle_obstacle_avoidance_on","get_vehicle_obstacle_avoidance_on");
  ADD_PROPERTY_DEFAULT("vehicle_obstacle_avoidance_on", false);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "vehicle_wall_avoidance_on"),"set_vehicle_wall_avoidance_on","get_vehicle_wall_avoidance_on");
  ADD_PROPERTY_DEFAULT("vehicle_wall_avoidance_on", false);

  ADD_PROPERTY(PropertyInfo(Variant::INT, "number_of_obstacles"),"set_number_of_obstacles","get_number_of_obstacles");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "obstacle_minimum_radius"),"set_obstacle_minimum_radius","get_obstacle_minimum_radius");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "obstacle_maximum_radius"),"set_obstacle_maximum_radius","get_obstacle_maximum_radius");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "number_of_cells_x"),"set_number_of_cells_x","get_number_of_cells_x");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "number_of_cells_y"),"set_number_of_cells_y","get_number_of_cells_y");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "number_of_smoothing_samples"),"set_number_of_smoothing_samples","get_number_of_smoothing_samples");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "steering_force_tweaker"),"set_steering_force_tweaker","get_steering_force_tweaker");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "maximum_steering_force"),"set_maximum_steering_force","get_maximum_steering_force");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "maximum_speed"),"set_maximum_speed","get_maximum_speed");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "vehicle_mass"),"set_vehicle_mass","get_vehicle_mass");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "vehicle_scale"),"set_vehicle_scale","get_vehicle_scale");
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_scale_from_vehicle"),"set_use_scale_from_vehicle","get_use_scale_from_vehicle");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "separation_weight"),"set_separation_weight","get_separation_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "alignment_weight"),"set_alignment_weight","get_alignment_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cohesion_weight"),"set_cohesion_weight","get_cohesion_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "obstacle_avoidance_weight"),"set_obstacle_avoidance_weight","get_obstacle_avoidance_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wall_avoidance_weight"),"set_wall_avoidance_weight","get_wall_avoidance_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wander_weight"),"set_wander_weight","get_wander_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "seek_weight"),"set_seek_weight","get_seek_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "flee_weight"),"set_flee_weight","get_flee_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "arrive_weight"),"set_arrive_weight","get_arrive_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pursuit_weight"),"set_pursuit_weight","get_pursuit_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "offset_pursuit_weight"),"set_offset_pursuit_weight","get_offset_pursuit_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "interpose_weight"),"set_interpose_weight","get_interpose_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "hide_weight"),"set_hide_weight","get_hide_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "evade_weight"),"set_evade_weight","get_evade_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "follow_path_weight"),"set_follow_path_weight","get_follow_path_weight");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "view_distance"),"set_view_distance","get_view_distance");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "minimum_detection_box_length"),"set_minimum_detection_box_length","get_minimum_detection_box_length");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wall_detection_feeler_length"),"set_wall_detection_feeler_length","get_wall_detection_feeler_length");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_wall_avoidance"),"set_probability_wall_avoidance","get_probability_wall_avoidance");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_obstacle_avoidance"),"set_probability_obstacle_avoidance","get_probability_obstacle_avoidance");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_separation"),"set_probability_separation","get_probability_separation");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_alignment"),"set_probability_alignment","get_probability_alignment");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_cohesion"),"set_probability_cohesion","get_probability_cohesion");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_wander"),"set_probability_wander","get_probability_wander");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_seek"),"set_probability_seek","get_probability_seek");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_flee"),"set_probability_flee","get_probability_flee");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_evade"),"set_probability_evade","get_probability_evade");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_hide"),"set_probability_hide","get_probability_hide");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_arrive"),"set_probability_arrive","get_probability_arrive");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability_interpose"),"set_probability_interpose","get_probability_interpose");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "maximum_turn_rate_per_second"),"set_maximum_turn_rate_per_second","get_maximum_turn_rate_per_second");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wander_radius"),"set_wander_radius","get_wander_radius");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wander_distance"),"set_wander_distance","get_wander_distance");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wander_jitter_per_second"),"set_wander_jitter_per_second","get_wander_jitter_per_second");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wander_force_multiplier"),"set_wander_force_multiplier","get_wander_force_multiplier");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "arrive_radius"),"set_arrive_radius","get_arrive_radius");
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_evade_threat_range"),"set_use_evade_threat_range","get_use_evade_threat_range");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "evade_threat_range"),"set_evade_threat_range","get_evade_threat_range");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "path_waypoint_seek_distance"),"set_path_waypoint_seek_distance","get_path_waypoint_seek_distance");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "path_predict_position"),"set_path_predict_position","get_path_predict_position");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "follow_path_force_multiplier"),"set_follow_path_force_multiplier","get_follow_path_force_multiplier");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "toroid_width"),"set_toroid_width","get_toroid_width");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "toroid_height"),"set_toroid_height","get_toroid_height");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "ai_logic", PROPERTY_HINT_ENUM, "AI By Example,Nature_of_Code"),"set_ai_logic","get_ai_logic");

  /*
    BIND_ENUM_CONSTANT(AILOGIC_AI_BY_EXAMPLE);
    BIND_ENUM_CONSTANT(AILOGIC_NATURE_OF_CODE);
  */
}

void NativeParameters::_init() {

  print_error("INIT HAPPEND!!!!");
    number_of_agents=300;

    vehicle_wander_on = true;
    vehicle_seek_on = false;
    vehicle_flee_on = false;
    vehicle_arrive_on = false;
    vehicle_obstacle_avoidance_on = false;
    vehicle_wall_avoidance_on = false;

    number_of_obstacles=7;
    obstacle_minimum_radius=5;
    obstacle_maximum_radius=100;

    number_of_cells_x=7;
    number_of_cells_y=7;

    number_of_smoothing_samples=10;

    steering_force_tweaker=200;
    maximum_steering_force=100.0;
    maximum_speed=80.0;
    vehicle_mass=0.1;
    use_scale_from_vehicle = false;
    vehicle_scale=1.0;

    separation_weight=1.0;
    alignment_weight=1.0;
    cohesion_weight=1.0;
    obstacle_avoidance_weight=10.0;
    wall_avoidance_weight=10.0;
    wander_weight=1.0;
    seek_weight=1.0;
    flee_weight=1.0;
    arrive_weight=1.0;
    pursuit_weight=1.0;
    offset_pursuit_weight=1.0;
    interpose_weight=1.0;
    hide_weight=1.0;
    evade_weight=0.01;
    follow_path_weight=0.05;

    view_distance=50.0;

    minimum_detection_box_length=50.0;
    wall_detection_feeler_length=40.0;

    probability_wall_avoidance=0.5;
    probability_obstacle_avoidance=0.5;
    probability_separation=0.2;
    probability_alignment=0.3;
    probability_cohesion=0.6;
    probability_wander=0.8;
    probability_seek=0.8;
    probability_flee=0.6;
    probability_evade=1.0;
    probability_hide=0.8;
    probability_arrive=0.5;
    probability_interpose=0.3;

    maximum_turn_rate_per_second=20;

    wander_radius = 10;
    wander_distance = 50.0;
    wander_jitter_per_second = 300.0;
    wander_force_multiplier = 1.0;

    arrive_radius = 30.0;

    use_evade_threat_range = true;
    evade_threat_range = 300;

    path_waypoint_seek_distance = 20;
    path_predict_position = 20;
    follow_path_force_multiplier = 1.0;

    toroid_width=1280;
    toroid_width=720;

    ai_logic = AILOGIC_AI_BY_EXAMPLE;
}
