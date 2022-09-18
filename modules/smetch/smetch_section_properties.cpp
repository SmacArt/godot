/* smetch_section_properties.cpp */

#include "smetch_section_properties.h"

void SmetchSectionProperties::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_chapter", "SmetchChapterProperties"), &SmetchSectionProperties::set_chapter);
	ClassDB::bind_method(D_METHOD("get_chapter"), &SmetchSectionProperties::get_chapter);
	ClassDB::bind_method(D_METHOD("get_identifier"), &SmetchSectionProperties::get_identifier);
	ClassDB::bind_method(D_METHOD("set_identifier", "identifier"), &SmetchSectionProperties::set_identifier);
	ClassDB::bind_method(D_METHOD("get_title"), &SmetchSectionProperties::get_title);
	ClassDB::bind_method(D_METHOD("set_title", "title"), &SmetchSectionProperties::set_title);
	ClassDB::bind_method(D_METHOD("get_description"), &SmetchSectionProperties::get_description);
	ClassDB::bind_method(D_METHOD("set_description", "description"), &SmetchSectionProperties::set_description);

	ClassDB::bind_method(D_METHOD("get_preview_file"), &SmetchSectionProperties::get_preview_file);
	ClassDB::bind_method(D_METHOD("set_preview_file", "preview_file"), &SmetchSectionProperties::set_preview_file);

	ClassDB::bind_method(D_METHOD("get_description_rect"), &SmetchSectionProperties::get_description_rect);
	ClassDB::bind_method(D_METHOD("set_description_rect", "description"), &SmetchSectionProperties::set_description_rect);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "chapter", PROPERTY_HINT_RESOURCE_TYPE, "SmetchChapterProperties"), "set_chapter", "get_chapter");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "identifier"), "set_identifier", "get_identifier");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "title"), "set_title", "get_title");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "description"), "set_description", "get_description");
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "description_rect"), "set_description_rect", "get_description_rect");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "preview_file", PROPERTY_HINT_FILE, "*.png"), "set_preview_file", "get_preview_file");
}

Ref<SmetchChapterProperties> SmetchSectionProperties::get_chapter() {
  return chapter;
}
void SmetchSectionProperties::set_chapter(const Ref<SmetchChapterProperties> &p_chapter) {
	chapter = p_chapter;
}

String SmetchSectionProperties::get_identifier() {
  return identifier;
}
void SmetchSectionProperties::set_identifier(String p_identifier) {
	identifier = p_identifier;
}

String SmetchSectionProperties::get_title() {
  return title;
}
void SmetchSectionProperties::set_title(String p_title) {
	title = p_title;
}

String SmetchSectionProperties::get_description() {
  return description;
}
void SmetchSectionProperties::set_description(String p_description) {
	description = p_description;
}

Rect2 SmetchSectionProperties::get_description_rect() {
  return description_rect;
}
void SmetchSectionProperties::set_description_rect(Rect2 p_description_rect) {
	description_rect = p_description_rect;
}

String SmetchSectionProperties::get_preview_file() {
  return preview_file;
}
void SmetchSectionProperties::set_preview_file(String p_preview_file) {
	preview_file = p_preview_file;
}
