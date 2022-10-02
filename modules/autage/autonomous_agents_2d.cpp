#include "autonomous_agents_2d.h"

#include "core/core_string_names.h"

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

void AutonomousAgents2D::set_amount(int p_amount) {
	ERR_FAIL_COND_MSG(p_amount < 1, "Amount of agents must be greater than 0.");

  agents.resize(p_amount);
  {
    Agent *a = agents.ptrw();
    for (int i = 0; i < p_amount; i++) {
      a[i].active = false;
    }
    agent_data.resize((8+4+4) * p_amount);  // 8 transform, 4 color, 4 custom

    RS::get_singleton()->multimesh_allocate_data(multimesh, p_amount, RS::MULTIMESH_TRANSFORM_2D, true, true);

    agent_order.resize(p_amount);
  }
}
int AutonomousAgents2D::get_amount() const {
  return agents.size();
}

void AutonomousAgents2D::set_draw_order(DrawOrder p_order) {
  draw_order = p_order;
}
AutonomousAgents2D::DrawOrder AutonomousAgents2D::get_draw_order() const{
  return draw_order;
}

void AutonomousAgents2D::set_explosiveness_ratio(real_t p_ratio){
  explosiveness_ratio = p_ratio;
}
real_t AutonomousAgents2D::get_explosiveness_ratio() const {
  return explosiveness_ratio;
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

void AutonomousAgents2D::set_lifetime(double p_lifetime) {
  lifetime = p_lifetime;
}
double AutonomousAgents2D::get_lifetime() const {
  return lifetime;
}

void AutonomousAgents2D::set_lifetime_randomness(double p_lifetime_randomness) {
  lifetime_randomness = p_lifetime_randomness;
}
double AutonomousAgents2D::get_lifetime_randonmess() const {
  return lifetime_randomness;
}

void AutonomousAgents2D::set_running(bool p_running) {
  if (p_running != running) {
    running = p_running;
    if (running) {
      set_process_internal(true);
    }
  }
}
bool AutonomousAgents2D::is_running() const {
  return running;
}

void AutonomousAgents2D::set_speed_scale(double p_speed_scale) {
	speed_scale = p_speed_scale;
}
double AutonomousAgents2D::get_speed_scale() const {
	return speed_scale;
}

void AutonomousAgents2D::set_texture(const Ref<Texture2D> &p_texture) {
  if (p_texture != texture) {
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
}

void AutonomousAgents2D::set_time_randomness_ratio(real_t p_ratio){
  time_randomness_ratio = p_ratio;
}
real_t AutonomousAgents2D::get_time_randomness_ratio() const {
  return time_randomness_ratio;
}

void AutonomousAgents2D::set_use_local_coordinates(bool p_use_local) {
  use_local_coordinates = p_use_local;
}
bool AutonomousAgents2D::is_using_local_coordinates() const {
  return use_local_coordinates;
}

void AutonomousAgents2D::_agents_process(double p_delta) {
	p_delta *= speed_scale;

	int agent_count = agents.size();
	Agent *agent = agents.ptrw();

	Agent *agent_array = agent;

	double prev_time = time;
	time += p_delta;
	if (time > lifetime) {
		time = Math::fmod(time, lifetime);
		cycle++;
		if (cycle > 0) {
			set_running(false);
			notify_property_list_changed();
		}
	}

	Transform2D agent_xform;
	Transform2D velocity_xform;
	if (!use_local_coordinates) {
		agent_xform = get_global_transform();
		velocity_xform = agent_xform;
		velocity_xform[2] = Vector2();
	}

	double system_phase = time / lifetime;

	for (int i = 0; i < agent_count; i++) {
		Agent &a = agent_array[i];

		if (!running && !a.active) {
			continue;
		}

		double local_delta = p_delta;

		// The phase is a ratio between 0 (birth) and 1 (end of life) for each agent.
		// While we use time in tests later on, for randomness we use the phase as done in the
		// original shader code, and we later multiply by lifetime to get the time.

		double restart_phase = double(i) / double(agent_count);

		if (time_randomness_ratio > 0.0) {
			uint32_t seed = cycle;
			if (restart_phase >= system_phase) {
				seed -= uint32_t(1);
			}
			seed *= uint32_t(agent_count);
			seed += uint32_t(i);
			double random = double(idhash(seed) % uint32_t(65536)) / 65536.0;
			restart_phase += time_randomness_ratio * random * 1.0 / double(agent_count);
		}

		restart_phase *= (1.0 - explosiveness_ratio);
		double restart_time = restart_phase * lifetime;
		bool restart = false;

		if (time > prev_time) {
			// restart_time >= prev_time is used so particles emit in the first frame they are processed

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

		if (a.time * (1.0 - explosiveness_ratio) > a.lifetime) {
			restart = true;
		}
		float tv = 0.0;

		if (restart) {
			if (!running) {
				a.active = false;
				continue;
			}
			a.active = true;

			real_t tex_angle = 0.0;
			if (curve_parameters[PARAM_ANGLE].is_valid()) {
				tex_angle = curve_parameters[PARAM_ANGLE]->sample(tv);
			}

			real_t tex_anim_offset = 0.0;
			if (curve_parameters[PARAM_ANGLE].is_valid()) {
				tex_anim_offset = curve_parameters[PARAM_ANGLE]->sample(tv);
			}

			a.seed = Math::rand();

			a.angle_rand = Math::randf();
			a.scale_rand = Math::randf();
			a.hue_rot_rand = Math::randf();
			a.anim_offset_rand = Math::randf();

			if (color_initial_ramp.is_valid()) {
				a.start_color_rand = color_initial_ramp->get_color_at_offset(Math::randf());
			} else {
				a.start_color_rand = Color(1, 1, 1, 1);
			}

			real_t angle1_rad = direction.angle() + Math::deg_to_rad((Math::randf() * 2.0 - 1.0) * spread);
			Vector2 rot = Vector2(Math::cos(angle1_rad), Math::sin(angle1_rad));
			a.velocity = rot * Math::lerp(parameters_min[PARAM_INITIAL_LINEAR_VELOCITY], parameters_max[PARAM_INITIAL_LINEAR_VELOCITY], (real_t)Math::randf());

			real_t base_angle = tex_angle * Math::lerp(parameters_min[PARAM_ANGLE], parameters_max[PARAM_ANGLE], a.angle_rand);
			a.rotation = Math::deg_to_rad(base_angle);

			a.custom[0] = 0.0; // unused
			a.custom[1] = 0.0; // phase [0..1]
			a.custom[2] = tex_anim_offset * Math::lerp(parameters_min[PARAM_ANIM_OFFSET], parameters_max[PARAM_ANIM_OFFSET], a.anim_offset_rand);
			a.custom[3] = 0.0;
			a.transform = Transform2D();
			a.time = 0;
			a.lifetime = lifetime * (1.0 - Math::randf() * lifetime_randomness);
			a.base_color = Color(1, 1, 1, 1);

			switch (emission_shape) {
				case EMISSION_SHAPE_POINT: {
					//do none
				} break;
				case EMISSION_SHAPE_SPHERE: {
					real_t t = Math_TAU * Math::randf();
					real_t radius = emission_sphere_radius * Math::randf();
					a.transform[2] = Vector2(Math::cos(t), Math::sin(t)) * radius;
				} break;
				case EMISSION_SHAPE_SPHERE_SURFACE: {
					real_t s = Math::randf(), t = Math_TAU * Math::randf();
					real_t radius = emission_sphere_radius * Math::sqrt(1.0 - s * s);
					a.transform[2] = Vector2(Math::cos(t), Math::sin(t)) * radius;
				} break;
				case EMISSION_SHAPE_RECTANGLE: {
					a.transform[2] = Vector2(Math::randf() * 2.0 - 1.0, Math::randf() * 2.0 - 1.0) * emission_rect_extents;
				} break;
				case EMISSION_SHAPE_POINTS:
				case EMISSION_SHAPE_DIRECTED_POINTS: {
					int pc = emission_points.size();
					if (pc == 0) {
						break;
					}

					int random_idx = Math::rand() % pc;

					a.transform[2] = emission_points.get(random_idx);

					if (emission_shape == EMISSION_SHAPE_DIRECTED_POINTS && emission_normals.size() == pc) {
						Vector2 normal = emission_normals.get(random_idx);
						Transform2D m2;
						m2.columns[0] = normal;
						m2.columns[1] = normal.orthogonal();
						a.velocity = m2.basis_xform(a.velocity);
					}

					if (emission_colors.size() == pc) {
						a.base_color = emission_colors.get(random_idx);
					}
				} break;
				case EMISSION_SHAPE_MAX: { // Max value for validity check.
					break;
				}
			}

			if (!use_local_coordinates) {
				a.velocity = velocity_xform.xform(a.velocity);
				a.transform = agent_xform * a.transform;
			}

		} else if (!a.active) {
			continue;
		} else if (a.time > a.lifetime) {
			a.active = false;
			tv = 1.0;
		} else {
			uint32_t alt_seed = a.seed;

			a.time += local_delta;
			a.custom[1] = a.time / lifetime;
			tv = a.time / a.lifetime;

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

			Vector2 force = gravity;
			Vector2 pos = a.transform[2];

			//apply linear acceleration
			force += a.velocity.length() > 0.0 ? a.velocity.normalized() * tex_linear_accel * Math::lerp(parameters_min[PARAM_LINEAR_ACCEL], parameters_max[PARAM_LINEAR_ACCEL], rand_from_seed(alt_seed)) : Vector2();
			//apply radial acceleration
			Vector2 org = agent_xform[2];
			Vector2 diff = pos - org;
			force += diff.length() > 0.0 ? diff.normalized() * (tex_radial_accel)*Math::lerp(parameters_min[PARAM_RADIAL_ACCEL], parameters_max[PARAM_RADIAL_ACCEL], rand_from_seed(alt_seed)) : Vector2();
			//apply tangential acceleration;
			Vector2 yx = Vector2(diff.y, diff.x);
			force += yx.length() > 0.0 ? yx.normalized() * (tex_tangential_accel * Math::lerp(parameters_min[PARAM_TANGENTIAL_ACCEL], parameters_max[PARAM_TANGENTIAL_ACCEL], rand_from_seed(alt_seed))) : Vector2();
			//apply attractor forces
			a.velocity += force * local_delta;
			//orbit velocity
			real_t orbit_amount = tex_orbit_velocity * Math::lerp(parameters_min[PARAM_ORBIT_VELOCITY], parameters_max[PARAM_ORBIT_VELOCITY], rand_from_seed(alt_seed));
			if (orbit_amount != 0.0) {
				real_t ang = orbit_amount * local_delta * Math_TAU;
				// Not sure why the ParticleProcessMaterial code uses a clockwise rotation matrix,
				// but we use -ang here to reproduce its behavior.
				Transform2D rot = Transform2D(-ang, Vector2());
				a.transform[2] -= diff;
				a.transform[2] += rot.basis_xform(diff);
			}
			if (curve_parameters[PARAM_INITIAL_LINEAR_VELOCITY].is_valid()) {
				a.velocity = a.velocity.normalized() * tex_linear_velocity;
			}

			if (parameters_max[PARAM_DAMPING] + tex_damping > 0.0) {
				real_t v = a.velocity.length();
				real_t damp = tex_damping * Math::lerp(parameters_min[PARAM_DAMPING], parameters_max[PARAM_DAMPING], rand_from_seed(alt_seed));
				v -= damp * local_delta;
				if (v < 0.0) {
					a.velocity = Vector2();
				} else {
					a.velocity = a.velocity.normalized() * v;
				}
			}
			real_t base_angle = (tex_angle)*Math::lerp(parameters_min[PARAM_ANGLE], parameters_max[PARAM_ANGLE], a.angle_rand);
			base_angle += a.custom[1] * lifetime * tex_angular_velocity * Math::lerp(parameters_min[PARAM_ANGULAR_VELOCITY], parameters_max[PARAM_ANGULAR_VELOCITY], rand_from_seed(alt_seed));
			a.rotation = Math::deg_to_rad(base_angle); //angle
			a.custom[2] = tex_anim_offset * Math::lerp(parameters_min[PARAM_ANIM_OFFSET], parameters_max[PARAM_ANIM_OFFSET], a.anim_offset_rand) + tv * tex_anim_speed * Math::lerp(parameters_min[PARAM_ANIM_SPEED], parameters_max[PARAM_ANIM_SPEED], rand_from_seed(alt_seed));
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

		real_t hue_rot_angle = (tex_hue_variation)*Math_TAU * Math::lerp(parameters_min[PARAM_HUE_VARIATION], parameters_max[PARAM_HUE_VARIATION], a.hue_rot_rand);
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
			a.color = color_ramp->get_color_at_offset(tv) * color;
		} else {
			a.color = color;
		}

		Vector3 color_rgb = hue_rot_mat.xform_inv(Vector3(a.color.r, a.color.g, a.color.b));
		a.color.r = color_rgb.x;
		a.color.g = color_rgb.y;
		a.color.b = color_rgb.z;

		a.color *= a.base_color * a.start_color_rand;

		if (particle_flags[PARTICLE_FLAG_ALIGN_Y_TO_VELOCITY]) {
			if (a.velocity.length() > 0.0) {
				a.transform.columns[1] = a.velocity.normalized();
				a.transform.columns[0] = a.transform.columns[1].orthogonal();
			}

		} else {
			a.transform.columns[0] = Vector2(Math::cos(a.rotation), -Math::sin(a.rotation));
			a.transform.columns[1] = Vector2(Math::sin(a.rotation), Math::cos(a.rotation));
		}

		//scale by scale
		Vector2 base_scale = tex_scale * Math::lerp(parameters_min[PARAM_SCALE], parameters_max[PARAM_SCALE], a.scale_rand);
		if (base_scale.x < 0.00001) {
			base_scale.x = 0.00001;
		}
		if (base_scale.y < 0.00001) {
			base_scale.y = 0.00001;
		}
		a.transform.columns[0] *= base_scale.x;
		a.transform.columns[1] *= base_scale.y;

		a.transform[2] += a.velocity * local_delta;
	}
}
void AutonomousAgents2D::_texture_changed() {
	if (texture.is_valid()) {
		queue_redraw();
		_update_mesh_texture();
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

void AutonomousAgents2D::_update_mesh_texture() {
  Size2 tex_size;
  if (texture.is_valid()) {
    tex_size = texture->get_size();
  } else {
    tex_size = Size2(1,1);
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
			inv_global_transform = get_global_transform().affine_inverse();

			if (!use_local_coordinates) {
				int agents_size = agents.size();

				float *agent_data = agent_data.ptrw();
				const Agent *agent = agents.ptr();
				float *ptr = agent_data;

				for (int i = 0; i < pc; i++) {
					Transform2D t = inv_global_transform * r[i].transform;

					if (agent[i].active) {
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

void AutonomousAgents2D::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_amount", "amount"), &AutonomousAgents2D::set_amount);
	ClassDB::bind_method(D_METHOD("set_draw_order", "draw_order"), &AutonomousAgents2D::set_draw_order);
	ClassDB::bind_method(D_METHOD("set_explosiveness_ratio", "explosiveness_ratio"), &AutonomousAgents2D::set_explosiveness_ratio);
	ClassDB::bind_method(D_METHOD("set_fixed_fps", "fixed_fps"), &AutonomousAgents2D::set_fixed_fps);
	ClassDB::bind_method(D_METHOD("set_fractional_delta", "fractional_delta"), &AutonomousAgents2D::set_fractional_delta);
	ClassDB::bind_method(D_METHOD("set_lifetime", "lifetime"), &AutonomousAgents2D::set_lifetime);
	ClassDB::bind_method(D_METHOD("set_lifetime_randomness", "lifetime_randomness"), &AutonomousAgents2D::set_lifetime_randomness);
	ClassDB::bind_method(D_METHOD("set_pre_process_time", "pre_process_time"), &AutonomousAgents2D::set_pre_process_time);
	ClassDB::bind_method(D_METHOD("set_running", "running"), &AutonomousAgents2D::set_running);
	ClassDB::bind_method(D_METHOD("set_speed_scale", "scale"), &AutonomousAgents2D::set_speed_scale);
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &AutonomousAgents2D::set_texture);
	ClassDB::bind_method(D_METHOD("set_time_randomness_ratio", "time_randomness_ratio"), &AutonomousAgents2D::set_time_randomness_ratio);
	ClassDB::bind_method(D_METHOD("set_use_local_coordinates", "use_local_coordinates"), &AutonomousAgents2D::set_use_local_coordinates);

	ClassDB::bind_method(D_METHOD("get_amount"), &AutonomousAgents2D::get_amount);
	ClassDB::bind_method(D_METHOD("get_draw_order"), &AutonomousAgents2D::get_draw_order);
	ClassDB::bind_method(D_METHOD("get_explosiveness_ratio"), &AutonomousAgents2D::get_explosiveness_ratio);
	ClassDB::bind_method(D_METHOD("get_fixed_fps"), &AutonomousAgents2D::get_fixed_fps);
	ClassDB::bind_method(D_METHOD("get_fractional_delta"), &AutonomousAgents2D::get_fractional_delta);
	ClassDB::bind_method(D_METHOD("get_lifetime"), &AutonomousAgents2D::get_lifetime);
	ClassDB::bind_method(D_METHOD("get_lifetime_randomness"), &AutonomousAgents2D::get_lifetime_randomness);
	ClassDB::bind_method(D_METHOD("get_pre_process_time"), &AutonomousAgents2D::get_pre_process_time);
	ClassDB::bind_method(D_METHOD("is_running"), &AutonomousAgents2D::is_running);
	ClassDB::bind_method(D_METHOD("get_speed_scale"), &AutonomousAgents2D::get_speed_scale);
	ClassDB::bind_method(D_METHOD("get_texture"), &AutonomousAgents2D::get_texture);
	ClassDB::bind_method(D_METHOD("get_time_randomness_ratio"), &AutonomousAgents2D::get_time_randomness_ratio);
	ClassDB::bind_method(D_METHOD("is_use_local_coordinates"), &AutonomousAgents2D::is_use_local_coordinates);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "running"), "set_running", "is_running");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "amount", PROPERTY_HINT_RANGE, "1,1000000,1,exp"), "set_amount", "get_amount");
  ADD_GROUP("Time","");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime", PROPERTY_HINT_RANGE, "0.01,600.0,0.01,or_greater,suffix:s"), "set_lifetime", "get_lifetime");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "fixed_fps", PROPERTY_HINT_RANGE, "0,1000,1,suffix:FPS"), "set_fixed_fps", "get_fixed_fps");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "preprocess", PROPERTY_HINT_RANGE, "0.00,600.0,0.01,suffix:s"), "set_pre_process_time", "get_pre_process_time");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed_scale", PROPERTY_HINT_RANGE, "0,64,0.01"), "set_speed_scale", "get_speed_scale");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "explosiveness", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_explosiveness_ratio", "get_explosiveness_ratio");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "randomness", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_time_randomness_ratio", "get_time_randomness_ratio");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_randomness", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_lifetime_randomness", "get_lifetime_randomness");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "fixed_fps", PROPERTY_HINT_RANGE, "0,1000,1,suffix:FPS"), "set_fixed_fps", "get_fixed_fps");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "fract_delta"), "set_fractional_delta", "get_fractional_delta");
  ADD_GROUP("Drawing","");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_local_coordinates"), "set_use_local_coordinates", "get_use_local_coordinates");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "draw_order", PROPERTY_HINT_ENUM, "Index,Lifetime"), "set_draw_order", "get_draw_order");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_texture", "get_texture");

	BIND_ENUM_CONSTANT(DRAW_ORDER_INDEX);
	BIND_ENUM_CONSTANT(DRAW_ORDER_LIFETIME);
}

AutonomousAgents2D::AutonomousAgents2D() {
  mesh = RenderingServer::get_singleton()->mesh_create();
  multimesh = RenderingServer::get_singleton()->multimesh_create();
  RenderingServer::get_singleton()->multimesh_set_mesh(multimesh, mesh);

  set_running(true);
  set_amount(10);
  set_use_local_coordinates(false);

  set_color(Color(1,1,1,1));
  _update_mesh_texture();
}

AutonomousAgents2D::~AutonomousAgents2D() {
	RS::get_singleton()->free(multimesh);
	RS::get_singleton()->free(mesh);
}
