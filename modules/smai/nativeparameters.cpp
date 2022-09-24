#include "nativeparameters.h"
#include "common/misc/utils.h"

using namespace godot;

void NativeParameters::_register_methods(){
  register_property<NativeParameters, int>("number_of_agents",&NativeParameters::number_of_agents,300);

  register_property<NativeParameters, bool>("vehicle_wander_on",&NativeParameters::vehicle_wander_on,true);
  register_property<NativeParameters, bool>("vehicle_seek_on",&NativeParameters::vehicle_seek_on,false);
  register_property<NativeParameters, bool>("vehicle_flee_on",&NativeParameters::vehicle_flee_on,false);
  register_property<NativeParameters, bool>("vehicle_arrive_on",&NativeParameters::vehicle_arrive_on,false);
  register_property<NativeParameters, bool>("vehicle_obstacle_avoidance_on",&NativeParameters::vehicle_obstacle_avoidance_on,false);
  register_property<NativeParameters, bool>("vehicle_wall_avoidance_on",&NativeParameters::vehicle_wall_avoidance_on,false);

  register_property<NativeParameters, int>("number_of_obstacles",&NativeParameters:: number_of_obstacles,7);
  register_property<NativeParameters, double>("obstacle_minimum_radius",&NativeParameters::obstacle_minimum_radius,5);
  register_property<NativeParameters, double>("obstacle_maximum_radius",&NativeParameters::obstacle_maximum_radius,100);

  register_property<NativeParameters, int>("number_of_cells_x",&NativeParameters:: number_of_cells_x,7);
  register_property<NativeParameters, int>("number_of_cells_y",&NativeParameters:: number_of_cells_y,7);

  register_property<NativeParameters, int>("number_of_smoothing_samples",&NativeParameters:: number_of_smoothing_samples,10);

  register_property<NativeParameters, double>("steering_force_tweaker",&NativeParameters::steering_force_tweaker,200);
  register_property<NativeParameters, double>("maximum_steering_force",&NativeParameters::maximum_steering_force,150.0);
  register_property<NativeParameters, double>("maximum_speed",&NativeParameters::maximum_speed,80.0);
  register_property<NativeParameters, double>("vehicle_mass",&NativeParameters::vehicle_mass,0.1);
  register_property<NativeParameters, double>("vehicle_scale",&NativeParameters::vehicle_scale,1.0);
  register_property<NativeParameters, bool>("use_scale_from_vehicle",&NativeParameters::use_scale_from_vehicle, false);


  register_property<NativeParameters, double>("separation_weight",&NativeParameters::separation_weight,1.0);
  register_property<NativeParameters, double>("alignment_weight",&NativeParameters::alignment_weight,1.0);
  register_property<NativeParameters, double>("cohesion_weight",&NativeParameters::cohesion_weight,1.0);
  register_property<NativeParameters, double>("obstacle_avoidance_weight",&NativeParameters::obstacle_avoidance_weight,10.0);
  register_property<NativeParameters, double>("wall_avoidance_weight",&NativeParameters::wall_avoidance_weight,10.0);
  register_property<NativeParameters, double>("wander_weight",&NativeParameters::wander_weight,1.0);
  register_property<NativeParameters, double>("seek_weight",&NativeParameters::seek_weight,1.0);
  register_property<NativeParameters, double>("flee_weight",&NativeParameters::flee_weight,1.0);
  register_property<NativeParameters, double>("arrive_weight",&NativeParameters::arrive_weight,1.0);
  register_property<NativeParameters, double>("pursuit_weight",&NativeParameters::pursuit_weight,1.0);
  register_property<NativeParameters, double>("offset_pursuit_weight",&NativeParameters::offset_pursuit_weight,1.0);
  register_property<NativeParameters, double>("interpose_weight",&NativeParameters::interpose_weight,1.0);
  register_property<NativeParameters, double>("hide_weight",&NativeParameters::hide_weight,1.0);
  register_property<NativeParameters, double>("evade_weight",&NativeParameters::evade_weight,0.01);
  register_property<NativeParameters, double>("follow_path_weight",&NativeParameters::follow_path_weight,0.05);

  register_property<NativeParameters, double>("view_distance",&NativeParameters::view_distance,50.0);

  register_property<NativeParameters, double>("minimum_detection_box_length",&NativeParameters::minimum_detection_box_length,50.0);
  register_property<NativeParameters, double>("wall_detection_feeler_length",&NativeParameters::wall_detection_feeler_length,40.0);

  register_property<NativeParameters, double>("probability_wall_avoidance",&NativeParameters::probability_wall_avoidance,0.5);
  register_property<NativeParameters, double>("probability_obstacle_avoidance",&NativeParameters::probability_obstacle_avoidance,0.5);
  register_property<NativeParameters, double>("probability_separation",&NativeParameters::probability_separation,0.2);
  register_property<NativeParameters, double>("probability_alignment",&NativeParameters::probability_alignment,0.3);
  register_property<NativeParameters, double>("probability_cohesion",&NativeParameters::probability_cohesion,0.6);
  register_property<NativeParameters, double>("probability_wander",&NativeParameters::probability_wander,0.8);
  register_property<NativeParameters, double>("probability_seek",&NativeParameters::probability_seek,0.8);
  register_property<NativeParameters, double>("probability_flee",&NativeParameters::probability_flee,0.6);
  register_property<NativeParameters, double>("probability_evade",&NativeParameters::probability_evade,1.0);
  register_property<NativeParameters, double>("probability_hide",&NativeParameters::probability_hide,0.8);
  register_property<NativeParameters, double>("probability_arrive",&NativeParameters::probability_arrive,0.5);
  register_property<NativeParameters, double>("probability_interpose",&NativeParameters::probability_interpose,0.3);

  register_property<NativeParameters, double>("maximum_turn_rate_per_second",&NativeParameters::maximum_turn_rate_per_second,20);

  register_property<NativeParameters, double>("wander_radius",&NativeParameters::wander_radius,10);
  register_property<NativeParameters, double>("wander_distance",&NativeParameters::wander_distance,50);
  register_property<NativeParameters, double>("wander_jitter_per_second",&NativeParameters::wander_jitter_per_second,300.0);
  register_property<NativeParameters, double>("wander_force_multiplier",&NativeParameters::wander_force_multiplier,1.0);

  register_property<NativeParameters, double>("arrive_radius",&NativeParameters::arrive_radius,30.0);

  register_property<NativeParameters, bool>("use_evade_threat_range_force",&NativeParameters::use_evade_threat_range,true);
  register_property<NativeParameters, double>("evade_threat_range",&NativeParameters::evade_threat_range,300.0);

  register_property<NativeParameters, double>("path_waypoint_seek_distance",&NativeParameters::path_waypoint_seek_distance,20.0);
  register_property<NativeParameters, double>("path_predict_position",&NativeParameters::path_predict_position,20.0);
  register_property<NativeParameters, double>("follow_path_force_multiplier",&NativeParameters::follow_path_force_multiplier,1.0);

  register_property<NativeParameters, int>("toroid_width",&NativeParameters::toroid_width,1080);
  register_property<NativeParameters, int>("toroid_height",&NativeParameters::toroid_height,720);

  register_property<NativeParameters, int>("logic",&NativeParameters::logic,1);
}

void NativeParameters::_init() {

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

    logic = 1;
}
