#ifndef NATIVE_GAME_H
#define NATIVE_GAME_H

#include "scene/main/node.h"

class NativeGame : public Node {

   GDCLASS(NativeGame, Node);

protected:
  static void _bind_methods();

public:

  void _init();

};
#endif
