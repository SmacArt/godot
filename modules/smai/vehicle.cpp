#include "vehicle.h"
#include "common/2d/c2dmatrix.h"
#include "common/2d/geometry.h"
#include "steeringbehaviors.h"
#include "common/2d/transformations.h"
#include "common/misc/utils.h"
#include "iostream"
#include "gameworld.h"
/*
  #include "misc/cellspacepartition.h"
  #include "misc/cgdi.h"
*/

using std::vector;
using std::list;

//--------- ctor ------------
//---------------------------
Vehicle::Vehicle(Vector2D position,
                 double rotation,
                 Vector2D velocity,
                 double mass,
                 double max_steering_force,
                 double max_speed,
                 double max_turn_rate,
                 Vector2D scale,
                 GameWorld* game_world,
                 int logic): MovingEntity(position,
                                          scale.x,
                                          velocity,
                                          max_speed,
                                          calculate_heading_from_rotation(rotation),
                                          mass,
                                          scale,
                                          max_turn_rate,
                                          max_steering_force),
                             smoothed_heading(Vector2D(0,0)),
                             smoothing_on(false),
                             time_elapsed(0.0),
                             world(game_world)
{
  configure(logic);
}

Vehicle::Vehicle() {
}
//--------- dtor ------------
//---------------------------
Vehicle::~Vehicle() {
  delete steering;
  delete heading_smoother;
}

void Vehicle::construct(Vector2D position,
                        double rotation,
                        Vector2D velocity,
                        double mass,
                        double max_steering_force,
                        double max_speed,
                        double max_turn_rate,
                        Vector2D scale,
                        GameWorld* _game_world,
                        int logic
                        ) {
  set_position(position);
  set_heading(calculate_heading_from_rotation(rotation));
  set_velocity(velocity);
  set_mass(mass);
  set_max_steering_force(max_steering_force);
  set_max_speed(max_speed);
  set_max_turn_rate(max_turn_rate);
  set_scale(scale);

  world = _game_world;

  configure(logic);
}

void Vehicle::configure(const int logic) {
  if (world) {
    world->add_vehicle(this);
    steering = new SteeringBehavior(this);
    steering->set_logic(logic);
  }
  // this is only used for smooth rendering --- so until I include it on the GUI Renderer then its not needed
  // heading_smoother = new Smoother<Vector2D>(world->parameters->number_of_smoothing_samples, Vector2D(0.0, 0.0));
}

//------------------------------ Update ----------------------------------
//
//  Updates the vehicle's position from a series of steering behaviors
//------------------------------------------------------------------------
void Vehicle::update(double _time_elapsed)
{

  //update the time elapsed
  time_elapsed = _time_elapsed;

  //keep a record of its old position so we can update its cell later
  //in this method
  Vector2D old_position = get_position();

  //Vector2D steering_force = Vector2D(RandGaussian(100.0, 50.0),RandGaussian(0.0, 100.0));

  //calculate the combined force from each steering behavior in the
  //vehicle's list
  Vector2D steering_force= steering->calculate();

  //Acceleration = Force/Mass
  Vector2D acceleration = steering_force / mass;

  //update velocity
  velocity += acceleration * time_elapsed;

  //make sure vehicle does not exceed maximum velocity
  velocity.Truncate(max_speed);

  //update the position
  position += velocity * time_elapsed;

  //update the heading if the vehicle has a non zero velocity
  if (velocity.LengthSq() > 0.00000001)
  {
    heading = Vec2DNormalize(velocity);
    side = heading.Perp();
  }

  //EnforceNonPenetrationConstraint(this, World()->Agents());

  //treat the screen as a toroid
  if (world && world->parameters) {
    WrapAround(position, world->parameters->toroid_width, world->parameters->toroid_height);
  }

  //update the vehicle's current cell if space partitioning is turned on
  /* TODO
  if (Steering()->isSpacePartitioningOn())
  {
    World()->CellSpace()->UpdateEntity(this, OldPos);
  }
  */
  /*
  if (is_smoothing_on())
  {
    smoothed_heading = heading_smoother->update(get_heading());
  }
  */
}

GameWorld* Vehicle::get_world() {
  return world;
}
