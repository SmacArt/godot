/* smetch_properties.h */

#ifndef SMETCH_PROPERTIES_H
#define SMETCH_PROPERTIES_H

#include "core/io/resource.h"
#include "smetch_section_properties.h"

class SmetchProperties : public Resource {
	GDCLASS(SmetchProperties, Resource);

private:
  Ref<SmetchSectionProperties> section;
  String identifier;
  String description;
  int random_seed;
  String preview_file;

protected:
	static void _bind_methods();

public:
  Ref<SmetchSectionProperties> get_section();
  void set_section(const Ref<SmetchSectionProperties> &section);
  String get_identifier();
  void set_identifier(String title);
  String get_description();
  void set_description(String title);
  int get_random_seed();
  void set_random_seed(int seed);
  void set_preview_file(String image_file);
  String get_preview_file();

private:
};

#endif // SMETCH_PROPERTIES_H
