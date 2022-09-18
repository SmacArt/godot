/* smetch_chapter_properties.h */

#ifndef SMETCH_CHAPTER_PROPERTIES_H
#define SMETCH_CHAPTER_PROPERTIES_H

#include "core/io/resource.h"

class SmetchChapterProperties : public Resource {
	GDCLASS(SmetchChapterProperties, Resource);

private:
  String identifier;
  String title;
  String description;
  String preview_file;

protected:
	static void _bind_methods();

public:
  String get_title();
  void set_title(String title);
  String get_identifier();
  void set_identifier(String title);
  String get_description();
  void set_description(String title);
  void set_preview_file(String image_file);
  String get_preview_file();

private:
};

#endif // SMETCH_CHAPTER_PROPERTIES_H
