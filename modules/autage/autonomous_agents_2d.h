#ifndef AUTONOMOUS_AGENTS_2D_H
#define AUTONOMOUS_AGENTS_2D_H

#include "scene/2d/node_2d.h"

class AutonomousAgents2D : public Node2D {
private:
  GDCLASS(AutonomousAgents2D, Node2D);

public:

private:

  bool running = false;

  struct Agent {
    Transform2D transform;
    Color color;
    real_t rotation = 0.0;
    Vector2 velocity;
    bool active = false;
  };

  RID mesh;
  RID multimesh;

  Vector<Agent> agents;
  Vector<float> agent_data;
  Vector<int> agent_order;

public:
  void set_running(bool p_emitting);
  void set_amount(int p_amount);

  bool is_running() const;
  int get_amount() const;


protected:
  static void _bind_methods();

  AutonomousAgents2D();
  ~AutonomousAgents2D();

};


#endif
