/* smetch_properties.cpp */

#include "smetch_properties.h"
#include "smetch_section_properties.h"

void SmetchProperties::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_section", "SmetchSectionProperties"), &SmetchProperties::set_section);
	ClassDB::bind_method(D_METHOD("get_section"), &SmetchProperties::get_section);
	ClassDB::bind_method(D_METHOD("get_identifier"), &SmetchProperties::get_identifier);
	ClassDB::bind_method(D_METHOD("set_identifier", "identifier"), &SmetchProperties::set_identifier);
	ClassDB::bind_method(D_METHOD("get_description"), &SmetchProperties::get_description);
	ClassDB::bind_method(D_METHOD("set_description", "description"), &SmetchProperties::set_description);
	ClassDB::bind_method(D_METHOD("get_preview_file"), &SmetchProperties::get_preview_file);
	ClassDB::bind_method(D_METHOD("set_preview_file", "image_file"), &SmetchProperties::set_preview_file);
	ClassDB::bind_method(D_METHOD("get_random_seed"), &SmetchProperties::get_random_seed);
	ClassDB::bind_method(D_METHOD("set_random_seed", "random_seed"), &SmetchProperties::set_random_seed);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "section", PROPERTY_HINT_RESOURCE_TYPE, "SmetchSectionProperties"), "set_section", "get_section");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "identifier"), "set_identifier", "get_identifier");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "description"), "set_description", "get_description");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "preview_file", PROPERTY_HINT_FILE, "*.png"), "set_preview_file", "get_preview_file");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "random_seed"), "set_random_seed", "get_random_seed");
}

Ref<SmetchSectionProperties> SmetchProperties::get_section() {
  return section;
}
void SmetchProperties::set_section(const Ref<SmetchSectionProperties> &p_section) {
	section = p_section;
}

String SmetchProperties::get_identifier() {
  return identifier;
}
void SmetchProperties::set_identifier(String p_identifier) {
	identifier = p_identifier;
}

String SmetchProperties::get_description() {
  return description;
}
void SmetchProperties::set_description(String p_description) {
	description = p_description;
}

int SmetchProperties::get_random_seed() {
  return random_seed;
}
void SmetchProperties::set_random_seed(int p_random_seed) {
	random_seed = p_random_seed;
}

String SmetchProperties::get_preview_file() {
  return preview_file;
}
void SmetchProperties::set_preview_file(String p_preview_file) {
	preview_file = p_preview_file;
}
