#include "crudetimer.h"

CrudeTimer* CrudeTimer::instance()
{
  static CrudeTimer instance;
  return &instance;
}
