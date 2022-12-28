#ifndef AUTONOMOUS_AGENTS_PATH_2D_H
#define AUTONOMOUS_AGENTS_PATH_2D_H

#include "scene/2d/node_2d.h"
#include "scene/resources/curve.h"
#include "autonomous_agents_2d.h"

class Agent;

class AutonomousAgentsPath2D : public Resource {
public:

  enum FollowDirection {
    FOLLOW_DIRECTION_FORWARDS,
    FOLLOW_DIRECTION_BACKWARDS
  };

private:
  GDCLASS(AutonomousAgentsPath2D, Resource);

  Ref<Curve2D> curve;
  double radius;

  int default_follow_direction = FOLLOW_DIRECTION_FORWARDS;
  double default_predict_distance = 50.0;

  PackedVector2Array baked_points_forward;
  PackedVector2Array baked_directions_forward;
  PackedFloat32Array baked_distances_forward;
  PackedFloat32Array baked_total_distances_forward;
  PackedVector2Array baked_directions_backward;
  double path_length = 0;

  bool dirty = true;

  void bake();

protected:
	static void _bind_methods();

public:

	void set_curve(const Ref<Curve2D> &p_curve);
	Ref<Curve2D> get_curve() const;
  void set_radius(const double p_radius);
  double get_radius() const;

  void set_default_follow_direction(const int p_default_follow_direction);
  int get_default_follow_direction() const;
  void set_default_predict_distance(const double p_default_predict_distance);
  double get_default_predict_distance() const;

  PackedVector2Array get_baked_points_forward();
  PackedVector2Array get_baked_directions_forward();
  PackedFloat32Array get_baked_distances_forward();
  PackedFloat32Array get_baked_total_distances_forward();

  AutonomousAgentsPath2D();

};

VARIANT_ENUM_CAST(AutonomousAgentsPath2D::FollowDirection);

class AutonomousAgentsPathNode2D : public Node2D {
public:

private:
  GDCLASS(AutonomousAgentsPathNode2D, Node2D);
  Ref<AutonomousAgentsPath2D> autonomous_agents_path;
  void _path_changed();

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
  virtual Rect2 _edit_get_rect() const override;
  virtual bool _edit_use_rect() const override;
  virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const override;
#endif

  void set_autonomous_agents_path(const Ref<AutonomousAgentsPath2D> &p_path);
  Ref<AutonomousAgentsPath2D> get_autonomous_agents_path() const;

  AutonomousAgentsPathNode2D(){};
};

#endif
