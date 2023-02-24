#ifndef AUTONOMOUS_AGENTS_STEERING_BEHAVIOR_H
#define AUTONOMOUS_AGENTS_STEERING_BEHAVIOR_H

enum SteeringBehavior {
  STEERING_BEHAVIOR_ALIGN = 1 << 0,
  STEERING_BEHAVIOR_ARRIVE = 1 << 1,
  STEERING_BEHAVIOR_COLLISION_AVOIDANCE = 1 << 2,
  STEERING_BEHAVIOR_EVADE = 1 << 3,
  STEERING_BEHAVIOR_FACE = 1 << 4,
  STEERING_BEHAVIOR_FLEE = 1 << 5,
  STEERING_BEHAVIOR_FLOCK = 1 << 6,
  STEERING_BEHAVIOR_LOOK_WHERE_YOURE_GOING = 1 << 7,
  STEERING_BEHAVIOR_REMOTELY_CONTROLLED = 1 << 8,
  STEERING_BEHAVIOR_PATH_FOLLOWING = 1 << 9,
  STEERING_BEHAVIOR_PURSUE = 1 << 10,
  STEERING_BEHAVIOR_RACE = 1 << 11,
  STEERING_BEHAVIOR_SEEK = 1 << 12,
  STEERING_BEHAVIOR_SEPARATION = 1 << 13,
  STEERING_BEHAVIOR_VELOCITY_MATCHING = 1 << 14,
  STEERING_BEHAVIOR_WANDER = 1 << 15
};

struct SteeringBehaviorFlag
{
  void set(SteeringBehavior flag)
  {
    value |= (int)flag;
  }

  void unset(SteeringBehavior flag)
  {
    value &= ~(int)flag;
  }

  void flip(SteeringBehavior flag)
  {
    value ^= (int)flag;
  }

  //Check whether a flag is set to true
  inline bool has(SteeringBehavior flag)
  {
    return (value & (int)flag) == (int)flag;
  }

  bool has_any(SteeringBehavior multi_flag)
  {
    return (value & (int)multi_flag) != 0;
  }

  void clear() {
    value = 0;
  }

  uint32_t value = 0;
};


#endif
