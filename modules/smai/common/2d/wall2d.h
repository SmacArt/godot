#ifndef WALL_H
#define WALL_H
//------------------------------------------------------------------------
//
//  Name:   wall2d.h
//
//  Desc:   class to create and render 2D walls. Defined as the two
//          vectors A - B with a perpendicular normal.
//
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "vector2d.h"


class Wall2D
{
protected:

  Vector2D vertex_a,
           vertex_b,
           normal;

  void calculate_normal()
  {
    Vector2D temp = Vec2DNormalize(vertex_b - vertex_a);

    normal.x = -temp.y;
    normal.y = temp.x;
  }

public:

  Wall2D(){}

  Wall2D(Vector2D a, Vector2D b):vertex_a(a), vertex_b(b)
  {
    calculate_normal();
  }

  Wall2D(Vector2D a, Vector2D b, Vector2D n):vertex_a(a), vertex_b(b), normal(n)
  {}

  Vector2D get_from()const {return vertex_a;}
  void set_from(Vector2D v){vertex_a = v; calculate_normal();}

  Vector2D get_to()const    {return vertex_b;}
  void set_to(Vector2D v){vertex_b = v; calculate_normal();}

  Vector2D get_normal()const{return normal;}
  void set_normal(Vector2D n){normal = n;}

  Vector2D get_center()const{return (vertex_a+vertex_b)/2.0;}

};

#endif
