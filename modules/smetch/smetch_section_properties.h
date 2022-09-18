/* smetch_section_properties.h */

#ifndef SMETCH_SECTION_PROPERTIES_H
#define SMETCH_SECTION_PROPERTIES_H

#include "core/io/resource.h"
#include "smetch_chapter_properties.h"

class SmetchSectionProperties : public Resource {
	GDCLASS(SmetchSectionProperties, Resource);

private:
  Ref<SmetchChapterProperties> chapter;
	String identifier;
	String title;
	String description;
  String preview_file;
	Rect2 description_rect;

protected:
	static void _bind_methods();

public:
  Ref<SmetchChapterProperties> get_chapter();
  void set_chapter(const Ref<SmetchChapterProperties> &chapter);
	String get_title();
	void set_title(String title);
	String get_identifier();
	void set_identifier(String title);
	String get_description();
	void set_description(String title);
	Rect2 get_description_rect();
	void set_description_rect(Rect2 seed);
  String get_preview_file();
  void set_preview_file(String preview_file);

private:
};

#endif // SMETCH_SECTION_PROPERTIES_H
