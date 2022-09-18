/* smetch_content.h */

#ifndef SMETCH_CONTENT_H
#define SMETCH_CONTENT_H

#include "core/io/resource.h"

class SmetchContent : public Resource {
	GDCLASS(SmetchContent, Resource);

protected:
	static void _bind_methods();

public:
	enum ContentType {
		SMETCH_ROOT,
		SMETCH_MODULE,
		SMETCH_CHAPTER,
		SMETCH_SECTION,
		SMETCH_SMETCH
	};

	ContentType get_content_type() const;
	void set_content_type(const ContentType type);
	String get_id();
	String get_id_();
	void set_id(String id);
	String get_title();
	void set_title(String description);
	String get_description();
	void set_description(String description);
	void set_image(String image_path);
	String get_image();
	void set_color(Color color);
	Color get_color();
  void set_scene(String scene_path);
  String get_scene();

private:
	ContentType content_type = SMETCH_SMETCH;
	String id;
	String title;
	String description;
	String image;
  Color color;
	String scene;

};

VARIANT_ENUM_CAST(SmetchContent::ContentType);

#endif // SMETCH_CONTENT_H
