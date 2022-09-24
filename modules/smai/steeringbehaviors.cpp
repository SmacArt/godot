#include "steeringbehaviors.h"
#include "vehicle.h"
#include "common/2d/transformations.h"
#include "common/misc/utils.h"
#include "common/2d/geometry.h"
#include "basegameentity.h"
#include "gameworld.h"
// TODO #include "misc/cellspacepartition.h"
// TODO #include "misc/Stream_Utility_Functions.h"
// TODO #include "EntityFunctionTemplates.h"

#include <cassert>
#include <iostream>


//------------------------- ctor -----------------------------------------
//
//------------------------------------------------------------------------
SteeringBehavior::SteeringBehavior(Vehicle* agent):
  vehicle(agent),
  flags(0),
  minimum_detection_box_length(vehicle->world->parameters->minimum_detection_box_length),
  weight_cohesion(vehicle->world->parameters->cohesion_weight),
  weight_alignment(vehicle->world->parameters->alignment_weight),
  weight_separation(vehicle->world->parameters->separation_weight),
  weight_obstacle_avoidance(vehicle->world->parameters->obstacle_avoidance_weight),
  weight_wander(vehicle->world->parameters->wander_weight),
  weight_wall_avoidance(vehicle->world->parameters->wall_avoidance_weight),
  view_distance(vehicle->world->parameters->view_distance),
  feelers_length(vehicle->world->parameters->wall_detection_feeler_length),
  feelers(3),
  deceleration(NORMAL),
  target_agent_1(NULL),
  target_agent_2(NULL),
  wander_distance(vehicle->world->parameters->wander_distance),
  wander_jitter(vehicle->world->parameters->wander_jitter_per_second),
  wander_radius(vehicle->world->parameters->wander_radius),
  wander_force_multiplier(vehicle->world->parameters->wander_force_multiplier),
  path_waypoint_seek_distance(vehicle->world->parameters->path_waypoint_seek_distance),
  path_waypoint_seek_distance_sq(path_waypoint_seek_distance*path_waypoint_seek_distance),
  path_predict_position(vehicle->world->parameters->path_predict_position),
  follow_path_force_multiplier(vehicle->world->parameters->follow_path_force_multiplier),
  weight_seek(vehicle->world->parameters->seek_weight),
  weight_flee(vehicle->world->parameters->flee_weight),
  weight_arrive(vehicle->world->parameters->arrive_weight),
  weight_pursuit(vehicle->world->parameters->pursuit_weight),
  weight_offset_pursuit(vehicle->world->parameters->offset_pursuit_weight),
  weight_hide(vehicle->world->parameters->hide_weight),
  weight_evade(vehicle->world->parameters->evade_weight),
  weight_interpose(vehicle->world->parameters->interpose_weight),
  weight_follow_path(vehicle->world->parameters->follow_path_weight),
  cell_space_on(false),
  probability_arrive(vehicle->world->parameters->probability_arrive),
  probability_wander(vehicle->world->parameters->probability_wander),
  probability_flee(vehicle->world->parameters->probability_flee),
  probability_seek(vehicle->world->parameters->probability_seek),
  probability_evade(vehicle->world->parameters->probability_evade),
  probability_interpose(vehicle->world->parameters->probability_interpose),
  probability_hide(vehicle->world->parameters->probability_hide),
  probability_obstacle_avoidance(vehicle->world->parameters->probability_obstacle_avoidance),
  probability_wall_avoidance(vehicle->world->parameters->probability_wall_avoidance),
  use_evade_threat_range(vehicle->world->parameters->use_evade_threat_range),
  evade_threat_range(vehicle->world->parameters->evade_threat_range),
  arrive_radius(vehicle->world->parameters->arrive_radius),
  summing_method(PRIORITIZED)
{
}

//---------------------------------dtor ----------------------------------
SteeringBehavior::~SteeringBehavior(){
  delete path;
}


/////////////////////////////////////////////////////////////////////////////// CALCULATE METHODS 


//----------------------- Calculate --------------------------------------
//
//  calculates the accumulated steering force according to the method set
//  in m_SummingMethod
//------------------------------------------------------------------------
Vector2D SteeringBehavior::calculate() {

  //reset the steering force
  steering_force.Zero();

  //use space partitioning to calculate the neighbours of this vehicle
  //if switched on. If not, use the standard tagging system
  if (!is_space_partitioning_on()) {
    //tag neighbors if any of the following 3 group behaviors are switched on
    if (is_on(SEPARATION) || is_on(ALIGNMENT) || is_on(COHESION)) {
      vehicle->world->tag_vehicles_within_view_range(vehicle, view_distance);
    }
  }
  else
  {
    //calculate neighbours in cell-space if any of the following 3 group
    //behaviors are switched on
    if (is_on(SEPARATION) || is_on(ALIGNMENT) || is_on(COHESION)) {
      /* TODO
      vehicle->World()->CellSpace()->CalculateNeighbors(vehicle->get_position(), m_dViewDistance);
      */
    }
  }

  switch (summing_method) {

  case WEIGHTED_AVERAGE:

    // TODO    steering_force = calculate_weighted_sum(); break;

  case PRIORITIZED:

    steering_force = calculate_prioritized(); break;

  case DITHERED:

    steering_force = calculate_dithered();break;

  default:steering_force = Vector2D(0,0);

  }//end switch

  return steering_force;
}


//---------------------- CalculatePrioritized ----------------------------
//
//  this method calls each active steering behavior in order of priority
//  and acumulates their forces until the max steering force magnitude
//  is reached, at which time the function returns the steering force
//  accumulated to that  point
//------------------------------------------------------------------------
Vector2D SteeringBehavior::calculate_prioritized() {

  Vector2D force;

   if (is_on(WALL_AVOIDANCE)) {
    force = wall_avoidance(vehicle->world->walls) * weight_wall_avoidance;
    if (!accumulate_force(steering_force, force)) return steering_force;
  }

  if (is_on(OBSTACLE_AVOIDANCE)) {
    force = obstacle_avoidance(vehicle->world->obstacles) * weight_obstacle_avoidance;
    if (!accumulate_force(steering_force, force)) return steering_force;
  }

  if (is_on(EVADE)) {
    assert(target_agent_1 && "Evade target not assigned");
    force = evade(target_agent_1) * weight_evade;
    if (!accumulate_force(steering_force, force)) return steering_force;
  }

  if (is_on(FLEE)) {
    force = flee(vehicle->world->get_cross_hair()) * weight_flee;
    if (!accumulate_force(steering_force, force)) {
      return steering_force;
    }
  }

  //these next three can be combined for flocking behavior (wander is
  //also a good behavior to add into this mix)
  if (!is_space_partitioning_on()) {
    if (is_on(SEPARATION)) {
      /* TODO
      force = Separation(vehicle->World()->Agents()) * m_dWeightSeparation;

      if (!accumulate_force(steering_force, force)) return steering_force;
      */
    }

    if (is_on(ALIGNMENT)) {
      /* TODO
      force = Alignment(vehicle->World()->Agents()) * m_dWeightAlignment;

      if (!accumulate_force(steering_force, force)) return steering_force;
      */
    }

    if (is_on(COHESION)) {
      /* TODO
      force = Cohesion(vehicle->World()->Agents()) * m_dWeightCohesion;

      if (!accumulate_force(steering_force, force)) return steering_force;
      */
    }
  }
  else
  {

    if (is_on(SEPARATION)) {
      /* TODO
      force = SeparationPlus(vehicle->World()->Agents()) * m_dWeightSeparation;

      if (!accumulate_force(steering_force, force)) return steering_force;
      */
    }

    if (is_on(ALIGNMENT)) {
      /*
      force = AlignmentPlus(vehicle->World()->Agents()) * m_dWeightAlignment;

      if (!accumulate_force(steering_force, force)) return steering_force;
      */
    }

    if (is_on(COHESION)) {
      /* TODO
      force = CohesionPlus(vehicle->World()->Agents()) * m_dWeightCohesion;

      if (!accumulate_force(steering_force, force)) return steering_force;
      */
    }
  }

  if (is_on(SEEK)) {

    force = seek(vehicle->world->get_cross_hair()) * weight_seek;
    if (!accumulate_force(steering_force, force)) {
      return steering_force;
    }
  }


  if (is_on(ARRIVE)) {
    force = arrive(vehicle->world->get_cross_hair(), deceleration) * weight_arrive;
    if (!accumulate_force(steering_force, force)) {
      return steering_force;
    }
  }

  if (is_on(WANDER)) {
    force = wander() * weight_wander;
    if (!accumulate_force(steering_force, force)) return steering_force;
  }


  if (is_on(PURSUIT))
  {
    assert(target_agent_1 && "pursuit target not assigned");
    force = pursuit(target_agent_1) * weight_pursuit;
    if (!accumulate_force(steering_force, force)) return steering_force;
  }
  /*
  if (is_on(offset_pursuit))
  {
    assert (target_agent_1 && "pursuit target not assigned");
    assert (!m_vOffset.isZero() && "No offset assigned");

    force = Offsetpursuit(target_agent_1, m_vOffset);

    if (!accumulate_force(steering_force, force)) return steering_force;
  }
  */

  if (is_on(INTERPOSE))
  {
    assert (target_agent_1 && target_agent_1 && "Interpose agents not assigned");

    force = interpose(target_agent_1, target_agent_2) * weight_obstacle_avoidance;

    if (!accumulate_force(steering_force, force)) return steering_force;
  }

  if (is_on(HIDE))
  {
    assert(target_agent_1 && "Hide target not assigned");

    force = hide(target_agent_1, vehicle->world->obstacles) * weight_hide;

    if (!accumulate_force(steering_force, force)) return steering_force;
  }

  if (is_on(FOLLOW_PATH))
  {
    force = follow_path() * weight_follow_path;

    if (!accumulate_force(steering_force, force)) return steering_force;
  }

  return steering_force;
}

//---------------------- CalculateWeightedSum ----------------------------
//
//  this simply sums up all the active behaviors X their weights and
//  truncates the result to the max available steering force before
//  returning
//------------------------------------------------------------------------
Vector2D SteeringBehavior::calculate_weighted_sum()
{
  if (is_on(WALL_AVOIDANCE)) {
    steering_force += wall_avoidance(vehicle->world->walls) * weight_wall_avoidance;
  }

  if (is_on(OBSTACLE_AVOIDANCE)) {
    steering_force += obstacle_avoidance(vehicle->world->obstacles) *
            weight_obstacle_avoidance;
  }

  if (is_on(EVADE)) {
    assert(target_agent_1 && "Evade target not assigned");
    steering_force += evade(target_agent_1) * weight_evade;
  }

  //these next three can be combined for flocking behavior (wander is
  //also a good behavior to add into this mix)
  if (!is_space_partitioning_on()) {
    if (is_on(SEPARATION)) {
      /*
      steering_force += Separation(vehicle->World()->Agents()) * m_dWeightSeparation;
      */
    }

    if (is_on(ALIGNMENT)) {
      /*
      steering_force += Alignment(vehicle->World()->Agents()) * m_dWeightAlignment;
      */
    }

    if (is_on(COHESION)) {
      /*
      steering_force += Cohesion(vehicle->World()->Agents()) * m_dWeightCohesion;
      */
    }
  }
  else
  {
    if (is_on(SEPARATION)) {
      /*
      steering_force += SeparationPlus(vehicle->World()->Agents()) * m_dWeightSeparation;
      */
    }

    if (is_on(ALIGNMENT)) {
      /*
      steering_force += AlignmentPlus(vehicle->World()->Agents()) * m_dWeightAlignment;
      */
    }

    if (is_on(COHESION)) {
      /*
      steering_force += CohesionPlus(vehicle->World()->Agents()) * m_dWeightCohesion;
      */
    }
  }


  if (is_on(WANDER)) {
    steering_force += wander() * weight_wander;
  }

  if (is_on(SEEK)) {
    steering_force += seek(vehicle->world->get_cross_hair()) * weight_seek;
  }

  if (is_on(FLEE)) {
    steering_force += flee(vehicle->world->get_cross_hair()) * weight_flee;
  }

  if (is_on(ARRIVE)) {
    steering_force += arrive(vehicle->world->get_cross_hair(), deceleration) * weight_arrive;
  }

  if (is_on(PURSUIT)) {
    assert(target_agent_1 && "pursuit target not assigned");
    steering_force += pursuit(target_agent_1) * weight_pursuit;
  }

  if (is_on(OFFSET_PURSUIT)) {
  /*
    assert (target_agent_1 && "pursuit target not assigned");
    assert (!m_vOffset.isZero() && "No offset assigned");

    steering_force += Offsetpursuit(target_agent_1, m_vOffset) * m_dWeightOffsetPursuit;
  */
  }

  if (is_on(INTERPOSE)) {
    assert (target_agent_1 && target_agent_2 && "Interpose agents not assigned");
    steering_force += interpose(target_agent_1, target_agent_2) * weight_interpose;
  }

  if (is_on(HIDE)) {
    assert(target_agent_1 && "Hide target not assigned");
    steering_force += hide(target_agent_1, vehicle->world->obstacles) * weight_hide;
  }

  if (is_on(FOLLOW_PATH)) {
    steering_force += follow_path() * weight_follow_path;
  }

  steering_force.Truncate(vehicle->get_max_steering_force());

  return steering_force;
}


//---------------------- CalculateDithered ----------------------------
//
//  this method sums up the active behaviors by assigning a probabiltyn
//  of being calculated to each behavior. It then tests the first priority
//  to see if it should be calcukated this simulation-step. If so, it
//  calculates the steering force resulting from this behavior. If it is
//  more than zero it returns the force. If zero, or if the behavior is
//  skipped it continues onto the next priority, and so on.
//
//  NOTE: Not all of the behaviors have been implemented in this method,
//        just a few, so you get the general idea
//------------------------------------------------------------------------
Vector2D SteeringBehavior::calculate_dithered() {

  //reset the steering force
  steering_force.Zero();
  if (is_on(WALL_AVOIDANCE) && RandFloat() < probability_wall_avoidance) {
    steering_force = wall_avoidance(vehicle->world->walls) *
                         weight_wall_avoidance / vehicle->world->parameters->probability_wall_avoidance;

    if (!steering_force.isZero())
    {
      steering_force.Truncate(vehicle->get_max_steering_force());

      return steering_force;
    }
  }

  if (is_on(OBSTACLE_AVOIDANCE) && RandFloat() < probability_obstacle_avoidance) {
    steering_force += obstacle_avoidance(vehicle->world->obstacles) *
            weight_obstacle_avoidance / probability_obstacle_avoidance;
    if (!steering_force.isZero())
    {
      steering_force.Truncate(vehicle->get_max_steering_force());
      return steering_force;
    }
  }

  if (!is_space_partitioning_on()) {
  /*
    if (is_on(separation) && RandFloat() < vehicle->world->parameters->probability_.prSeparation)
    {
      steering_force += Separation(vehicle->World()->Agents()) *
                          m_dWeightSeparation / vehicle->world->parameters->probability_.prSeparation;

      if (!steering_force.isZero())
      {
        steering_force.Truncate(vehicle->MaxForce());

        return steering_force;
      }
    }
  */
  } else {
  /*
    if (is_on(separation) && RandFloat() < vehicle->world->parameters->probability_.prSeparation)
    {
      steering_force += SeparationPlus(vehicle->World()->Agents()) *
                          m_dWeightSeparation / vehicle->world->parameters->probability_.prSeparation;

      if (!steering_force.isZero())
      {
        steering_force.Truncate(vehicle->MaxForce());

        return steering_force;
      }
    }
  */
  }


  if (is_on(FLEE) && RandFloat() < probability_flee) {
    steering_force += flee(vehicle->world->get_cross_hair()) * weight_flee / probability_flee;
    if (!steering_force.isZero()) {
      steering_force.Truncate(vehicle->get_max_steering_force());
      return steering_force;
    }
  }


  if (is_on(EVADE) && RandFloat() < probability_evade) {
    assert(target_agent_1 && "Evade target not assigned");
    steering_force += evade(target_agent_1) * weight_evade / probability_evade;
    if (!steering_force.isZero())
    {
      steering_force.Truncate(vehicle->get_max_steering_force());
      return steering_force;
    }
  }


  if (!is_space_partitioning_on()) {
  /*
    if (is_on(allignment) && RandFloat() < vehicle->world->parameters->probability_.prAlignment)  // TODO change from using parameter
    {
      steering_force += Alignment(vehicle->World()->Agents()) *
                          m_dWeightAlignment / vehicle->world->parameters->probability_.prAlignment;

      if (!steering_force.isZero())
      {
        steering_force.Truncate(vehicle->MaxForce());

        return steering_force;
      }
    }

    if (is_on(cohesion) && RandFloat() < vehicle->world->parameters->probability_.prCohesion)  // TODO change from using parameter
    {
      steering_force += Cohesion(vehicle->World()->Agents()) *
                          m_dWeightCohesion / vehicle->world->parameters->probability_.prCohesion;

      if (!steering_force.isZero())
      {
        steering_force.Truncate(vehicle->MaxForce());

        return steering_force;
      }
    }
  */
  } else {
  /*
    if (is_on(allignment) && RandFloat() < vehicle->world->parameters->probability_.prAlignment)  // TODO change from using parameter
    {
      steering_force += AlignmentPlus(vehicle->World()->Agents()) *
                          m_dWeightAlignment / vehicle->world->parameters->probability_.prAlignment;

      if (!steering_force.isZero())
      {
        steering_force.Truncate(vehicle->MaxForce());

        return steering_force;
      }
    }

    if (is_on(cohesion) && RandFloat() < vehicle->world->parameters->probability_.prCohesion)  // TODO change from using parameter
    {
      steering_force += CohesionPlus(vehicle->World()->Agents()) *
                          m_dWeightCohesion / vehicle->world->parameters->probability_.prCohesion;

      if (!steering_force.isZero())
      {
        steering_force.Truncate(vehicle->MaxForce());

        return steering_force;
      }
    }
  */
  }

  if (is_on(WANDER) && RandFloat() < probability_wander) {
    steering_force += wander() * weight_wander / probability_wander;
    if (!steering_force.isZero()) {
      steering_force.Truncate(vehicle->get_max_steering_force());
      return steering_force;
    }
  }

  if (is_on(SEEK) && RandFloat() < probability_seek) {
    steering_force += seek(vehicle->world->get_cross_hair()) * weight_seek / probability_seek;
    if (!steering_force.isZero()) {
      steering_force.Truncate(vehicle->get_max_steering_force());
      return steering_force;
    }
  }

  if (is_on(ARRIVE) && RandFloat() < probability_arrive) {
    steering_force += arrive(vehicle->world->get_cross_hair(), deceleration) * weight_arrive / probability_arrive;
    if (!steering_force.isZero()) {
      steering_force.Truncate(vehicle->get_max_steering_force());
      return steering_force;
    }
  }

  return steering_force;
}



//--------------------- accumulate_force ----------------------------------
//
//  This function calculates how much of its max steering force the
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool SteeringBehavior::accumulate_force(Vector2D &running_total,
                                        Vector2D force_to_add) {

  //calculate how much steering force the vehicle has used so far
  double magnitude_so_far = running_total.Length();

  //calculate how much steering force remains to be used by this vehicle
  double magnitude_remaining = vehicle->get_max_steering_force() - magnitude_so_far;

  //return false if there is no more force left to use
  if (magnitude_remaining <= 0.0) return false;

  //calculate the magnitude of the force we want to add
  double magnitude_to_add = force_to_add.Length();

  //if the magnitude of the sum of ForceToAdd and the running total
  //does not exceed the maximum force available to this vehicle, just
  //add together. Otherwise add as much of the ForceToAdd vector is
  //possible without going over the max.
  if (magnitude_to_add < magnitude_remaining) {
    running_total += force_to_add;
  }
  else
  {
    //add it to the steering force
    running_total += (Vec2DNormalize(force_to_add) * magnitude_remaining);
  }

  return true;
}


//--------------------------- Wander -------------------------------------
//
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
Vector2D SteeringBehavior::wander()
{
  if (logic == NATURE_OF_CODE) {
    return noc_wander();
  }

  //this behavior is dependent on the update rate, so this line must
  //be included when using time independent framerate.
  double jitter_this_time_slice = wander_jitter * vehicle->get_time_elapsed();

  //first, add a small random vector to the target's position
  wander_target += Vector2D(RandomClamped() * jitter_this_time_slice,
                            RandomClamped() * jitter_this_time_slice);


  //reproject this new vector back on to a unit circle
  wander_target.Normalize();

  //increase the length of the vector to the same as the radius
  //of the wander circle
  wander_target *= wander_radius;

  //move the target into a position WanderDist in front of the agent
  Vector2D target = wander_target + Vector2D(wander_distance,0);

  //project the target into world space
  //  Vector2D final_target= Vector2D(vehicle->get_position().x + target.x, vehicle->get_position().y + target.y);  // I changed from PointToWorldSpace
  Vector2D final_target = PointToWorldSpace(target,
                                      vehicle->get_heading(),
                                      vehicle->get_side(),
                                      vehicle->get_position());
  //and steer towards it
  Vector2D return_target = Vector2D(final_target.x-vehicle->get_position().x, final_target.y-vehicle->get_position().y);

  return return_target * wander_force_multiplier;

}


Vector2D SteeringBehavior::noc_wander() {
  wander_theta += RandInRange(-wander_jitter, wander_jitter);

  // Now we have to calculate the new position to steer towards on the wander circle
  Vector2D circle_position = vehicle->get_velocity();
  circle_position.Normalize();
  circle_position *= wander_distance;
  circle_position += vehicle->get_position();

  float h  = vehicle->get_velocity().get_angle_in_radians(); // We need to know the heading to offset wandertheta

  Vector2D circle_offset = Vector2D(wander_radius*cos(wander_theta+h), wander_radius*sin(wander_theta+h));
  Vector2D target = circle_position + circle_offset;
  return seek(target) * wander_force_multiplier;
}

//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent towards the target
//------------------------------------------------------------------------
Vector2D SteeringBehavior::seek(Vector2D target_position)
{
  if (logic == NATURE_OF_CODE) return noc_seek(target_position);

  Vector2D desired_velocity = Vec2DNormalize(target_position - vehicle->get_position()) * vehicle->get_max_speed();
  return (desired_velocity - vehicle->get_velocity());
}

Vector2D SteeringBehavior::noc_seek(Vector2D target_position)
{
  Vector2D desired = target_position - vehicle->get_position();
  desired.SetMagnitude(vehicle->get_max_speed());
  return (desired - vehicle->get_velocity());
}

//------------------------------- Flee -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent away from the target
//------------------------------------------------------------------------
Vector2D SteeringBehavior::flee(Vector2D target_position)
{
  Vector2D desired_velocity = Vec2DNormalize(vehicle->get_position() - target_position) * vehicle->get_max_speed();
  return (desired_velocity - vehicle->get_velocity());
}

//--------------------------- Arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------
Vector2D SteeringBehavior::arrive(Vector2D target_position, Deceleration deceleration)
{
  if (logic==NATURE_OF_CODE) {
    return noc_arrive(target_position);
  }

  Vector2D to_target = target_position - vehicle->get_position();

  //calculate the distance to the target
  double dist = to_target.Length();

  if (dist > 0) {

    if (dist < arrive_radius) {
      //calculate the speed required to reach the target given the desired
      //deceleration
      double speed =  dist / ((double)deceleration * deceleration_tweaker);

      //make sure the velocity does not exceed the max
      speed = fmin(speed, vehicle->get_max_speed());

      //from here proceed just like Seek except we don't need to normalize 
      //the ToTarget vector because we have already gone to the trouble
      //of calculating its length: dist.
      Vector2D desired_velocity =  to_target * speed / dist;


      return (desired_velocity - vehicle->get_velocity());
    } else {
      return seek(target_position);
    }

  }

  return Vector2D(0,0);
}

Vector2D SteeringBehavior::noc_arrive(Vector2D target_position) {
  Vector2D desired = target_position - vehicle->get_position();
  double d = desired.Length();
  if (d < arrive_radius) {
      double m = Map(d,0.0,arrive_radius,0.0,vehicle->get_max_speed(),false);
      desired.SetMagnitude(m);
  } else {
    desired.SetMagnitude(vehicle->get_max_speed());
  }
  return (desired - vehicle->get_velocity());
}

//------------------------------ Pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the 
//  evader
//------------------------------------------------------------------------
Vector2D SteeringBehavior::pursuit(const Vehicle* evader)
{
  //if the evader is ahead and facing the agent then we can just seek
  //for the evader's current position.
  Vector2D to_evader = evader->get_position() - vehicle->get_position();

  double relative_heading = vehicle->get_heading().Dot(evader->get_heading());

  if ( (to_evader.Dot(vehicle->get_heading()) > 0) &&
       (relative_heading < -0.95))  //acos(0.95)=18 degs
  {
    return seek(evader->get_position());
  }

  //Not considered ahead so we predict where the evader will be.

  //the lookahead time is propotional to the distance between the evader
  //and the pursuer; and is inversely proportional to the sum of the
  //agent's velocities
  double look_ahead_time = to_evader.Length() / (vehicle->get_max_speed() + evader->get_speed());

  //now seek to the predicted future position of the evader
  return seek(evader->get_position() + evader->get_velocity() * look_ahead_time);
}

//----------------------------- Evade ------------------------------------
//
//  similar to pursuit except the agent Flees from the estimated future
//  position of the pursuer
//------------------------------------------------------------------------
Vector2D SteeringBehavior::evade(const Vehicle* pursuer)
{
  /* Not necessary to include the check for facing direction this time */

  Vector2D to_pursuer = pursuer->get_position() - vehicle->get_position();

  //uncomment the following two lines to have Evade only consider pursuers
  //within a 'threat range'
  if (use_evade_threat_range && to_pursuer.LengthSq() > evade_threat_range * evade_threat_range) return Vector2D();

  //the lookahead time is propotional to the distance between the pursuer
  //and the pursuer; and is inversely proportional to the sum of the
  //agents' velocities
  double look_ahead_time = to_pursuer.Length() /
                         (vehicle->get_max_speed() + pursuer->get_speed());

  //now flee away from predicted future position of the pursuer
  return flee(pursuer->get_position() + pursuer->get_velocity()* look_ahead_time);
}

//---------------------- ObstacleAvoidance -------------------------------
//
//  Given a vector of obstacles, this method returns a steering force
//  that will prevent the agent colliding with the closest obstacle
//------------------------------------------------------------------------

Vector2D SteeringBehavior::obstacle_avoidance(const std::vector<BaseGameEntity*>& obstacles) {

  //the detection box length is proportional to the agent's velocity
  detection_box_length = minimum_detection_box_length +
    (vehicle->get_speed()/vehicle->get_max_speed()) *
    minimum_detection_box_length;

  //tag all obstacles within range of the box for processing
  vehicle->world->tag_obstacles_within_view_range(vehicle, detection_box_length);

  //this will keep track of the closest intersecting obstacle (CIB)
  BaseGameEntity* closest_interecting_obstacle = NULL;

  //this will be used to track the distance to the CIB
  double distance_to_closest_ip = MaxDouble;

  //this will record the transformed local coordinates of the CIB
  Vector2D local_position_of_closest_obstacle;

  std::vector<BaseGameEntity*>::const_iterator current_object = obstacles.begin();

  while (current_object != obstacles.end()) {

    //if the obstacle has been tagged within range proceed
    if ((*current_object)->is_tagged()) {

      //calculate this obstacle's position in local space
      Vector2D local_position = PointToLocalSpace((*current_object)->get_position(),
                                                  vehicle->get_heading(),
                                                  vehicle->get_side(),
                                                  vehicle->get_position());

      //if the local position has a negative x value then it must lay
      //behind the agent. (in which case it can be ignored)
      if (local_position.x >= 0)
      {
        //if the distance from the x axis to the object's position is less
        //than its radius + half the width of the detection box then there
        //is a potential intersection.
        double expanded_radius = (*current_object)->get_bounding_radius() + vehicle->get_bounding_radius();

        if (fabs(local_position.y) < expanded_radius)
        {
          //now to do a line/circle intersection test. The center of the
          //circle is represented by (cX, cY). The intersection points are
          //given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0.
          //We only need to look at the smallest positive value of x because
          //that will be the closest point of intersection.
          double cX = local_position.x;
          double cY = local_position.y;

          //we only need to calculate the sqrt part of the above equation once

          double sqrt_part = sqrt(expanded_radius*expanded_radius - cY*cY);

          double ip = cX - sqrt_part;

          if (ip <= 0.0)
          {
            ip = cX + sqrt_part;
          }

          //test to see if this is the closest so far. If it is keep a
          //record of the obstacle and its local coordinates
          if (ip < distance_to_closest_ip)
          {
            distance_to_closest_ip = ip;

            closest_interecting_obstacle = *current_object;

            local_position_of_closest_obstacle = local_position;
          }
        }
      }
    }

    ++current_object;
  }

  //if we have found an intersecting obstacle, calculate a steering
  //force away from it
  Vector2D steering_force;

  if (closest_interecting_obstacle)
  {
    //the closer the agent is to an object, the stronger the
    //steering force should be
    double multiplier = 1.0 + (detection_box_length - local_position_of_closest_obstacle.x) /
                        detection_box_length;

    //calculate the lateral force
    steering_force.y = (closest_interecting_obstacle->get_bounding_radius()-
                       local_position_of_closest_obstacle.y)  * multiplier;

    //apply a braking force proportional to the obstacles distance from
    //the vehicle.
    const double braking_weight = 0.2;

    steering_force.x = (closest_interecting_obstacle->get_bounding_radius() -
                       local_position_of_closest_obstacle.x) *
                       braking_weight;
  }

  //finally, convert the steering vector from local to world space
  return VectorToWorldSpace(steering_force,
                            vehicle->get_heading(),
                            vehicle->get_side());

}

//--------------------------- WallAvoidance --------------------------------
//
//  This returns a steering force that will keep the agent away from any
//  walls it may encounter
//------------------------------------------------------------------------
Vector2D SteeringBehavior::wall_avoidance(const std::vector<Wall2D*> walls)
  
{
  //the feelers are contained in a std::vector, feelers
  create_feelers();

  double distance_to_this_ip    = 0.0;
  double distance_to_closest_ip = MaxDouble;

  //this will hold an index into the vector of walls
  int closest_wall = -1;

  Vector2D steering_force,
           point,         //used for storing temporary info
           closest_point;  //holds the closest intersection point

  //examine each feeler in turn
  for (unsigned int flr=0; flr<feelers.size(); ++flr)
  {
    //run through each wall checking for any intersection points
    for (unsigned int w=0; w<walls.size(); ++w)
    {
      if (LineIntersection2D(vehicle->get_position(),
                             feelers[flr],
                             walls[w]->get_from(),
                             walls[w]->get_to(),
                             distance_to_this_ip,
                             point))
      {
        //is this the closest found so far? If so keep a record
        if (distance_to_this_ip < distance_to_closest_ip)
        {
          distance_to_closest_ip = distance_to_this_ip;

          closest_wall = w;

          closest_point = point;
        }
      }
    }//next wall


    //if an intersection point has been detected, calculate a force
    //that will direct the agent away
    if (closest_wall >=0)
    {
      //calculate by what distance the projected position of the agent
      //will overshoot the wall
      Vector2D over_shoot = feelers[flr] - closest_point;

      //create a force in the direction of the wall normal, with a
      //magnitude of the overshoot
      steering_force = walls[closest_wall]->get_normal() * over_shoot.Length();
    }

  }//next feeler

  return steering_force;
}

//------------------------------- CreateFeelers --------------------------
//
//  Creates the antenna utilized by WallAvoidance
//------------------------------------------------------------------------
void SteeringBehavior::create_feelers()
{
  //feeler pointing straight in front
  feelers[0] = vehicle->get_position() + feelers_length * vehicle->get_heading();

  //feeler to left
  Vector2D temp = vehicle->get_heading();
  Vec2DRotateAroundOrigin(temp, HalfPi * 3.5f);
  feelers[1] = vehicle->get_position() + feelers_length/2.0f * temp;

  //feeler to right
  temp = vehicle->get_heading();
  Vec2DRotateAroundOrigin(temp, HalfPi * 0.5f);
  feelers[2] = vehicle->get_position() + feelers_length/2.0f * temp;
}

//--------------------------- Interpose ----------------------------------
//
//  Given two agents, this method returns a force that attempts to
//  position the vehicle between them
//------------------------------------------------------------------------
Vector2D SteeringBehavior::interpose(const Vehicle* agent_a,
                                     const Vehicle* agent_b)
{
  //first we need to figure out where the two agents are going to be at
  //time T in the future. This is approximated by determining the time
  //taken to reach the mid way point at the current time at at max speed.
  Vector2D mid_point = (agent_a->get_position() + agent_b->get_position()) / 2.0;

  double TimeToReachMidPoint = Vec2DDistance(vehicle->get_position(), mid_point) /
                               vehicle->get_max_speed();

  //now we have T, we assume that agent A and agent B will continue on a
  //straight trajectory and extrapolate to get their future positions
  Vector2D APos = agent_a->get_position() + agent_a->get_velocity() * TimeToReachMidPoint;
  Vector2D BPos = agent_b->get_position() + agent_b->get_velocity() * TimeToReachMidPoint;

  //calculate the mid point of these predicted positions
  mid_point = (APos + BPos) / 2.0;

  //then steer to Arrive at it
  return arrive(mid_point, FAST);
}

//--------------------------- Hide ---------------------------------------
//
//------------------------------------------------------------------------
Vector2D SteeringBehavior::hide(const Vehicle* hunter,
                                const std::vector<BaseGameEntity*>& obstacles)
{
  double distance_to_closest = MaxDouble;
  Vector2D best_hiding_spot;

  std::vector<BaseGameEntity*>::const_iterator current_object = obstacles.begin();
  std::vector<BaseGameEntity*>::const_iterator closest;

  while(current_object != obstacles.end())
  {
    //calculate the position of the hiding spot for this obstacle
    Vector2D hiding_spot = get_hiding_position((*current_object)->get_position(),
                                             (*current_object)->get_bounding_radius(),
                                              hunter->get_position());

    //work in distance-squared space to find the closest hiding
    //spot to the agent
    double dist = Vec2DDistanceSq(hiding_spot, vehicle->get_position());

    if (dist < distance_to_closest)
    {
      distance_to_closest = dist;

      best_hiding_spot = hiding_spot;

      closest = current_object;
    }

    ++current_object;

  }//end while

  //if no suitable obstacles found then Evade the hunter
  if (distance_to_closest == MaxFloat)
  {
    return evade(hunter);
  }

  //else use Arrive on the hiding spot
  return arrive(best_hiding_spot, FAST);
}

//------------------------- GetHidingPosition ----------------------------
//
//  Given the position of a hunter, and the position and radius of
//  an obstacle, this method calculates a position DistanceFromBoundary
//  away from its bounding radius and directly opposite the hunter
//------------------------------------------------------------------------
Vector2D SteeringBehavior::get_hiding_position(const Vector2D& position_object,
                                               const double radius_object,
                                               const Vector2D& posHunter)
{
  //calculate how far away the agent is to be from the chosen obstacle's
  //bounding radius
  const double distance_from_boundary = 30.0;
  double distance_away = radius_object + distance_from_boundary;

  //calculate the heading toward the object from the hunter
  Vector2D to_object = Vec2DNormalize(position_object - posHunter);

  //scale it to size and add to the obstacles position to get
  //the hiding spot.
  return (to_object * distance_away) + position_object;
}

//------------------------------- FollowPath -----------------------------
//
//  Given a series of Vector2Ds, this method produces a force that will
//  move the agent along the waypoints in order. The agent uses the
// 'Seek' behavior to move to the next waypoint - unless it is the last
//  waypoint, in which case it 'Arrives'
//------------------------------------------------------------------------
Vector2D SteeringBehavior::follow_path() {

  if (logic == NATURE_OF_CODE) {
    return noc_follow_path();
  }

  //move to next target if close enough to current target (working in
  //distance squared space)
  if(Vec2DDistanceSq(path->get_current_waypoint(path_id), vehicle->get_position()) < path_waypoint_seek_distance_sq) {
    path->set_next_waypoint(path_id);
  }

  if (!path->is_finished(path_id)) {
    return seek(path->get_current_waypoint(path_id)) * follow_path_force_multiplier;
  } else {
    return arrive(path->get_current_waypoint(path_id), NORMAL) * follow_path_force_multiplier;
  }
}

Vector2D SteeringBehavior::noc_follow_path() {

  /*
  if (vehicle->get_velocity().x == 0 && vehicle->get_velocity().y == 0) {
    return seek(path->get_current_waypoint(path_id));
  }
  */

  // Predict position 50 (arbitrary choice) frames ahead
  // This could be based on speed 
  Vector2D predict = vehicle->get_velocity();
  predict.Normalize();
  predict *= path_predict_position;
  Vector2D predict_position = vehicle->get_position() +  predict;

  // Now we must find the normal to the path from the predicted position
  // We look at the normal for each line segment and pick out the closest one
  Vector2D normal;
  Vector2D target;
  double worldRecord = 1000000;  // Start with a very high record distance that can easily be beaten

  // Loop through all points of the path
  for (unsigned int i = 0; i < path->waypoints.size(); i++) {

    // Look at a line segment
    Vector2D a = path->waypoints[i];
    Vector2D b = path->waypoints[i+1];

    // Get the normal point to that line
    Vector2D normal_point = get_normal_point(predict_position, a, b);

    // This only works because we know our path goes from left to right
    // We could have a more sophisticated test to tell if the point is in the line segment or not
    if (normal_point.x < a.x || normal_point.x > b.x) {
      // This is something of a hacky solution, but if it's not within the line segment
      // consider the normal to just be the end of the line segment (point b)
      normal_point = b;
    }

    // How far away are we from the path?
    float distance = Vec2DDistanceSq(predict_position, normal_point);
    // Did we beat the record and find the closest line segment?
    if (distance < worldRecord) {
      worldRecord = distance;
      // If so the target we want to steer towards is the normal
      normal = normal_point;

      // Look at the direction of the line segment so we can seek a little bit ahead of the normal
      Vector2D dir = b - a;
      dir.Normalize();

      // This is an oversimplification
      // Should be based on distance to path & velocity
      dir *= 10;
      target = normal_point;
      target += dir;
    }
  }

  // Only if the distance is greater than the path's radius do we bother to steer
  if (worldRecord > path->get_radius()) {
    seek(target);
  }

  return Vector2D();//temp - remove
}










