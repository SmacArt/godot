#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <iostream>
#include <math.h>

struct Telegram {
public:
  double dispatch_time;
  int sender;
  int receiver;
  int msg;
  void* extra_info;

  Telegram():dispatch_time(-1),
             sender(-1),
             receiver(-1),
             msg(-1)
  {}

  Telegram(double _time,
           int _sender,
           int _receiver,
           int _msg,
           void* _info = NULL): dispatch_time(_time),
                                sender(_sender),
                                receiver(_receiver),
                                msg(_msg),
                                extra_info(_info)
  {}
};

const double SMALLEST_DELAY= 0.25;

inline bool operator==(const Telegram& t1, const Telegram& t2){
  return (fabs(t1.dispatch_time-t2.dispatch_time) < SMALLEST_DELAY) &&
          (t1.sender==t2.sender) &&
          (t1.receiver==t2.receiver) &&
          (t1.msg==t2.msg);
}

inline bool operator<(const Telegram& t1, const Telegram& t2) {
  if (t1==t2){
    return false;
  } else {
    return (t1.dispatch_time < t2.dispatch_time);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Telegram& t){
  os << "time: " << t.dispatch_time << " sender: " << t.sender << " receiver: " << t.receiver << " msg: " << t.msg;
  return os;
}

template <class T>
inline T deference_to_type(void* p) {
  return *(T*)(p);
}


#endif
