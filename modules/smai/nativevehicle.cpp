#include "nativevehicle.h"
#include "steeringbehaviors.h"
#include "gameworld.h"

using namespace godot;

void NativeVehicle::_bind_methods(){

  ClassDB::bind_method(D_METHOD("_init"), &NativeVehicle::_init);
  ClassDB::bind_method(D_METHOD("_process"), &NativeVehicle::_process);
  ClassDB::bind_method(D_METHOD("_ready"), &NativeVehicle::_ready);

  ClassDB::bind_method(D_METHOD("get_steering_force"), &NativeVehicle::get_steering_force);

  ClassDB::bind_method(D_METHOD("set_seek_on"), &NativeVehicle::set_seek_on);
  ClassDB::bind_method(D_METHOD("set_flee_on"), &NativeVehicle::set_flee_on);
  ClassDB::bind_method(D_METHOD("set_wander_on"), &NativeVehicle::set_wander_on);
  ClassDB::bind_method(D_METHOD("set_arrive_on"), &NativeVehicle::set_arrive_on);
  ClassDB::bind_method(D_METHOD("set_pursuit_on"), &NativeVehicle::set_pursuit_on);
  ClassDB::bind_method(D_METHOD("set_evade_on"), &NativeVehicle::set_evade_on);
  ClassDB::bind_method(D_METHOD("set_interpose_on"), &NativeVehicle::set_interpose_on);
  ClassDB::bind_method(D_METHOD("set_hide_on"), &NativeVehicle::set_hide_on);
  ClassDB::bind_method(D_METHOD("set_obstacle_avoidance_on"), &NativeVehicle::set_obstacle_avoidance_on);
  ClassDB::bind_method(D_METHOD("set_wall_avoidance_on"), &NativeVehicle::set_wall_avoidance_on);
  ClassDB::bind_method(D_METHOD("set_follow_path_on"), &NativeVehicle::set_follow_path_on);

  ClassDB::bind_method(D_METHOD("set_weight_wander"), &NativeVehicle::set_weight_wander);
  ClassDB::bind_method(D_METHOD("set_weight_arrive"), &NativeVehicle::set_weight_arrive);
  ClassDB::bind_method(D_METHOD("set_weight_flee"), &NativeVehicle::set_weight_flee);
  ClassDB::bind_method(D_METHOD("set_weight_seek"), &NativeVehicle::set_weight_seek);
  ClassDB::bind_method(D_METHOD("set_weight_pursuit"), &NativeVehicle::set_weight_pursuit);
  ClassDB::bind_method(D_METHOD("set_weight_evade"), &NativeVehicle::set_weight_evade);
  ClassDB::bind_method(D_METHOD("set_weight_interpose"), &NativeVehicle::set_weight_interpose);
  ClassDB::bind_method(D_METHOD("set_weight_hide"), &NativeVehicle::set_weight_hide);
  ClassDB::bind_method(D_METHOD("set_weight_obstacle_avoidance"), &NativeVehicle::set_weight_obstacle_avoidance);
  ClassDB::bind_method(D_METHOD("set_weight_wall_avoidance"), &NativeVehicle::set_weight_wall_avoidance);
  ClassDB::bind_method(D_METHOD("set_weight_follow_path"), &NativeVehicle::set_weight_follow_path);

  ClassDB::bind_method(D_METHOD("set_probability_wander"), &NativeVehicle::set_probability_wander);
  ClassDB::bind_method(D_METHOD("set_probability_arrive"), &NativeVehicle::set_probability_arrive);
  ClassDB::bind_method(D_METHOD("set_probability_flee"), &NativeVehicle::set_probability_flee);
  ClassDB::bind_method(D_METHOD("set_probability_seek"), &NativeVehicle::set_probability_seek);
  ClassDB::bind_method(D_METHOD("set_probability_evade"), &NativeVehicle::set_probability_evade);
  ClassDB::bind_method(D_METHOD("set_probability_interpose"), &NativeVehicle::set_probability_interpose);
  ClassDB::bind_method(D_METHOD("set_probability_hide"), &NativeVehicle::set_probability_hide);
  ClassDB::bind_method(D_METHOD("set_probability_obstacle_avoidance"), &NativeVehicle::set_probability_obstacle_avoidance);
  ClassDB::bind_method(D_METHOD("set_probability_wall_avoidance"), &NativeVehicle::set_probability_wall_avoidance);
  ClassDB::bind_method(D_METHOD("set_probability_follow_path"), &NativeVehicle::set_probability_follow_path);

  ClassDB::bind_method(D_METHOD("is_wander_on"), &NativeVehicle::is_wander_on);
  ClassDB::bind_method(D_METHOD("get_wander_radius"), &NativeVehicle::get_wander_radius);
  ClassDB::bind_method(D_METHOD("get_wander_target_position"), &NativeVehicle::get_wander_target_position);
  ClassDB::bind_method(D_METHOD("get_wander_circle_position"), &NativeVehicle::get_wander_circle_position);
  ClassDB::bind_method(D_METHOD("automatically_generated"), &NativeVehicle::automatically_generated);
  ClassDB::bind_method(D_METHOD("is_obstacle_avoidance_on"), &NativeVehicle::is_obstacle_avoidance_on);
  ClassDB::bind_method(D_METHOD("is_wall_avoidance_on"), &NativeVehicle::is_wall_avoidance_on);

  ClassDB::bind_method(D_METHOD("set_wander_jitter"), &NativeVehicle::set_wander_jitter);
  ClassDB::bind_method(D_METHOD("set_wander_radius"), &NativeVehicle::set_wander_radius);
  ClassDB::bind_method(D_METHOD("set_wander_distance"), &NativeVehicle::set_wander_distance);
  ClassDB::bind_method(D_METHOD("set_wander_force_multiplier"), &NativeVehicle::set_wander_force_multiplier);

  ClassDB::bind_method(D_METHOD("set_arrive_radius"), &NativeVehicle::set_arrive_radius);

  ClassDB::bind_method(D_METHOD("set_minimum_detection_box_length"), &NativeVehicle::set_minimum_detection_box_length);
  ClassDB::bind_method(D_METHOD("get_detection_box_length"), &NativeVehicle::get_detection_box_length);
  ClassDB::bind_method(D_METHOD("get_bounding_radius"), &NativeVehicle::get_bounding_radius);

  ClassDB::bind_method(D_METHOD("get_number_of_feelers"), &NativeVehicle::get_number_of_feelers);
  ClassDB::bind_method(D_METHOD("get_feeler"), &NativeVehicle::get_feeler);

  ClassDB::bind_method(D_METHOD("set_deceleration"), &NativeVehicle::set_deceleration);
  ClassDB::bind_method(D_METHOD("set_deceleration_tweaker"), &NativeVehicle::set_deceleration_tweaker);

  ClassDB::bind_method(D_METHOD("set_use_evade_threat_range"), &NativeVehicle::set_use_evade_threat_range);
  ClassDB::bind_method(D_METHOD("set_evade_threat_range"), &NativeVehicle::set_evade_threat_range);

  ClassDB::bind_method(D_METHOD("set_path_waypoint_seek_distance"), &NativeVehicle::set_path_waypoint_seek_distance);
  ClassDB::bind_method(D_METHOD("set_path_predict_position"), &NativeVehicle::set_path_predict_position);
  ClassDB::bind_method(D_METHOD("set_follow_path_force_multiplier"), &NativeVehicle::set_follow_path_force_multiplier);

  ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"mass"), "set_mass", "get_mass");
  ClassDB::bind_method(D_METHOD("get_mass"), &NativeVehicle::get_mass);
  ClassDB::bind_method(D_METHOD("set_mass"), &NativeVehicle::set_mass);

  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_steering_force"),"set_max_steering_force", "get_max_steering_force");
  ClassDB::bind_method(D_METHOD("get_max_steering_force"), &NativeVehicle::get_max_steering_force);
  ClassDB::bind_method(D_METHOD("set_max_steering_force"), &NativeVehicle::set_max_steering_force);

  ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"max_speed"), "set_max_speed", "get_max_speed");
  ClassDB::bind_method(D_METHOD("get_max_speed"), &NativeVehicle::get_max_speed);
  ClassDB::bind_method(D_METHOD("set_max_speed"), &NativeVehicle::set_max_speed);

  ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"max_turn_rate"), "set_max_turn_rate", "get_max_turn_rate");
  ClassDB::bind_method(D_METHOD("get_max_turn_rate"), &NativeVehicle::get_max_turn_rate);
  ClassDB::bind_method(D_METHOD("set_max_turn_rate"), &NativeVehicle::set_max_turn_rate);


  ADD_PROPERTY(PropertyInfo(Variant::VECTOR2,"initial_velocity"), "set_initial_velocity", "get_initial_velocity");
  ClassDB::bind_method(D_METHOD("get_initial_velocity"), &NativeVehicle::get_initial_velocity);
  ClassDB::bind_method(D_METHOD("set_initial_velocity"), &NativeVehicle::set_initial_velocity);

  ADD_PROPERTY(PropertyInfo(Variant::VECTOR2,"scale_factor"), "set_scale_factor", "get_scale_factor");
  ClassDB::bind_method(D_METHOD("get_scale_factor"), &NativeVehicle::get_scale_factor);
  ClassDB::bind_method(D_METHOD("set_scale_factor"), &NativeVehicle::set_scale_factor);

  ADD_PROPERTY(PropertyInfo(Variant::INT, "ai_logic", PROPERTY_HINT_ENUM, "AI By Example,Nature_of_Code"),"set_ai_logic","get_ai_logic");
  ClassDB::bind_method(D_METHOD("get_ai_logic"), &NativeVehicle::get_ai_logic);
  ClassDB::bind_method(D_METHOD("set_ai_logic"), &NativeVehicle::set_ai_logic);

  ClassDB::bind_method(D_METHOD("get_heading"), &NativeVehicle::get_heading);
  ClassDB::bind_method(D_METHOD("get_side"), &NativeVehicle::get_side);

  //temp
  ClassDB::bind_method(D_METHOD("get_temp_final_target"), &NativeVehicle::get_temp_final_target);
  ClassDB::bind_method(D_METHOD("get_temp_returned_wander"), &NativeVehicle::get_temp_returned_wander);
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
                             NativeParameters::AiLogic ai_logic) {

  vehicle = new Vehicle(position,
                        rotation,
                        velocity,
                        mass,
                        max_steering_force,
                        max_speed,
                        max_turn_rate,
                        Vector2D(scale.x * scale_factor.x, scale.y * scale_factor.y),
                        world,
                        ai_logic);
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
  ai_logic = NativeParameters::AILOGIC_AI_BY_EXAMPLE;
}

void NativeVehicle::_ready() {
  if (!is_defined) {
    create_world_vehicle(Vector2D(get_position().x,get_position().y), get_rotation(), Vector2D(get_initial_velocity().x, get_initial_velocity().y), get_mass(), get_max_steering_force(), get_max_speed(), get_max_turn_rate(), Vector2D(get_scale().x,get_scale().y), Vector2D(get_scale_factor().x,get_scale_factor().y),NULL,NativeParameters::AILOGIC_AI_BY_EXAMPLE);
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
                                         NativeParameters::AiLogic ai_logic){

  scale.x = scale.x * scale_factor.x;
  scale.y = scale.y * scale_factor.y;

  vehicle->construct(position, rotation, velocity, mass, max_steering_force, max_speed, max_turn_rate, scale, game_world, ai_logic);
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
