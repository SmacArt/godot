#include "autonomous_agents_2d.h"

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
      iset_process_internal(true);
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
	ClassDB::bind_method(D_METHOD("set_running", "running"), &AutonomousAgents2D::set_running);
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &AutonomousAgents2D::set_texture);

	ClassDB::bind_method(D_METHOD("get_amount"), &AutonomousAgents2D::get_amount);
	ClassDB::bind_method(D_METHOD("get_texture"), &AutonomousAgents2D::get_texture);
	ClassDB::bind_method(D_METHOD("is_running"), &AutonomousAgents2D::is_running);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "running"), "set_running", "is_running");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "amount", PROPERTY_HINT_RANGE, "1,1000000,1,exp"), "set_amount", "get_amount");
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
