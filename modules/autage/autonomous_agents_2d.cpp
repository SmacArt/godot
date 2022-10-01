#include "autonomous_agents_2d.h"

void AutonomousAgents2D::set_running(bool p_running) {
  if (running == p_running) {
    return;
  }
  running = p_running;
  if (running) {
    set_process_internal(true);
  }
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

bool AutonomousAgents2D::is_running() const {
  return running;
}

int AutonomousAgents2D::get_amount() const {
  return agents.size();
}

void AutonomousAgents2D::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_running", "running"), &AutonomousAgents2D::set_running);
	ClassDB::bind_method(D_METHOD("set_amount", "amount"), &AutonomousAgents2D::set_amount);

	ClassDB::bind_method(D_METHOD("is_running"), &AutonomousAgents2D::is_running);
	ClassDB::bind_method(D_METHOD("get_amount"), &AutonomousAgents2D::get_amount);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "running"), "set_running", "is_running");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "amount", PROPERTY_HINT_RANGE, "1,1000000,1,exp"), "set_amount", "get_amount");
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
