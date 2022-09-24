#ifndef VEHICLE_H
#define VEHICLE_H

#include "movingentity.h"
#include "common/2d/vector2d.h"
#include "common/misc/smoother.h"

#include <vector>
#include <list>
#include <string>

// these are forward references
class GameWorld;
class SteeringBehavior;

class Vehicle : public MovingEntity {

private:


  Smoother<Vector2D> * heading_smoother;

  Vector2D smoothed_heading;

  bool smoothing_on;

  double time_elapsed;

  Vehicle(const Vehicle&);
  Vehicle& operator=(const Vehicle&);

public:

  GameWorld* world;
  SteeringBehavior* steering;

  Vehicle(Vector2D position,
          double rotation,
          Vector2D velocity,
          double mass,
          double max_steering_force,
          double max_speed,
          double max_turn_rate,
          Vector2D scale,
          GameWorld* world,
          int logic);

  Vehicle();
  ~Vehicle();

  void construct(Vector2D position,
                 double rotation,
                 Vector2D velocity,
                 double mass,
                 double max_steering_force,
                 double max_speed,
                 double max_turn_rate,
                 Vector2D scale,
                 GameWorld* world,
                 int logic);

  void configure(const int logic);

  void update(double time_elapsed);

  GameWorld* get_world();
  void set_game_world(GameWorld* game_world) {world=game_world;}

  Vector2D get_smoothed_heading()const{return smoothed_heading;}

  bool is_smoothing_on() const { return smoothing_on;}
  void turn_smoothing_on() {smoothing_on = true;}
  void turn_smoothing_off() {smoothing_on = false;}
  void toggle_smoothing() {smoothing_on = !smoothing_on;}

  double get_time_elapsed()const{return time_elapsed;}

  Vector2D calculate_heading_from_rotation(const double rotation){
    return Vector2D(sin(rotation),-cos(rotation));
    //return Vector2D(cos(rotation-Pi/2),-sin(rotation-Pi/2));   // i think I am assuming this is going right -- i changed this from original source code
  }
};


#endif
