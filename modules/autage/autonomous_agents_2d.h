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

#ifndef AUTONOMOUS_AGENTS_2D_H
#define AUTONOMOUS_AGENTS_2D_H

#include "scene/2d/node_2d.h"
#include "core/math/dynamic_bvh.h"
#include "core/templates/paged_array.h"
#include "autonomous_agents_path_2d.h"
#include "autonomous_agents_steering_behavior.h"

class AutonomousAgents2D : public Node2D {
private:
  GDCLASS(AutonomousAgents2D, Node2D);

public:
  enum DrawOrder {
    DRAW_ORDER_INDEX,
    DRAW_ORDER_LIFETIME
  };

  enum Parameter {
    PARAM_AGENT_MASS,
    PARAM_AGENT_MAX_SPEED,
    PARAM_AGENT_MAX_ACCELERATION,
    PARAM_AGENT_MAX_ROTATION_SPEED,
    PARAM_AGENT_MAX_ANGULAR_ACCELERATION,

    PARAM_ANGLE,
    PARAM_ANGULAR_VELOCITY,
    PARAM_ANIM_OFFSET,
    PARAM_ANIM_SPEED,
    PARAM_DAMPING,
    PARAM_HUE_VARIATION,
    PARAM_INITIAL_LINEAR_VELOCITY,
    PARAM_LINEAR_ACCEL,
    PARAM_ORBIT_VELOCITY,
    PARAM_RADIAL_ACCEL,
    PARAM_SCALE,
    PARAM_TANGENTIAL_ACCEL,

    PARAM_ALIGN_SLOW_RADIUS,
    PARAM_ALIGN_TARGET_RADIUS,
    PARAM_ALIGN_TIME_TO_TARGET,

    PARAM_ARRIVE_SLOW_RADIUS,
    PARAM_ARRIVE_TARGET_RADIUS,
    PARAM_ARRIVE_TIME_TO_TARGET,

    PARAM_COLLISION_AVOIDANCE_DECAY_COEFFICIENT,
    PARAM_COLLISION_AVOIDANCE_BACK_VIEW_DISTANCE_RATIO,
    PARAM_COLLISION_AVOIDANCE_FRONT_VIEW_DISTANCE_RATIO,
    PARAM_COLLISION_AVOIDANCE_SIDE_VIEW_DISTANCE_RATIO,
    PARAM_COLLISION_AVOIDANCE_FIELD_OF_VIEW_ANGLE,
    PARAM_COLLISION_AVOIDANCE_FIELD_OF_VIEW_DISTANCE,
    PARAM_COLLISION_AVOIDANCE_FIELD_OF_VIEW_OFFSET,

    PARAM_EVADE_MAX_PREDICTION,

    PARAM_FLOCK_NEIGHBORHOOD_DISTANCE,
    PARAM_FLOCK_COHESION_WEIGHT,
    PARAM_FLOCK_SEPARATION_WEIGHT,
    PARAM_FLOCK_VELOCITY_MATCHING_WEIGHT,
    PARAM_FLOCK_WANDER_WEIGHT,

    PARAM_PATH_FOLLOWING_PREDICTION_DISTANCE,
    PARAM_PATH_FOLLOWING_START_DISTANCE,

    PARAM_PURSUE_MAX_PREDICTION,

    PARAM_SEPARATION_DECAY_COEFFICIENT,
    PARAM_SEPARATION_THRESHOLD,

    PARAM_VELOCITY_MATCHING_TIME_TO_TARGET,

    PARAM_WANDER_OFFSET,
    PARAM_WANDER_RADIUS,
    PARAM_WANDER_RATE,

    PARAM_MAX,
  };

  enum AgentFlag {
    AGENT_FLAG_ALIGN,
    AGENT_FLAG_ARRIVE,
    AGENT_FLAG_EVADE,
    AGENT_FLAG_FACE,
    AGENT_FLAG_FLEE,
    AGENT_FLAG_FLOCK,
    AGENT_FLAG_LOOK_WHERE_YOURE_GOING,
    AGENT_FLAG_COLLISION_AVOIDANCE,
    AGENT_FLAG_COLLISION_AVOIDANCE_FOV_SCALE_TO_SIZE,
    AGENT_FLAG_REMOTELY_CONTROLLED,
    AGENT_FLAG_PATH_FOLLOWING,
    AGENT_FLAG_PURSUE,
    AGENT_FLAG_SEEK,
    AGENT_FLAG_SEPARATION,
    AGENT_FLAG_VELOCITY_MATCHING,
    AGENT_FLAG_WANDER,
    AGENT_FLAG_ALIGN_ROTATION_TO_VELOCITY,
    AGENT_FLAG_MAX
  };

  enum EmissionShape {
    EMISSION_SHAPE_POINT,
    EMISSION_SHAPE_SPHERE,
    EMISSION_SHAPE_SPHERE_SURFACE,
    EMISSION_SHAPE_RECTANGLE,
    EMISSION_SHAPE_POINTS,
    EMISSION_SHAPE_DIRECTED_POINTS,
    EMISSION_SHAPE_MAX
  };

  struct Agent {

    uint32_t seed = 0;

    bool is_new = true;
    uint32_t ai_phase = 0;

    Transform2D transform;
    real_t mass = 1.0;
    real_t max_speed = 0.0;
    real_t max_acceleration = 0.0;
    real_t max_rotation_speed = 0.0;
    real_t max_angular_acceleration = 0.0;
    Color color;
    real_t custom[4] = {};
    bool active = false;
    bool steering = false;
    real_t angle_rand = 0.0;
    real_t scale_rand = 0.0;
    real_t hue_rot_rand = 0.0;
    real_t anim_offset_rand = 0.0;
    Color start_color_rand;
    double time = 0.0;
    double lifetime = 0.0;
    Color base_color;

    DynamicBVH::ID bvh_leaf;
    AABB aabb;
    AABB aabb_collision_avoidance;
    real_t current_width;
    real_t current_height;

    Vector2 velocity;
    real_t rotation_velocity = 0.0;
    real_t rotation = 0.0;
    bool align_rotation_to_velocity = false;

    SteeringBehaviorFlag steering_behavior;

    real_t align_target_radius = 0.0;
    real_t align_slow_radius = 0.0;
    real_t align_time_to_target = 0.0;

    real_t arrive_target_radius = 0.0;
    real_t arrive_slow_radius = 0.0;
    real_t arrive_time_to_target = 0.0;

    real_t evade_max_prediction = 0.0;

    real_t collision_avoidance_decay_coefficient = 0.0;
    real_t collision_avoidance_back_view_distance_ratio = 0.0;
    real_t collision_avoidance_front_view_distance_ratio = 1.0;
    real_t collision_avoidance_side_view_distance_ratio = 0.0;

    real_t collision_avoidance_field_of_view_angle = 0.0;
    real_t collision_avoidance_field_of_view_min_distance = 0.0;
    real_t collision_avoidance_field_of_view_max_distance = 0.0;
    real_t collision_avoidance_field_of_view_distance = 0.0;
    real_t collision_avoidance_field_of_view_base_distance = 0.0;
    real_t collision_avoidance_field_of_view_offset = 0.0;
    real_t collision_avoidance_field_of_view_base_offset = 0.0;
    Vector2 collision_avoidance_field_of_view_left_angle;
    Vector2 collision_avoidance_field_of_view_right_angle;
    bool collision_avoidance_fov_scale_to_size = false;

    real_t flock_neighborhood_distance = 0.0;
    real_t flock_cohesion_weight = 1.0;
    real_t flock_separation_weight = 1.0;
    real_t flock_velocity_matching_weight = 1.0;
    real_t flock_wander_weight = 1.0;

    Ref<AutonomousAgentsPath2D> path_following_path;
    real_t path_following_prediction_distance = 0.0;
    real_t path_following_distance = 0.0;
    AutonomousAgentsPath2D::FollowDirection path_following_direction = AutonomousAgentsPath2D::FollowDirection::FOLLOW_DIRECTION_FORWARDS;

    real_t pursue_max_prediction = 0.0;
    SteeringBehavior pursue_delegate_steering_behavior;

    real_t separation_threshold = 0.0;
    real_t separation_decay_coefficient = 0.0;

    real_t velocity_matching_time_to_target = 0.0;

    real_t wander_offset = 0.0;
    real_t wander_radius = 0.0;
    real_t wander_rate = 0.0;
    real_t wander_target_theta = 0.0;

    int32_t target_agent = -1;

#ifdef DEBUG_ENABLED
    Vector2 steering_output_linear;
    Vector2 wander_circle_position;
    Vector2 wander_target;
    bool aabb_culled = false;
    AABB separation_aabb;
    AABB flock_aabb;
    AABB collision_avoidance_avoiding_aabb;
    AABB collision_avoidance_fov_aabb;
    Vector2 path_following_predicted_position;
    Vector2 path_following_normal_position;
    Vector2 path_following_segment_start;
    Vector2 path_following_segment_end;
    AABB predicted_position_aabb;
    Vector2 collision_avoidance_fov_start_position;
    Vector2 collision_avoidance_fov_left_position;
    Vector2 collision_avoidance_fov_right_position;
    Vector2 collision_avoidance_fov_left_end_position;
    Vector2 collision_avoidance_fov_right_end_position;
    Vector2 collision_avoidance_predicted_position;
    real_t align_target;
    Vector2 arrive_target;
    Vector2 evade_target;
    Vector2 face_target;
    Vector2 flee_target;
    Vector2 pursue_target;
    Vector2 seek_target;
    Vector2 velocity_matching_target;
    bool aligning_in_slow_radius = false;
    bool aligning_in_target_radius = false;
    bool arriving_in_slow_radius = false;
    bool arriving_in_target_radius = false;
    bool did_align = false;
    bool did_arrive = false;
    bool did_evade = false;
    bool did_face = false;
    bool did_flee = false;
    bool did_flock = false;
    bool did_path_following = false;
    bool did_pursue = false;
    bool did_seek = false;
    bool did_velocity_matching = false;
    bool did_wander = false;
#endif

  };

private:

  struct SteeringOutput {
    Vector2 linear;
    real_t angular;

    inline SteeringOutput() {
      linear.x = 0.0f;
      linear.y = 0.0f;
      angular = 0.0f;
    }

    inline SteeringOutput(const Vector2 p_linear, const double p_angular) {
      linear = p_linear;
      angular = p_angular;
    }

    inline SteeringOutput operator+(const SteeringOutput &steering_output) const{
      return SteeringOutput(Vector2(linear + steering_output.linear), angular + steering_output.angular);
    }

    inline SteeringOutput &operator+=(const SteeringOutput &steering_output) {
      linear += steering_output.linear;
      angular += steering_output.angular;

      return *this;
    }

    inline SteeringOutput operator-(const SteeringOutput &steering_output) const{
      return SteeringOutput(Vector2(linear - steering_output.linear), angular - steering_output.angular);
    }

    inline SteeringOutput &operator-=(const SteeringOutput &steering_output) {
      linear -= steering_output.linear;
      angular -= steering_output.angular;

      return *this;
    }

    inline SteeringOutput operator*(const double multiplier) const{
      return SteeringOutput(Vector2(linear * multiplier), angular * multiplier);
    }

    inline SteeringOutput &operator*=(const double multiplier) {
      linear *= multiplier;
      angular *= multiplier;

      return *this;
    }
  };

  bool running = false;
  bool use_bvh = false;

  double time = 0.0;
  double inactive_time = 0.0;
  double frame_remainder = 0.0;
  int cycle = 0;
  bool do_redraw = false;
  uint32_t ai_phase = 1;
  Transform2D old_transform;

  RID mesh;
  RID multimesh;

  Vector<Agent> agents;
  Vector<float> agent_data;
  Vector<int> agent_order;

  Agent *agents_arr = nullptr;

  struct SortLifetime {
    const Agent *agents = nullptr;

    bool operator()(int p_a, int p_b) const {
      return agents[p_a].time > agents[p_b].time;
    }
  };

  struct SortAxis {
    const Agent *agents = nullptr;
    Vector2 axis;
    bool operator()(int p_a, int p_b) const {
      return axis.dot(agents[p_a].transform[2]) < axis.dot(agents[p_b].transform[2]);
    }
  };

  //

  bool one_shot = false;

  double lifetime = 1.0;
  double pre_process_time = 0.0;
  real_t explosiveness_ratio = 0.0;
  real_t randomness_ratio = 0.0;
  double lifetime_randomness = 0.0;
  double speed_scale = 1.0;
  bool local_coords = false;
  int fixed_fps = 0;
  bool fractional_delta = true;
  double behaviour_delay = 0.0;
  uint32_t number_of_ai_phases = 1;

  Transform2D inv_emission_transform;

  DrawOrder draw_order = DRAW_ORDER_INDEX;

  Ref<Texture2D> texture;

  ////////

  Vector2 direction = Vector2(1, 0);
  real_t spread = 45.0;
  real_t half_pi = Math_PI * 0.5;
  real_t two_pi = Math_PI * 2.0;

  real_t parameters_min[PARAM_MAX];
  real_t parameters_max[PARAM_MAX];

  Ref<Curve> curve_parameters[PARAM_MAX];
  Color color;
  Ref<Gradient> color_ramp;
  Ref<Gradient> color_initial_ramp;

  bool agent_flags[AGENT_FLAG_MAX];

  EmissionShape emission_shape = EMISSION_SHAPE_POINT;
  real_t emission_sphere_radius = 1.0;
  Vector2 emission_rect_extents = Vector2(1, 1);
  Vector<Vector2> emission_points;
  Vector<Vector2> emission_normals;
  Vector<Color> emission_colors;
  int emission_point_count = 0;

  Ref<Curve> scale_curve_x;
  Ref<Curve> scale_curve_y;
  bool split_scale = false;

  Vector2 gravity = Vector2(0, 980);
  Size2 agent_base_size = Size2(10, 10);
  double agent_aabb_expansion_ratio = 1.2;
  double agent_collision_base_speed = 100.0;

  int pursue_delegate_steering_behavior = 0;
  int look_where_yourre_going_delegate_steering_behavior = 0;
  Ref<AutonomousAgentsPath2D> path_following_path;
  AutonomousAgentsPath2D::FollowDirection path_following_direction;

  void _update_internal();
  void _agents_process(double p_delta);
  void _update_agent_data_buffer();

  Mutex update_mutex;

  void _update_render_thread();

  void _update_mesh_texture();

  void _set_do_redraw(bool p_do_redraw);

  void _texture_changed();

  void apply_steering_behaviors(Agent *agent, int index, double delta);
  SteeringOutput align(Agent *agent, double delta);
  SteeringOutput align(Agent *agent, double target_rotation, double delta);
  SteeringOutput arrive(Agent *agent, double delta);
  SteeringOutput arrive(Agent *agent, Vector2 target, double delta);
  SteeringOutput evade(Agent *agent);
  SteeringOutput evade(Agent *agent, Vector2 target_position, Vector2 target_velocity);
  SteeringOutput face(Agent *agent, double delta);
  SteeringOutput face(Agent *agent, Vector2 target_position, double delta);
  SteeringOutput flee(Agent *agent);
  SteeringOutput flee(Agent *agent, Vector2 target);
  SteeringOutput flock(Agent *agent, double delta);
  SteeringOutput look_where_youre_going(Agent *agent, double delta);
  SteeringOutput collision_avoidance(Agent *agent, double delta);
  SteeringOutput path_following(Agent *agent, double delta);
  SteeringOutput pursue(Agent *agent, double delta);
  SteeringOutput pursue(Agent *agent, Vector2 target_position, Vector2 target_velocity, double delta);
  SteeringOutput seek(Agent *agent);
  SteeringOutput seek(Agent *agent, Vector2 target);
  SteeringOutput separation(Agent *agent);
  SteeringOutput velocity_matching(Agent *agent, double delta);
  SteeringOutput velocity_matching(Agent *agent, Vector2 target, double delta);
  SteeringOutput wander(Agent *agent, double delta);

  DynamicBVH agent_bvh;
  void agent_cull_aabb_query(const AABB &p_aabb);
  void agent_cull_convext_query();
  void create_avoidance_aabb_for_agent(Agent *agent);

  template <class QueryResult>
  _FORCE_INLINE_ void aabb_query(const AABB &p_aabb, QueryResult &r_result);

#ifdef DEBUG_ENABLED
  bool is_debug = false;
#endif

protected:
  static void _bind_methods();
  void _notification(int p_what);
  void _validate_property(PropertyInfo &p_property) const;

public:
  void set_running(bool p_running);
  void set_number_of_agents(int p_number_of_agents);
  void set_lifetime(double p_lifetime);
  void set_pre_process_time(double p_time);
  void set_one_shot(bool p_one_shot);
  void set_explosiveness_ratio(real_t p_ratio);
  void set_randomness_ratio(real_t p_ratio);
  void set_lifetime_randomness(double p_random);
  void set_use_local_coordinates(bool p_enable);
  void set_speed_scale(double p_scale);
  void set_behaviour_delay(double p_delay);
  void set_number_of_ai_phases(int p_number_of_phases);
  void set_use_bvh(bool p_use_bvh);

  bool is_running() const;
  int get_number_of_agents() const;
  double get_lifetime() const;
  bool get_one_shot() const;
  double get_pre_process_time() const;
  real_t get_explosiveness_ratio() const;
  real_t get_randomness_ratio() const;
  double get_lifetime_randomness() const;
  bool get_use_local_coordinates() const;
  double get_speed_scale() const;
  double get_behaviour_delay() const;
  int get_number_of_ai_phases() const;
  bool is_using_bvh() const;

  void set_fixed_fps(int p_count);
  int get_fixed_fps() const;

  void set_fractional_delta(bool p_enable);
  bool get_fractional_delta() const;

  void set_draw_order(DrawOrder p_order);
  DrawOrder get_draw_order() const;

  void set_texture(const Ref<Texture2D> &p_texture);
  Ref<Texture2D> get_texture() const;

  ///////////////////

  void set_direction(Vector2 p_direction);
  Vector2 get_direction() const;

  void set_spread(real_t p_spread);
  real_t get_spread() const;

  void set_param_min(Parameter p_param, real_t p_value);
  real_t get_param_min(Parameter p_param) const;

  void set_param_max(Parameter p_param, real_t p_value);
  real_t get_param_max(Parameter p_param) const;

  void set_param_curve(Parameter p_param, const Ref<Curve> &p_curve);
  Ref<Curve> get_param_curve(Parameter p_param) const;

  void set_color(const Color &p_color);
  Color get_color() const;

  void set_color_ramp(const Ref<Gradient> &p_ramp);
  Ref<Gradient> get_color_ramp() const;

  void set_color_initial_ramp(const Ref<Gradient> &p_ramp);
  Ref<Gradient> get_color_initial_ramp() const;

  void set_agent_flag(AgentFlag p_agent_flag, bool p_enable);
  bool get_agent_flag(AgentFlag p_agent_flag) const;

  void set_emission_shape(EmissionShape p_shape);
  void set_emission_sphere_radius(real_t p_radius);
  void set_emission_rect_extents(Vector2 p_extents);
  void set_emission_points(const Vector<Vector2> &p_points);
  void set_emission_normals(const Vector<Vector2> &p_normals);
  void set_emission_colors(const Vector<Color> &p_colors);
  void set_scale_curve_x(Ref<Curve> p_scale_curve);
  void set_scale_curve_y(Ref<Curve> p_scale_curve);
  void set_split_scale(bool p_split_scale);

  EmissionShape get_emission_shape() const;
  real_t get_emission_sphere_radius() const;
  Vector2 get_emission_rect_extents() const;
  Vector<Vector2> get_emission_points() const;
  Vector<Vector2> get_emission_normals() const;
  Vector<Color> get_emission_colors() const;
  Ref<Curve> get_scale_curve_x() const;
  Ref<Curve> get_scale_curve_y() const;
  bool get_split_scale();

  void set_gravity(const Vector2 &p_gravity);
  Vector2 get_gravity() const;

  void set_agent_base_size(const Size2 &p_size);
  Size2 get_agent_base_size() const;

  void set_agent_collision_base_speed(const double p_base_speed);
  double get_agent_collision_base_speed() const;

  void set_agent_aabb_expansion_ratio(const double p_ratio);
  double get_agent_aabb_expansion_ratio() const;

  PackedStringArray get_configuration_warnings() const override;

  PagedArrayPool<Agent *> agent_cull_aabb_page_pool;
  PagedArray<Agent *> agent_cull_aabb_result;

  bool is_agent_steering(int index);
  void set_behavior(Agent *agent, SteeringBehavior behavior, bool is_on);
  void set_agent_behavior(int index, SteeringBehavior behavior);
  void unset_agent_behavior(int index, SteeringBehavior behavior);
  void clear_agent_behavior(int index);
  bool has_agent_behavior(int index, SteeringBehavior behavior);
  void set_agent_target_agent(int index, int index_to_target);
  Vector2 get_agent_position(int index);
  void set_agent_position_from_remote(int index, Vector2 position);
  void set_agent_orientation_from_remote(int index, real_t orientation);

  void setup_agent_with_align(Agent *agent);
  void setup_agent_with_arrive(Agent *agent);
  void setup_agent_with_evade(Agent *agent);
  void setup_agent_with_face(Agent *agent);
  void setup_agent_with_flock(Agent *agent);
  void setup_agent_with_look_where_youre_going(Agent *agent);
  void setup_agent_with_collision_avoidance(Agent *agent);
  void setup_agent_with_path_following(Agent *agent);
  void setup_agent_with_pursue(Agent *agent);
  void setup_agent_with_separation(Agent *agent);
  void setup_agent_with_velocity_matching(Agent *agent);
  void setup_agent_with_wander(Agent *agent);

  void set_pursue_delegate_steering_behavior(int p_behavior);
  int get_pursue_delegate_steering_behavior() const;

  void set_path_following_path(const Ref<AutonomousAgentsPath2D> &p_path);
  Ref<AutonomousAgentsPath2D> get_path_following_path() const ;
  void set_path_following_direction(const AutonomousAgentsPath2D::FollowDirection p_direction);
  AutonomousAgentsPath2D::FollowDirection get_path_following_direction() const;

#ifdef DEBUG_ENABLED
  bool is_debugging() {return is_debug;};
  void set_is_debug(bool p_is_debug) {is_debug = p_is_debug;}
  Vector2 get_agent_steering_output_linear(int index);
  AABB get_agent_aabb(int index);
  AABB get_agent_separation_aabb(int index);
  AABB get_agent_collision_avoidance_avoiding_aabb(int index);
  AABB get_agent_collision_avoidance_fov_aabb(int index);
  AABB get_agent_predicted_position_aabb(int index);
  Vector2 get_agent_collision_avoidance_fov_start_position(int index);
  Vector2 get_agent_collision_avoidance_fov_left_position(int index);
  Vector2 get_agent_collision_avoidance_fov_right_position(int index);
  Vector2 get_agent_collision_avoidance_fov_left_end_position(int index);
  Vector2 get_agent_collision_avoidance_fov_right_end_position(int index);
  Vector2 get_agent_collision_avoidance_predicted_position(int index);
  Vector2 get_agent_collision_avoidance_normal_position(int index);
  AABB get_agent_flock_aabb(int index);
  Vector2 get_agent_wander_circle_position(int index);
  real_t get_agent_wander_radius(int index);
  Vector2 get_agent_wander_target(int index);
  bool is_agent_aabb_culled(int index);
  int get_agent_ai_phase(int index);
  bool get_did_agent_wander(int index);
  bool get_did_agent_path_following(int index);
  bool get_did_agent_pursue(int index);
  Vector2 get_agent_pursue_target(int index);
  bool get_did_agent_seek(int index);
  Vector2 get_agent_seek_target(int index);
  bool get_did_agent_evade(int index);
  Vector2 get_agent_evade_target(int index);
  bool get_did_agent_face(int index);
  Vector2 get_agent_face_target(int index);
  bool get_did_agent_flee(int index);
  Vector2 get_agent_flee_target(int index);
  bool get_did_agent_flock(int index);
  bool get_did_agent_align(int index);
  real_t get_agent_align_target(int index);
  bool is_agent_aligning_in_slow_radius(int index);
  bool is_agent_aligning_in_target_radius(int index);
  real_t get_agent_align_slow_radius(int index);
  real_t get_agent_align_target_radius(int index);
  bool get_did_agent_arrive(int index);
  Vector2 get_agent_arrive_target(int index);
  bool is_agent_arriving_in_slow_radius(int index);
  bool is_agent_arriving_in_target_radius(int index);
  real_t get_agent_arrive_slow_radius(int index);
  real_t get_agent_arrive_target_radius(int index);
  bool get_did_agent_velocity_matching(int index);
  Vector2 get_agent_velocity_matching_target(int index);
  Vector2 get_agent_path_following_predicted_position(int index);
  Vector2 get_agent_path_following_normal_position(int index);
  Vector2 get_agent_path_following_segment_start(int index);
  Vector2 get_agent_path_following_segment_end(int index);

#endif

  void restart();

  AutonomousAgents2D();
  ~AutonomousAgents2D();

  inline double map_orientation_to_pi_randian_range(const double orientation) {
    double r = fmod(orientation, two_pi);
    if (Math::abs(r) > Math_PI) {
      if (r < 0.0f)
        r += two_pi;
      else
        r -= two_pi;
    }
    return r;
  }

  inline double short_angle_distance(const double from, const double to) {
    double difference = fmod(to - from, two_pi);
    return fmod(2.0 * difference, two_pi) - difference;
  }

  inline bool aabbs_intersect(const AABB &aabb1, const AABB &aabb2) {
    if (aabb1.position.x >= (aabb2.position.x + aabb2.size.x)) {
      return false;
    }
    if ((aabb1.position.x + aabb1.size.x) <= aabb2.position.x) {
      return false;
    }
    if (aabb1.position.y >= (aabb2.position.y + aabb2.size.y)) {
      return false;
    }
    if ((aabb1.position.y + aabb1.size.y) <= aabb2.position.y) {
      return false;
    }
    return true;
  }

  inline AABB aabbs_intersection(const AABB &aabb1, const AABB &aabb2) const {
    Vector3 src_min = aabb1.position;
    Vector3 src_max = aabb1.position + aabb1.size;
    Vector3 dst_min = aabb2.position;
    Vector3 dst_max = aabb2.position +aabb2.size;

    Vector3 min, max;

    if (src_min.x > dst_max.x || src_max.x < dst_min.x) {
      return AABB();
    } else {
      min.x = (src_min.x > dst_min.x) ? src_min.x : dst_min.x;
      max.x = (src_max.x < dst_max.x) ? src_max.x : dst_max.x;
    }

    if (src_min.y > dst_max.y || src_max.y < dst_min.y) {
      return AABB();
    } else {
      min.y = (src_min.y > dst_min.y) ? src_min.y : dst_min.y;
      max.y = (src_max.y < dst_max.y) ? src_max.y : dst_max.y;
    }

    return AABB(min, max - min);
  }

  inline double distance_between_aabbs(const AABB &aabb1, const AABB &aabb2) {
    Vector3 c1 = aabb1.get_center();
    Vector3 c2 = aabb2.get_center();
    double w1 = aabb1.get_size().x;
    double w2 = aabb2.get_size().x;
    double h1 = aabb1.get_size().y;
    double h2 = aabb2.get_size().y;
    return fmax( Math::abs(c1.x-c2.x) - (w1+w2) * 0.5, Math::abs(c1.y-c2.y) - (h1+h2) * 0.5);
  }

  inline Vector2 get_normal_point(const Vector2 p, const Vector2 a, const Vector2 b) const {
    /*
    Vector2 ap = p -a;
    Vector2 ab = (b-a).normalized();
    ab = ab * (ap.dot(ab));
    return a + ab;
    */
    Vector2 ab = (b - a).normalized();
    return a + (ab * ((p-a).dot(ab)));
  }
};

VARIANT_ENUM_CAST(AutonomousAgents2D::DrawOrder)
VARIANT_ENUM_CAST(AutonomousAgents2D::Parameter)
VARIANT_ENUM_CAST(AutonomousAgents2D::AgentFlag)
VARIANT_ENUM_CAST(AutonomousAgents2D::EmissionShape)
VARIANT_ENUM_CAST(SteeringBehavior)

#endif // AUTONOMOUS_AGENTS_2D_H
