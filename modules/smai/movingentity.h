#ifndef MOVING_ENTITY_H
#define MOVING_ENTITY_H

#include <cassert>

#include "basegameentity.h"

class MovingEntity : public BaseGameEntity {

protected:

  Vector2D velocity;
  Vector2D heading;
  Vector2D side;

  double mass;
  double max_speed;
  double max_steering_force;
  double max_turn_rate;  // radians per second

public:
  MovingEntity(){};

  MovingEntity(Vector2D _position,
               double _radius,
               Vector2D _velocity,
               double _max_speed,
               Vector2D _heading,
               double _mass,
               Vector2D _scale,
               double _turn_rate,
               double _max_steering_force):
    BaseGameEntity(0, _position, _radius),
    heading(_heading),
    velocity(_velocity),
    mass(_mass),
    side(_heading.Perp()),
    max_speed(_max_speed),
    max_turn_rate(_turn_rate),
    max_steering_force(_max_steering_force)
  {
    scale = _scale;
  }


  Vector2D get_velocity()const {return velocity;}
  void set_velocity(const Vector2D& new_velocity){velocity = new_velocity;}

  void set_mass(double new_mass) {mass=new_mass;}
  double get_mass()const{return mass;}

  Vector2D get_side()const{return side;}

  double get_max_speed()const{return max_speed;}
  void set_max_speed(double new_speed){max_speed = new_speed;}

  double get_max_steering_force()const{return max_steering_force;}
  void set_max_steering_force(double new_max_steering_force){max_steering_force = new_max_steering_force;}

  bool is_speed_maxed_out()const{return max_speed*max_speed >= velocity.LengthSq();}
  double get_speed()const{return velocity.Length();}
  double get_speed_sq()const{return velocity.LengthSq();}

  Vector2D get_heading()const{return heading;}
  void set_heading(Vector2D new_heading) {heading = new_heading;};
  bool rotate_heading_to_face_position(Vector2D target);

  double get_max_turn_rate()const{return max_turn_rate;}
  void set_max_turn_rate(double new_max_turn_rate){max_turn_rate = new_max_turn_rate;}

};


#endif
