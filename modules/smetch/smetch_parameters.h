/* smetch_parameters.h */

#ifndef SMETCH_PARAMETERS_H
#define SMETCH_PARAMETERS_H

#include "core/io/resource.h"

class SmetchParameters : public Resource {
	GDCLASS(SmetchParameters, Resource);

private:
  int random_seed = 0;
  bool randomize = true;

protected:
	static void _bind_methods();

public:
  int get_random_seed();
  void set_random_seed(int seed);
  bool get_randomize();
  void set_randomize(bool randonize);

private:
};

#endif // SMETCH_PARAMETERS_H
