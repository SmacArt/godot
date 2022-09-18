/* smetch.cpp */

#include "smetch.h"
#include "servers/rendering/rendering_server_globals.h"
#include <math.h>
#include <iostream>

void Smetch::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_parameters", "SmetchParameters"), &Smetch::set_parameters);
	ClassDB::bind_method(D_METHOD("get_parameters"), &Smetch::get_parameters);
	ClassDB::bind_method(D_METHOD("set_content", "SmetchContent"), &Smetch::set_content);
	ClassDB::bind_method(D_METHOD("get_content"), &Smetch::get_content);

	ClassDB::bind_method(D_METHOD("color_mode", "mode", "value1", "value2", "value3", "value4"), &Smetch::color_mode, DEFVAL(RGB), DEFVAL(-1), DEFVAL(-1), DEFVAL(-1), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("prime_color", "value1", "value2", "value3", "value4"), &Smetch::prime_color, DEFVAL(Color()), DEFVAL(1), DEFVAL(-1), DEFVAL(-1), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("fill", "value1", "value2", "value3", "value4"), &Smetch::fill, DEFVAL(1), DEFVAL(1), DEFVAL(1), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("background", "value1", "value2", "value3"), &Smetch::background, DEFVAL(1), DEFVAL(-1), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("stroke", "value1", "value2", "value3", "value4"), &Smetch::stroke, DEFVAL(1), DEFVAL(-1), DEFVAL(-1), DEFVAL(-1));

	ClassDB::bind_method(D_METHOD("set_color_alpha"), &Smetch::set_color_alpha);
	ClassDB::bind_method(D_METHOD("redraw_background"), &Smetch::redraw_background);
	ClassDB::bind_method(D_METHOD("create_canvas"), &Smetch::create_canvas);
	ClassDB::bind_method(D_METHOD("create"), &Smetch::create);
	ClassDB::bind_method(D_METHOD("clear"), &Smetch::clear);
	ClassDB::bind_method(D_METHOD("resize_canvas"), &Smetch::resize_canvas);
	ClassDB::bind_method(D_METHOD("resize"), &Smetch::resize);
	ClassDB::bind_method(D_METHOD("continuous_drawing"), &Smetch::continuous_drawing);
	ClassDB::bind_method(D_METHOD("clear_mode"), &Smetch::clear_mode);
	ClassDB::bind_method(D_METHOD("fill_with_color"), &Smetch::fill_with_color);
	ClassDB::bind_method(D_METHOD("rect"), &Smetch::rect);
	ClassDB::bind_method(D_METHOD("line"), &Smetch::line);
	ClassDB::bind_method(D_METHOD("translate"), &Smetch::translate);
	ClassDB::bind_method(D_METHOD("push"), &Smetch::push);
	ClassDB::bind_method(D_METHOD("pop"), &Smetch::pop);
	ClassDB::bind_method(D_METHOD("translate_reset"), &Smetch::translate_reset);
	ClassDB::bind_method(D_METHOD("stroke_weight"), &Smetch::stroke_weight);
	ClassDB::bind_method(D_METHOD("stroke_cap"), &Smetch::stroke_cap);
	ClassDB::bind_method(D_METHOD("stroke_color"), &Smetch::stroke_color);
	ClassDB::bind_method(D_METHOD("gradient_rect"), &Smetch::gradient_rect);

	ClassDB::bind_method(D_METHOD("no_cursor"), &Smetch::no_cursor);
	ClassDB::bind_method(D_METHOD("set_cursor_busy"), &Smetch::set_cursor_busy);
	ClassDB::bind_method(D_METHOD("set_cursor_not_busy"), &Smetch::set_cursor_not_busy);
	ClassDB::bind_method(D_METHOD("no_stroke"), &Smetch::no_stroke);
	ClassDB::bind_method(D_METHOD("rect_mode"), &Smetch::rect_mode);
	ClassDB::bind_method(D_METHOD("save_canvas"), &Smetch::save_canvas);
	ClassDB::bind_method(D_METHOD("get_mouse_position"), &Smetch::get_mouse_position);
	ClassDB::bind_method(D_METHOD("get_current_color"), &Smetch::get_current_color);
	ClassDB::bind_method(D_METHOD("lerp_color"), &Smetch::lerp_color);
	ClassDB::bind_method(D_METHOD("update_cursor"), &Smetch::update_cursor);
	ClassDB::bind_method(D_METHOD("_ready"), &Smetch::_ready);
	ClassDB::bind_method(D_METHOD("_process"), &Smetch::_process);

	ClassDB::bind_method(D_METHOD("write_to_palette"), &Smetch::write_to_palette);
	ClassDB::bind_method(D_METHOD("read_from_palette"), &Smetch::read_from_palette);
	ClassDB::bind_method(D_METHOD("reset_palette"), &Smetch::reset_palette);
	ClassDB::bind_method(D_METHOD("sort_palette"), &Smetch::sort_palette);
	ClassDB::bind_method(D_METHOD("save_palette"), &Smetch::save_palette);

	ClassDB::bind_method(D_METHOD("load_image"), &Smetch::load_image);
	ClassDB::bind_method(D_METHOD("load_image_texture"), &Smetch::load_image_texture);
	ClassDB::bind_method(D_METHOD("open_file_dialog"), &Smetch::open_file_dialog);

	ClassDB::bind_method(D_METHOD("constrain"), &Smetch::constrain);
	ClassDB::bind_method(D_METHOD("fconstrain"), &Smetch::fconstrain);
	ClassDB::bind_method(D_METHOD("map"), &Smetch::map);
	ClassDB::bind_method(D_METHOD("fmap"), &Smetch::fmap);
	ClassDB::bind_method(D_METHOD("random"), &Smetch::random);
	ClassDB::bind_method(D_METHOD("frandom"), &Smetch::frandom);
	ClassDB::bind_method(D_METHOD("seed_random_number_generator"), &Smetch::seed_random_number_generator);

	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("RGB"), RGB);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("HSB"), HSB);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("HSL"), HSL);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("CORNER"), CORNER);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("CORNERS"), CORNERS);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("RADIUS"), RADIUS);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("CENTER"), CENTER);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("GIMP"), GIMP);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("SQUARE"), SQUARE);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("ROUND"), ROUND);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("Constants"), StringName("PROJECT"), PROJECT);

	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("PaletteSortMode"), StringName("RED"), RED);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("PaletteSortMode"), StringName("GREEN"), GREEN);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("PaletteSortMode"), StringName("BLUE"), BLUE);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("PaletteSortMode"), StringName("HUE"), HUE);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("PaletteSortMode"), StringName("SATURATION"), SATURATION);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("PaletteSortMode"), StringName("BRIGHTNESS"), BRIGHTNESS);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("PaletteSortMode"), StringName("GRAYSCALE"), GRAYSCALE);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("PaletteSortMode"), StringName("ALPHA"), ALPHA);
	ClassDB::bind_integer_constant(StringName("Smetch"), StringName("PaletteSortMode"), StringName("NONE"), NONE);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "SmetchParameters", PROPERTY_HINT_RESOURCE_TYPE, "SmetchParameters"), "set_parameters", "get_parameters");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "SmetchContent", PROPERTY_HINT_RESOURCE_TYPE, "SmetchContent"), "set_content", "get_content");

	ADD_SIGNAL(MethodInfo("file_selected"));
}

void Smetch::set_parameters(const Ref<SmetchParameters> &p_parameters) {
	parameters = p_parameters;
}
Ref<SmetchParameters> Smetch::get_parameters() const {
	return parameters;
}

void Smetch::set_content(const Ref<SmetchContent> &p_content) {
	content = p_content;
}
Ref<SmetchContent> Smetch::get_content() const {
	return content;
}

Color Smetch::apply_color(float value1, float value2, float value3, float value4, Color to_color) {
	if (value4 == -1) {
		value4 = maxes[3];
	}
	if (clr_mode == HSB) {
		to_color.set_hsv(value1 / maxes[0], value2 / maxes[1], value3 / maxes[2], value4 / maxes[3]);
	} else {
		// RGB
		to_color.r = value1 / maxes[0];
		to_color.g = value2 / maxes[1];
		to_color.b = value3 / maxes[2];
		to_color.a = value4 / maxes[3];
	}
	return to_color;
}

Color Smetch::prime_color(Color color, float value1, float value2, float value3, float value4) {
	return apply_color(value1, value2, value3, value4, color);
}

Color Smetch::set_color_alpha(Color color, float alpha) {
	color.a = alpha / maxes[3];
	return color;
}

void Smetch::redraw_background() {
	draw_background_draw_count = 0;
}

void Smetch::background(float value1, float value2, float value3) {
	if (clear_mde == SubViewport::CLEAR_MODE_NEVER) {
		if (draw_background_draw_count > draw_background_draw_times) {
			return;
		}
		draw_background_draw_count++;
	}

	if (value2 == -1) {
		value2 = value1;
	}
	if (value3 == -1) {
		value3 = value1;
	}
	background_color = apply_color(value1, value2, value3, -1, background_color);
	draw_rect(background_rect, background_color);
}

void Smetch::fill(float value1, float value2, float value3, float value4) {
	if (value2 == -1) {
		value2 = value1;
	}
	if (value3 == -1) {
		value3 = value1;
	}
	fill_color = apply_color(value1, value2, value3, value4, fill_color);
}

void Smetch::fill_with_color(Color color) {
	fill_color = color;
}

void Smetch::rect(float x, float y, float w, float h) {
	if (rct_mode == CORNER) {
		Point2 pos = Point2(x, y);
		Size2 rect_size = Size2(w, h);
		Size2 parent_size = get_size(); // TODO : maybe can cache this, but only will work if always the same parent

		if (pos.x + rect_size.x > parent_size.x) {
			rect_size.x = rect_size.x - (pos.x + rect_size.x - parent_size.x);
		}
		if (pos.y + rect_size.y > parent_size.y) {
			rect_size.y = rect_size.y - (pos.y + rect_size.y - parent_size.y);
		}

		Rect2 rect = Rect2(pos, rect_size);
		draw_rect(rect, fill_color);

	} else if (rct_mode == CENTER) {
		// use x and y as the max w and h radius's
		if (w > x * 2)
			w = x * 2;
		if (h > y * 2)
			h = y * 2;

		Size2 rect_size = Size2(w, h);
		Size2 parent_size = get_size(); // TODO : maybe can cache this, but only will work if always the same parent

		if (rect_size.x > parent_size.x)
			rect_size.x = parent_size.x;
		if (rect_size.y > parent_size.y)
			rect_size.y = parent_size.y;

		Point2 pos = Point2(0, 0);
		if (rect_size.x < parent_size.x) {
			pos.x = (parent_size.x - rect_size.x) * 0.5;
		}
		if (rect_size.y < parent_size.y) {
			pos.y = (parent_size.y - rect_size.y) * 0.5;
		}
		Rect2 rect = Rect2(pos, rect_size);
		draw_rect(rect, fill_color); // TODO : if no_stroke is false can add a border
	}
}

void Smetch::line(float start_x, float start_y, float end_x, float end_y) {
	if (stroke_cp == SQUARE) {
		draw_line(Vector2(translation.x + start_x, translation.y + start_y), Vector2(translation.x + end_x, translation.y + end_y), stroke_clr, stroke_wgt);
	} else if (stroke_cp == ROUND) {
		draw_line(Vector2(translation.x + start_x, translation.y + start_y), Vector2(translation.x + end_x, translation.y + end_y), stroke_clr, stroke_wgt);
		draw_circle(Vector2(start_x, start_y), fmax(stroke_wgt / 2.0, 0.0), stroke_clr);
		draw_circle(Vector2(end_x, end_y), fmax(stroke_wgt / 2.0, 0.0), stroke_clr);
	} else if (stroke_cp == PROJECT) {
		start_x += translation.x;
		start_y += translation.y;
		end_x += translation.x;
		end_y += translation.y;
		float project = stroke_wgt * 0.5;
		float len_line = sqrt(pow(start_x - end_x, 2.0) + pow(start_y - end_y, 2.0));
		float unit_len_x = (end_x - start_x) / len_line;
		float unit_len_y = (end_y - start_y) / len_line;
		Vector2 new_start = Vector2(start_x - project * unit_len_x, start_y - project * unit_len_y);
		Vector2 new_end = Vector2(end_x + project * unit_len_x, end_y + project * unit_len_y);
		draw_line(new_start, new_end, stroke_clr, stroke_wgt);
	}
}

void Smetch::translate(float x, float y) {
	translation.x += x;
	translation.y += y;
}
void Smetch::translate_reset() {
	translation = Vector2(0, 0);
}

void Smetch::stroke_color(Color color) {
	stroke_clr = color;
}

void Smetch::stroke(float value1, float value2, float value3, float value4) {
	if (value2 == -1) {
		value2 = value1;
	}
	if (value3 == -1) {
		value3 = value1;
	}
	if (value4 == -1) {
		value4 = stroke_clr.a;
	}
	stroke_clr = apply_color(value1, value2, value3, value4, stroke_clr);
}

void Smetch::stroke_weight(float weight) {
	stroke_wgt = weight;
}

void Smetch::stroke_cap(int stroke_cap) {
	stroke_cp = stroke_cap;
}

void Smetch::color_mode(int mode, float value1, float value2, float value3, float value4) {
	if (mode == RGB || mode == HSB || mode == HSL) {
		clr_mode = mode;

		for (int i = 0; i < 4; i++) {
			maxes[i] = color_maxes[clr_mode][i];
		}

		if (value1 == -1) {
			// do nothing .. will uses the color_maxes defaults
		} else if (value2 == -1) {
			maxes[0] = value1; // Red
			maxes[1] = value1; // Green
			maxes[2] = value1; // Blue
			maxes[3] = value1; // Alpha
		} else if (value4 == -1) {
			maxes[0] = value1; // Red
			maxes[1] = value2; // Green
			maxes[2] = value3; // Blue
		} else {
			maxes[0] = value1; // Red
			maxes[1] = value2; // Green
			maxes[2] = value3; // Blue
			maxes[3] = value4; // Alpha
		}
	}
}

void Smetch::gradient_rect(float x, float y, float w, float h, Color c1, Color c2) {
	geometry_points.clear();
	geometry_colors.clear();

	geometry_points.append(Point2(x, y));
	geometry_points.append(Point2(x + w, y));
	geometry_points.append(Point2(x + w, y + h));
	geometry_points.append(Point2(x, y + h));

	// top down
	geometry_colors.append(c1);
	geometry_colors.append(c1);
	geometry_colors.append(c2);
	geometry_colors.append(c2);

	// TODO : left - right -> parameter driven
	draw_polygon(geometry_points, geometry_colors, geometry_points);
}

void Smetch::pop() {
	do_buffer_image_copy = true;
}

void Smetch::push() {
	// NOTE : the push and pop wasnt used so this might not work
	if (buffer_image_is_available) {
		Ref<ImageTexture> texture(memnew(ImageTexture));
		texture->create_from_image(buffer_image);
		draw_texture(texture, Point2(0, 0));
	}
}

void Smetch::create_canvas(double x, double y) {
	create(x, y);
	background_rect = Rect2(Point2(0, 0), get_size());
	canvas_texture = memnew(CanvasTexture);
	set_texture(canvas_texture);
}

void Smetch::create(double x, double y) {
	// TODO : if this gets called again then need to clear the old objects
	resize(x, y);
	parent_mouse_mode = Input::get_singleton()->get_mouse_mode();
	color_mode(RGB, -1, -1, -1, -1);
}

void Smetch::clear() {
	if (clr_mode == RGB) {
		background(maxes[0], maxes[1], maxes[2]);
	} else if (clr_mode == HSB) {
		background(0, 0, maxes[2]);
	}
}

void Smetch::resize_canvas(double x, double y) {
	resize(x, y);
	background_rect.set_size(Size2(x, y));
}

void Smetch::resize(double x, double y) {
	this->set_size(Size2(x, y));
}

void Smetch::no_stroke() {
	// TODO : Add no_stroke functionality
	renderer_no_stroke = false;
	print_line("todo : no_stroke does nothing");
}

void Smetch::no_cursor() {
	cursor_mode = CURSOR_HIDDEN;
	update_cursor();
}

void Smetch::update_cursor() {
	if (cursor_mode == CURSOR_HIDDEN) {
		Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_HIDDEN);
	} else {
		Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
	}
}

void Smetch::set_cursor_busy() {
	set_default_cursor_shape(CURSOR_BUSY);
}

void Smetch::set_cursor_not_busy() {
	set_default_cursor_shape(CURSOR_ARROW);
}

void Smetch::continuous_drawing(bool is_continuous) {
	is_continuous_drawing = is_continuous;
}

void Smetch::clear_mode(SubViewport::ClearMode clear_mode) {
	clear_mde = clear_mode;
	RS::get_singleton()->viewport_set_clear_mode(get_viewport_rid(), RS::ViewportClearMode(clear_mode));
}

void Smetch::rect_mode(int mode) {
	if (mode == CORNER ||
			mode == CORNERS ||
			mode == RADIUS ||
			mode == CENTER) {
		rct_mode = mode;
	}
}

Vector2 Smetch::get_mouse_position() {
	return get_local_mouse_position();
}

Color Smetch::get_current_color() {
	return fill_color;
}

void Smetch::mouse_entered() {
	update_cursor();
}

void Smetch::mouse_exited() {
	Input::get_singleton()->set_mouse_mode(parent_mouse_mode);
}

void Smetch::seed_random_number_generator(int seed, bool randomize) {
	random_number_generator->set_seed(seed);
	if (randomize) {
		random_number_generator->randomize();
	}
}

void Smetch::_ready() {
	connect("mouse_entered", callable_mp(this, &Smetch::mouse_entered));
	connect("mouse_exited", callable_mp(this, &Smetch::mouse_exited));

	if (parameters != nullptr) {
		print_line("Using parameter random seed: " + itos(parameters->get_random_seed()));
		seed_random_number_generator(parameters->get_random_seed(), parameters->get_randomize());
	} else {
		print_line("Auto Seeding and Randomizing");
		seed_random_number_generator(OS::get_singleton()->get_unix_time(), true);
	}
}

void Smetch::_process(float delta) {
	translate_reset();
	if (is_continuous_drawing) {
		queue_redraw();
	}
	if (do_buffer_image_copy && get_texture() != nullptr) {
		buffer_image = get_viewport()->get_texture()->get_image();
		print_line("width:" + itos(buffer_image->get_width()));
		//Ref<ImageTexture> texture(memnew(ImageTexture));
		//texture->create_from_image(buffer_image);
		//buffer_image_texture = texture;
		buffer_image_is_available = true;
		do_buffer_image_copy = false;
		print_line("popped");
	}
}

void Smetch::reset_palette(int size) {
	if (size > palette_cache_size) {
		if (palette_cache_size > -1) {
			memdelete_arr(palette);
		}
		palette = memnew_arr(Color, size);
		palette_cache_size = size;
	}
	palette_size = size;
	palette_index = -1;
}

void Smetch::write_to_palette(Color color) {
	palette[++palette_index] = color;
}

Color Smetch::read_from_palette(int index) {
	if (index < palette_size) {
		return palette[index];
	}
	print_error("get_color_from_palette failed as the position " + itos(index) + " is out of range");
	return Color(); // TODO - proper error control
}

String Smetch::save_canvas(String file_name) {
	return save_canvas_with_image(file_name, get_viewport()->get_texture()->get_image());
}

String Smetch::save_canvas_with_image(String file_name, Ref<Image> image) {
	String dir = OS::get_singleton()->get_user_data_dir();
	String save_location = dir + "/" + file_name + "_" + itos(OS::get_singleton()->get_unix_time()) + ".png";
	image->save_png(save_location);

	return save_location;
}

String Smetch::save_palette(String file_name, int format, double columns) {
	// TODO : check palette exists -- otherwise will crash
	if (format == GIMP) {
		String new_line = "\n";
		String s = "GIMP Palette" + new_line;
		s += "Name: " + file_name + new_line;
		s += "Columns: " + itos(columns) + new_line;
		s += "#" + new_line;

		for (int i = 0; i < palette_size; i++) {
			s += itos(palette[i].r * 255) + " " +
				 itos(palette[i].g * 255) + " " +
				 itos(palette[i].b * 255) + new_line;
		}

		String path = OS::get_singleton()->get_user_data_dir() + "/" + file_name + "_" + itos(OS::get_singleton()->get_unix_time()) + ".gpl";
		Ref<FileAccess> file = FileAccess::open(path, FileAccess::WRITE);
    ERR_FAIL_COND_V_MSG(file.is_null(),"", "Cannot open file '" + path + "'");
		file->store_string(s);
		//file->close();
		return path;
	}
	return "";
}

void Smetch::open_file_dialog() {
	if (file_dialog == nullptr) {
		file_dialog = memnew(FileDialog);
		file_dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
		file_dialog->set_file_mode(FileDialog::FILE_MODE_OPEN_FILE);
		file_dialog->set_title("Open Image");
		file_dialog->add_filter("*.jpg ; JPG Images");
		file_dialog->add_filter("*.png ; PNG Images");
		add_child(file_dialog);

		file_dialog->connect("file_selected", callable_mp(this, &Smetch::_on_FileDialog_file_selected));
	}
	file_dialog->popup_file_dialog();
}

Ref<Image> Smetch::load_image(String path) {
	Ref<Image> image = memnew(Image);
	image->load(path);
	return image;
}

Ref<ImageTexture> Smetch::load_image_texture(String path, double max_width, double max_height) {
	Ref<ImageTexture> image_texture = memnew(ImageTexture);
	Ref<Image> image = load_image(path);

	if (image->get_size().x > max_width) {
		float scale = max_width / image->get_size().x;
		image->resize(max_width, image->get_size().y * scale);
	}

	if (image->get_size().y > max_height) {
		float scale = max_height / image->get_size().y;
		image->resize(image->get_size().x * scale, max_height);
	}

	image_texture->create_from_image(image);
	return image_texture;
}

void Smetch::_on_FileDialog_file_selected(const String path) {
	emit_signal(SNAME("file_selected"), path);
}

Smetch::Smetch() {
	random_number_generator = memnew(RandomNumberGenerator);
}

Smetch::~Smetch() {
	if (palette_cache_size > -1) {
		memdelete_arr(palette);
	}
}

///////////////////////////////////////////////////////////////////
int Smetch::random(int from, int to) {
	return random_number_generator->randi_range(from, to);
}

float Smetch::frandom(float from, float to) {
	return random_number_generator->randf_range(from, to);
}

double Smetch::constrain(double n, double low, double high) {
	return max(min(n, high), low);
}
float Smetch::fconstrain(float n, float low, float high) {
	return fmax(fmin(n, high), low);
}

double Smetch::map(double n, double start1, double stop1, double start2, double stop2) {
	return ((n - start1) / (stop1 - start1)) * (stop2 - start2) + start2;
}
float Smetch::fmap(float n, float start1, float stop1, float start2, float stop2) {
	return ((n - start1) / (stop1 - start1)) * (stop2 - start2) + start2;
}

double Smetch::min(double n1, double n2) {
	return n1 < n2 ? n1 : n2;
}
double Smetch::max(double n1, double n2) {
	return n1 > n2 ? n1 : n2;
}

Color Smetch::lerp_color(Color c1, Color c2, float amt) {
	float l0, l1, l2, l3;
	float from_array[4];
	float to_array[4];
	Color color;

	if (clr_mode == RGB) {
		from_array[0] = c1.r;
		from_array[1] = c1.g;
		from_array[2] = c1.b;
		from_array[3] = c1.a;
		to_array[0] = c2.r;
		to_array[1] = c2.g;
		to_array[2] = c2.b;
		to_array[3] = c2.a;
	} else if (clr_mode == HSB) {
		from_array[0] = c1.get_h();
		from_array[1] = c1.get_s();
		from_array[2] = c1.get_v();
		from_array[3] = c1.a;
		to_array[0] = c2.get_h();
		to_array[1] = c2.get_s();
		to_array[2] = c2.get_v();
		to_array[3] = c2.a;
	} else if (clr_mode == HSL) {
		print_error("Lerp_color for HSL not implemented!");
	}

	amt = fmax(fmin(amt, 1.0), 0);

	l0 = Math::lerp(from_array[0], to_array[0], amt);
	l1 = Math::lerp(from_array[1], to_array[1], amt);
	l2 = Math::lerp(from_array[2], to_array[2], amt);
	l3 = Math::lerp(from_array[3], to_array[3], amt);

	if (clr_mode == HSB) {
		color.set_hsv(l0, l1, l2, l3);
	} else if (clr_mode == HSL) {
		print_error("Lerp_color for HSL not implemented!");
	} else {
		// RGB
		color.r = l0;
		color.g = l1;
		color.b = l2;
		color.a = l3;
	}
	return color;
}

////////////////////////////////////////////////////////
void Smetch::sort_palette(int sort_mode) {
	if (sort_mode != Smetch::PaletteSortMode::NONE) {
		if (palette_size > 1) {
			switch (sort_mode) {
				case PaletteSortMode::RED:
					red_palette_sorter.sort(palette, palette_size);
					break;
				case PaletteSortMode::GREEN:
					green_palette_sorter.sort(palette, palette_size);
					break;
				case PaletteSortMode::BLUE:
					blue_palette_sorter.sort(palette, palette_size);
					break;
				case PaletteSortMode::HUE:
					hue_palette_sorter.sort(palette, palette_size);
					break;
				case PaletteSortMode::SATURATION:
					saturation_palette_sorter.sort(palette, palette_size);
					break;
				case PaletteSortMode::BRIGHTNESS:
					brightness_palette_sorter.sort(palette, palette_size);
					break;
				case PaletteSortMode::GRAYSCALE:
					grayscale_palette_sorter.sort(palette, palette_size);
					break;
				case PaletteSortMode::ALPHA:
					alpha_palette_sorter.sort(palette, palette_size);
					break;
				default:
					print_error("Sort mode not supported :" + itos(sort_mode));
			}
		}
	}
}
