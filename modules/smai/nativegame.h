#ifndef NATIVE_GAME_H
#define NATIVE_GAME_H

#include "scene/main/node.h"

class NativeGame : public Node {

   GDCLASS(NativeGame, Node);

public:

  static void _register_methods();

  void _init();

};
#endif
