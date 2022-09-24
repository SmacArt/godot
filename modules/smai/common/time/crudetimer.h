#ifndef CRUDE_TIMER_H
#define CRUDE_TIMER_H

#include <ctime>

class CrudeTimer {

private:

  double start_time; // in seconds

  CrudeTimer() {
    start_time = time(0) * 1000;
  }

  CrudeTimer(const CrudeTimer&);
  CrudeTimer& operator=(const CrudeTimer&);

public:
  static CrudeTimer* instance();
  double get_current_time() {
    return time(0) * 1000 - start_time;
  }
};


#endif
