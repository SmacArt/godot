/* smetch.h */

#ifndef SMETCH_H
#define SMETCH_H

#include "core/io/file_access.h"
#include "core/math/color.h"
#include "core/math/random_number_generator.h"
#include "core/os/os.h"
#include "core/templates/sort_array.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/texture_rect.h"
#include "scene/main/viewport.h"
#include "smetch_parameters.h"
#include "smetch_content.h"

class Smetch : public TextureRect {
	GDCLASS(Smetch, TextureRect);

protected:
	static void _bind_methods();

public:
	struct Col {
		int r, g, b, a;
	};

	enum Constants {
		RGB,
		HSB,
		HSL,
		CORNER,
		CORNERS,
		RADIUS,
		CENTER,
		CURSOR_HIDDEN,
		CURSOR_VISIBLE,
		GIMP,
    SQUARE,
    ROUND,
    PROJECT
	};

	enum PaletteSortMode {
		RED,
		GREEN,
		BLUE,
		HUE,
		SATURATION,
		BRIGHTNESS,
		GRAYSCALE,
		ALPHA,
		NONE
	};

	struct RedPaletteComparator {
		_FORCE_INLINE_ bool operator()(const Color &a, const Color &b) const {
			return (a.r < b.r);
		}
	};
	struct GreenPaletteComparator {
		_FORCE_INLINE_ bool operator()(const Color &a, const Color &b) const {
			return (a.g < b.g);
		}
	};
	struct BluePaletteComparator {
		_FORCE_INLINE_ bool operator()(const Color &a, const Color &b) const {
			return (a.b < b.b);
		}
	};
	struct HuePaletteComparator {
		_FORCE_INLINE_ bool operator()(const Color &a, const Color &b) const {
			return (a.get_h() < b.get_h());
		}
	};
	struct SaturationPaletteComparator {
		_FORCE_INLINE_ bool operator()(const Color &a, const Color &b) const {
			return (a.get_s() < b.get_s());
		}
  };
	struct BrightnessPaletteComparator {
		_FORCE_INLINE_ bool operator()(const Color &a, const Color &b) const {
			return (a.get_v() < b.get_v());
		}
  };
	struct AlphaPaletteComparator {
		_FORCE_INLINE_ bool operator()(const Color &a, const Color &b) const {
			return (a.a < b.a);
		}
  };
	struct GrayscalePaletteComparator {
		_FORCE_INLINE_ bool operator()(const Color &a, const Color &b) const {
			return ((a.r + a.g + a.b) / 3.0 < (b.r + b.g + b.b) / 3.0);
		}
  };

	void set_parameters(const Ref<SmetchParameters> &parameters);
	Ref<SmetchParameters> get_parameters() const;
	void set_content(const Ref<SmetchContent> &content);
	Ref<SmetchContent> get_content() const;

  void seed_random_number_generator(int seed, bool randomize);
	void background(float value1, float value2, float value3);
  void redraw_background();
	void color_mode(int mode, float value1, float value2, float value3, float value4);
	void create_canvas(double x, double y);
	void create(double x, double y);
	void resize_canvas(double x, double y);
	void resize(double x, double y);
  void clear();
	void fill(float value1, float value2, float value3, float value4);
	Color prime_color(Color color, float value1, float value2, float value3, float value4);
	Color set_color_alpha(Color color, float alpha);
	void fill_with_color(Color color);
	void rect(float x, float y, float w, float h);
	void line(float start_x, float start_y, float end_x, float end_y);
	void stroke_color(Color c);
	void stroke(float value1, float value2, float value3, float value4);
	void translate(float x, float y);
	void translate_reset();
	void stroke_weight(float weight);
	void stroke_cap(int stroke_cap);
	void gradient_rect(float x, float y, float w, float h, Color c1, Color c2);
	void no_cursor();
	void no_stroke();
	void continuous_drawing(bool is_continuous);
	void clear_mode(SubViewport::ClearMode clear_mode);
	void rect_mode(int mode);
	String save_canvas(String file_name);
	String save_canvas_with_image(String file_name, Ref<Image> image);

	void push();
	void pop();

	Vector2 get_mouse_position();
	void update_cursor();
  void set_cursor_busy();
  void set_cursor_not_busy();
	Color get_current_color();
	Color lerp_color(Color c1, Color c2, float amt);
	double map(double n, double start1, double stop1, double start2, double stop2);
	float fmap(float n, float start1, float stop1, float start2, float stop2);
	double constrain(double n, double low, double high);
	float fconstrain(float n, float low, float high);
	double min(double n1, double n2);
	double max(double n1, double n2);
	int random(int from, int to);
	float frandom(float from, float to);

	void write_to_palette(Color color);
	void reset_palette(int size);
	Color read_from_palette(int index);
	void sort_palette(int order);
	String save_palette(String file_name, int format, double columns);

	void open_file_dialog();
	Ref<Image> load_image(String path);
	Ref<ImageTexture> load_image_texture(String path, double max_width, double max_height);

	void _on_FileDialog_file_selected(const String path);
	void _ready();
	void _process(float delta);

	Smetch();
	~Smetch();

private:
	Color apply_color(float value1, float value2, float value3, float value4, Color to_color);
	void mouse_entered();
	void mouse_exited();

	//  static int sort_comparator(const void * a, const void  * b );
	void palette_quicksort(int low, int high);
	int palette_sort_partition(int low, int high);

	Ref<SmetchParameters> parameters;
  Ref<SmetchContent> content;

	Color fill_color;
	Color background_color;
	Rect2 background_rect;
  Vector2 translation = Vector2(0.0,0.0);
  float stroke_wgt = 1;
  Color stroke_clr = Color(0,0,0);
  int stroke_cp = SQUARE;

	CanvasTexture *canvas_texture;
	RandomNumberGenerator *random_number_generator;
	FileDialog *file_dialog = nullptr;

  int draw_background_draw_times = 5;
  int draw_background_draw_count = 0;

	Ref<Image> buffer_image;
  Ref<ImageTexture> buffer_image_texture;
  bool do_buffer_image_copy = false;
  bool buffer_image_is_available = false;

  SubViewport::ClearMode clear_mde;

	int clr_mode = RGB;
	int color_maxes[3][4] = {
		{ 255, 255, 255, 255 }, // RGB
		{ 360, 100, 100, 1 }, // HSB
		{ 360, 100, 100, 1 } // HSL
	};
	int maxes[4];
	bool renderer_no_stroke;
	bool is_continuous_drawing = true;
	int rct_mode = CORNER;
	int cursor_mode = CURSOR_ARROW;
	Input::MouseMode parent_mouse_mode;

	Color *palette;
	int palette_size = -1;
	int palette_index;
	int palette_cache_size = -1;

  Vector<Vector2> geometry_points;
  Vector<Color> geometry_colors;

	SortArray<Color, RedPaletteComparator> red_palette_sorter;
	SortArray<Color, GreenPaletteComparator> green_palette_sorter;
	SortArray<Color, BluePaletteComparator> blue_palette_sorter;
	SortArray<Color, HuePaletteComparator> hue_palette_sorter;
	SortArray<Color, SaturationPaletteComparator> saturation_palette_sorter;
	SortArray<Color, BrightnessPaletteComparator> brightness_palette_sorter;
	SortArray<Color, GrayscalePaletteComparator> grayscale_palette_sorter;
	SortArray<Color, AlphaPaletteComparator> alpha_palette_sorter;
};

#endif // SMETCH_H
