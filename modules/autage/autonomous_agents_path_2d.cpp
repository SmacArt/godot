#include "autonomous_agents_path_2d.h"

#include "core/math/geometry_2d.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_scale.h"
#endif

#ifdef TOOLS_ENABLED
Rect2 AutonomousAgentsPathNode2D::_edit_get_rect() const {
  if (!get_autonomous_agents_path().is_valid()) {
    return Rect2(0, 0, 0, 0);
  }

  Ref<Curve2D> curve = get_autonomous_agents_path()->get_curve();
  if (!curve.is_valid() || curve->get_point_count() == 0) {
    return Rect2(0, 0, 0, 0);
  }

  Rect2 aabb = Rect2(curve->get_point_position(0), Vector2(0, 0));

  for (int i = 0; i < curve->get_point_count(); i++) {
    for (int j = 0; j <= 8; j++) {
      real_t frac = j / 8.0;
      Vector2 p = curve->sample(i, frac);
      aabb.expand_to(p);
    }
  }

  return aabb;
}

bool AutonomousAgentsPathNode2D::_edit_use_rect() const {
  if (!get_autonomous_agents_path().is_valid()) {
    return false;
  }
  Ref<Curve2D> curve = get_autonomous_agents_path()->get_curve();
  return curve.is_valid() && curve->get_point_count() != 0;
}

bool AutonomousAgentsPathNode2D::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
  if (!get_autonomous_agents_path().is_valid()) {
    return false;
  }
  Ref<Curve2D> curve = get_autonomous_agents_path()->get_curve();
  if (curve.is_null()) {
    return false;
  }

  for (int i = 0; i < curve->get_point_count(); i++) {
    Vector2 s[2];
    s[0] = curve->get_point_position(i);

    for (int j = 1; j <= 8; j++) {
      real_t frac = j / 8.0;
      s[1] = curve->sample(i, frac);

      Vector2 p = Geometry2D::get_closest_point_to_segment(p_point, s);
      if (p.distance_to(p_point) <= p_tolerance) {
        return true;
      }

      s[0] = s[1];
    }
  }

  return false;
}
#endif

void AutonomousAgentsPathNode2D::_notification(int p_what) {
  if (!get_autonomous_agents_path().is_valid()) {
    return;
  }
  Ref<Curve2D> curve = get_autonomous_agents_path()->get_curve();
  switch (p_what) {
    // Draw the curve if path debugging is enabled.
  case NOTIFICATION_DRAW: {
    if (!curve.is_valid()) {
      break;
    }

    if (!Engine::get_singleton()->is_editor_hint() && !get_tree()->is_debugging_paths_hint()) {
      return;
    }

    if (curve->get_point_count() < 2) {
      return;
    }

#ifdef TOOLS_ENABLED
    const real_t line_width = get_tree()->get_debug_paths_width() * EDSCALE;
#else
    const real_t line_width = get_tree()->get_debug_paths_width();
#endif
    real_t interval = 10;
    const real_t length = curve->get_baked_length();

    if (length > CMP_EPSILON) {
      const int sample_count = int(length / interval) + 2;
      interval = length / (sample_count - 1); // Recalculate real interval length.

      Vector<Transform2D> frames;
      frames.resize(sample_count);

      {
        Transform2D *w = frames.ptrw();

        for (int i = 0; i < sample_count; i++) {
          w[i] = curve->sample_baked_with_rotation(i * interval, false);
        }
      }

      const Transform2D *r = frames.ptr();
      // Draw curve segments
      {
        PackedVector2Array v2p;
        v2p.resize(sample_count);
        Vector2 *w = v2p.ptrw();

        for (int i = 0; i < sample_count; i++) {
          w[i] = r[i].get_origin();
        }
        draw_polyline(v2p, get_tree()->get_debug_paths_color(), line_width, false);
      }

      // Draw fish bones
      {
        PackedVector2Array v2p;
        v2p.resize(3);
        Vector2 *w = v2p.ptrw();

        for (int i = 0; i < sample_count; i++) {
          const Vector2 p = r[i].get_origin();
          const Vector2 side = r[i].columns[0];
          const Vector2 forward = r[i].columns[1];

          // Fish Bone.
          w[0] = p + (side - forward) * 5;
          w[1] = p;
          w[2] = p + (-side - forward) * 5;

          draw_polyline(v2p, get_tree()->get_debug_paths_color(), line_width * 0.5, false);
        }
      }
    }
  } break;
  }
}

void AutonomousAgentsPathNode2D::_path_changed() {
  queue_redraw();
  if (!is_inside_tree()) {
    return;
  }

  if (!Engine::get_singleton()->is_editor_hint() && !get_tree()->is_debugging_paths_hint()) {
    return;
  }

  queue_redraw();
}

void AutonomousAgentsPathNode2D::set_autonomous_agents_path(const Ref<AutonomousAgentsPath2D> &p_path) {
  if (autonomous_agents_path.is_valid()) {
    autonomous_agents_path->disconnect("changed", callable_mp(this, &AutonomousAgentsPathNode2D::_path_changed));
    if (autonomous_agents_path->get_curve().is_valid()) {
      autonomous_agents_path->get_curve()->disconnect("changed", callable_mp(this, &AutonomousAgentsPathNode2D::_path_changed));
    }
  }

  autonomous_agents_path = p_path;

  if (autonomous_agents_path.is_valid()) {
    autonomous_agents_path->connect("changed", callable_mp(this, &AutonomousAgentsPathNode2D::_path_changed));
    if (autonomous_agents_path->get_curve().is_valid()) {
      autonomous_agents_path->get_curve()->connect("changed", callable_mp(this, &AutonomousAgentsPathNode2D::_path_changed));
    }
  }

  _path_changed();
}

Ref<AutonomousAgentsPath2D> AutonomousAgentsPathNode2D::get_autonomous_agents_path() const {
  return autonomous_agents_path;
}

void AutonomousAgentsPathNode2D::_bind_methods() {

  ClassDB::bind_method(D_METHOD("set_autonomous_agents_path", "path"), &AutonomousAgentsPathNode2D::set_autonomous_agents_path);
  ClassDB::bind_method(D_METHOD("get_autonomous_agents_path"), &AutonomousAgentsPathNode2D::get_autonomous_agents_path);

  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "path", PROPERTY_HINT_RESOURCE_TYPE, "AutonomousAgentsPath2D", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT), "set_autonomous_agents_path", "get_autonomous_agents_path");

}
//
//

void AutonomousAgentsPath2D::set_follow_direction(const int p_follow_direction) {
  follow_direction=p_follow_direction;
}

int AutonomousAgentsPath2D::get_follow_direction() const {
  return follow_direction;
}

void AutonomousAgentsPath2D::set_number_of_agents(const int p_number_of_agents){
  ERR_FAIL_COND_MSG(p_number_of_agents < 1, "Number of agents must be greater than 0.");
  agents_on_path.resize(p_number_of_agents);
  agents_on_path_arr = agents_on_path.ptrw();

  for (int i = 0; i < p_number_of_agents; i++) {
    agents_on_path_arr[i].follow_direction = follow_direction;
  }
}

void AutonomousAgentsPath2D::add_agent(const Agent *agent) {
  add_agent_index++;
  agents_on_path_arr[add_agent_index].agent = agent;
}

int AutonomousAgentsPath2D::get_number_of_agents() {
  return number_of_agents;
}

AutonomousAgentsPath2D::AutonomousAgentsPath2D() {
  set_follow_direction(AutonomousAgentsPath2D::FOLLOW_DIRECTION_FORWARDS);
}

void AutonomousAgentsPath2D::set_curve(const Ref<Curve2D> &p_curve) {
  curve = p_curve;
}

Ref<Curve2D> AutonomousAgentsPath2D::get_curve() const {
  return curve;
}

void AutonomousAgentsPath2D::_bind_methods() {
  BIND_ENUM_CONSTANT(FOLLOW_DIRECTION_FORWARDS);
  BIND_ENUM_CONSTANT(FOLLOW_DIRECTION_BACKWARDS);

  ClassDB::bind_method(D_METHOD("set_curve", "curve"), &AutonomousAgentsPath2D::set_curve);
  ClassDB::bind_method(D_METHOD("get_curve"), &AutonomousAgentsPath2D::get_curve);
  ClassDB::bind_method(D_METHOD("set_follow_direction", "follow_direction"), &AutonomousAgentsPath2D::set_follow_direction);
  ClassDB::bind_method(D_METHOD("get_follow_direction"), &AutonomousAgentsPath2D::get_follow_direction);

  ADD_PROPERTY(PropertyInfo(Variant::INT, "follow_direction", PROPERTY_HINT_ENUM, "Forwards,Backwards"), "set_follow_direction", "get_follow_direction");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve2D", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT), "set_curve", "get_curve");
}
