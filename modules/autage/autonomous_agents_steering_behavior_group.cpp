#include "autonomous_agents_steering_behavior_group.h"

void AutonomousAgentsSteeringBehaviorGroup::_bind_methods() {
}

/*
  
void AutonomousAgentsSteeringBehaviorGroup::set_behavior(Agent *agent, SteeringBehavior behavior, bool is_on) {
  if (is_on) {
    if (!steering_behavior.has(behavior)) {
      steering_behavior.set(behavior);
      if (behavior & STEERING_BEHAVIOR_ALIGN) {
        setup_agent_with_align(agent);
      }
      if (behavior & STEERING_BEHAVIOR_ARRIVE) {
        setup_agent_with_arrive(agent);
      }
      if (behavior & STEERING_BEHAVIOR_EVADE) {
        setup_agent_with_evade(agent);
      }
      if (behavior & STEERING_BEHAVIOR_FACE) {
        setup_agent_with_face(agent);
      }
      if (behavior & STEERING_BEHAVIOR_LOOK_WHERE_YOURE_GOING) {
        setup_agent_with_look_where_youre_going(agent);
      }
      if (behavior & STEERING_BEHAVIOR_COLLISION_AVOIDANCE) {
        setup_agent_with_collision_avoidance(agent);
      }
      if (behavior & STEERING_BEHAVIOR_PATH_FOLLOWING) {
        setup_agent_with_path_following(agent);
      }
      if (behavior & STEERING_BEHAVIOR_PURSUE) {
        setup_agent_with_pursue(agent);
      }
      if (behavior & STEERING_BEHAVIOR_SEPARATION) {
        setup_agent_with_separation(agent);
      }
      if (behavior & STEERING_BEHAVIOR_VELOCITY_MATCHING) {
        setup_agent_with_velocity_matching(agent);
      }
      if (behavior & STEERING_BEHAVIOR_WANDER) {
        setup_agent_with_wander(agent);
      }
    }
  } else {
    if (steering_behavior.has(behavior)) {
      steering_behavior.unset(behavior);
    }
  }
}
*/

AutonomousAgentsSteeringBehaviorGroup::AutonomousAgentsSteeringBehaviorGroup() {
}
