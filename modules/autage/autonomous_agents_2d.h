#ifndef AUTONOMOUS_AGENTS_2D_H
#define AUTONOMOUS_AGENTS_2D_H

#include "scene/2d/node_2d.h"

class AutonomousAgents2D : public Node2D {
private:
  GDCLASS(AutonomousAgents2D, Node2D);

public:
  enum DrawOrder {
    DRAW_ORDER_INDEX,
    DRAW_ORDER_LIFETIME
  };

private:


  struct Agent {
    Transform2D transform;
    Color color;
    real_t rotation = 0.0;
    Vector2 velocity;
    bool active = false;
  };

  bool do_redraw = false;
  double inactive_time = 0.0;
  double frame_remainder = 0.0;
  bool running = false;

  RID mesh;
  RID multimesh;

  Vector<Agent> agents;
  Vector<float> agent_data;
  Vector<int> agent_order;

  double lifetime = 1.0;
  double pre_process_time = 0.0;
  bool local_coords = false;
  DrawOrder draw_order = DRAW_ORDER_INDEX;

  Ref<Texture2D> texture;
  Mutex update_mutex;

  void _update_internal();
  void _update_mesh_texture();
  void _update_render_thread();
  void _set_do_redraw(bool p_do_redraw);
  void _texture_changed();

public:
  void set_amount(int p_amount);
  void set_draw_order(DrawOrder p_order);
  void set_lifetime(double p_lifetime);
  void set_pre_process_time(double p_time);
  void set_running(bool p_emitting);
  void set_texture(const Ref<Texture2D> &p_texture);

  int get_amount() const;
  DrawOrder get_draw_order() const;
  double get_lifetime() const;
  double get_pre_process_time() const;
  bool is_running() const;
  Ref<Texture2D> get_texture() const;

protected:
  static void _bind_methods();
  void _notification(int p_what);
  void _validate_property(PropertyInfo &p_property) const;

  AutonomousAgents2D();
  ~AutonomousAgents2D();

};

VARIANT_ENUM_CAST(AutonomousAgents2D::DrawOrder)

#endif
