/* smetch_chapter_properties.cpp */

#include "smetch_chapter_properties.h"

void SmetchChapterProperties::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_identifier"), &SmetchChapterProperties::get_identifier);
	ClassDB::bind_method(D_METHOD("set_identifier", "identifier"), &SmetchChapterProperties::set_identifier);
	ClassDB::bind_method(D_METHOD("get_title"), &SmetchChapterProperties::get_title);
	ClassDB::bind_method(D_METHOD("set_title", "title"), &SmetchChapterProperties::set_title);
	ClassDB::bind_method(D_METHOD("get_description"), &SmetchChapterProperties::get_description);
	ClassDB::bind_method(D_METHOD("set_description", "description"), &SmetchChapterProperties::set_description);
	ClassDB::bind_method(D_METHOD("get_preview_file"), &SmetchChapterProperties::get_preview_file);
	ClassDB::bind_method(D_METHOD("set_preview_file", "image_file"), &SmetchChapterProperties::set_preview_file);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "identifier"), "set_identifier", "get_identifier");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "title"), "set_title", "get_title");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "description"), "set_description", "get_description");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "preview_file", PROPERTY_HINT_FILE, "*.png"), "set_preview_file", "get_preview_file");
}

String SmetchChapterProperties::get_identifier() {
  return identifier;
}
void SmetchChapterProperties::set_identifier(String p_identifier) {
	identifier = p_identifier;
}

String SmetchChapterProperties::get_title() {
  return title;
}
void SmetchChapterProperties::set_title(String p_title) {
	title = p_title;
}

String SmetchChapterProperties::get_description() {
  return description;
}
void SmetchChapterProperties::set_description(String p_description) {
	description = p_description;
}
String SmetchChapterProperties::get_preview_file() {
  return preview_file;
}
void SmetchChapterProperties::set_preview_file(String p_preview_file) {
	preview_file = p_preview_file;
}
