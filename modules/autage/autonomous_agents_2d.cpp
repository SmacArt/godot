#include "autonomous_agents_2d.h"

#include "core/core_string_names.h"

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

void AutonomousAgents2D::set_draw_order(DrawOrder p_order) {
  draw_order = p_order;
}

void AutonomousAgents2D::set_running(bool p_running) {
  if (p_running != running) {
    running = p_running;
    if (running) {
      set_process_internal(true);
    }
  }
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

int AutonomousAgents2D::get_amount() const {
  return agents.size();
}

AutonomousAgents2D::DrawOrder AutonomousAgents2D::get_draw_order() const{
  return draw_order;
}

bool AutonomousAgents2D::is_running() const {
  return running;
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

			if (!local_coords) {
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
	ClassDB::bind_method(D_METHOD("set_lifetime", "lifetime"), &AutonomousAgents2D::set_lifetime);
	ClassDB::bind_method(D_METHOD("set_pre_process_time", "pre_process_time"), &AutonomousAgents2D::set_pre_process_time);
	ClassDB::bind_method(D_METHOD("set_running", "running"), &AutonomousAgents2D::set_running);
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &AutonomousAgents2D::set_texture);

	ClassDB::bind_method(D_METHOD("get_amount"), &AutonomousAgents2D::get_amount);
	ClassDB::bind_method(D_METHOD("get_lifetime"), &AutonomousAgents2D::get_lifetime);
	ClassDB::bind_method(D_METHOD("get_pre_process_time"), &AutonomousAgents2D::get_pre_process_time);
	ClassDB::bind_method(D_METHOD("is_running"), &AutonomousAgents2D::is_running);
	ClassDB::bind_method(D_METHOD("get_texture"), &AutonomousAgents2D::get_texture);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "running"), "set_running", "is_running");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "amount", PROPERTY_HINT_RANGE, "1,1000000,1,exp"), "set_amount", "get_amount");
  ADD_GROUP("Time","");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime", PROPERTY_HINT_RANGE, "0.01,600.0,0.01,or_greater,suffix:s"), "set_lifetime", "get_lifetime");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "preprocess", PROPERTY_HINT_RANGE, "0.00,600.0,0.01,suffix:s"), "set_pre_process_time", "get_pre_process_time");
  ADD_GROUP("Drawing","");
	// No visibility_rect property contrarily to Particles2D, it's updated automatically.
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "local_coords"), "set_use_local_coordinates", "get_use_local_coordinates");
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
}
AutonomousAgents2D::~AutonomousAgents2D() {
}
