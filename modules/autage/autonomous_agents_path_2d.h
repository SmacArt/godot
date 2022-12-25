#ifndef AUTONOMOUS_AGENTS_PATH_2D_H
#define AUTONOMOUS_AGENTS_PATH_2D_H

#include "scene/2d/node_2d.h"
#include "scene/resources/curve.h"
#include "autonomous_agents_2d.h"

class AutonomousAgentsPath2D : public Node2D {
public:

  enum FollowDirection {
    FOLLOW_DIRECTION_FORWARDS,
    FOLLOW_DIRECTION_BACKWARDS
  };

  struct AgentOnPath {
    const AutonomousAgents2D::Agent *agent = nullptr;
    int follow_direction = FOLLOW_DIRECTION_FORWARDS;
  };

private:
  GDCLASS(AutonomousAgentsPath2D, Node2D);
  Ref<Curve2D> curve;
  void _curve_changed();
  bool is_forward = true;
  Vector<AgentOnPath> agents_on_path;
  AgentOnPath *agents_on_path_arr = nullptr;
  int number_of_agents = 0;
  int add_agent_index = -1;
  int follow_direction = 0;

todo next: determine the lengths between points

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
  virtual Rect2 _edit_get_rect() const override;
  virtual bool _edit_use_rect() const override;
  virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const override;
#endif

  void set_curve(const Ref<Curve2D> &p_curve);
  Ref<Curve2D> get_curve() const;

  void set_number_of_agents(const int p_number_of_agents);
  int get_number_of_agents();

  void set_follow_direction(const int p_follow_direction);
  int get_follow_direction() const;

  void add_agent(const AutonomousAgents2D::Agent *agent);

  AutonomousAgentsPath2D();
};

VARIANT_ENUM_CAST(AutonomousAgentsPath2D::FollowDirection);

#endif
