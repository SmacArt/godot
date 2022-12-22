#ifndef AUTONOMOUS_AGENTS_PATH_2D_H
#define AUTONOMOUS_AGENTS_PATH_2D_H

#include "scene/2d/node_2d.h"
#include "scene/resources/curve.h"

class AutonomousAgentsPath2D : public Node2D {
private:
  GDCLASS(AutonomousAgentsPath2D, Node2D);
  Ref<Curve2D> curve;
  void _curve_changed();

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

  AutonomousAgentsPath2D() {};
};

#endif
