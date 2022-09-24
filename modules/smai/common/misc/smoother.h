#ifndef SMOOTHER
#define SMOOTHER
//------------------------------------------------------------------------
//
//  Name: Smoother.h
//
//  Desc: Template class to help calculate the average value of a history
//        of values. This can only be used with types that have a 'zero'
//        value and that have the += and / operators overloaded.
//
//        Example: Used to smooth frame rate calculations.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <vector>

template <class T>
class Smoother
{
private:

  //this holds the history
  std::vector<T>  history;

  int next_update_slot;

  //an example of the 'zero' value of the type to be smoothed. This
  //would be something like Vector2D(0,0)
  T zero_value;

public:

  //to instantiate a Smoother pass it the number of samples you want
  //to use in the smoothing, and an exampe of a 'zero' type
  Smoother(int _sample_size, T _zero_value):history(_sample_size, _zero_value),
                                            zero_value(_zero_value),
                                            next_update_slot(0)
  {}

  //each time you want to get a new average, feed it the most recent value
  //and this method will return an average over the last sample_size updates
  T update(const T& most_recent_value)
  {
    //overwrite the oldest value with the newest
    history[next_update_slot++] = most_recent_value;

    //make sure next_update_slot wraps around.
    if (next_update_slot == history.size()) next_update_slot = 0;

    //now to calculate the average of the history list
    T sum = zero_value;

    typename std::vector<T>::iterator it = history.begin();

    for (it; it != history.end(); ++it)
    {
      sum += *it;
    }

    return sum / (double)history.size();
  }
};


#endif
