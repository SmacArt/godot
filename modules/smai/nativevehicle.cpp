#include "nativevehicle.h"
#include "steeringbehaviors.h"
#include "gameworld.h"

using namespace godot;

void NativeVehicle::_register_methods(){
  register_method("_process", &NativeVehicle::_process);
  register_method("_ready", &NativeVehicle::_ready);

  register_method("get_steering_force", &NativeVehicle::get_steering_force);

  register_method("set_seek_on", &NativeVehicle::set_seek_on);
  register_method("set_flee_on", &NativeVehicle::set_flee_on);
  register_method("set_wander_on", &NativeVehicle::set_wander_on);
  register_method("set_arrive_on", &NativeVehicle::set_arrive_on);
  register_method("set_pursuit_on", &NativeVehicle::set_pursuit_on);
  register_method("set_evade_on", &NativeVehicle::set_evade_on);
  register_method("set_interpose_on", &NativeVehicle::set_interpose_on);
  register_method("set_hide_on", &NativeVehicle::set_hide_on);
  register_method("set_obstacle_avoidance_on", &NativeVehicle::set_obstacle_avoidance_on);
  register_method("set_wall_avoidance_on", &NativeVehicle::set_wall_avoidance_on);
  register_method("set_follow_path_on", &NativeVehicle::set_follow_path_on);

  register_method("set_weight_wander", &NativeVehicle::set_weight_wander);
  register_method("set_weight_arrive", &NativeVehicle::set_weight_arrive);
  register_method("set_weight_flee", &NativeVehicle::set_weight_flee);
  register_method("set_weight_seek", &NativeVehicle::set_weight_seek);
  register_method("set_weight_pursuit", &NativeVehicle::set_weight_pursuit);
  register_method("set_weight_evade", &NativeVehicle::set_weight_evade);
  register_method("set_weight_interpose", &NativeVehicle::set_weight_interpose);
  register_method("set_weight_hide", &NativeVehicle::set_weight_hide);
  register_method("set_weight_obstacle_avoidance", &NativeVehicle::set_weight_obstacle_avoidance);
  register_method("set_weight_wall_avoidance", &NativeVehicle::set_weight_wall_avoidance);
  register_method("set_weight_follow_path", &NativeVehicle::set_weight_follow_path);

  register_method("set_probability_wander", &NativeVehicle::set_probability_wander);
  register_method("set_probability_arrive", &NativeVehicle::set_probability_arrive);
  register_method("set_probability_flee", &NativeVehicle::set_probability_flee);
  register_method("set_probability_seek", &NativeVehicle::set_probability_seek);
  register_method("set_probability_evade", &NativeVehicle::set_probability_evade);
  register_method("set_probability_interpose", &NativeVehicle::set_probability_interpose);
  register_method("set_probability_hide", &NativeVehicle::set_probability_hide);
  register_method("set_probability_obstacle_avoidance", &NativeVehicle::set_probability_obstacle_avoidance);
  register_method("set_probability_wall_avoidance", &NativeVehicle::set_probability_wall_avoidance);
  register_method("set_probability_follow_path", &NativeVehicle::set_probability_follow_path);

  register_method("is_wander_on", &NativeVehicle::is_wander_on);
  register_method("get_wander_radius", &NativeVehicle::get_wander_radius);
  register_method("get_wander_target_position", &NativeVehicle::get_wander_target_position);
  register_method("get_wander_circle_position", &NativeVehicle::get_wander_circle_position);
  register_method("automatically_generated", &NativeVehicle::automatically_generated);
  register_method("is_obstacle_avoidance_on", &NativeVehicle::is_obstacle_avoidance_on);
  register_method("is_wall_avoidance_on", &NativeVehicle::is_wall_avoidance_on);

  register_method("set_wander_jitter", &NativeVehicle::set_wander_jitter);
  register_method("set_wander_radius", &NativeVehicle::set_wander_radius);
  register_method("set_wander_distance", &NativeVehicle::set_wander_distance);
  register_method("set_wander_force_multiplier", &NativeVehicle::set_wander_force_multiplier);

  register_method("set_arrive_radius", &NativeVehicle::set_arrive_radius);

  register_method("set_minimum_detection_box_length", &NativeVehicle::set_minimum_detection_box_length);
  register_method("get_minimum_detection_box_length", &NativeVehicle::get_minimum_detection_box_length);
  register_method("get_detection_box_length", &NativeVehicle::get_detection_box_length);
  register_method("get_bounding_radius", &NativeVehicle::get_bounding_radius);

  register_method("get_number_of_feelers", &NativeVehicle::get_number_of_feelers);
  register_method("get_feeler", &NativeVehicle::get_feeler);

  register_method("set_deceleration", &NativeVehicle::set_deceleration);
  register_method("set_deceleration_tweaker", &NativeVehicle::set_deceleration_tweaker);

  register_method("set_use_evade_threat_range", &NativeVehicle::set_use_evade_threat_range);
  register_method("set_evade_threat_range", &NativeVehicle::set_evade_threat_range);

  register_method("set_path_waypoint_seek_distance", &NativeVehicle::set_path_waypoint_seek_distance);
  register_method("set_path_predict_position", &NativeVehicle::set_path_predict_position);
  register_method("set_follow_path_force_multiplier", &NativeVehicle::set_follow_path_force_multiplier);

  register_property<NativeVehicle, double>("mass", &NativeVehicle::mass, 1.0);
  register_property<NativeVehicle, double>("max_steering_force", &NativeVehicle::max_steering_force, 100.0);
  register_property<NativeVehicle, double>("max_speed", &NativeVehicle::max_speed, 150.0);
  register_property<NativeVehicle, double>("max_turn_rate", &NativeVehicle::max_turn_rate, 30.0);
  register_property<NativeVehicle, Vector2>("initial_velocity", &NativeVehicle::initial_velocity, Vector2(0,0));
  register_property<NativeVehicle, Vector2>("scale_factor", &NativeVehicle::scale_factor, Vector2(1,1));
  register_property<NativeVehicle, int>("logic", &NativeVehicle::logic, 1);

  register_method("get_heading", &NativeVehicle::get_heading);
  register_method("get_side", &NativeVehicle::get_side);

  //temp
  register_method("get_temp_final_target", &NativeVehicle::get_temp_final_target);
  register_method("get_temp_returned_wander", &NativeVehicle::get_temp_returned_wander);
}

NativeVehicle::NativeVehicle(Vector2D position,
                             double rotation,
                             Vector2D velocity,
                             double mass,
                             double max_steering_force,
                             double max_speed,
                             double max_turn_rate,
                             Vector2D scale,
                             Vector2D scale_factor,
                             GameWorld* world,
                             int logic) {

  vehicle = new Vehicle(position,
                        rotation,
                        velocity,
                        mass,
                        max_steering_force,
                        max_speed,
                        max_turn_rate,
                        Vector2D(scale.x * scale_factor.x, scale.y * scale_factor.y),
                        world,
                        logic);
  is_defined = true;
}

NativeVehicle::NativeVehicle() {
  vehicle = new Vehicle(Vector2D(0,0),
                        0,
                        Vector2D(0,0),
                        1,
                        100,
                        300,
                        10,
                        Vector2D(3,3),
                        NULL,
                        1);
}

NativeVehicle::~NativeVehicle() {
  // TODO - current causes crash ---->delete vehicle;
}

void NativeVehicle::_init() {
  mass = 1.0;
  max_steering_force = 100.0;
  max_speed = 150.0;
  max_turn_rate = 10.0;
  scale_factor = Vector2(1,1);
  initial_velocity = Vector2(0,0);
  logic = 1;
}

void NativeVehicle::_ready() {
  if (!is_defined) {
    create_world_vehicle(Vector2D(get_position().x,get_position().y), get_rotation(), Vector2D(get_initial_velocity().x, get_initial_velocity().y), get_mass(), get_max_steering_force(), get_max_speed(), get_max_turn_rate(), Vector2D(get_scale().x,get_scale().y), Vector2D(get_scale_factor().x,get_scale_factor().y),NULL,1);
  }
}

void NativeVehicle::_process(float delta) {

  vehicle->update(delta);
  set_global_position(Vector2(vehicle->get_position().x, vehicle->get_position().y)); // TODO this creates a new vector each time -- might be able to use the existing one

  //  std::cout << vehicle->get_heading() << "\n";
  set_global_rotation(vehicle->get_heading().get_angle_in_radians()); // TODO is there a better way than calculating this all the time
}

void NativeVehicle::create_world_vehicle(Vector2D position,
                                         double rotation,
                                         Vector2D velocity,
                                         double mass,
                                         double max_steering_force,
                                         double max_speed,
                                         double max_turn_rate,
                                         Vector2D scale,
                                         Vector2D scale_factor,
                                         GameWorld* game_world,
                                         int logic){

  scale.x = scale.x * scale_factor.x;
  scale.y = scale.y * scale_factor.y;

  vehicle->construct(position, rotation, velocity, mass, max_steering_force, max_speed, max_turn_rate, scale, game_world, logic);
  is_defined=true;
}

void NativeVehicle::set_seek_on() {
  vehicle->steering->seek_on();
}

void NativeVehicle::set_flee_on() {
  vehicle->steering->flee_on();
}

void NativeVehicle::set_wander_on() {
  vehicle->steering->wander_on();
}

void NativeVehicle::set_arrive_on() {
  vehicle->steering->arrive_on();
}


void NativeVehicle::set_interpose_on(const NativeVehicle* target_a, const NativeVehicle* target_b) {
  vehicle->steering->interpose_on(target_a->vehicle, target_b->vehicle);
}

void NativeVehicle::set_pursuit_on(const NativeVehicle* target) {
  vehicle->steering->pursuit_on(target->vehicle);
}

void NativeVehicle::set_evade_on(const NativeVehicle* target) {
  vehicle->steering->evade_on(target->vehicle);
}

void NativeVehicle::set_hide_on(const NativeVehicle* hunter) {
  vehicle->steering->hide_on(hunter->vehicle);
}

void NativeVehicle::set_obstacle_avoidance_on() {
  vehicle->steering->obstacle_avoidance_on();
}

void NativeVehicle::set_wall_avoidance_on() {
  vehicle->steering->wall_avoidance_on();
}

void NativeVehicle::set_follow_path_on(const NativePath* path) {
  vehicle->steering->follow_path_on(path->path);
}

bool NativeVehicle::is_wander_on() {
  return vehicle->steering->is_wander_on();
}

double NativeVehicle::get_wander_radius() {
  return vehicle->steering->get_wander_radius();
}

Vector2 NativeVehicle::get_wander_target_position() {

  Vector2 v = Vector2(vehicle->steering->get_wander_target().x,vehicle->steering->get_wander_target().y);
  v.x = v.x + vehicle->steering->get_wander_distance()*vehicle->get_bounding_radius();
  return v;
}

Vector2 NativeVehicle::get_wander_circle_position() {
  return Vector2(vehicle->steering->get_wander_distance()*vehicle->get_bounding_radius(),0);
}

Vector2 NativeVehicle::get_steering_force() {
  Vector2D steering = vehicle->steering->get_steering_force();
  return Vector2(steering.x,steering.y);
}

bool NativeVehicle::is_obstacle_avoidance_on() {
  return vehicle->steering->is_obstacle_avoidance_on();
}
bool NativeVehicle::is_wall_avoidance_on() {
  return vehicle->steering->is_wall_avoidance_on();
}

void NativeVehicle::set_weight_wander(const double weight) {
  vehicle->steering->set_weight_wander(weight);
}
void NativeVehicle::set_weight_arrive(const double weight) {
  vehicle->steering->set_weight_arrive(weight);
}
void NativeVehicle::set_weight_flee(const double weight) {
  vehicle->steering->set_weight_flee(weight);
}
void NativeVehicle::set_weight_seek(const double weight) {
  vehicle->steering->set_weight_seek(weight);
}
void NativeVehicle::set_weight_pursuit(const double weight) {
  vehicle->steering->set_weight_pursuit(weight);
}
void NativeVehicle::set_weight_evade(const double weight) {
  vehicle->steering->set_weight_evade(weight);
}
void NativeVehicle::set_weight_interpose(const double weight) {
  vehicle->steering->set_weight_interpose(weight);
}
void NativeVehicle::set_weight_hide(const double weight) {
  vehicle->steering->set_weight_hide(weight);
}
void NativeVehicle::set_weight_obstacle_avoidance(const double weight) {
  vehicle->steering->set_weight_obstacle_avoidance(weight);
}

void NativeVehicle::set_weight_wall_avoidance(const double weight) {
  vehicle->steering->set_weight_wall_avoidance(weight);
}

void NativeVehicle::set_weight_follow_path(const double weight) {
  vehicle->steering->set_weight_follow_path(weight);
}

void NativeVehicle::set_probability_wander(const double probability) {
  vehicle->steering->set_probability_wander(probability);
}
void NativeVehicle::set_probability_arrive(const double probability) {
  vehicle->steering->set_probability_arrive(probability);
}
void NativeVehicle::set_probability_flee(const double probability) {
  vehicle->steering->set_probability_flee(probability);
}
void NativeVehicle::set_probability_seek(const double probability) {
  vehicle->steering->set_probability_seek(probability);
}
void NativeVehicle::set_probability_evade(const double probability) {
  vehicle->steering->set_probability_evade(probability);
}
void NativeVehicle::set_probability_interpose(const double probability) {
  vehicle->steering->set_probability_interpose(probability);
}
void NativeVehicle::set_probability_hide(const double probability) {
  vehicle->steering->set_probability_hide(probability);
}
void NativeVehicle::set_probability_obstacle_avoidance(const double probability) {
  vehicle->steering->set_probability_obstacle_avoidance(probability);
}
void NativeVehicle::set_probability_wall_avoidance(const double probability) {
  vehicle->steering->set_probability_wall_avoidance(probability);
}
void NativeVehicle::set_probability_follow_path(const double probability) {
  vehicle->steering->set_probability_follow_path(probability);
}

void NativeVehicle::set_wander_jitter(const double jitter) {
  vehicle->steering->set_wander_jitter(jitter);
}
void NativeVehicle::set_wander_distance(const double distance) {
  vehicle->steering->set_wander_distance(distance);
}
void NativeVehicle::set_wander_radius(const double radius) {
  vehicle->steering->set_wander_radius(radius);
}
void NativeVehicle::set_wander_force_multiplier(const double force_multiplier) {
  vehicle->steering->set_wander_force_multiplier(force_multiplier);
}

void NativeVehicle::set_arrive_radius(const double radius) {
  vehicle->steering->set_arrive_radius(radius);
}

void NativeVehicle::set_deceleration(const int deceleration) {
  vehicle->steering->set_deceleration(deceleration);
}
void NativeVehicle::set_deceleration_tweaker(const double deceleration_tweaker) {
  vehicle->steering->set_deceleration_tweaker(deceleration_tweaker);
}

void NativeVehicle::set_evade_threat_range(const double range){
  vehicle->steering->set_evade_threat_range(range);
}
void NativeVehicle::set_use_evade_threat_range(const bool use){
  vehicle->steering->set_use_evade_threat_range(use);
}

void NativeVehicle::set_path_waypoint_seek_distance(const double distance){
  vehicle->steering->set_path_waypoint_seek_distance(distance);
}

void NativeVehicle::set_path_predict_position(const double predict_position){
  vehicle->steering->set_path_predict_position(predict_position);
}
void NativeVehicle::set_follow_path_force_multiplier(const double force_multiplier) {
  vehicle->steering->set_follow_path_force_multiplier(force_multiplier);
}

Vector2 NativeVehicle::get_heading() {
  return Vector2(vehicle->get_heading().x,vehicle->get_heading().y);
}
Vector2 NativeVehicle::get_side() {
  return Vector2(vehicle->get_side().x,vehicle->get_side().y);
}

Vector2 NativeVehicle::get_temp_returned_wander() {
  return Vector2(vehicle->steering->get_temp_returned_wander().x,vehicle->steering->get_temp_returned_wander().y);
}
Vector2 NativeVehicle::get_temp_final_target() {
  return Vector2(vehicle->steering->get_temp_final_target().x,vehicle->steering->get_temp_final_target().y);
}

void NativeVehicle::set_minimum_detection_box_length(const double length) {
  vehicle->steering->set_minimum_detection_box_length(length);
}
double NativeVehicle::get_minimum_detection_box_length()const {
  return vehicle->steering->get_minimum_detection_box_length();
}

double NativeVehicle::get_detection_box_length()const {
  return vehicle->steering->get_detection_box_length();
}

double NativeVehicle::get_bounding_radius()const {
  return vehicle->get_bounding_radius();
}

int NativeVehicle::get_number_of_feelers() const{
  return vehicle->steering->get_feelers().size();
}

Vector2 NativeVehicle::get_feeler(const int position) const {
  Vector2D feeler = vehicle->steering->get_feelers()[position];
  feeler = PointToLocalSpace(feeler, vehicle->get_heading(), vehicle->get_side(), vehicle->get_position());
  return Vector2(feeler.x, feeler.y);

}
