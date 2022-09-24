#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <cassert>
#include <string>

#include "state.h"
#include "messaging/telegram.h"

template <class entity_type>
class StateMachine
{
private:
  entity_type* owner;

  State<entity_type>* current_state;

  State<entity_type>* previous_state;

  State<entity_type>* global_state;

public:
  StateMachine(entity_type* owner):owner(owner),
                                   current_state(NULL),
                                   previous_state(NULL),
                                   global_state(NULL)
  {}

  virtual ~StateMachine(){}

  void set_current_state(State<entity_type>* s) {current_state = s;}
  void set_global_state(State<entity_type>* s) {global_state = s;}
  void set_previous_state(State<entity_type>* s) {previous_state = s;}

  void update() const {
    if (global_state) global_state->execute(owner);
    if (current_state) current_state->execute(owner);
  }

  bool handle_message(const Telegram& msg) const {
    if (current_state && current_state->on_message(owner, msg)) {
      return true;
    }

    if (global_state && global_state->on_message(owner, msg)) {
      return true;
    }

    return false;
  }

  void change_state(State<entity_type>* new_state) {
    assert(new_state && "StateMachine::change_state>: trying to change to NULL state");
    previous_state = current_state;
    current_state->exit(owner);
    current_state = new_state;
    current_state->enter(owner);
  }

  void revert_to_previous_state() {
    change_state(previous_state);
  }

  bool is_in_state(const State<entity_type>& state)const{
    return typeid(*current_state) == typeid(state);
  }

  State<entity_type>* get_current_state() const {return current_state;}
  State<entity_type>* get_global_state() const {return global_state;}
  State<entity_type>* get_previous_state() const {return previous_state;}

};

#endif
