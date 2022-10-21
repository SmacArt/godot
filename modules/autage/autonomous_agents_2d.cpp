/*************************************************************************/
/*  cpu_agents_2d.cpp                                                    */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "autonomous_agents_2d.h"

#include "core/core_string_names.h"

void AutonomousAgents2D::set_running(bool p_running) {
  if (running == p_running) {
    return;
  }

  running = p_running;
  if (running) {
    set_process_internal(true);
  }
}

void AutonomousAgents2D::set_use_bvh(bool p_use_bvh) {
  use_bvh = p_use_bvh;
}
bool AutonomousAgents2D::is_using_bvh() const {
  return use_bvh;
}

void AutonomousAgents2D::set_behaviour_delay(double p_delay){
  behaviour_delay = p_delay;
}
double AutonomousAgents2D::get_behaviour_delay() const {
  return behaviour_delay;
}

void AutonomousAgents2D::set_number_of_ai_phases(int p_number_of_ai_phases){
  number_of_ai_phases = p_number_of_ai_phases;
}
int AutonomousAgents2D::get_number_of_ai_phases() const {
  return number_of_ai_phases;
}

void AutonomousAgents2D::set_number_of_agents(int p_number_of_agents) {
  ERR_FAIL_COND_MSG(p_number_of_agents < 1, "Number of agents must be greater than 0.");

  agent_bvh.clear();

  agents.resize(p_number_of_agents);
  {
    Agent *w = agents.ptrw();

    for (int i = 0; i < p_number_of_agents; i++) {
      w[i].active = false;
    }
  }

  agent_data.resize((8 + 4 + 4) * p_number_of_agents);
  RS::get_singleton()->multimesh_allocate_data(multimesh, p_number_of_agents, RS::MULTIMESH_TRANSFORM_2D, true, true);

  agent_order.resize(p_number_of_agents);

  agents_arr = agents.ptrw();
}

void AutonomousAgents2D::set_lifetime(double p_lifetime) {
  ERR_FAIL_COND_MSG(p_lifetime <= 0, "Agents lifetime must be greater than 0.");
  lifetime = p_lifetime;
}

void AutonomousAgents2D::set_one_shot(bool p_one_shot) {
  one_shot = p_one_shot;
}

void AutonomousAgents2D::set_pre_process_time(double p_time) {
  pre_process_time = p_time;
}

void AutonomousAgents2D::set_explosiveness_ratio(real_t p_ratio) {
  explosiveness_ratio = p_ratio;
}

void AutonomousAgents2D::set_randomness_ratio(real_t p_ratio) {
  randomness_ratio = p_ratio;
}

void AutonomousAgents2D::set_lifetime_randomness(double p_random) {
  lifetime_randomness = p_random;
}

void AutonomousAgents2D::set_use_local_coordinates(bool p_enable) {
  local_coords = p_enable;
  set_notify_transform(!p_enable);
}

void AutonomousAgents2D::set_speed_scale(double p_scale) {
  speed_scale = p_scale;
}

bool AutonomousAgents2D::is_running() const {
  return running;
}

int AutonomousAgents2D::get_number_of_agents() const {
  return agents.size();
}

double AutonomousAgents2D::get_lifetime() const {
  return lifetime;
}

bool AutonomousAgents2D::get_one_shot() const {
  return one_shot;
}

double AutonomousAgents2D::get_pre_process_time() const {
  return pre_process_time;
}

real_t AutonomousAgents2D::get_explosiveness_ratio() const {
  return explosiveness_ratio;
}

real_t AutonomousAgents2D::get_randomness_ratio() const {
  return randomness_ratio;
}

double AutonomousAgents2D::get_lifetime_randomness() const {
  return lifetime_randomness;
}

bool AutonomousAgents2D::get_use_local_coordinates() const {
  return local_coords;
}

double AutonomousAgents2D::get_speed_scale() const {
  return speed_scale;
}

void AutonomousAgents2D::set_draw_order(DrawOrder p_order) {
  draw_order = p_order;
}

AutonomousAgents2D::DrawOrder AutonomousAgents2D::get_draw_order() const {
  return draw_order;
}

void AutonomousAgents2D::_update_mesh_texture() {
  Size2 tex_size;
  if (texture.is_valid()) {
    tex_size = texture->get_size();
  } else {
    tex_size = Size2(1, 1);
  }

  Vector<Vector2> vertices = {
    -tex_size * 0.5,
    -tex_size * 0.5 + Vector2(tex_size.x, 0),
    -tex_size * 0.5 + tex_size,
    -tex_size * 0.5 + Vector2(0, tex_size.y)
  };

  Vector<Vector2> uvs;
  AtlasTexture *atlas_texure = Object::cast_to<AtlasTexture>(*texture);
  if (atlas_texure && atlas_texure->get_atlas().is_valid()) {
    Rect2 region_rect = atlas_texure->get_region();
    Size2 atlas_size = atlas_texure->get_atlas()->get_size();
    uvs.push_back(Vector2(region_rect.position.x / atlas_size.x, region_rect.position.y / atlas_size.y));
    uvs.push_back(Vector2((region_rect.position.x + region_rect.size.x) / atlas_size.x, region_rect.position.y / atlas_size.y));
    uvs.push_back(Vector2((region_rect.position.x + region_rect.size.x) / atlas_size.x, (region_rect.position.y + region_rect.size.y) / atlas_size.y));
    uvs.push_back(Vector2(region_rect.position.x / atlas_size.x, (region_rect.position.y + region_rect.size.y) / atlas_size.y));
  } else {
    uvs.push_back(Vector2(0, 0));
    uvs.push_back(Vector2(1, 0));
    uvs.push_back(Vector2(1, 1));
    uvs.push_back(Vector2(0, 1));
  }

  Vector<Color> colors = {
    Color(1, 1, 1, 1),
    Color(1, 1, 1, 1),
    Color(1, 1, 1, 1),
    Color(1, 1, 1, 1)
  };

  Vector<int> indices = { 0, 1, 2, 2, 3, 0 };

  Array arr;
  arr.resize(RS::ARRAY_MAX);
  arr[RS::ARRAY_VERTEX] = vertices;
  arr[RS::ARRAY_TEX_UV] = uvs;
  arr[RS::ARRAY_COLOR] = colors;
  arr[RS::ARRAY_INDEX] = indices;

  RS::get_singleton()->mesh_clear(mesh);
  RS::get_singleton()->mesh_add_surface_from_arrays(mesh, RS::PRIMITIVE_TRIANGLES, arr);
}

void AutonomousAgents2D::set_texture(const Ref<Texture2D> &p_texture) {
  if (p_texture == texture) {
    return;
  }

  if (texture.is_valid()) {
    texture->disconnect(CoreStringNames::get_singleton()->changed, callable_mp(this, &AutonomousAgents2D::_texture_changed));
  }

  texture = p_texture;

  if (texture.is_valid()) {
    texture->connect(CoreStringNames::get_singleton()->changed, callable_mp(this, &AutonomousAgents2D::_texture_changed));
  }

  queue_redraw();
  _update_mesh_texture();
}

void AutonomousAgents2D::_texture_changed() {
  if (texture.is_valid()) {
    queue_redraw();
    _update_mesh_texture();
  }
}

Ref<Texture2D> AutonomousAgents2D::get_texture() const {
  return texture;
}

void AutonomousAgents2D::set_fixed_fps(int p_count) {
  fixed_fps = p_count;
}

int AutonomousAgents2D::get_fixed_fps() const {
  return fixed_fps;
}

void AutonomousAgents2D::set_fractional_delta(bool p_enable) {
  fractional_delta = p_enable;
}

bool AutonomousAgents2D::get_fractional_delta() const {
  return fractional_delta;
}

PackedStringArray AutonomousAgents2D::get_configuration_warnings() const {
  PackedStringArray warnings = Node2D::get_configuration_warnings();

  CanvasItemMaterial *mat = Object::cast_to<CanvasItemMaterial>(get_material().ptr());

  if (get_material().is_null() || (mat && !mat->get_particles_animation())) {
    if (get_param_max(PARAM_ANIM_SPEED) != 0.0 || get_param_max(PARAM_ANIM_OFFSET) != 0.0 ||
        get_param_curve(PARAM_ANIM_SPEED).is_valid() || get_param_curve(PARAM_ANIM_OFFSET).is_valid()) {
      warnings.push_back(RTR("AutonomousAgents2D animation requires the usage of a CanvasItemMaterial with \"Agents Animation\" enabled."));
    }
  }

  return warnings;
}

void AutonomousAgents2D::restart() {
  time = 0;
  inactive_time = 0;
  frame_remainder = 0;
  cycle = 0;
  running = false;

  {
    int pc = agents.size();

    for (int i = 0; i < pc; i++) {
      agents_arr[i].active = false;
    }
  }

  set_running(true);
}

void AutonomousAgents2D::set_direction(Vector2 p_direction) {
  direction = p_direction;
}

Vector2 AutonomousAgents2D::get_direction() const {
  return direction;
}

void AutonomousAgents2D::set_spread(real_t p_spread) {
  spread = p_spread;
}

real_t AutonomousAgents2D::get_spread() const {
  return spread;
}

void AutonomousAgents2D::set_param_min(Parameter p_param, real_t p_value) {
  ERR_FAIL_INDEX(p_param, PARAM_MAX);

  parameters_min[p_param] = p_value;
  if (parameters_min[p_param] > parameters_max[p_param]) {
    set_param_max(p_param, p_value);
  }
}

real_t AutonomousAgents2D::get_param_min(Parameter p_param) const {
  ERR_FAIL_INDEX_V(p_param, PARAM_MAX, 0);

  return parameters_min[p_param];
}

void AutonomousAgents2D::set_param_max(Parameter p_param, real_t p_value) {
  ERR_FAIL_INDEX(p_param, PARAM_MAX);

  parameters_max[p_param] = p_value;
  if (parameters_min[p_param] > parameters_max[p_param]) {
    set_param_min(p_param, p_value);
  }

  update_configuration_warnings();
}

real_t AutonomousAgents2D::get_param_max(Parameter p_param) const {
  ERR_FAIL_INDEX_V(p_param, PARAM_MAX, 0);

  return parameters_max[p_param];
}

static void _adjust_curve_range(const Ref<Curve> &p_curve, real_t p_min, real_t p_max) {
  Ref<Curve> curve = p_curve;
  if (!curve.is_valid()) {
    return;
  }

  curve->ensure_default_setup(p_min, p_max);
}

void AutonomousAgents2D::set_param_curve(Parameter p_param, const Ref<Curve> &p_curve) {
  ERR_FAIL_INDEX(p_param, PARAM_MAX);

  curve_parameters[p_param] = p_curve;

  switch (p_param) {
  case PARAM_INITIAL_LINEAR_VELOCITY: {
    //do none for this one
  } break;
  case PARAM_ANGULAR_VELOCITY: {
    _adjust_curve_range(p_curve, -360, 360);
  } break;
  case PARAM_ORBIT_VELOCITY: {
    _adjust_curve_range(p_curve, -500, 500);
  } break;
  case PARAM_LINEAR_ACCEL: {
    _adjust_curve_range(p_curve, -200, 200);
  } break;
  case PARAM_RADIAL_ACCEL: {
    _adjust_curve_range(p_curve, -200, 200);
  } break;
  case PARAM_TANGENTIAL_ACCEL: {
    _adjust_curve_range(p_curve, -200, 200);
  } break;
  case PARAM_DAMPING: {
    _adjust_curve_range(p_curve, 0, 100);
  } break;
  case PARAM_ANGLE: {
    _adjust_curve_range(p_curve, -360, 360);
  } break;
  case PARAM_SCALE: {
  } break;
  case PARAM_HUE_VARIATION: {
    _adjust_curve_range(p_curve, -1, 1);
  } break;
  case PARAM_ANIM_SPEED: {
    _adjust_curve_range(p_curve, 0, 200);
  } break;
  case PARAM_ANIM_OFFSET: {
  } break;
  default: {
  }
  }

  update_configuration_warnings();
}

Ref<Curve> AutonomousAgents2D::get_param_curve(Parameter p_param) const {
  ERR_FAIL_INDEX_V(p_param, PARAM_MAX, Ref<Curve>());

  return curve_parameters[p_param];
}

void AutonomousAgents2D::set_color(const Color &p_color) {
  color = p_color;
}

Color AutonomousAgents2D::get_color() const {
  return color;
}

void AutonomousAgents2D::set_color_ramp(const Ref<Gradient> &p_ramp) {
  color_ramp = p_ramp;
}

Ref<Gradient> AutonomousAgents2D::get_color_ramp() const {
  return color_ramp;
}

void AutonomousAgents2D::set_color_initial_ramp(const Ref<Gradient> &p_ramp) {
  color_initial_ramp = p_ramp;
}

Ref<Gradient> AutonomousAgents2D::get_color_initial_ramp() const {
  return color_initial_ramp;
}

void AutonomousAgents2D::set_agent_flag(AgentFlags p_agent_flag, bool p_enable) {
  ERR_FAIL_INDEX(p_agent_flag, AGENT_FLAG_MAX);
  agent_flags[p_agent_flag] = p_enable;
}

bool AutonomousAgents2D::get_agent_flag(AgentFlags p_agent_flag) const {
  ERR_FAIL_INDEX_V(p_agent_flag, AGENT_FLAG_MAX, false);
  return agent_flags[p_agent_flag];
}

void AutonomousAgents2D::set_emission_shape(EmissionShape p_shape) {
  ERR_FAIL_INDEX(p_shape, EMISSION_SHAPE_MAX);
  emission_shape = p_shape;
  notify_property_list_changed();
}

void AutonomousAgents2D::set_emission_sphere_radius(real_t p_radius) {
  emission_sphere_radius = p_radius;
}

void AutonomousAgents2D::set_emission_rect_extents(Vector2 p_extents) {
  emission_rect_extents = p_extents;
}

void AutonomousAgents2D::set_emission_points(const Vector<Vector2> &p_points) {
  emission_points = p_points;
}

void AutonomousAgents2D::set_emission_normals(const Vector<Vector2> &p_normals) {
  emission_normals = p_normals;
}

void AutonomousAgents2D::set_emission_colors(const Vector<Color> &p_colors) {
  emission_colors = p_colors;
}

real_t AutonomousAgents2D::get_emission_sphere_radius() const {
  return emission_sphere_radius;
}

Vector2 AutonomousAgents2D::get_emission_rect_extents() const {
  return emission_rect_extents;
}

Vector<Vector2> AutonomousAgents2D::get_emission_points() const {
  return emission_points;
}

Vector<Vector2> AutonomousAgents2D::get_emission_normals() const {
  return emission_normals;
}

Vector<Color> AutonomousAgents2D::get_emission_colors() const {
  return emission_colors;
}

AutonomousAgents2D::EmissionShape AutonomousAgents2D::get_emission_shape() const {
  return emission_shape;
}

void AutonomousAgents2D::set_gravity(const Vector2 &p_gravity) {
  gravity = p_gravity;
}

Vector2 AutonomousAgents2D::get_gravity() const {
  return gravity;
}

void AutonomousAgents2D::set_agent_base_size(const Size2 &p_size) {
  agent_base_size = p_size;
}

Size2 AutonomousAgents2D::get_agent_base_size() const {
  return agent_base_size;
}

void AutonomousAgents2D::set_agent_aabb_expansion_ratio(const double p_ratio) {
  agent_aabb_expansion_ratio = p_ratio;
}

double AutonomousAgents2D::get_agent_aabb_expansion_ratio() const {
  return agent_aabb_expansion_ratio;
}

void AutonomousAgents2D::set_scale_curve_x(Ref<Curve> p_scale_curve) {
  scale_curve_x = p_scale_curve;
}

void AutonomousAgents2D::set_scale_curve_y(Ref<Curve> p_scale_curve) {
  scale_curve_y = p_scale_curve;
}

void AutonomousAgents2D::set_split_scale(bool p_split_scale) {
  split_scale = p_split_scale;
  notify_property_list_changed();
}

Ref<Curve> AutonomousAgents2D::get_scale_curve_x() const {
  return scale_curve_x;
}

Ref<Curve> AutonomousAgents2D::get_scale_curve_y() const {
  return scale_curve_y;
}

bool AutonomousAgents2D::get_split_scale() {
  return split_scale;
}

void AutonomousAgents2D::_validate_property(PropertyInfo &p_property) const {
  if (p_property.name == "emission_sphere_radius" && (emission_shape != EMISSION_SHAPE_SPHERE && emission_shape != EMISSION_SHAPE_SPHERE_SURFACE)) {
    p_property.usage = PROPERTY_USAGE_NONE;
  }

  if (p_property.name == "emission_rect_extents" && emission_shape != EMISSION_SHAPE_RECTANGLE) {
    p_property.usage = PROPERTY_USAGE_NONE;
  }

  if ((p_property.name == "emission_point_texture" || p_property.name == "emission_color_texture") && (emission_shape < EMISSION_SHAPE_POINTS)) {
    p_property.usage = PROPERTY_USAGE_NONE;
  }

  if (p_property.name == "emission_normals" && emission_shape != EMISSION_SHAPE_DIRECTED_POINTS) {
    p_property.usage = PROPERTY_USAGE_NONE;
  }

  if (p_property.name == "emission_points" && emission_shape != EMISSION_SHAPE_POINTS && emission_shape != EMISSION_SHAPE_DIRECTED_POINTS) {
    p_property.usage = PROPERTY_USAGE_NONE;
  }

  if (p_property.name == "emission_colors" && emission_shape != EMISSION_SHAPE_POINTS && emission_shape != EMISSION_SHAPE_DIRECTED_POINTS) {
    p_property.usage = PROPERTY_USAGE_NONE;
  }
  if (p_property.name.begins_with("scale_curve_") && !split_scale) {
    p_property.usage = PROPERTY_USAGE_NONE;
  }
}

static uint32_t idhash(uint32_t x) {
  x = ((x >> uint32_t(16)) ^ x) * uint32_t(0x45d9f3b);
  x = ((x >> uint32_t(16)) ^ x) * uint32_t(0x45d9f3b);
  x = (x >> uint32_t(16)) ^ x;
  return x;
}

static real_t rand_from_seed(uint32_t &seed) {
  int k;
  int s = int(seed);
  if (s == 0) {
    s = 305420679;
  }
  k = s / 127773;
  s = 16807 * (s - k * 127773) - 2836 * k;
  if (s < 0) {
    s += 2147483647;
  }
  seed = uint32_t(s);
  return (seed % uint32_t(65536)) / 65535.0;
}

void AutonomousAgents2D::_update_internal() {
  if (agents.size() == 0 || !is_visible_in_tree()) {
    _set_do_redraw(false);
    return;
  }

  double delta = get_process_delta_time();
  if (running) {
    inactive_time = 0;
  } else {
    inactive_time += delta;
    if (inactive_time > lifetime * 1.2) {
      set_process_internal(false);
      _set_do_redraw(false);

      //reset variables
      time = 0;
      inactive_time = 0;
      frame_remainder = 0;
      cycle = 0;
      return;
    }
  }
  _set_do_redraw(true);

  if (time == 0 && pre_process_time > 0.0) {
    double frame_time;
    if (fixed_fps > 0) {
      frame_time = 1.0 / fixed_fps;
    } else {
      frame_time = 1.0 / 30.0;
    }

    double todo = pre_process_time;

    while (todo >= 0) {
      _agents_process(frame_time);
      todo -= frame_time;
    }
  }

  if (fixed_fps > 0) {
    double frame_time = 1.0 / fixed_fps;
    double decr = frame_time;

    double ldelta = delta;
    if (ldelta > 0.1) { //avoid recursive stalls if fps goes below 10
      ldelta = 0.1;
    } else if (ldelta <= 0.0) { //unlikely but..
      ldelta = 0.001;
    }
    double todo = frame_remainder + ldelta;

    while (todo >= frame_time) {
      _agents_process(frame_time);
      todo -= decr;
    }

    frame_remainder = todo;

  } else {
    _agents_process(delta);
  }

  _update_agent_data_buffer();
}

void AutonomousAgents2D:: setup_agent_with_obstacle_avoidance(Agent *agent){
  agent->avoid_obstacles_field_of_view_angle = Math::deg_to_rad(Math::lerp(parameters_min[PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_ANGLE], parameters_max[PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_ANGLE], rand_from_seed(agent->seed)));
  agent->avoid_obstacles_field_of_view_min_distance = parameters_min[PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_DISTANCE];
  agent->avoid_obstacles_field_of_view_max_distance = parameters_max[PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_DISTANCE];
  agent->avoid_obstacles_field_of_view_base_distance = Math::lerp(parameters_min[PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_DISTANCE], parameters_max[PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_DISTANCE], rand_from_seed(agent->seed));
  agent->avoid_obstacles_field_of_view_distance = agent->avoid_obstacles_field_of_view_base_distance;
  agent->avoid_obstacles_field_of_view_offset = Math::lerp(parameters_min[PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_OFFSET], parameters_max[PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_OFFSET], rand_from_seed(agent->seed));
  agent->avoid_obstacles_field_of_view_base_offset = agent->avoid_obstacles_field_of_view_offset;
  agent->avoid_obstacles_decay_coefficient = Math::lerp(parameters_min[PARAM_OBSTACLE_AVOIDANCE_DECAY_COEFFICIENT], parameters_max[PARAM_OBSTACLE_AVOIDANCE_DECAY_COEFFICIENT], rand_from_seed(agent->seed));
  agent->avoid_obstacles_fov_scale_to_size = agent_flags[AGENT_FLAG_OBSTACLE_AVOIDANCE_FOV_SCALE_TO_SIZE];
}

void AutonomousAgents2D::setup_agent_with_arrive(Agent *agent){
  agent->arrive_target_radius = Math::lerp(parameters_min[PARAM_ARRIVE_TARGET_RADIUS], parameters_max[PARAM_ARRIVE_TARGET_RADIUS], rand_from_seed(agent->seed));
  agent->arrive_slow_radius = Math::lerp(parameters_min[PARAM_ARRIVE_SLOW_RADIUS], parameters_max[PARAM_ARRIVE_SLOW_RADIUS], rand_from_seed(agent->seed));
  agent->arrive_time_to_target = Math::lerp(parameters_min[PARAM_ARRIVE_TIME_TO_TARGET], parameters_max[PARAM_ARRIVE_TIME_TO_TARGET], rand_from_seed(agent->seed));
}

void AutonomousAgents2D::setup_agent_with_separate(Agent *agent){
  agent->separate_neighbourhood_expansion = Math::lerp(parameters_min[PARAM_SEPARATE_NEIGHBOURHOOD_EXPANSION], parameters_max[PARAM_SEPARATE_NEIGHBOURHOOD_EXPANSION], rand_from_seed(agent->seed));
  agent->separate_decay_coefficient = Math::lerp(parameters_min[PARAM_SEPARATE_DECAY_COEFFICIENT], parameters_max[PARAM_SEPARATE_DECAY_COEFFICIENT], rand_from_seed(agent->seed));
}

void AutonomousAgents2D::setup_agent_with_wander(Agent *agent){
  agent->wander_rate_of_change = Math::lerp(parameters_min[PARAM_WANDER_RATE_OF_CHANGE], parameters_max[PARAM_WANDER_RATE_OF_CHANGE], rand_from_seed(agent->seed));
  agent->wander_circle_distance = Math::lerp(parameters_min[PARAM_WANDER_CIRCLE_DISTANCE], parameters_max[PARAM_WANDER_CIRCLE_DISTANCE], rand_from_seed(agent->seed));
  agent->wander_circle_radius = Math::lerp(parameters_min[PARAM_WANDER_CIRCLE_RADIUS], parameters_max[PARAM_WANDER_CIRCLE_RADIUS], rand_from_seed(agent->seed));
}

void AutonomousAgents2D::_agents_process(double p_delta) {
  p_delta *= speed_scale;

  int pcount = agents.size();

  double prev_time = time;
  time += p_delta;
  if (time > lifetime) {
    time = Math::fmod(time, lifetime);
    cycle++;
    if (one_shot && cycle > 0) {
      set_running(false);
      notify_property_list_changed();
    }
  }

  Transform2D emission_xform;
  Transform2D velocity_xform;
  if (!local_coords) {
    emission_xform = get_global_transform();
    velocity_xform = emission_xform;
    velocity_xform[2] = Vector2();
  }

  double system_phase = time / lifetime;

#ifdef DEBUG_ENABLED
  if (is_debug) {
    for (int i = 0; i < pcount; i++) {
      Agent &p = agents_arr[i];
      p.aabb_culled = true;
    }
  }
#endif

  for (int i = 0; i < pcount; i++) {
    Agent &p = agents_arr[i];

    if (!running && !p.active) {
      continue;
    }

    double local_delta = p_delta;

    // The phase is a ratio between 0 (birth) and 1 (end of life) for each agent.
    // While we use time in tests later on, for randomness we use the phase as done in the
    // original shader code, and we later multiply by lifetime to get the time.
    double restart_phase = double(i) / double(pcount);

    if (randomness_ratio > 0.0) {
      uint32_t seed = cycle;
      if (restart_phase >= system_phase) {
        seed -= uint32_t(1);
      }
      seed *= uint32_t(pcount);
      seed += uint32_t(i);
      double random = double(idhash(seed) % uint32_t(65536)) / 65536.0;
      restart_phase += randomness_ratio * random * 1.0 / double(pcount);
    }

    restart_phase *= (1.0 - explosiveness_ratio);
    double restart_time = restart_phase * lifetime;
    bool restart = false;

    if (time > prev_time) {
      // restart_time >= prev_time is used so agents emit in the first frame they are processed

      if (restart_time >= prev_time && restart_time < time) {
        restart = true;
        if (fractional_delta) {
          local_delta = time - restart_time;
        }
      }

    } else if (local_delta > 0.0) {
      if (restart_time >= prev_time) {
        restart = true;
        if (fractional_delta) {
          local_delta = lifetime - restart_time + time;
        }

      } else if (restart_time < time) {
        restart = true;
        if (fractional_delta) {
          local_delta = time - restart_time;
        }
      }
    }

    if (p.time * (1.0 - explosiveness_ratio) > p.lifetime) {
      restart = true;
    }

    float tv = 0.0;

    if (restart && !p.steering) {
      if (!running) {
        p.active = false;
        continue;
      }
      p.active = true;

      real_t tex_angle = 0.0;
      if (curve_parameters[PARAM_ANGLE].is_valid()) {
        tex_angle = curve_parameters[PARAM_ANGLE]->sample(tv);
      }

      real_t tex_anim_offset = 0.0;
      if (curve_parameters[PARAM_ANGLE].is_valid()) {
        tex_anim_offset = curve_parameters[PARAM_ANGLE]->sample(tv);
      }

      p.seed = Math::rand();
      p.ai_phase = i % number_of_ai_phases + 1;

      p.angle_rand = Math::randf();
      p.scale_rand = Math::randf();
      p.hue_rot_rand = Math::randf();
      p.anim_offset_rand = Math::randf();

      if (agent_flags[AGENT_FLAG_ARRIVE]) {
        set_agent_behavior(i, STEERING_BEHAVIOR_ARRIVE);
      }
      if (agent_flags[AGENT_FLAG_FLEE]) {
        set_agent_behavior(i, STEERING_BEHAVIOR_FLEE);
      }
      if (agent_flags[AGENT_FLAG_OBSTACLE_AVOIDANCE]) {
        set_agent_behavior(i, STEERING_BEHAVIOR_OBSTACLE_AVOIDANCE);
      }
      if (agent_flags[AGENT_FLAG_SEEK]) {
        set_agent_behavior(i, STEERING_BEHAVIOR_SEEK);
      }
      if (agent_flags[AGENT_FLAG_SEPARATE]) {
        set_agent_behavior(i, STEERING_BEHAVIOR_SEPARATE);
      }
      if (agent_flags[AGENT_FLAG_WANDER]) {
        set_agent_behavior(i, STEERING_BEHAVIOR_WANDER);
      }

      p.mass = Math::lerp(parameters_min[PARAM_AGENT_MASS], parameters_max[PARAM_AGENT_MASS], rand_from_seed(p.seed));
      p.max_speed = Math::lerp(parameters_min[PARAM_AGENT_MAX_SPEED], parameters_max[PARAM_AGENT_MAX_SPEED], rand_from_seed(p.seed));
      p.max_acceleration = Math::lerp(parameters_min[PARAM_AGENT_MAX_ACCELERATION], parameters_max[PARAM_AGENT_MAX_ACCELERATION], rand_from_seed(p.seed));
      p.max_steering_force = Math::lerp(parameters_min[PARAM_AGENT_MAX_STEERING_FORCE], parameters_max[PARAM_AGENT_MAX_STEERING_FORCE], rand_from_seed(p.seed));
      p.max_turn_rate = Math::lerp(parameters_min[PARAM_AGENT_MAX_TURN_RATE], parameters_max[PARAM_AGENT_MAX_TURN_RATE], rand_from_seed(p.seed));

      if (color_initial_ramp.is_valid()) {
        p.start_color_rand = color_initial_ramp->get_color_at_offset(Math::randf());
      } else {
        p.start_color_rand = Color(1, 1, 1, 1);
      }

      real_t angle1_rad = direction.angle() + Math::deg_to_rad((Math::randf() * 2.0 - 1.0) * spread);
      Vector2 rot = Vector2(Math::cos(angle1_rad), Math::sin(angle1_rad));
      p.velocity = rot * Math::lerp(parameters_min[PARAM_INITIAL_LINEAR_VELOCITY], parameters_max[PARAM_INITIAL_LINEAR_VELOCITY], (real_t)Math::randf());

      real_t base_angle = tex_angle * Math::lerp(parameters_min[PARAM_ANGLE], parameters_max[PARAM_ANGLE], p.angle_rand);
      p.rotation = Math::deg_to_rad(base_angle);

      if (agent_flags[AGENT_FLAG_ALIGN_HEADING_TO_VELOCITY]) {
        old_transform = Transform2D(p.transform);
      }

      p.custom[0] = 0.0; // unused
      p.custom[1] = 0.0; // phase [0..1]
      p.custom[2] = tex_anim_offset * Math::lerp(parameters_min[PARAM_ANIM_OFFSET], parameters_max[PARAM_ANIM_OFFSET], p.anim_offset_rand);
      p.custom[3] = 0.0;
      p.transform = Transform2D();
      p.time = 0;
      p.lifetime = lifetime * (1.0 - Math::randf() * lifetime_randomness);
      p.base_color = Color(1, 1, 1, 1);

      switch (emission_shape) {
      case EMISSION_SHAPE_POINT: {
        //do none
      } break;
      case EMISSION_SHAPE_SPHERE: {
        real_t t = Math_TAU * Math::randf();
        real_t radius = emission_sphere_radius * Math::randf();
        p.transform[2] = Vector2(Math::cos(t), Math::sin(t)) * radius;
      } break;
      case EMISSION_SHAPE_SPHERE_SURFACE: {
        real_t s = Math::randf(), t = Math_TAU * Math::randf();
        real_t radius = emission_sphere_radius * Math::sqrt(1.0 - s * s);
        p.transform[2] = Vector2(Math::cos(t), Math::sin(t)) * radius;
      } break;
      case EMISSION_SHAPE_RECTANGLE: {
        p.transform[2] = Vector2(Math::randf() * 2.0 - 1.0, Math::randf() * 2.0 - 1.0) * emission_rect_extents;
      } break;
      case EMISSION_SHAPE_POINTS:
      case EMISSION_SHAPE_DIRECTED_POINTS: {
        int pc = emission_points.size();
        if (pc == 0) {
          break;
        }

        int random_idx = Math::rand() % pc;

        p.transform[2] = emission_points.get(random_idx);

        if (emission_shape == EMISSION_SHAPE_DIRECTED_POINTS && emission_normals.size() == pc) {
          Vector2 normal = emission_normals.get(random_idx);
          Transform2D m2;
          m2.columns[0] = normal;
          m2.columns[1] = normal.orthogonal();
          p.velocity = m2.basis_xform(p.velocity);
        }

        if (emission_colors.size() == pc) {
          p.base_color = emission_colors.get(random_idx);
        }
      } break;
      case EMISSION_SHAPE_MAX: { // Max value for validity check.
        break;
      }
      }

      if (!local_coords) {
        p.velocity = velocity_xform.xform(p.velocity);
        p.transform = emission_xform * p.transform;
      }

    } else if (!p.active) {
      continue;
    } else if (!p.steering && p.time > p.lifetime) {
      p.active = false;
      tv = 1.0;
    } else {

      uint32_t alt_seed = p.seed;

      p.time += local_delta;
      p.custom[1] = p.time / lifetime;
      tv = p.time / p.lifetime;

      Vector2 force = gravity;
      Vector2 pos = p.transform[2];

      if (p.time < behaviour_delay) {

        real_t tex_linear_velocity = 1.0;
        if (curve_parameters[PARAM_INITIAL_LINEAR_VELOCITY].is_valid()) {
          tex_linear_velocity = curve_parameters[PARAM_INITIAL_LINEAR_VELOCITY]->sample(tv);
        }

        real_t tex_orbit_velocity = 1.0;
        if (curve_parameters[PARAM_ORBIT_VELOCITY].is_valid()) {
          tex_orbit_velocity = curve_parameters[PARAM_ORBIT_VELOCITY]->sample(tv);
        }

        real_t tex_angular_velocity = 1.0;
        if (curve_parameters[PARAM_ANGULAR_VELOCITY].is_valid()) {
          tex_angular_velocity = curve_parameters[PARAM_ANGULAR_VELOCITY]->sample(tv);
        }

        real_t tex_linear_accel = 1.0;
        if (curve_parameters[PARAM_LINEAR_ACCEL].is_valid()) {
          tex_linear_accel = curve_parameters[PARAM_LINEAR_ACCEL]->sample(tv);
        }

        real_t tex_tangential_accel = 1.0;
        if (curve_parameters[PARAM_TANGENTIAL_ACCEL].is_valid()) {
          tex_tangential_accel = curve_parameters[PARAM_TANGENTIAL_ACCEL]->sample(tv);
        }

        real_t tex_radial_accel = 1.0;
        if (curve_parameters[PARAM_RADIAL_ACCEL].is_valid()) {
          tex_radial_accel = curve_parameters[PARAM_RADIAL_ACCEL]->sample(tv);
        }

        real_t tex_damping = 1.0;
        if (curve_parameters[PARAM_DAMPING].is_valid()) {
          tex_damping = curve_parameters[PARAM_DAMPING]->sample(tv);
        }

        real_t tex_angle = 1.0;
        if (curve_parameters[PARAM_ANGLE].is_valid()) {
          tex_angle = curve_parameters[PARAM_ANGLE]->sample(tv);
        }
        real_t tex_anim_speed = 1.0;
        if (curve_parameters[PARAM_ANIM_SPEED].is_valid()) {
          tex_anim_speed = curve_parameters[PARAM_ANIM_SPEED]->sample(tv);
        }

        real_t tex_anim_offset = 1.0;
        if (curve_parameters[PARAM_ANIM_OFFSET].is_valid()) {
          tex_anim_offset = curve_parameters[PARAM_ANIM_OFFSET]->sample(tv);
        }

        //apply linear acceleration
        force += p.velocity.length() > 0.0 ? p.velocity.normalized() * tex_linear_accel * Math::lerp(parameters_min[PARAM_LINEAR_ACCEL], parameters_max[PARAM_LINEAR_ACCEL], rand_from_seed(alt_seed)) : Vector2();

        //apply radial acceleration
        Vector2 org = emission_xform[2];
        Vector2 diff = pos - org;
        force += diff.length() > 0.0 ? diff.normalized() * (tex_radial_accel)*Math::lerp(parameters_min[PARAM_RADIAL_ACCEL], parameters_max[PARAM_RADIAL_ACCEL], rand_from_seed(alt_seed)) : Vector2();
        //apply tangential acceleration;
        Vector2 yx = Vector2(diff.y, diff.x);
        force += yx.length() > 0.0 ? yx.normalized() * (tex_tangential_accel * Math::lerp(parameters_min[PARAM_TANGENTIAL_ACCEL], parameters_max[PARAM_TANGENTIAL_ACCEL], rand_from_seed(alt_seed))) : Vector2();
        //apply attractor forces
        p.velocity += force * local_delta;
        //orbit velocity
        real_t orbit_amount = tex_orbit_velocity * Math::lerp(parameters_min[PARAM_ORBIT_VELOCITY], parameters_max[PARAM_ORBIT_VELOCITY], rand_from_seed(alt_seed));
        if (orbit_amount != 0.0) {
          real_t ang = orbit_amount * local_delta * Math_TAU;
          // Not sure why the AgentProcessMaterial code uses a clockwise rotation matrix,
          // but we use -ang here to reproduce its behavior.
          Transform2D rot = Transform2D(-ang, Vector2());
          p.transform[2] -= diff;
          p.transform[2] += rot.basis_xform(diff);
        }

        if (curve_parameters[PARAM_INITIAL_LINEAR_VELOCITY].is_valid()) {
          p.velocity = p.velocity.normalized() * tex_linear_velocity;
        }

        if (parameters_max[PARAM_DAMPING] + tex_damping > 0.0) {
          real_t v = p.velocity.length();
          real_t damp = tex_damping * Math::lerp(parameters_min[PARAM_DAMPING], parameters_max[PARAM_DAMPING], rand_from_seed(alt_seed));
          v -= damp * local_delta;
          if (v < 0.0) {
            p.velocity = Vector2();
          } else {
            p.velocity = p.velocity.normalized() * v;
          }
        }

        real_t base_angle = (tex_angle)*Math::lerp(parameters_min[PARAM_ANGLE], parameters_max[PARAM_ANGLE], p.angle_rand);
        base_angle += p.custom[1] * lifetime * tex_angular_velocity * Math::lerp(parameters_min[PARAM_ANGULAR_VELOCITY], parameters_max[PARAM_ANGULAR_VELOCITY], rand_from_seed(alt_seed));
        p.rotation = Math::deg_to_rad(base_angle); //angle

        p.custom[2] = tex_anim_offset * Math::lerp(parameters_min[PARAM_ANIM_OFFSET], parameters_max[PARAM_ANIM_OFFSET], p.anim_offset_rand) + tv * tex_anim_speed * Math::lerp(parameters_min[PARAM_ANIM_SPEED], parameters_max[PARAM_ANIM_SPEED], rand_from_seed(alt_seed));
      }
      else {
        p.steering = true;
        if (ai_phase == p.ai_phase) {
          apply_steering_behaviors(&p, i, local_delta);
        }
      }
    }
    //apply color
    //apply hue rotation

    Vector2 tex_scale = Vector2(1.0, 1.0);
    if (split_scale) {
      if (scale_curve_x.is_valid()) {
        tex_scale.x = scale_curve_x->sample(tv);
      } else {
        tex_scale.x = 1.0;
      }
      if (scale_curve_y.is_valid()) {
        tex_scale.y = scale_curve_y->sample(tv);
      } else {
        tex_scale.y = 1.0;
      }
    } else {
      if (curve_parameters[PARAM_SCALE].is_valid()) {
        real_t tmp_scale = curve_parameters[PARAM_SCALE]->sample(tv);
        tex_scale.x = tmp_scale;
        tex_scale.y = tmp_scale;
      }
    }

    real_t tex_hue_variation = 0.0;
    if (curve_parameters[PARAM_HUE_VARIATION].is_valid()) {
      tex_hue_variation = curve_parameters[PARAM_HUE_VARIATION]->sample(tv);
    }

    real_t hue_rot_angle = (tex_hue_variation)*Math_TAU * Math::lerp(parameters_min[PARAM_HUE_VARIATION], parameters_max[PARAM_HUE_VARIATION], p.hue_rot_rand);
    real_t hue_rot_c = Math::cos(hue_rot_angle);
    real_t hue_rot_s = Math::sin(hue_rot_angle);

    Basis hue_rot_mat;
    {
      Basis mat1(0.299, 0.587, 0.114, 0.299, 0.587, 0.114, 0.299, 0.587, 0.114);
      Basis mat2(0.701, -0.587, -0.114, -0.299, 0.413, -0.114, -0.300, -0.588, 0.886);
      Basis mat3(0.168, 0.330, -0.497, -0.328, 0.035, 0.292, 1.250, -1.050, -0.203);

      for (int j = 0; j < 3; j++) {
        hue_rot_mat[j] = mat1[j] + mat2[j] * hue_rot_c + mat3[j] * hue_rot_s;
      }
    }

    if (color_ramp.is_valid()) {
      p.color = color_ramp->get_color_at_offset(tv) * color;
    } else {
      p.color = color;
    }

    Vector3 color_rgb = hue_rot_mat.xform_inv(Vector3(p.color.r, p.color.g, p.color.b));
    p.color.r = color_rgb.x;
    p.color.g = color_rgb.y;
    p.color.b = color_rgb.z;

    p.color *= p.base_color * p.start_color_rand;

    if (agent_flags[AGENT_FLAG_ALIGN_HEADING_TO_VELOCITY]) {
      if (p.velocity.length() > 0.0) {
        p.transform.columns[1] = p.velocity.normalized();
        p.transform.columns[0] = p.transform.columns[1].orthogonal();
      }
      else {
        p.transform.columns[0] = old_transform.columns[0];
        p.transform.columns[1] = old_transform.columns[1];
      }
    } else {
      p.transform.columns[0] = Vector2(Math::cos(p.rotation), -Math::sin(p.rotation));
      p.transform.columns[1] = Vector2(Math::sin(p.rotation), Math::cos(p.rotation));
    }

    //scale by scale
    Vector2 base_scale = tex_scale * Math::lerp(parameters_min[PARAM_SCALE], parameters_max[PARAM_SCALE], p.scale_rand);
    if (base_scale.x < 0.00001) {
      base_scale.x = 0.00001;
    }
    if (base_scale.y < 0.00001) {
      base_scale.y = 0.00001;
    }
    p.transform.columns[0] *= base_scale.x;
    p.transform.columns[1] *= base_scale.y;

    p.transform[2] += p.velocity * local_delta;

    if (p.steering) {

      if (p.steering_behavior.has(STEERING_BEHAVIOR_OBSTACLE_AVOIDANCE)) {
        if (p.avoid_obstacles_fov_scale_to_size){
          double l = base_scale.length();
          p.avoid_obstacles_field_of_view_min_distance = p.avoid_obstacles_field_of_view_min_distance * l;
          p.avoid_obstacles_field_of_view_max_distance = p.avoid_obstacles_field_of_view_max_distance * l;
          p.avoid_obstacles_field_of_view_offset = p.avoid_obstacles_field_of_view_base_offset * l;
          p.avoid_obstacles_fov_scale_to_size = false; // only need to set the scaled distance the first time
        }
      }
      if (p.transform[2].x > 1280) {
        p.transform[2].x = 0;
      }
      if (p.transform[2].x < 0) {
        p.transform[2].x = 1280;
      }
      if (p.transform[2].y > 720) {
        p.transform[2].y = 0;
      }
      if (p.transform[2].y < 0) {
        p.transform[2].y = 720;
      }
    }

    if (ai_phase == p.ai_phase || p.is_new) {
      double width = get_agent_base_size().x * base_scale.x * agent_aabb_expansion_ratio;
      double height = get_agent_base_size().y * base_scale.y * agent_aabb_expansion_ratio;
      p.aabb = AABB(Vector3(p.transform[2].x-width*0.5,p.transform[2].y-height*0.5,0), Vector3(width,height,1.0));
      if (use_bvh) {
        if (p.is_new) {
          p.bvh_leaf = agent_bvh.insert(p.aabb, &p);
        } else {
          agent_bvh.update(p.bvh_leaf, p.aabb);
        }
      }
    }
    p.is_new = false;
  }

  ai_phase++;
  if (ai_phase > number_of_ai_phases) {
    ai_phase = 1;
  }
}
struct AABBQueryResult {
  _FORCE_INLINE_ bool operator() (void *p_data) {
    return true;
  }
};

void AutonomousAgents2D::apply_steering_behaviors(Agent *agent, int i, double delta) {

  SteeringOutput steering_output;

#ifdef DEBUG_ENABLED
  if (is_debug) {
    agent->did_arrive=false;
    agent->did_flee=false;
    agent->did_seek=false;
    agent->did_wander=false;
  }
#endif
  if (agent->steering_behavior.has(STEERING_BEHAVIOR_REMOTELY_CONTROLLED)) {
  } else {
    if (agent->steering_behavior.has(STEERING_BEHAVIOR_ARRIVE)) {
      steering_output += arrive(agent);
    }
    if (agent->steering_behavior.has(STEERING_BEHAVIOR_FLEE)) {
      steering_output += flee(agent);
    }
    if (agent->steering_behavior.has(STEERING_BEHAVIOR_SEEK)) {
      steering_output += seek(agent);
    }
    if (agent->steering_behavior.has(STEERING_BEHAVIOR_SEPARATE)) {
      steering_output += separate(agent);
    }
    if (agent->steering_behavior.has(STEERING_BEHAVIOR_OBSTACLE_AVOIDANCE)) {
      steering_output += avoid_obstacles(agent);
    }
    if (agent->steering_behavior.has(STEERING_BEHAVIOR_WANDER) && steering_output.linear.length_squared() == 0) {
      steering_output += wander(agent, delta);
    }

    // todo - include mass i.e agent->velocity += steering_output.linear * delta;
    agent->velocity += steering_output.linear * delta;
    agent->rotation += steering_output.angular * delta;

    if (agent->velocity.length() > agent->max_speed) {
      agent->velocity = agent->velocity.normalized() * agent->max_speed;
    }
  }
}

AutonomousAgents2D::SteeringOutput AutonomousAgents2D::avoid_obstacles(Agent *agent) {

  SteeringOutput steering_output;

  AABB aabb = create_avoidance_aabb_for_agent(agent);
  agent_cull_aabb_query(aabb);

#ifdef DEBUG_ENABLED
  if (is_debug) {
    agent->avoidance_fov_aabb = aabb;
  }
#endif

  for (int i = 0; i < (int)agent_cull_aabb_result.size(); i++) {
    Agent *other_agent = agent_cull_aabb_result[i];
    if (other_agent != agent) {

      Vector2 direction_to_other = (other_agent->transform[2]-agent->transform[2]).normalized();
      if (direction_to_other.length() > agent->avoid_obstacles_field_of_view_distance) {
        continue;

      }
      Vector2 facing_direction = Vector2(0,1).rotated(agent->transform[0].angle());
      if (abs(direction_to_other.angle_to(facing_direction)) > agent->avoid_obstacles_field_of_view_angle * 0.5) {
        continue;
      }

      double dist = direction_to_other.length();
      double strength = agent->avoid_obstacles_decay_coefficient / (dist * dist);
      if (strength > agent->max_steering_force) {
        strength = agent->max_steering_force;
      }
      direction_to_other.normalize();
      steering_output.linear += strength * direction_to_other * -1;

#ifdef DEBUG_ENABLED
      if (is_debug) {
        other_agent->aabb_culled = false;
      }
#endif
    }
  }
  steering_output.linear = (steering_output.linear / agent->mass).limit_length(agent->max_steering_force);
  return steering_output;
}

AutonomousAgents2D::SteeringOutput AutonomousAgents2D::arrive(Agent *agent){
  if (agent->target_agent > -1) {
#ifdef DEBUG_ENABLED
    if (is_debug) {
      agent->did_arrive=true;
      agent->arrive_target = agents_arr[agent->target_agent].transform[2];
      agent->arriving_in_slow_radius = false;
      agent->arriving_in_target_radius = false;
    }
#endif
    return arrive(agent, agents_arr[agent->target_agent].transform[2]);
  }
  return SteeringOutput();
}

AutonomousAgents2D::SteeringOutput AutonomousAgents2D::arrive(Agent *agent, Vector2 target){
  SteeringOutput steering_output;

  Vector2 direction = target - agent->transform[2];
  double distance = direction.length();

  if (distance < agent->arrive_target_radius) {
#ifdef DEBUG_ENABLED
    if (is_debug) {
      agent->arriving_in_target_radius = true;
    }
#endif
    return steering_output;
  }

  double target_speed = 0.0f;

  if (distance > agent->arrive_slow_radius) {
    target_speed = agent->max_speed;
  } else {
#ifdef DEBUG_ENABLED
    if (is_debug) {
      agent->arriving_in_slow_radius = true;
    }
#endif
    target_speed = agent->max_speed * distance / agent->arrive_slow_radius;
  }

  steering_output.linear = ((direction.normalized() * target_speed) - agent->velocity) / agent->arrive_time_to_target;
  steering_output.linear = steering_output.linear.limit_length(agent->max_acceleration);
  steering_output.angular = 0;

  return steering_output;
}

AutonomousAgents2D::SteeringOutput AutonomousAgents2D::flee(Agent *agent){
  if (agent->target_agent > -1) {
#ifdef DEBUG_ENABLED
    if (is_debug) {
      agent->did_flee=true;
      agent->flee_target = agents_arr[agent->target_agent].transform[2];
    }
#endif
    return flee(agent, agents_arr[agent->target_agent].transform[2]);
  }
  return SteeringOutput();
}

AutonomousAgents2D::SteeringOutput AutonomousAgents2D::flee(Agent *agent, Vector2 target){
  SteeringOutput steering_output;
  steering_output.linear = agent->transform[2] - target;
  steering_output.linear.normalize();
  steering_output.linear *= agent->max_acceleration;
  steering_output.angular = 0;
  return steering_output;
}

AutonomousAgents2D::SteeringOutput AutonomousAgents2D::seek(Agent *agent){
  if (agent->target_agent > -1) {
#ifdef DEBUG_ENABLED
    if (is_debug) {
      agent->did_seek=true;
      agent->seek_target = agents_arr[agent->target_agent].transform[2];
    }
#endif
    return seek(agent, agents_arr[agent->target_agent].transform[2]);
  }
  return SteeringOutput();
}

AutonomousAgents2D::SteeringOutput AutonomousAgents2D::seek(Agent *agent, Vector2 target){
  SteeringOutput steering_output;
  steering_output.linear = target - agent->transform[2];
  steering_output.linear.normalize();
  steering_output.linear *= agent->max_acceleration;
  steering_output.angular = 0;
  return steering_output;
}

AutonomousAgents2D::SteeringOutput AutonomousAgents2D::separate(Agent *agent) {

  SteeringOutput steering_output;

  AABB aabb = agent->aabb.grow(agent->separate_neighbourhood_expansion * (agent->scale_rand + 1));
  agent_cull_aabb_query(aabb);

#ifdef DEBUG_ENABLED
  if (is_debug) {
    agent->separation_aabb = aabb;
  }
#endif

  for (int i = 0; i < (int)agent_cull_aabb_result.size(); i++) {
    Agent *other_agent = agent_cull_aabb_result[i];
    if (other_agent != agent) {

      Vector2 dir = other_agent->transform[2] - agent->transform[2];
      double dist = dir.length();
      double strength = agent->separate_decay_coefficient / (dist * dist);
      if (strength > agent->max_steering_force) {
        strength = agent->max_steering_force;
      }
      dir.normalize();
      steering_output.linear += strength * dir * -1;

#ifdef DEBUG_ENABLED
      if (is_debug) {
        other_agent->aabb_culled = false;
      }
#endif
    }
  }

  steering_output.linear = (steering_output.linear / agent->mass).limit_length(agent->max_steering_force);
  return steering_output;
}

AutonomousAgents2D::SteeringOutput AutonomousAgents2D::wander(Agent *agent, double delta) {
  agent->wander_target_theta += agent->wander_rate_of_change * (rand_from_seed(agent->seed) * 2.0 - 1) * delta;
  Vector2 circle_position = agent->velocity.normalized() * (agent->wander_circle_distance + agent->wander_circle_radius * 0.5) + agent->transform[2];
  double heading = agent->velocity.angle();
  Vector2 circle_offset = Vector2(agent->wander_circle_radius * Math::cos(agent->wander_target_theta + heading), agent->wander_circle_radius * Math::sin(agent->wander_target_theta + heading));

#ifdef DEBUG_ENABLED
  if (is_debug) {
    agent->did_wander=true;
    agent->wander_circle_position = circle_position;
    agent->wander_target = circle_position + circle_offset;
  }
#endif

  return seek(agent, circle_position + circle_offset);
}

AABB AutonomousAgents2D::create_avoidance_aabb_for_agent(Agent *agent) {
  Vector2 normalized_velocity = agent->velocity.normalized();
  Vector2 fov_start_position = agent->transform[2] + normalized_velocity * agent->avoid_obstacles_field_of_view_offset;

  double speed = agent->velocity.length_squared() / (agent->max_speed * agent->max_speed);
  double far_distance = fmax(agent->avoid_obstacles_field_of_view_min_distance, agent->avoid_obstacles_field_of_view_max_distance * speed);

  double axis_ratio = fmin(agent->avoid_obstacles_field_of_view_angle, half_pi) / (half_pi);
  agent->avoid_obstacles_field_of_view_left_angle = normalized_velocity.rotated(-half_pi);
  Vector2 fov_left_position = fov_start_position + (agent->avoid_obstacles_field_of_view_left_angle * far_distance * axis_ratio);
  agent->avoid_obstacles_field_of_view_right_angle = normalized_velocity.rotated(half_pi);
  Vector2 fov_right_position = fov_start_position + (agent->avoid_obstacles_field_of_view_right_angle * far_distance * axis_ratio);
  Vector2 far_distance_point = fov_start_position + normalized_velocity * far_distance;

  AABB aabb;
  aabb.position = Vector3(fov_start_position.x, fov_start_position.y, 1.0);

  Vector3 fpv3 = Vector3(far_distance_point.x, far_distance_point.y, 1.0);
  Vector3 lpv3 = Vector3(fov_left_position.x, fov_left_position.y, 1.0);
  Vector3 rpv3 = Vector3(fov_right_position.x, fov_right_position.y, 1.0);
  aabb.expand_to(fpv3);
  aabb.expand_to(lpv3);
  aabb.expand_to(rpv3);

  // adjust if > 180.  the left and right positions will need to be place to the rear
  if (agent->avoid_obstacles_field_of_view_angle > 180) {
    double remaining_fov = fmin(Math_PI, agent->avoid_obstacles_field_of_view_angle - Math_PI) * 0.5;
    double distance_ratio = far_distance / (half_pi);
    double rear_distance = remaining_fov * distance_ratio;
    Vector2 fov_left_back_position = fov_left_position + (normalized_velocity.rotated(-Math_PI)).normalized() * rear_distance * axis_ratio;
    Vector3 lbpv3 = Vector3(fov_left_back_position.x, fov_left_back_position.y, 1.0);
    aabb.expand_to(lbpv3);

    Vector2 fov_right_back_position = fov_right_position + (normalized_velocity.rotated(Math_PI)).normalized() * rear_distance * axis_ratio;
    Vector3 rbpv3 = Vector3(fov_right_back_position.x, fov_right_back_position.y, 1.0);
    aabb.expand_to(rbpv3);
  }

#ifdef DEBUG_ENABLED
  if (is_debug) {
    agent->avoidance_fov_start_position = fov_start_position;
    agent->avoidance_fov_left_position = fov_left_position;
    agent->avoidance_fov_right_position = fov_right_position;

    agent->avoidance_fov_left_end_position = fov_start_position + (normalized_velocity.rotated(-agent->avoid_obstacles_field_of_view_angle * 0.5)) * far_distance;
    agent->avoidance_fov_right_end_position = fov_start_position + (normalized_velocity.rotated(agent->avoid_obstacles_field_of_view_angle * 0.5)) * far_distance;
  }
#endif
  return aabb;
}

void AutonomousAgents2D::agent_cull_aabb_query(const AABB &p_aabb) {

  agent_cull_aabb_result.clear();

  if (use_bvh) {
    struct CullAABBBVH {
      PagedArray<Agent *> * result;
      _FORCE_INLINE_ bool operator()(void *p_data) {
        Agent *p_agent = (Agent *) p_data;
        result->push_back(p_agent);
        return false;
      }
    };
    CullAABBBVH cull_aabb;
    cull_aabb.result = &agent_cull_aabb_result;
    agent_bvh.aabb_query(p_aabb, cull_aabb);
  } else {
    for (int i = 0; i < agents.size(); i++) {
      Agent &p = agents_arr[i];
      if (p.aabb.intersects(p_aabb)) {
        agent_cull_aabb_result.push_back(&p);
        // todo - logic not to continue after the first one found -maybe
        // this will give it similar behavior to bvh
      }
    }
  }

}

void AutonomousAgents2D::_update_agent_data_buffer() {
  MutexLock lock(update_mutex);

  int pc = agents.size();

  int *ow;
  int *order = nullptr;

  float *w = agent_data.ptrw();
  float *ptr = w;

  if (draw_order != DRAW_ORDER_INDEX) {
    ow = agent_order.ptrw();
    order = ow;

    for (int i = 0; i < pc; i++) {
      order[i] = i;
    }
    if (draw_order == DRAW_ORDER_LIFETIME) {
      SortArray<int, SortLifetime> sorter;
      sorter.compare.agents = agents_arr;
      sorter.sort(order, pc);
    }
  }

  for (int i = 0; i < pc; i++) {
    int idx = order ? order[i] : i;

    Transform2D t = agents_arr[idx].transform;

    if (!local_coords) {
      t = inv_emission_transform * t;
    }

    if (agents_arr[idx].active) {
      ptr[0] = t.columns[0][0];
      ptr[1] = t.columns[1][0];
      ptr[2] = 0;
      ptr[3] = t.columns[2][0];
      ptr[4] = t.columns[0][1];
      ptr[5] = t.columns[1][1];
      ptr[6] = 0;
      ptr[7] = t.columns[2][1];

    } else {
      memset(ptr, 0, sizeof(float) * 8);
    }

    Color c = agents_arr[idx].color;

    ptr[8] = c.r;
    ptr[9] = c.g;
    ptr[10] = c.b;
    ptr[11] = c.a;

    ptr[12] = agents_arr[idx].custom[0];
    ptr[13] = agents_arr[idx].custom[1];
    ptr[14] = agents_arr[idx].custom[2];
    ptr[15] = agents_arr[idx].custom[3];

    ptr += 16;
  }
}

void AutonomousAgents2D::_set_do_redraw(bool p_do_redraw) {
  if (do_redraw == p_do_redraw) {
    return;
  }
  do_redraw = p_do_redraw;

  {
    MutexLock lock(update_mutex);

    if (do_redraw) {
      RS::get_singleton()->connect("frame_pre_draw", callable_mp(this, &AutonomousAgents2D::_update_render_thread));
      RS::get_singleton()->canvas_item_set_update_when_visible(get_canvas_item(), true);

      RS::get_singleton()->multimesh_set_visible_instances(multimesh, -1);
    } else {
      if (RS::get_singleton()->is_connected("frame_pre_draw", callable_mp(this, &AutonomousAgents2D::_update_render_thread))) {
        RS::get_singleton()->disconnect("frame_pre_draw", callable_mp(this, &AutonomousAgents2D::_update_render_thread));
      }
      RS::get_singleton()->canvas_item_set_update_when_visible(get_canvas_item(), false);

      RS::get_singleton()->multimesh_set_visible_instances(multimesh, 0);
    }
  }

  queue_redraw(); // redraw to update render list
}

void AutonomousAgents2D::_update_render_thread() {
  MutexLock lock(update_mutex);

  RS::get_singleton()->multimesh_set_buffer(multimesh, agent_data);
}

void AutonomousAgents2D::_notification(int p_what) {
  switch (p_what) {
  case NOTIFICATION_ENTER_TREE: {
    set_process_internal(running);
  } break;

  case NOTIFICATION_EXIT_TREE: {
    _set_do_redraw(false);
  } break;

  case NOTIFICATION_DRAW: {
    // first update before rendering to avoid one frame delay after running starts
    if (running && (time == 0)) {
      _update_internal();
    }

    if (!do_redraw) {
      return; // don't add to render list
    }

    RID texrid;
    if (texture.is_valid()) {
      texrid = texture->get_rid();
    }

    RS::get_singleton()->canvas_item_add_multimesh(get_canvas_item(), multimesh, texrid);
  } break;

  case NOTIFICATION_INTERNAL_PROCESS: {
    _update_internal();
  } break;

  case NOTIFICATION_TRANSFORM_CHANGED: {
    inv_emission_transform = get_global_transform().affine_inverse();

    if (!local_coords) {
      int pc = agents.size();

      float *w = agent_data.ptrw();
      float *ptr = w;

      for (int i = 0; i < pc; i++) {
        Transform2D t = inv_emission_transform * agents_arr[i].transform;

        if (agents_arr[i].active) {
          ptr[0] = t.columns[0][0];
          ptr[1] = t.columns[1][0];
          ptr[2] = 0;
          ptr[3] = t.columns[2][0];
          ptr[4] = t.columns[0][1];
          ptr[5] = t.columns[1][1];
          ptr[6] = 0;
          ptr[7] = t.columns[2][1];

        } else {
          memset(ptr, 0, sizeof(float) * 8);
        }

        ptr += 16;
      }
    }
  } break;
  }
}

bool AutonomousAgents2D::is_agent_steering(int index) {
  return agents_arr[index].steering;
}

void AutonomousAgents2D::set_behavior(Agent *agent, SteeringBehavior behavior, bool is_on) {
  if (is_on) {
    if (!agent->steering_behavior.has(behavior)) {
      agent->steering_behavior.set(behavior);
      if (behavior & STEERING_BEHAVIOR_ARRIVE) {
        setup_agent_with_arrive(agent);
      }
      if (behavior & STEERING_BEHAVIOR_OBSTACLE_AVOIDANCE) {
        setup_agent_with_obstacle_avoidance(agent);
      }
      if (behavior & STEERING_BEHAVIOR_SEPARATE) {
        setup_agent_with_separate(agent);
      }
      if (behavior & STEERING_BEHAVIOR_WANDER) {
        setup_agent_with_wander(agent);
      }
    }
  } else {
    if (agent->steering_behavior.has(behavior)) {
      agent->steering_behavior.unset(behavior);
    }
  }
  print_line("");
}

void AutonomousAgents2D::set_agent_behavior(int index, SteeringBehavior behavior) {
  set_behavior(&(agents_arr[index]), behavior, true);
}

void AutonomousAgents2D::unset_agent_behavior(int index, SteeringBehavior behavior) {
  set_behavior(&agents_arr[index], behavior, false);
}

void AutonomousAgents2D::clear_agent_behavior(int index) {
  agents_arr[index].steering_behavior.clear();
}

bool AutonomousAgents2D::has_agent_behavior(int index, SteeringBehavior behavior) {
  return agents_arr[index].steering_behavior.has(behavior);
}

void AutonomousAgents2D::set_agent_target_agent(int index, int index_to_target){
  agents_arr[index].target_agent = index_to_target;
}

void AutonomousAgents2D::set_agent_position_from_remote(int index, Vector2 p_position){
  if (has_agent_behavior(index, STEERING_BEHAVIOR_REMOTELY_CONTROLLED)) {
    agents_arr[index].velocity = Vector2(0,0);
    agents_arr[index].transform[2] = p_position;
  }
}

Vector2 AutonomousAgents2D::get_agent_position(int index){
  return agents_arr[index].transform[2];
}

#ifdef DEBUG_ENABLED
AABB AutonomousAgents2D::get_agent_aabb(int index){
  return agents_arr[index].aabb;
}
AABB AutonomousAgents2D::get_agent_separation_aabb(int index){
  return agents_arr[index].separation_aabb;
}
AABB AutonomousAgents2D::get_agent_avoidance_fov_aabb(int index){
  return agents_arr[index].avoidance_fov_aabb;
}
bool AutonomousAgents2D::is_agent_aabb_culled(int index){
  return agents_arr[index].aabb_culled;
}
bool AutonomousAgents2D::is_agent_arriving_in_slow_radius(int index) {
  return agents_arr[index].arriving_in_slow_radius;
}
bool AutonomousAgents2D::is_agent_arriving_in_target_radius(int index) {
  return agents_arr[index].arriving_in_target_radius;
}
real_t AutonomousAgents2D::get_agent_arrive_slow_radius(int index){
  return agents_arr[index].arrive_slow_radius;
}
real_t AutonomousAgents2D::get_agent_arrive_target_radius(int index){
  return agents_arr[index].arrive_target_radius;
}
Vector2 AutonomousAgents2D::get_agent_avoidance_fov_start_position(int index){
  return agents_arr[index].avoidance_fov_start_position;
}
Vector2 AutonomousAgents2D::get_agent_avoidance_fov_left_position(int index){
  return agents_arr[index].avoidance_fov_left_position;
}
Vector2 AutonomousAgents2D::get_agent_avoidance_fov_right_position(int index){
  return agents_arr[index].avoidance_fov_right_position;
}
Vector2 AutonomousAgents2D::get_agent_avoidance_fov_left_end_position(int index){
  return agents_arr[index].avoidance_fov_left_end_position;
}
Vector2 AutonomousAgents2D::get_agent_avoidance_fov_right_end_position(int index){
  return agents_arr[index].avoidance_fov_right_end_position;
}
Vector2 AutonomousAgents2D::get_agent_wander_circle_position(int index){
  return agents_arr[index].wander_circle_position;
}
real_t AutonomousAgents2D::get_agent_wander_circle_radius(int index){
  return agents_arr[index].wander_circle_radius;
}
Vector2 AutonomousAgents2D::get_agent_wander_target(int index){
  return agents_arr[index].wander_target;
}
int AutonomousAgents2D::get_agent_ai_phase(int index) {
  return agents_arr[index].ai_phase;
}
bool AutonomousAgents2D::get_did_agent_arrive(int index){
  return agents_arr[index].did_arrive;
}
bool AutonomousAgents2D::get_did_agent_flee(int index){
  return agents_arr[index].did_flee;
}
bool AutonomousAgents2D::get_did_agent_wander(int index){
  return agents_arr[index].did_wander;
}
bool AutonomousAgents2D::get_did_agent_seek(int index){
  return agents_arr[index].did_seek;
}
Vector2 AutonomousAgents2D::get_agent_arrive_target(int index){
  return agents_arr[index].arrive_target;
}
Vector2 AutonomousAgents2D::get_agent_flee_target(int index){
  return agents_arr[index].flee_target;
}
Vector2 AutonomousAgents2D::get_agent_seek_target(int index){
  return agents_arr[index].seek_target;
}
#endif

void AutonomousAgents2D::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_running", "running"), &AutonomousAgents2D::set_running);
  ClassDB::bind_method(D_METHOD("set_number_of_agents", "number_of_agents"), &AutonomousAgents2D::set_number_of_agents);
  ClassDB::bind_method(D_METHOD("set_behaviour_delay", "behaviour_delay"), &AutonomousAgents2D::set_behaviour_delay);
  ClassDB::bind_method(D_METHOD("set_number_of_ai_phases", "number_of_ai_phases"), &AutonomousAgents2D::set_number_of_ai_phases);
  ClassDB::bind_method(D_METHOD("set_lifetime", "secs"), &AutonomousAgents2D::set_lifetime);
  ClassDB::bind_method(D_METHOD("set_one_shot", "enable"), &AutonomousAgents2D::set_one_shot);
  ClassDB::bind_method(D_METHOD("set_pre_process_time", "secs"), &AutonomousAgents2D::set_pre_process_time);
  ClassDB::bind_method(D_METHOD("set_explosiveness_ratio", "ratio"), &AutonomousAgents2D::set_explosiveness_ratio);
  ClassDB::bind_method(D_METHOD("set_randomness_ratio", "ratio"), &AutonomousAgents2D::set_randomness_ratio);
  ClassDB::bind_method(D_METHOD("set_lifetime_randomness", "random"), &AutonomousAgents2D::set_lifetime_randomness);
  ClassDB::bind_method(D_METHOD("set_use_local_coordinates", "enable"), &AutonomousAgents2D::set_use_local_coordinates);
  ClassDB::bind_method(D_METHOD("set_fixed_fps", "fps"), &AutonomousAgents2D::set_fixed_fps);
  ClassDB::bind_method(D_METHOD("set_fractional_delta", "enable"), &AutonomousAgents2D::set_fractional_delta);
  ClassDB::bind_method(D_METHOD("set_speed_scale", "scale"), &AutonomousAgents2D::set_speed_scale);
  ClassDB::bind_method(D_METHOD("set_use_bvh", "use_bvh"), &AutonomousAgents2D::set_use_bvh);

  ClassDB::bind_method(D_METHOD("is_running"), &AutonomousAgents2D::is_running);
  ClassDB::bind_method(D_METHOD("get_number_of_agents"), &AutonomousAgents2D::get_number_of_agents);
  ClassDB::bind_method(D_METHOD("get_behaviour_delay"), &AutonomousAgents2D::get_behaviour_delay);
  ClassDB::bind_method(D_METHOD("get_number_of_ai_phases"), &AutonomousAgents2D::get_number_of_ai_phases);
  ClassDB::bind_method(D_METHOD("get_lifetime"), &AutonomousAgents2D::get_lifetime);
  ClassDB::bind_method(D_METHOD("get_one_shot"), &AutonomousAgents2D::get_one_shot);
  ClassDB::bind_method(D_METHOD("get_pre_process_time"), &AutonomousAgents2D::get_pre_process_time);
  ClassDB::bind_method(D_METHOD("get_explosiveness_ratio"), &AutonomousAgents2D::get_explosiveness_ratio);
  ClassDB::bind_method(D_METHOD("get_randomness_ratio"), &AutonomousAgents2D::get_randomness_ratio);
  ClassDB::bind_method(D_METHOD("get_lifetime_randomness"), &AutonomousAgents2D::get_lifetime_randomness);
  ClassDB::bind_method(D_METHOD("get_use_local_coordinates"), &AutonomousAgents2D::get_use_local_coordinates);
  ClassDB::bind_method(D_METHOD("get_fixed_fps"), &AutonomousAgents2D::get_fixed_fps);
  ClassDB::bind_method(D_METHOD("get_fractional_delta"), &AutonomousAgents2D::get_fractional_delta);
  ClassDB::bind_method(D_METHOD("get_speed_scale"), &AutonomousAgents2D::get_speed_scale);
  ClassDB::bind_method(D_METHOD("is_using_bvh"), &AutonomousAgents2D::is_using_bvh);

  ClassDB::bind_method(D_METHOD("set_draw_order", "order"), &AutonomousAgents2D::set_draw_order);

  ClassDB::bind_method(D_METHOD("get_draw_order"), &AutonomousAgents2D::get_draw_order);

  ClassDB::bind_method(D_METHOD("set_texture", "texture"), &AutonomousAgents2D::set_texture);
  ClassDB::bind_method(D_METHOD("get_texture"), &AutonomousAgents2D::get_texture);

  ClassDB::bind_method(D_METHOD("restart"), &AutonomousAgents2D::restart);

  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "running"), "set_running", "is_running");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "number_of_agents", PROPERTY_HINT_RANGE, "1,1000000,1,exp"), "set_number_of_agents", "get_number_of_agents");
  ADD_GROUP("Optimizations", "");
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_bvh"), "set_use_bvh", "is_using_bvh");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "number_of_ai_phases", PROPERTY_HINT_RANGE, "1,1000,1,or_greater"), "set_number_of_ai_phases", "get_number_of_ai_phases");
  ADD_GROUP("Time", "");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "behaviour_delay", PROPERTY_HINT_RANGE, "0.00,100.0,0.01,or_greater,suffix:s"), "set_behaviour_delay", "get_behaviour_delay");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime", PROPERTY_HINT_RANGE, "0.01,600.0,0.01,or_greater,suffix:s"), "set_lifetime", "get_lifetime");
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "one_shot"), "set_one_shot", "get_one_shot");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "preprocess", PROPERTY_HINT_RANGE, "0.00,600.0,0.01,suffix:s"), "set_pre_process_time", "get_pre_process_time");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed_scale", PROPERTY_HINT_RANGE, "0,64,0.01"), "set_speed_scale", "get_speed_scale");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "explosiveness", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_explosiveness_ratio", "get_explosiveness_ratio");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "randomness", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_randomness_ratio", "get_randomness_ratio");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_randomness", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_lifetime_randomness", "get_lifetime_randomness");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "fixed_fps", PROPERTY_HINT_RANGE, "0,1000,1,suffix:FPS"), "set_fixed_fps", "get_fixed_fps");
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "fract_delta"), "set_fractional_delta", "get_fractional_delta");

  ////////////////////////////////

  ClassDB::bind_method(D_METHOD("set_direction", "direction"), &AutonomousAgents2D::set_direction);
  ClassDB::bind_method(D_METHOD("get_direction"), &AutonomousAgents2D::get_direction);

  ClassDB::bind_method(D_METHOD("set_spread", "spread"), &AutonomousAgents2D::set_spread);
  ClassDB::bind_method(D_METHOD("get_spread"), &AutonomousAgents2D::get_spread);

  ClassDB::bind_method(D_METHOD("set_param_min", "param", "value"), &AutonomousAgents2D::set_param_min);
  ClassDB::bind_method(D_METHOD("get_param_min", "param"), &AutonomousAgents2D::get_param_min);

  ClassDB::bind_method(D_METHOD("set_param_max", "param", "value"), &AutonomousAgents2D::set_param_max);
  ClassDB::bind_method(D_METHOD("get_param_max", "param"), &AutonomousAgents2D::get_param_max);

  ClassDB::bind_method(D_METHOD("set_param_curve", "param", "curve"), &AutonomousAgents2D::set_param_curve);
  ClassDB::bind_method(D_METHOD("get_param_curve", "param"), &AutonomousAgents2D::get_param_curve);

  ClassDB::bind_method(D_METHOD("set_color", "color"), &AutonomousAgents2D::set_color);
  ClassDB::bind_method(D_METHOD("get_color"), &AutonomousAgents2D::get_color);

  ClassDB::bind_method(D_METHOD("set_color_ramp", "ramp"), &AutonomousAgents2D::set_color_ramp);
  ClassDB::bind_method(D_METHOD("get_color_ramp"), &AutonomousAgents2D::get_color_ramp);

  ClassDB::bind_method(D_METHOD("set_color_initial_ramp", "ramp"), &AutonomousAgents2D::set_color_initial_ramp);
  ClassDB::bind_method(D_METHOD("get_color_initial_ramp"), &AutonomousAgents2D::get_color_initial_ramp);

  ClassDB::bind_method(D_METHOD("set_agent_flag", "agent_flag", "enable"), &AutonomousAgents2D::set_agent_flag);
  ClassDB::bind_method(D_METHOD("get_agent_flag", "agent_flag"), &AutonomousAgents2D::get_agent_flag);

  ClassDB::bind_method(D_METHOD("set_emission_shape", "shape"), &AutonomousAgents2D::set_emission_shape);
  ClassDB::bind_method(D_METHOD("get_emission_shape"), &AutonomousAgents2D::get_emission_shape);

  ClassDB::bind_method(D_METHOD("set_emission_sphere_radius", "radius"), &AutonomousAgents2D::set_emission_sphere_radius);
  ClassDB::bind_method(D_METHOD("get_emission_sphere_radius"), &AutonomousAgents2D::get_emission_sphere_radius);

  ClassDB::bind_method(D_METHOD("set_emission_rect_extents", "extents"), &AutonomousAgents2D::set_emission_rect_extents);
  ClassDB::bind_method(D_METHOD("get_emission_rect_extents"), &AutonomousAgents2D::get_emission_rect_extents);

  ClassDB::bind_method(D_METHOD("set_emission_points", "array"), &AutonomousAgents2D::set_emission_points);
  ClassDB::bind_method(D_METHOD("get_emission_points"), &AutonomousAgents2D::get_emission_points);

  ClassDB::bind_method(D_METHOD("set_emission_normals", "array"), &AutonomousAgents2D::set_emission_normals);
  ClassDB::bind_method(D_METHOD("get_emission_normals"), &AutonomousAgents2D::get_emission_normals);

  ClassDB::bind_method(D_METHOD("set_emission_colors", "array"), &AutonomousAgents2D::set_emission_colors);
  ClassDB::bind_method(D_METHOD("get_emission_colors"), &AutonomousAgents2D::get_emission_colors);

  ClassDB::bind_method(D_METHOD("get_gravity"), &AutonomousAgents2D::get_gravity);
  ClassDB::bind_method(D_METHOD("set_gravity", "gravity"), &AutonomousAgents2D::set_gravity);

  ClassDB::bind_method(D_METHOD("get_agent_base_size"), &AutonomousAgents2D::get_agent_base_size);
  ClassDB::bind_method(D_METHOD("set_agent_base_size", "agent_base_size"), &AutonomousAgents2D::set_agent_base_size);
  ClassDB::bind_method(D_METHOD("get_agent_aabb_expansion_ratio"), &AutonomousAgents2D::get_agent_aabb_expansion_ratio);
  ClassDB::bind_method(D_METHOD("set_agent_aabb_expansion_ratio","agent_aabb_expansion_ratio"), &AutonomousAgents2D::set_agent_aabb_expansion_ratio);

  ClassDB::bind_method(D_METHOD("get_split_scale"), &AutonomousAgents2D::get_split_scale);
  ClassDB::bind_method(D_METHOD("set_split_scale", "split_scale"), &AutonomousAgents2D::set_split_scale);

  ClassDB::bind_method(D_METHOD("get_scale_curve_x"), &AutonomousAgents2D::get_scale_curve_x);
  ClassDB::bind_method(D_METHOD("set_scale_curve_x", "scale_curve"), &AutonomousAgents2D::set_scale_curve_x);

  ClassDB::bind_method(D_METHOD("get_scale_curve_y"), &AutonomousAgents2D::get_scale_curve_y);
  ClassDB::bind_method(D_METHOD("set_scale_curve_y", "scale_curve"), &AutonomousAgents2D::set_scale_curve_y);


  ADD_GROUP("Agent", "agent_");
  ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "agent_base_size", PROPERTY_HINT_RANGE, "0,10000,0.01,or_greater,suffix:px"), "set_agent_base_size", "get_agent_base_size");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "agent_aabb_expansion_ratio", PROPERTY_HINT_RANGE, "0.1,100,0.01,or_greater,suffix:px"), "set_agent_aabb_expansion_ratio", "get_agent_aabb_expansion_ratio");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_mass_min", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_greater,suffix:kg"), "set_param_min", "get_param_min", PARAM_AGENT_MASS);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_mass_max", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_greater,suffix:kg"), "set_param_max", "get_param_max", PARAM_AGENT_MASS);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_max_speed_min", PROPERTY_HINT_RANGE, "0,100000,0.01,or_greater,suffix:px/s"), "set_param_min", "get_param_min", PARAM_AGENT_MAX_SPEED);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_max_speed_max", PROPERTY_HINT_RANGE, "0,100000,0.01,or_greater,suffix:px/s"), "set_param_max", "get_param_max", PARAM_AGENT_MAX_SPEED);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_max_acceleration_min", PROPERTY_HINT_RANGE, "0,100000,0.01,or_greater,suffix:px/s"), "set_param_min", "get_param_min", PARAM_AGENT_MAX_ACCELERATION);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_max_acceleration_max", PROPERTY_HINT_RANGE, "0,100000,0.01,or_greater,suffix:px/s"), "set_param_max", "get_param_max", PARAM_AGENT_MAX_ACCELERATION);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_max_steering_force_min", PROPERTY_HINT_RANGE, "0,100000,0.01,or_greater,suffix:N"), "set_param_min", "get_param_min", PARAM_AGENT_MAX_STEERING_FORCE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_max_steering_force_max", PROPERTY_HINT_RANGE, "0,100000,0.01,or_greater,suffix:N"), "set_param_max", "get_param_max", PARAM_AGENT_MAX_STEERING_FORCE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_max_turn_rate_min", PROPERTY_HINT_RANGE, "0,100000,0.01,or_greater,suffix:px/s"), "set_param_min", "get_param_min", PARAM_AGENT_MAX_TURN_RATE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "agent_max_turn_rate", PROPERTY_HINT_RANGE, "0,100000,0.01,or_greater,suffix:px/s"), "set_param_max", "get_param_max", PARAM_AGENT_MAX_TURN_RATE);
  ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "agent_align_heading_to_velocity"), "set_agent_flag", "get_agent_flag", AGENT_FLAG_ALIGN_HEADING_TO_VELOCITY);

  ClassDB::bind_method(D_METHOD("is_agent_steering"), &AutonomousAgents2D::is_agent_steering);
  ClassDB::bind_method(D_METHOD("set_agent_behavior","behavior"), &AutonomousAgents2D::set_agent_behavior);
  ClassDB::bind_method(D_METHOD("unset_agent_behavior","behavior"), &AutonomousAgents2D::unset_agent_behavior);
  ClassDB::bind_method(D_METHOD("clear_agent_behavior"), &AutonomousAgents2D::clear_agent_behavior);
  ClassDB::bind_method(D_METHOD("has_agent_behavior"), &AutonomousAgents2D::has_agent_behavior);
  ClassDB::bind_method(D_METHOD("set_agent_target_agent","target_agent"), &AutonomousAgents2D::set_agent_target_agent);
  ClassDB::bind_method(D_METHOD("set_agent_position_from_remote"), &AutonomousAgents2D::set_agent_position_from_remote);

  ADD_GROUP("Behaviour", "agent_flag_");
  ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "agent_flag_remotely_controlled"), "set_agent_flag", "get_agent_flag", AGENT_FLAG_REMOTELY_CONTROLLED);
  ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "agent_flag_arrive"), "set_agent_flag", "get_agent_flag", AGENT_FLAG_ARRIVE);
  ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "agent_flag_flee"), "set_agent_flag", "get_agent_flag", AGENT_FLAG_FLEE);
  ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "agent_flag_obstacle_avoidance"), "set_agent_flag", "get_agent_flag", AGENT_FLAG_OBSTACLE_AVOIDANCE);
  ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "agent_flag_seek"), "set_agent_flag", "get_agent_flag", AGENT_FLAG_SEEK);
  ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "agent_flag_separate"), "set_agent_flag", "get_agent_flag", AGENT_FLAG_SEPARATE);
  ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "agent_flag_wander"), "set_agent_flag", "get_agent_flag", AGENT_FLAG_WANDER);

  ADD_GROUP("Arrive", "arrive_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "arrive_slow_radius_distance_min", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater,suffix:px"), "set_param_min", "get_param_min", PARAM_ARRIVE_SLOW_RADIUS);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "arrive_slow_radius_distance_max", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater,suffix:px"), "set_param_max", "get_param_max", PARAM_ARRIVE_SLOW_RADIUS);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "arrive_target_radius_distance_min", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater,suffix:px"), "set_param_min", "get_param_min", PARAM_ARRIVE_TARGET_RADIUS);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "arrive_target_radius_distance_max", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater,suffix:px"), "set_param_max", "get_param_max", PARAM_ARRIVE_TARGET_RADIUS);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "arrive_time_to_target_min", PROPERTY_HINT_RANGE, "0.0,1000,0.01,or_greater,suffix:px"), "set_param_min", "get_param_min", PARAM_ARRIVE_TIME_TO_TARGET);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "arrive_time_to_target_max", PROPERTY_HINT_RANGE, "0.0,1000,0.01,or_greater,suffix:px"), "set_param_max", "get_param_max", PARAM_ARRIVE_TIME_TO_TARGET);

  ADD_GROUP("Obstacle Avoidance", "obstacle_avoidance_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "obstacle_avoidance_fov_angle_min", PROPERTY_HINT_RANGE, "1,360,0.01,or_greater,suffix:degrees"), "set_param_min", "get_param_min", PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_ANGLE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "obstacle_avoidance_fov_angle_max", PROPERTY_HINT_RANGE, "1,360,0.01,or_greater,suffix:degrees"), "set_param_max", "get_param_max", PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_ANGLE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "obstacle_avoidance_fov_distance_min", PROPERTY_HINT_RANGE, "1,10000,0.01,or_greater,suffix:px"), "set_param_min", "get_param_min", PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_DISTANCE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "obstacle_avoidance_fov_distance_max", PROPERTY_HINT_RANGE, "1,10000,0.01,or_greater,suffix:px"), "set_param_max", "get_param_max", PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_DISTANCE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "obstacle_avoidance_fov_offset_min", PROPERTY_HINT_NONE, U"suffix:px"), "set_param_min", "get_param_min", PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_OFFSET);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "obstacle_avoidance_fov_offset_max", PROPERTY_HINT_NONE, U"suffix:px"), "set_param_max", "get_param_max", PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_OFFSET);
  ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "obstacle_avoidance_fov_scale_to_size"), "set_agent_flag", "get_agent_flag", AGENT_FLAG_OBSTACLE_AVOIDANCE_FOV_SCALE_TO_SIZE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "obstacle_avoidance_decay_coefficient_min", PROPERTY_HINT_RANGE, "0,1000000,0.01,or_greater"), "set_param_min", "get_param_min", PARAM_OBSTACLE_AVOIDANCE_DECAY_COEFFICIENT);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "obstacle_avoidance_decay_coefficient_max", PROPERTY_HINT_RANGE, "0,1000000,0.01,or_greater"), "set_param_max", "get_param_max", PARAM_OBSTACLE_AVOIDANCE_DECAY_COEFFICIENT);

  ADD_GROUP("Separate", "separate_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "separate_neighbourhood_expansion_min", PROPERTY_HINT_RANGE, "-10000,10000,0.01,or_greater,suffix:px"), "set_param_min", "get_param_min", PARAM_SEPARATE_NEIGHBOURHOOD_EXPANSION);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "separate_neighbourhood_expansion_max", PROPERTY_HINT_RANGE, "-10000,10000,0.01,or_greater,suffix:px"), "set_param_max", "get_param_max", PARAM_SEPARATE_NEIGHBOURHOOD_EXPANSION);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "separate_decay_coefficient_min", PROPERTY_HINT_RANGE, "0,1000000,0.01,or_greater"), "set_param_min", "get_param_min", PARAM_SEPARATE_DECAY_COEFFICIENT);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "separate_decay_coefficient_max", PROPERTY_HINT_RANGE, "0,1000000,0.01,or_greater"), "set_param_max", "get_param_max", PARAM_SEPARATE_DECAY_COEFFICIENT);

  ADD_GROUP("Wander", "wander_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "wander_circle_distance_min", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_greater,suffix:px"), "set_param_min", "get_param_min", PARAM_WANDER_CIRCLE_DISTANCE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "wander_circle_distance_max", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_greater,suffix:px"), "set_param_max", "get_param_max", PARAM_WANDER_CIRCLE_DISTANCE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "wander_circle_radius_min", PROPERTY_HINT_RANGE, "1,1000,0.01,or_greater,suffix:px"), "set_param_min", "get_param_min", PARAM_WANDER_CIRCLE_RADIUS);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "wander_circle_radius_max", PROPERTY_HINT_RANGE, "1,1000,0.01,or_greater,suffix:px"), "set_param_max", "get_param_max", PARAM_WANDER_CIRCLE_RADIUS);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "wander_rate_of_change_min", PROPERTY_HINT_RANGE, "0,1000,0.0001,or_greater,suffix:px"), "set_param_min", "get_param_min", PARAM_WANDER_RATE_OF_CHANGE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "wander_rate_of_change_max", PROPERTY_HINT_RANGE, "0,1000,0.0001,or_greater,suffix:px"), "set_param_max", "get_param_max", PARAM_WANDER_RATE_OF_CHANGE);

  ADD_GROUP("Drawing", "");
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "local_coords"), "set_use_local_coordinates", "get_use_local_coordinates");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "draw_order", PROPERTY_HINT_ENUM, "Index,Lifetime"), "set_draw_order", "get_draw_order");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_texture", "get_texture");

  ADD_GROUP("Emission Shape", "emission_");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "emission_shape", PROPERTY_HINT_ENUM, "Point,Sphere,Sphere Surface,Rectangle,Points,Directed Points", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_emission_shape", "get_emission_shape");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "emission_sphere_radius", PROPERTY_HINT_RANGE, "0.01,128,0.01,suffix:px"), "set_emission_sphere_radius", "get_emission_sphere_radius");
  ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "emission_rect_extents", PROPERTY_HINT_NONE, "suffix:px"), "set_emission_rect_extents", "get_emission_rect_extents");
  ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "emission_points"), "set_emission_points", "get_emission_points");
  ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "emission_normals"), "set_emission_normals", "get_emission_normals");
  ADD_PROPERTY(PropertyInfo(Variant::PACKED_COLOR_ARRAY, "emission_colors"), "set_emission_colors", "get_emission_colors");
  ADD_GROUP("Direction", "");
  ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "direction"), "set_direction", "get_direction");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spread", PROPERTY_HINT_RANGE, "0,180,0.01"), "set_spread", "get_spread");
  ADD_GROUP("Gravity", "");
  ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "gravity", PROPERTY_HINT_NONE, U"suffix:px/s\u00B2"), "set_gravity", "get_gravity");
  ADD_GROUP("Initial Velocity", "initial_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "initial_velocity_min", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater,suffix:px/s"), "set_param_min", "get_param_min", PARAM_INITIAL_LINEAR_VELOCITY);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "initial_velocity_max", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater,suffix:px/s"), "set_param_max", "get_param_max", PARAM_INITIAL_LINEAR_VELOCITY);
  ADD_GROUP("Angular Velocity", "angular_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "angular_velocity_min", PROPERTY_HINT_RANGE, "-720,720,0.01,or_less,or_greater"), "set_param_min", "get_param_min", PARAM_ANGULAR_VELOCITY);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "angular_velocity_max", PROPERTY_HINT_RANGE, "-720,720,0.01,or_less,or_greater"), "set_param_max", "get_param_max", PARAM_ANGULAR_VELOCITY);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "angular_velocity_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_ANGULAR_VELOCITY);
  ADD_GROUP("Orbit Velocity", "orbit_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "orbit_velocity_min", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_less,or_greater"), "set_param_min", "get_param_min", PARAM_ORBIT_VELOCITY);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "orbit_velocity_max", PROPERTY_HINT_RANGE, "-1000,1000,0.01,or_less,or_greater"), "set_param_max", "get_param_max", PARAM_ORBIT_VELOCITY);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "orbit_velocity_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_ORBIT_VELOCITY);
  ADD_GROUP("Linear Accel", "linear_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "linear_accel_min", PROPERTY_HINT_RANGE, "-100,100,0.01,or_less,or_greater"), "set_param_min", "get_param_min", PARAM_LINEAR_ACCEL);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "linear_accel_max", PROPERTY_HINT_RANGE, "-100,100,0.01,or_less,or_greater"), "set_param_max", "get_param_max", PARAM_LINEAR_ACCEL);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "linear_accel_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_LINEAR_ACCEL);
  ADD_GROUP("Radial Accel", "radial_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "radial_accel_min", PROPERTY_HINT_RANGE, "-100,100,0.01,or_less,or_greater"), "set_param_min", "get_param_min", PARAM_RADIAL_ACCEL);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "radial_accel_max", PROPERTY_HINT_RANGE, "-100,100,0.01,or_less,or_greater"), "set_param_max", "get_param_max", PARAM_RADIAL_ACCEL);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "radial_accel_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_RADIAL_ACCEL);
  ADD_GROUP("Tangential Accel", "tangential_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "tangential_accel_min", PROPERTY_HINT_RANGE, "-100,100,0.01,or_less,or_greater"), "set_param_min", "get_param_min", PARAM_TANGENTIAL_ACCEL);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "tangential_accel_max", PROPERTY_HINT_RANGE, "-100,100,0.01,or_less,or_greater"), "set_param_max", "get_param_max", PARAM_TANGENTIAL_ACCEL);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "tangential_accel_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_TANGENTIAL_ACCEL);
  ADD_GROUP("Damping", "");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "damping_min", PROPERTY_HINT_RANGE, "0,100,0.01"), "set_param_min", "get_param_min", PARAM_DAMPING);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "damping_max", PROPERTY_HINT_RANGE, "0,100,0.01"), "set_param_max", "get_param_max", PARAM_DAMPING);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "damping_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_DAMPING);
  ADD_GROUP("Angle", "");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "angle_min", PROPERTY_HINT_RANGE, "-720,720,0.1,or_less,or_greater,degrees"), "set_param_min", "get_param_min", PARAM_ANGLE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "angle_max", PROPERTY_HINT_RANGE, "-720,720,0.1,or_less,or_greater,degrees"), "set_param_max", "get_param_max", PARAM_ANGLE);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "angle_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_ANGLE);
  ADD_GROUP("Scale", "");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "scale_amount_min", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater"), "set_param_min", "get_param_min", PARAM_SCALE);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "scale_amount_max", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater"), "set_param_max", "get_param_max", PARAM_SCALE);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "scale_amount_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_SCALE);
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "split_scale"), "set_split_scale", "get_split_scale");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scale_curve_x", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_scale_curve_x", "get_scale_curve_x");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scale_curve_y", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_scale_curve_y", "get_scale_curve_y");

  ADD_GROUP("Color", "");
  ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "color_ramp", PROPERTY_HINT_RESOURCE_TYPE, "Gradient"), "set_color_ramp", "get_color_ramp");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "color_initial_ramp", PROPERTY_HINT_RESOURCE_TYPE, "Gradient"), "set_color_initial_ramp", "get_color_initial_ramp");

  ADD_GROUP("Hue Variation", "hue_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "hue_variation_min", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_param_min", "get_param_min", PARAM_HUE_VARIATION);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "hue_variation_max", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_param_max", "get_param_max", PARAM_HUE_VARIATION);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "hue_variation_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_HUE_VARIATION);
  ADD_GROUP("Animation", "anim_");
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "anim_speed_min", PROPERTY_HINT_RANGE, "0,128,0.01,or_greater,or_less"), "set_param_min", "get_param_min", PARAM_ANIM_SPEED);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "anim_speed_max", PROPERTY_HINT_RANGE, "0,128,0.01,or_greater,or_less"), "set_param_max", "get_param_max", PARAM_ANIM_SPEED);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "anim_speed_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_ANIM_SPEED);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "anim_offset_min", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_param_min", "get_param_min", PARAM_ANIM_OFFSET);
  ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "anim_offset_max", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_param_max", "get_param_max", PARAM_ANIM_OFFSET);
  ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "anim_offset_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_param_curve", "get_param_curve", PARAM_ANIM_OFFSET);

  BIND_ENUM_CONSTANT(DRAW_ORDER_INDEX);
  BIND_ENUM_CONSTANT(DRAW_ORDER_LIFETIME);

  BIND_ENUM_CONSTANT(PARAM_AGENT_MASS);
  BIND_ENUM_CONSTANT(PARAM_AGENT_MAX_SPEED);
  BIND_ENUM_CONSTANT(PARAM_AGENT_MAX_ACCELERATION);
  BIND_ENUM_CONSTANT(PARAM_AGENT_MAX_STEERING_FORCE);
  BIND_ENUM_CONSTANT(PARAM_AGENT_MAX_TURN_RATE);

  BIND_ENUM_CONSTANT(PARAM_ARRIVE_SLOW_RADIUS);
  BIND_ENUM_CONSTANT(PARAM_ARRIVE_TARGET_RADIUS);
  BIND_ENUM_CONSTANT(PARAM_ARRIVE_TIME_TO_TARGET);

  BIND_ENUM_CONSTANT(PARAM_OBSTACLE_AVOIDANCE_DECAY_COEFFICIENT);
  BIND_ENUM_CONSTANT(PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_ANGLE);
  BIND_ENUM_CONSTANT(PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_DISTANCE);
  BIND_ENUM_CONSTANT(PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_OFFSET);

  BIND_ENUM_CONSTANT(PARAM_SEPARATE_NEIGHBOURHOOD_EXPANSION);
  BIND_ENUM_CONSTANT(PARAM_SEPARATE_DECAY_COEFFICIENT);

  BIND_ENUM_CONSTANT(PARAM_WANDER_CIRCLE_DISTANCE);
  BIND_ENUM_CONSTANT(PARAM_WANDER_CIRCLE_RADIUS);
  BIND_ENUM_CONSTANT(PARAM_WANDER_RATE_OF_CHANGE);

  BIND_ENUM_CONSTANT(PARAM_INITIAL_LINEAR_VELOCITY);
  BIND_ENUM_CONSTANT(PARAM_ANGULAR_VELOCITY);
  BIND_ENUM_CONSTANT(PARAM_ORBIT_VELOCITY);
  BIND_ENUM_CONSTANT(PARAM_LINEAR_ACCEL);
  BIND_ENUM_CONSTANT(PARAM_RADIAL_ACCEL);
  BIND_ENUM_CONSTANT(PARAM_TANGENTIAL_ACCEL);
  BIND_ENUM_CONSTANT(PARAM_DAMPING);
  BIND_ENUM_CONSTANT(PARAM_ANGLE);
  BIND_ENUM_CONSTANT(PARAM_SCALE);
  BIND_ENUM_CONSTANT(PARAM_HUE_VARIATION);
  BIND_ENUM_CONSTANT(PARAM_ANIM_SPEED);
  BIND_ENUM_CONSTANT(PARAM_ANIM_OFFSET);
  BIND_ENUM_CONSTANT(PARAM_MAX);

  BIND_ENUM_CONSTANT(AGENT_FLAG_ARRIVE);
  BIND_ENUM_CONSTANT(AGENT_FLAG_FLEE);
  BIND_ENUM_CONSTANT(AGENT_FLAG_OBSTACLE_AVOIDANCE);
  BIND_ENUM_CONSTANT(AGENT_FLAG_REMOTELY_CONTROLLED);
  BIND_ENUM_CONSTANT(AGENT_FLAG_SEEK);
  BIND_ENUM_CONSTANT(AGENT_FLAG_SEPARATE);
  BIND_ENUM_CONSTANT(AGENT_FLAG_WANDER);
  BIND_ENUM_CONSTANT(AGENT_FLAG_ALIGN_HEADING_TO_VELOCITY);
  BIND_ENUM_CONSTANT(AGENT_FLAG_OBSTACLE_AVOIDANCE_FOV_SCALE_TO_SIZE);
  BIND_ENUM_CONSTANT(AGENT_FLAG_MAX);

  BIND_ENUM_CONSTANT(EMISSION_SHAPE_POINT);
  BIND_ENUM_CONSTANT(EMISSION_SHAPE_SPHERE);
  BIND_ENUM_CONSTANT(EMISSION_SHAPE_SPHERE_SURFACE);
  BIND_ENUM_CONSTANT(EMISSION_SHAPE_RECTANGLE);
  BIND_ENUM_CONSTANT(EMISSION_SHAPE_POINTS);
  BIND_ENUM_CONSTANT(EMISSION_SHAPE_DIRECTED_POINTS);
  BIND_ENUM_CONSTANT(EMISSION_SHAPE_MAX);

  BIND_ENUM_CONSTANT(STEERING_BEHAVIOR_ARRIVE);
  BIND_ENUM_CONSTANT(STEERING_BEHAVIOR_FLEE);
  BIND_ENUM_CONSTANT(STEERING_BEHAVIOR_OBSTACLE_AVOIDANCE);
  BIND_ENUM_CONSTANT(STEERING_BEHAVIOR_REMOTELY_CONTROLLED);
  BIND_ENUM_CONSTANT(STEERING_BEHAVIOR_SEEK);
  BIND_ENUM_CONSTANT(STEERING_BEHAVIOR_SEPARATE);
  BIND_ENUM_CONSTANT(STEERING_BEHAVIOR_WANDER);

#ifdef DEBUG_ENABLED
  ClassDB::bind_method(D_METHOD("is_debugging"), &AutonomousAgents2D::is_debugging);
  ClassDB::bind_method(D_METHOD("set_is_debug", "is_debug"), &AutonomousAgents2D::set_is_debug);
  ClassDB::bind_method(D_METHOD("get_agent_position"), &AutonomousAgents2D::get_agent_position);
  ClassDB::bind_method(D_METHOD("get_agent_aabb"), &AutonomousAgents2D::get_agent_aabb);
  ClassDB::bind_method(D_METHOD("get_agent_separation_aabb"), &AutonomousAgents2D::get_agent_separation_aabb);
  ClassDB::bind_method(D_METHOD("get_agent_arrive_slow_radius"), &AutonomousAgents2D::get_agent_arrive_slow_radius);
  ClassDB::bind_method(D_METHOD("get_agent_arrive_target_radius"), &AutonomousAgents2D::get_agent_arrive_target_radius);
  ClassDB::bind_method(D_METHOD("get_agent_arriving_in_slow_radius"), &AutonomousAgents2D::is_agent_arriving_in_slow_radius);
  ClassDB::bind_method(D_METHOD("get_agent_arriving_in_target_radius"), &AutonomousAgents2D::is_agent_arriving_in_target_radius);
  ClassDB::bind_method(D_METHOD("get_agent_avoidance_fov_aabb"), &AutonomousAgents2D::get_agent_avoidance_fov_aabb);
  ClassDB::bind_method(D_METHOD("get_agent_avoidance_fov_start_position"), &AutonomousAgents2D::get_agent_avoidance_fov_start_position);
  ClassDB::bind_method(D_METHOD("get_agent_avoidance_fov_left_position"), &AutonomousAgents2D::get_agent_avoidance_fov_left_position);
  ClassDB::bind_method(D_METHOD("get_agent_avoidance_fov_right_end_position"), &AutonomousAgents2D::get_agent_avoidance_fov_right_end_position);
  ClassDB::bind_method(D_METHOD("get_agent_avoidance_fov_left_end_position"), &AutonomousAgents2D::get_agent_avoidance_fov_left_end_position);
  ClassDB::bind_method(D_METHOD("get_agent_avoidance_fov_right_position"), &AutonomousAgents2D::get_agent_avoidance_fov_right_position);
  ClassDB::bind_method(D_METHOD("get_agent_wander_circle_position"), &AutonomousAgents2D::get_agent_wander_circle_position);
  ClassDB::bind_method(D_METHOD("get_agent_wander_circle_radius"), &AutonomousAgents2D::get_agent_wander_circle_radius);
  ClassDB::bind_method(D_METHOD("get_agent_wander_target"), &AutonomousAgents2D::get_agent_wander_target);
  ClassDB::bind_method(D_METHOD("is_agent_aabb_culled"), &AutonomousAgents2D::is_agent_aabb_culled);
  ClassDB::bind_method(D_METHOD("get_agent_ai_phase"), &AutonomousAgents2D::get_agent_ai_phase);
  ClassDB::bind_method(D_METHOD("get_did_agent_flee"), &AutonomousAgents2D::get_did_agent_flee);
  ClassDB::bind_method(D_METHOD("get_did_agent_arrive"), &AutonomousAgents2D::get_did_agent_arrive);
  ClassDB::bind_method(D_METHOD("get_did_agent_seek"), &AutonomousAgents2D::get_did_agent_seek);
  ClassDB::bind_method(D_METHOD("get_did_agent_wander"), &AutonomousAgents2D::get_did_agent_wander);
  ClassDB::bind_method(D_METHOD("get_agent_arrive_target"), &AutonomousAgents2D::get_agent_arrive_target);
  ClassDB::bind_method(D_METHOD("get_agent_flee_target"), &AutonomousAgents2D::get_agent_flee_target);
  ClassDB::bind_method(D_METHOD("get_agent_seek_target"), &AutonomousAgents2D::get_agent_seek_target);
#endif
}

AutonomousAgents2D::AutonomousAgents2D() {
  mesh = RenderingServer::get_singleton()->mesh_create();
  multimesh = RenderingServer::get_singleton()->multimesh_create();
  RenderingServer::get_singleton()->multimesh_set_mesh(multimesh, mesh);

  set_running(true);
  set_behaviour_delay(0);
  set_number_of_agents(8);
  set_use_local_coordinates(false);

  set_agent_base_size(Size2(10,10));
  set_agent_aabb_expansion_ratio(1.2);

  set_use_bvh(false);
  set_number_of_ai_phases(1);

  set_param_min(PARAM_AGENT_MASS, 1);
  set_param_min(PARAM_AGENT_MAX_SPEED, 1);
  set_param_min(PARAM_AGENT_MAX_ACCELERATION, 1);
  set_param_min(PARAM_AGENT_MAX_STEERING_FORCE, 1);
  set_param_min(PARAM_AGENT_MAX_TURN_RATE, 1);
  set_param_max(PARAM_AGENT_MASS, 1);
  set_param_max(PARAM_AGENT_MAX_SPEED, 1);
  set_param_max(PARAM_AGENT_MAX_ACCELERATION, 1);
  set_param_max(PARAM_AGENT_MAX_STEERING_FORCE, 1);
  set_param_max(PARAM_AGENT_MAX_TURN_RATE, 1);

  set_param_min(PARAM_ARRIVE_SLOW_RADIUS, 40);
  set_param_max(PARAM_ARRIVE_SLOW_RADIUS, 40);
  set_param_min(PARAM_ARRIVE_TARGET_RADIUS, 10);
  set_param_max(PARAM_ARRIVE_TARGET_RADIUS, 10);
  set_param_min(PARAM_ARRIVE_TIME_TO_TARGET, 0.1);
  set_param_max(PARAM_ARRIVE_TIME_TO_TARGET, 0.1);

  set_param_min(PARAM_OBSTACLE_AVOIDANCE_DECAY_COEFFICIENT, 5000);
  set_param_max(PARAM_OBSTACLE_AVOIDANCE_DECAY_COEFFICIENT, 5000);
  set_param_min(PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_ANGLE, 90);
  set_param_max(PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_ANGLE, 90);
  set_param_min(PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_DISTANCE, 50);
  set_param_max(PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_DISTANCE, 50);
  set_param_min(PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_OFFSET, 0);
  set_param_max(PARAM_OBSTACLE_AVOIDANCE_FIELD_OF_VIEW_OFFSET, 0);

  set_param_min(PARAM_SEPARATE_NEIGHBOURHOOD_EXPANSION, 20);
  set_param_max(PARAM_SEPARATE_NEIGHBOURHOOD_EXPANSION, 20);
  set_param_min(PARAM_SEPARATE_DECAY_COEFFICIENT, 5000);
  set_param_max(PARAM_SEPARATE_DECAY_COEFFICIENT, 5000);

  set_param_min(PARAM_WANDER_CIRCLE_DISTANCE, 20);
  set_param_max(PARAM_WANDER_CIRCLE_DISTANCE, 20);
  set_param_min(PARAM_WANDER_CIRCLE_RADIUS, 40);
  set_param_max(PARAM_WANDER_CIRCLE_RADIUS, 40);
  set_param_min(PARAM_WANDER_RATE_OF_CHANGE, 1);
  set_param_max(PARAM_WANDER_RATE_OF_CHANGE, 1);

  set_param_min(PARAM_INITIAL_LINEAR_VELOCITY, 0);
  set_param_min(PARAM_ANGULAR_VELOCITY, 0);
  set_param_min(PARAM_ORBIT_VELOCITY, 0);
  set_param_min(PARAM_LINEAR_ACCEL, 0);
  set_param_min(PARAM_RADIAL_ACCEL, 0);
  set_param_min(PARAM_TANGENTIAL_ACCEL, 0);
  set_param_min(PARAM_DAMPING, 0);
  set_param_min(PARAM_ANGLE, 0);
  set_param_min(PARAM_SCALE, 1);
  set_param_min(PARAM_HUE_VARIATION, 0);
  set_param_min(PARAM_ANIM_SPEED, 0);
  set_param_min(PARAM_ANIM_OFFSET, 0);

  set_param_max(PARAM_INITIAL_LINEAR_VELOCITY, 0);
  set_param_max(PARAM_ANGULAR_VELOCITY, 0);
  set_param_max(PARAM_ORBIT_VELOCITY, 0);
  set_param_max(PARAM_LINEAR_ACCEL, 0);
  set_param_max(PARAM_RADIAL_ACCEL, 0);
  set_param_max(PARAM_TANGENTIAL_ACCEL, 0);
  set_param_max(PARAM_DAMPING, 0);
  set_param_max(PARAM_ANGLE, 0);
  set_param_max(PARAM_SCALE, 1);
  set_param_max(PARAM_HUE_VARIATION, 0);
  set_param_max(PARAM_ANIM_SPEED, 0);
  set_param_max(PARAM_ANIM_OFFSET, 0);

  for (int i = 0; i < AGENT_FLAG_MAX; i++) {
    agent_flags[i] = false;
  }

  set_color(Color(1, 1, 1, 1));

  agent_cull_aabb_result.set_page_pool(&agent_cull_aabb_page_pool);

  _update_mesh_texture();
}

AutonomousAgents2D::~AutonomousAgents2D() {
  RS::get_singleton()->free(multimesh);
  RS::get_singleton()->free(mesh);
}
