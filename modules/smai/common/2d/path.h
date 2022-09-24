#ifndef PATH_H
#define PATH_H
//------------------------------------------------------------------------
//
//  Name:   Path.h
//
//  Desc:   class to define, manage, and traverse a path (defined by a series of 2D vectors)
//
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <list>
#include <cassert>

#include "vector2d.h"
#include <iostream>
#include <vector>

class Path
{
private:


  //flag to indicate if the path should be looped
  //(The last waypoint connected to the first)
  bool is_looped;
  double radius;

  //points to the current waypoint
  std::vector<unsigned int> current_waypoint;

public:

  std::vector<Vector2D> waypoints;
  Path():is_looped(false) {}

  //constructor for creating a path with initial random waypoints. minimim_x/Y
  //& maximum_x/Y define the bounding box of the path.
  Path(int number_way_points,
       double minimim_x,
       double minimum_y,
       double maximum_x,
       double maximum_y,
       bool looped):is_looped(looped) {
    create_random_path(number_way_points, minimim_x, minimum_y, maximum_x, maximum_y);
  }

  // register a vehicle on the path and return their position in the current waypoint list
  int register_vehicle() {
    current_waypoint.push_back(0);
    return current_waypoint.size() - 1;
  }

  //returns the current waypoint
  Vector2D get_current_waypoint(const int registered_position)const {
    return waypoints[current_waypoint[registered_position]];
  }

  //returns true if the end of the list has been reached
  bool is_finished(const int registered_position){return !(current_waypoint[registered_position] != waypoints.size());}

  //moves the iterator on to the next waypoint in the list
  inline void set_next_waypoint(const int registered_position);

  //creates a random path which is bound by rectangle described by
  //the min/max values
  std::vector<Vector2D> create_random_path(int number_way_points,
                                         double minimim_x,
                                         double minimum_y,
                                         double maximum_x,
                                         double maximum_y);
  void set_looped_on(){is_looped = true;}
  void set_looped_off(){is_looped = false;}
  void set_radius(const double r) {radius=r;}
  double get_radius() const {return radius;}

  //adds a waypoint to the end of the path
  void add_waypoint(Vector2D new_point){
    waypoints.push_back(new_point);
  }

  //methods for setting the path with either another Path or a list of vectors
  void set(std::vector<Vector2D> new_path){waypoints = new_path;}

  void set(const Path& path){waypoints=path.get_path();}

  void clear(){waypoints.clear();}

  std::vector<Vector2D> get_path()const{return waypoints;}

};


//-------------------- Methods -------------------------------------------

inline void Path::set_next_waypoint(const int registered_position) {
  assert (waypoints.size() > 0);
  if (++current_waypoint[registered_position] == waypoints.size()) {
    if (is_looped)
    {
      current_waypoint[registered_position] = 0;
    }
  }
}

#endif
