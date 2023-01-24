#ifndef AUTONOMOUS_AGENTS_STEERING_BEHAVIOR_GROUP_H
#define AUTONOMOUS_AGENTS_STEERING_BEHAVIOR_GROUP_H

#include "scene/2d/node_2d.h"
#include "autonomous_agents_steering_behavior.h"

class AutonomousAgentsSteeringBehaviorGroup : public Resource {

private:
  GDCLASS(AutonomousAgentsSteeringBehaviorGroup, Resource);

  SteeringBehaviorFlag steering_behavior;

protected:
  static void _bind_methods();

public:

  /*
  void set_behavior(SteeringBehavior behavior, bool is_on);
  void unset_behavior(SteeringBehavior behavior);

  void set_agent_flag(AgentFlag p_agent_flag, bool p_enable);
  bool get_agent_flag(AgentFlag p_agent_flag) const;
  */

  AutonomousAgentsSteeringBehaviorGroup();

};

#endif
