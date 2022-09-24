#ifndef BASE_GAME_ENTITY_H
#define BASE_GAME_ENTITY_H

#include "core/variant/variant.h"
#include "common/2d/vector2d.h"
#include "common/2d/geometry.h"
#include "common/misc/utils.h"
#include "common/messaging/telegram.h"


class BaseGameEntity
{
public:

  enum {ENTITY_TYPE_DEFAULT = -1};

private:

  int id;
  int entity_type;
  bool tagged;

  int next_id(){static int next_id = 0; return next_id++;}

protected:


  //its location in the environment
  Vector2D position;
  Vector2D scale;

  //the length of this object's bounding radius
  double  bounding_radius;


  BaseGameEntity():
    id(next_id()),
    entity_type(ENTITY_TYPE_DEFAULT),
    tagged(false),
    position(Vector2D()),
    scale(Vector2D(1.0,1.0)),
    bounding_radius(0.0)
  {}

  BaseGameEntity(int entity_type):
    id(next_id()),
    entity_type(entity_type),
    tagged(false),
    position(Vector2D()),
    scale(Vector2D(1.0,1.0)),
    bounding_radius(0.0)
  {}

  BaseGameEntity(int entity_type, Vector2D pos, double r):
    id(next_id()),
    entity_type(entity_type),
    tagged(false),
    position(pos),
    scale(Vector2D(1.0,1.0)),
    bounding_radius(r)
  {}

  //this can be used to create an entity with a 'forced' ID. It can be used
  //when a previously created entity has been removed and deleted from the
  //game for some reason. For example, The Raven map editor uses this ctor
  //in its undo/redo operations.
  //USE WITH CAUTION!
  BaseGameEntity(int entity_type, int forced_id):
    id(forced_id),
    entity_type(entity_type),
    tagged(false),
    position(Vector2D()),
    scale(Vector2D(1.0,1.0)),
    bounding_radius(0.0)
  {}


public:

  virtual ~BaseGameEntity(){}

  virtual void update(double time_elapsed){};

  virtual void render(){};

  virtual bool handle_message(const Telegram& msg){return false;}

  //entities should be able to read/write their data to a stream
  virtual void write(std::ostream&  os)const{}
  virtual void read (std::ifstream& is){}

  Vector2D     get_position()const{return position;}
  void         set_position(Vector2D new_pos){position=new_pos;}

  double       get_bounding_radius()const{return bounding_radius;}
  void         set_bounding_radius(double r){bounding_radius = r;}

  int          get_id()const{return id;}

  bool         is_tagged()const{return tagged;}
  void         tag(){tagged = true;}
  void         un_tag(){tagged = false;}

  Vector2D     get_scale()const{return scale;}
  void         set_scale(Vector2D val){bounding_radius *= MaxOf(val.x, val.y) / MaxOf(scale.x, scale.y); scale = val;}
  void         set_scale(double val){bounding_radius *= (val/MaxOf(scale.x, scale.y)); scale = Vector2D(val, val);}

  int          get_entity_type()const{return entity_type;}
  void         set_entity_type(int new_type){entity_type = new_type;}

};

#endif




