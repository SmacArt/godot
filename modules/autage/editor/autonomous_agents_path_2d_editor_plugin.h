#ifndef AUTONOMOUS_AGENTS_PATH_2D_EDITOR_PLUGIN_H
#define AUTONOMOUS_AGENTS_PATH_2D_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/2d/path_2d.h"
#include "scene/gui/box_container.h"
#include "scene/gui/separator.h"
#include "../autonomous_agents_path_2d.h"

class CanvasItemEditor;
class MenuButton;

class AutonomousAgentsPath2DEditor : public HBoxContainer {
	GDCLASS(AutonomousAgentsPath2DEditor, HBoxContainer);

	CanvasItemEditor *canvas_item_editor = nullptr;
	Panel *panel = nullptr;
	AutonomousAgentsPathNode2D *node = nullptr;

	HBoxContainer *base_hb = nullptr;
	Separator *sep = nullptr;

	enum Mode {
		MODE_CREATE,
		MODE_EDIT,
		MODE_EDIT_CURVE,
		MODE_DELETE,
		ACTION_CLOSE
	};

	Mode mode;
	Button *curve_create = nullptr;
	Button *curve_edit = nullptr;
	Button *curve_edit_curve = nullptr;
	Button *curve_del = nullptr;
	Button *curve_close = nullptr;
	MenuButton *handle_menu = nullptr;

	bool mirror_handle_angle;
	bool mirror_handle_length;
	bool on_edge;

	enum HandleOption {
		HANDLE_OPTION_ANGLE,
		HANDLE_OPTION_LENGTH
	};

	enum Action {
		ACTION_NONE,
		ACTION_MOVING_POINT,
		ACTION_MOVING_IN,
		ACTION_MOVING_OUT,
	};

	Action action;
	int action_point = 0;
	Point2 moving_from;
	Point2 moving_screen_from;
	float orig_in_length = 0.0f;
	float orig_out_length = 0.0f;
	Vector2 edge_point;

	void _mode_selected(int p_mode);
	void _handle_option_pressed(int p_option);

	void _node_visibility_changed();
	friend class AutonomousAgentsPath2DEditorPlugin;

protected:
	void _notification(int p_what);
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	bool forward_gui_input(const Ref<InputEvent> &p_event);
	void forward_canvas_draw_over_viewport(Control *p_overlay);
	void edit(Node *p_path2d);
	AutonomousAgentsPath2DEditor();
};

class AutonomousAgentsPath2DEditorPlugin : public EditorPlugin {
	GDCLASS(AutonomousAgentsPath2DEditorPlugin, EditorPlugin);

	AutonomousAgentsPath2DEditor *path2d_editor = nullptr;

public:
	virtual bool forward_canvas_gui_input(const Ref<InputEvent> &p_event) override { return path2d_editor->forward_gui_input(p_event); }
	virtual void forward_canvas_draw_over_viewport(Control *p_overlay) override { path2d_editor->forward_canvas_draw_over_viewport(p_overlay); }

	virtual String get_name() const override { return "AutonomousAgentsPathNode2D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	AutonomousAgentsPath2DEditorPlugin();
	~AutonomousAgentsPath2DEditorPlugin();
};


#endif
