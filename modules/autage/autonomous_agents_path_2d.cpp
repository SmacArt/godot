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

AutonomousAgentsPath2D::AutonomousAgentsPath2D() {
  set_radius(50.0);
}

void AutonomousAgentsPath2D::set_curve(const Ref<Curve2D> &p_curve) {
  curve = p_curve;
  if (curve != nullptr) {
    bake();
  }
}

Ref<Curve2D> AutonomousAgentsPath2D::get_curve() const {
  return curve;
}

void AutonomousAgentsPath2D::set_radius(const double p_radius) {
  radius = p_radius;
}

double AutonomousAgentsPath2D::get_radius() const {
  return radius;
}

void AutonomousAgentsPath2D::bake() {
  // TODO check if more than one points
  // TODO if not looping don't need to link to the start
  // TODO backwards

  if (dirty && curve != nullptr && curve->get_baked_points().size() > 0) {
    PackedVector2Array points = curve->get_baked_points();

    // determine points for the aa path by removing redundant ones from the curve
    PackedVector2Array aa_points;
    aa_points.resize(points.size());
    Vector2 *aap = aa_points.ptrw();
    number_of_points = 0;
    for (int i = 0; i < points.size(); i++) {
      if (i == 0) {
        aap[0] = points[0];
        number_of_points = 1;
      } else {
        Vector2 direction = (points[i]-points[i-1]).normalized();
        Vector2 next_direction;
        if (i == points.size() - 1) {
          if (points[0].is_equal_approx(points[i])) {
            continue;
          }
          next_direction = (points[0]-points[i]).normalized();
        } else {
          next_direction = (points[i+1]-points[i]).normalized();
        }
        if (!direction.is_equal_approx(next_direction)) {
          number_of_points++;
          aap[number_of_points-1] = points[i];
        }
      }
    }

    baked_points_forward.resize(number_of_points);
    baked_directions_forward.resize(number_of_points);
    baked_distances_forward.resize(number_of_points);
    baked_total_distances_forward.resize(number_of_points);
    Vector2 * bpf = baked_points_forward.ptrw();
    Vector2 * bdirf = baked_directions_forward.ptrw();
    float_t * bdistf = baked_distances_forward.ptrw();
    float_t * btotdistf = baked_total_distances_forward.ptrw();
    path_length = 0;

    bpf[0] = aap[0];
    for (int i = 1; i < number_of_points; i++) {
      bpf[i] = aap[i];
      Vector2 p1 = aap[i-1];
      Vector2 p2 = aap[i];
      bdirf[i-1] = (p2-p1).normalized();
      bdistf[i-1] = (p2-p1).length();
      path_length+= bdistf[i-1];
      btotdistf[i-1] = path_length;
      if (i == number_of_points - 1) {
        bdirf[i] = (aap[0]-p2).normalized();
        bdistf[i] = (aap[0]-p2).length();
        path_length += bdistf[i];
        btotdistf[i] = path_length;
      }
    }
    dirty=false;
  }
}

PackedVector2Array AutonomousAgentsPath2D::get_baked_points_forward() {
  bake();
  return baked_points_forward;
}

PackedVector2Array AutonomousAgentsPath2D::get_baked_directions_forward() {
  bake();
  return baked_directions_forward;
}

PackedFloat32Array AutonomousAgentsPath2D::get_baked_distances_forward() {
  bake();
  return baked_distances_forward;
}

PackedFloat32Array AutonomousAgentsPath2D::get_baked_total_distances_forward() {
  bake();
  return baked_total_distances_forward;
}

int AutonomousAgentsPath2D::get_next_index(const int p_index, const FollowDirection p_direction) const {
  if (p_direction == FOLLOW_DIRECTION_FORWARDS) {
    if (p_index < number_of_points - 1) {
      return p_index + 1;
    } else {
      return 0;
    }
  }
  return 0;
}

Vector2 AutonomousAgentsPath2D::get_next_point(const int p_index, const FollowDirection p_direction) const {
  if (p_direction == FOLLOW_DIRECTION_FORWARDS) {
    if (p_index < number_of_points - 1) {
      return baked_points_forward[p_index + 1];
    } else {
      return baked_points_forward[0];
    }
  }
  return Vector2();
}

int AutonomousAgentsPath2D::get_point_index_for_distance_on_path(const double p_distance, const FollowDirection p_direction) {
  if (p_direction == FOLLOW_DIRECTION_FORWARDS) {
    double clamped_distance = fmod(p_distance, path_length);
    for (int i = 0; i < number_of_points; i++) {
      if (clamped_distance <= baked_total_distances_forward[i]) {
        return i;
      }
    }
  }
  return 0;
}

Vector2 AutonomousAgentsPath2D::get_position_for_distance_from_point(const int p_point_index, const double p_distance, const FollowDirection p_direction) {
  if (p_direction == FOLLOW_DIRECTION_FORWARDS) {
    double clamped_distance = fmax(0,fmod(p_distance, path_length));
    double dist_along_segment = clamped_distance;
    if (p_point_index > 0) {
      dist_along_segment = clamped_distance - baked_total_distances_forward[p_point_index-1];
    }
    return baked_points_forward[p_point_index] + baked_directions_forward[p_point_index] * dist_along_segment;
  }
}

void AutonomousAgentsPath2D::_bind_methods() {

  BIND_ENUM_CONSTANT(FOLLOW_DIRECTION_FORWARDS);
  BIND_ENUM_CONSTANT(FOLLOW_DIRECTION_BACKWARDS);

  ClassDB::bind_method(D_METHOD("set_curve", "curve"), &AutonomousAgentsPath2D::set_curve);
  ClassDB::bind_method(D_METHOD("get_curve"), &AutonomousAgentsPath2D::get_curve);
  ClassDB::bind_method(D_METHOD("set_radius", "curve"), &AutonomousAgentsPath2D::set_radius);
  ClassDB::bind_method(D_METHOD("get_radius"), &AutonomousAgentsPath2D::get_radius);
  ClassDB::bind_method(D_METHOD("get_baked_directions_forward"), &AutonomousAgentsPath2D::get_baked_directions_forward);
  ClassDB::bind_method(D_METHOD("get_baked_distances_forward"), &AutonomousAgentsPath2D::get_baked_distances_forward);
  ClassDB::bind_method(D_METHOD("get_baked_total_distances_forward"), &AutonomousAgentsPath2D::get_baked_total_distances_forward);
  ClassDB::bind_method(D_METHOD("get_baked_points_forward"), &AutonomousAgentsPath2D::get_baked_points_forward);
  ClassDB::bind_method(D_METHOD("get_path_length"), &AutonomousAgentsPath2D::get_path_length);
  ClassDB::bind_method(D_METHOD("get_point_index_for_distance_on_path"), &AutonomousAgentsPath2D::get_point_index_for_distance_on_path);
  ClassDB::bind_method(D_METHOD("get_position_for_distance_from_point"), &AutonomousAgentsPath2D::get_position_for_distance_from_point);
  ClassDB::bind_method(D_METHOD("get_next_index"), &AutonomousAgentsPath2D::get_next_index);
  ClassDB::bind_method(D_METHOD("get_next_point"), &AutonomousAgentsPath2D::get_next_point);

  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius"), "set_radius", "get_radius");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve2D", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT), "set_curve", "get_curve");
}
