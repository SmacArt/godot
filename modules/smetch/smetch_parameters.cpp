/* smetch_parameters.cpp */

#include "smetch_parameters.h"

void SmetchParameters::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_random_seed"), &SmetchParameters::get_random_seed);
	ClassDB::bind_method(D_METHOD("set_random_seed", "random_seed"), &SmetchParameters::set_random_seed);
	ClassDB::bind_method(D_METHOD("get_randomize"), &SmetchParameters::get_randomize);
	ClassDB::bind_method(D_METHOD("set_randomize", "randomize"), &SmetchParameters::set_randomize);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "random_seed"), "set_random_seed", "get_random_seed");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "randomize"), "set_randomize", "get_randomize");
}

int SmetchParameters::get_random_seed() {
	return random_seed;
}
void SmetchParameters::set_random_seed(int p_random_seed) {
	random_seed = p_random_seed;
}

bool SmetchParameters::get_randomize() {
	return randomize;
}
void SmetchParameters::set_randomize(bool p_randomize) {
	randomize = p_randomize;
}


