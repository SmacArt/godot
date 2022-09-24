#ifndef STEERING_BEHAVIORS_H
#define STEERING_BEHAVIORS_H

#include "vehicle.h"
#include "basegameentity.h"
#include "common/2d/wall2d.h"
#include "common/2d/path.h"


class SteeringBehavior
{
public:

  enum Logic{AI_BY_EXAMPLE, NATURE_OF_CODE};

  enum SummingMethod{WEIGHTED_AVERAGE, PRIORITIZED, DITHERED};

  void set_logic (const int logic_id) {
    if (logic_id == 2) {
      set_logic_nature_of_code();
    } else {
      set_logic_game_ai_by_example();
    }
  }

private:

  enum behavior_type
    {
     NONE               = 0x00000,
     SEEK               = 0x00002,
     FLEE               = 0x00004,
     ARRIVE             = 0x00008,
     WANDER             = 0x00010,
     COHESION           = 0x00020,
     SEPARATION         = 0x00040,
     ALIGNMENT          = 0x00080,
     OBSTACLE_AVOIDANCE = 0x00100,
     WALL_AVOIDANCE     = 0x00200,
     FOLLOW_PATH        = 0x00400,
     PURSUIT            = 0x00800,
     EVADE              = 0x01000,
     INTERPOSE          = 0x02000,
     HIDE               = 0x04000,
     FLOCK              = 0x08000,
     OFFSET_PURSUIT     = 0x10000,
    };

private:

  Logic logic = AI_BY_EXAMPLE;

  //a pointer to the owner of this instance
  Vehicle* vehicle;

  //the steering force created by the combined effect of all
  //the selected behaviors
  Vector2D steering_force;

  //these can be used to keep track of friends, pursuers, or prey
  Vehicle* target_agent_1;
  Vehicle* target_agent_2;

  //the current target
  Vector2D target;

  //length of the 'detection box' utilized in obstacle avoidance
  double detection_box_length;
  double minimum_detection_box_length;

  //a vertex buffer to contain the feelers rqd for wall avoidance
  std::vector<Vector2D> feelers;

  //the length of the 'feeler/s' used in wall detection
  double feelers_length;

  //the current position on the wander circle the agent is
  //attempting to steer towards
  Vector2D wander_target;

  //explained above
  double wander_jitter;
  double wander_radius;
  double wander_distance;
  double wander_force_multiplier;

  //multipliers. These can be adjusted to effect strength of the
  //appropriate behavior. Useful to get flocking the way you require
  //for example.
  double weight_wander;
  double weight_arrive;
  double weight_seek;
  double weight_flee;
  double weight_evade;
  double weight_pursuit;
  double weight_offset_pursuit;
  double weight_interpose;
  double weight_hide;
  double weight_separation;
  double weight_cohesion;
  double weight_alignment;
  double weight_obstacle_avoidance;
  double weight_wall_avoidance;
  double weight_follow_path;

  double probability_wander;
  double probability_arrive;
  double probability_seek;
  double probability_flee;
  double probability_evade;
  double probability_interpose;
  double probability_hide;
  double probability_obstacle_avoidance;
  double probability_wall_avoidance;
  double probability_follow_path;

  //how far the agent can 'see'
  double view_distance;

  //pointer to any current path
  Path* path;

  //the distance (squared) a vehicle has to be from a path waypoint before
  //it starts seeking to the next waypoint
  double path_waypoint_seek_distance;
  double path_waypoint_seek_distance_sq;
  double path_predict_position;
  double follow_path_force_multiplier;

  //any offset used for formations or offset pursuit
  Vector2D offset;

  //Arrive makes use of these to determine how quickly a vehicle
  //should decelerate to its target
  enum Deceleration{SLOW = 3, NORMAL = 2, FAST = 1};

  //default
  Deceleration deceleration;
  //because Deceleration is enumerated as an int, this value is required
  //to provide fine tweaking of the deceleration..
  double deceleration_tweaker = 0.3;

  //is cell space partitioning to be used or not?
  bool cell_space_on;

  //what type of method is used to sum any active behavior
  SummingMethod summing_method;

  //binary flags to indicate whether or not a behavior should be active
  int flags;

  //this function tests if a specific bit of Flags is set
  bool is_on(behavior_type bt){return (flags & bt) == bt;}

  bool accumulate_force(Vector2D &sf, Vector2D ForceToAdd);

  bool use_evade_threat_range;
  double evade_threat_range;

  double arrive_radius;
  double wander_theta = 0;
  double path_id = 0;

  //creates the antenna utilized by the wall avoidance behavior
  void create_feelers();

  void set_logic_game_ai_by_example() {logic = AI_BY_EXAMPLE;}
  void set_logic_nature_of_code() {logic = NATURE_OF_CODE;}


/* .......................................................
   BEGIN BEHAVIOR DECLARATIONS - AI BY EXAMPLE
      .......................................................*/

  //this behavior moves the agent towards a target position
  Vector2D seek(Vector2D target_position);

  //this behavior returns a vector that moves the agent away
  //from a target position
  Vector2D flee(Vector2D target_position);

  //this behavior is similar to seek but it attempts to arrive
  //at the target position with a zero velocity
  Vector2D arrive(Vector2D     target_position,
                  Deceleration deceleration);

  //this behavior predicts where an agent will be in time T and seeks
  //towards that point to intercept it.
  Vector2D pursuit(const Vehicle* agent);

  //this behavior maintains a position, in the direction of offset
  //from the target vehicle
  Vector2D offset_pursuit(const Vehicle* agent, const Vector2D offset);

  //this behavior attempts to evade a pursuer
  Vector2D evade(const Vehicle* agent);

  //this behavior makes the agent wander about randomly
  Vector2D wander();

  //this returns a steering force which will attempt to keep the agent
  //away from any obstacles it may encounter
  Vector2D obstacle_avoidance(const std::vector<BaseGameEntity*>& obstacles);

  //this returns a steering force which will keep the agent away from any
  //walls it may encounter
  Vector2D wall_avoidance(const std::vector<Wall2D*> walls);

  //given a series of Vector2Ds, this method produces a force that will
  //move the agent along the waypoints in order
  Vector2D follow_path();

  //this results in a steering force that attempts to steer the vehicle
  //to the center of the vector connecting two moving agents.
  Vector2D interpose(const Vehicle* vehicle_a, const Vehicle* vehicle_b);

  //given another agent position to hide from and a list of BaseGameEntitys this
  //method attempts to put an obstacle between itself and its opponent
  Vector2D hide(const Vehicle* hunter, const std::vector<BaseGameEntity*>& obstacles);

  // -- Group Behaviors -- //

  Vector2D cohesion(const std::vector<Vehicle*> &agents);

  Vector2D separation(const std::vector<Vehicle*> &agents);

  Vector2D alignment(const std::vector<Vehicle*> &agents);

  //the following three are the same as above but they use cell-space
  //partitioning to find the neighbors
  Vector2D cohesion_plus(const std::vector<Vehicle*> &agents);
  Vector2D separation_plus(const std::vector<Vehicle*> &agents);
  Vector2D alignment_plus(const std::vector<Vehicle*> &agents);

    /* .......................................................
                       END BEHAVIOR DECLARATIONS - AI BY EXAMPLE
      .......................................................*/


  /* .......................................................
     BEGIN BEHAVIOR DECLARATIONS - NATURE OF CODE
     .......................................................*/

  //this behavior moves the agent towards a target position
  Vector2D noc_seek(Vector2D target_position);
  Vector2D noc_arrive(Vector2D target_position);
  Vector2D noc_wander();
  Vector2D noc_follow_path();

  /* .......................................................
     END BEHAVIOR DECLARATIONS - AI BY EXAMPLE
     .......................................................*/

  //calculates and sums the steering forces from any active behaviors
  Vector2D calculate_weighted_sum();
  Vector2D calculate_prioritized();
  Vector2D calculate_dithered();

  //helper method for Hide. Returns a position located on the other
  //side of an obstacle to the pursuer
  Vector2D get_hiding_position(const Vector2D& position_of_obstable,
                               const double radius_of_obstacle,
                               const Vector2D& position_of_hunter);

  // temp for debugging
  Vector2D temp_final_target;
  Vector2D temp_returned_wander;

public:
  //temp
  void set_temp_final_target(Vector2D t) {temp_final_target = t;}
  Vector2D get_temp_final_target() {return temp_final_target;}
  void set_temp_returned_wander(Vector2D t) {temp_returned_wander = t;}
  Vector2D get_temp_returned_wander() {return temp_returned_wander;}
  //temp

  SteeringBehavior(Vehicle* agent);

  virtual ~SteeringBehavior();

  //calculates and sums the steering forces from any active behaviors
  Vector2D calculate();

  //calculates the component of the steering force that is parallel
  //with the vehicle heading
  double    forward_component();

  //calculates the component of the steering force that is perpendicuar
  //with the vehicle heading
  double    side_component();

  void      set_target(const Vector2D _target){target = _target;}

  void      set_target_agent1(Vehicle* agent){target_agent_1 = agent;}
  void      set_target_agent2(Vehicle* agent){target_agent_2 = agent;}

  void      set_offset(const Vector2D _offset){offset = _offset;}
  Vector2D  get_offset()const{return offset;}

  Vector2D get_wander_target() { return wander_target;}

  void set_path(std::vector<Vector2D> new_path){path->set(new_path);}
  void create_random_path(int num_waypoints, int mx, int my, int cx, int cy) const {path->create_random_path(num_waypoints, mx, my, cx, cy);}

  Vector2D get_steering_force()const{return steering_force;}

  void toggle_space_partitioning_on_off(){cell_space_on = !cell_space_on;}
  bool is_space_partitioning_on()const{return cell_space_on;}

  void set_summing_method(SummingMethod sm){summing_method = sm;}


  void flee_on(){flags |= FLEE;}
  void seek_on(){flags |= SEEK;}
  void arrive_on(){flags |= ARRIVE;}

  void wander_on(){
    flags |= WANDER;
    double theta = RandFloat() * TwoPi;
    wander_target = Vector2D(wander_radius * cos(theta),
                             wander_radius * sin(theta));
  }

  void pursuit_on(Vehicle* v){flags |= PURSUIT; target_agent_1 = v;}
  void evade_on(Vehicle* v){flags |= EVADE; target_agent_1 = v;}
  void cohesion_on(){flags |= COHESION;}
  void separation_on(){flags |= SEPARATION;}
  void alignment_on(){flags |= ALIGNMENT;}
  void obstacle_avoidance_on(){flags |= OBSTACLE_AVOIDANCE;}
  void wall_avoidance_on(){flags |= WALL_AVOIDANCE;}

  void follow_path_on(Path* follow_path){
    flags |= FOLLOW_PATH;
    if (follow_path) {
      path = follow_path;
    } else {
      path = new Path();
      path->set_looped_on();
    }
    path_id = path->register_vehicle();
  }

  void interpose_on(Vehicle* v1, Vehicle* v2){flags |= INTERPOSE; target_agent_1 = v1; target_agent_2 = v2;}
  void hide_on(Vehicle* v){flags |= HIDE; target_agent_1 = v;}
  void offset_pursuit_on(Vehicle* v1, const Vector2D _offset){flags |= OFFSET_PURSUIT; offset = _offset; target_agent_1 = v1;}
  void flocking_on(){cohesion_on(); alignment_on(); separation_on(); wander_on();}

  void flee_off()  {if(is_on(FLEE))   flags ^=FLEE;}
  void seek_off()  {if(is_on(SEEK))   flags ^=SEEK;}
  void arrive_off(){if(is_on(ARRIVE)) flags ^=ARRIVE;}
  void wander_off(){if(is_on(WANDER)) flags ^=WANDER;}
  void pursuit_off(){if(is_on(PURSUIT)) flags ^=PURSUIT;}
  void evade_off(){if(is_on(EVADE)) flags ^=EVADE;}
  void cohesion_off(){if(is_on(COHESION)) flags ^=COHESION;}
  void separation_off(){if(is_on(SEPARATION)) flags ^=SEPARATION;}
  void alignment_off(){if(is_on(ALIGNMENT)) flags ^=ALIGNMENT;}
  void obstacle_avoidance_off(){if(is_on(OBSTACLE_AVOIDANCE)) flags ^=OBSTACLE_AVOIDANCE;}
  void wall_avoidance_off(){if(is_on(WALL_AVOIDANCE)) flags ^=WALL_AVOIDANCE;}
  void follow_path_off(){if(is_on(FOLLOW_PATH)) flags ^=FOLLOW_PATH;}
  void interpose_off(){if(is_on(INTERPOSE)) flags ^=INTERPOSE;}
  void hide_off(){if(is_on(HIDE)) flags ^=HIDE;}
  void offset_pursuit_off(){if(is_on(OFFSET_PURSUIT)) flags ^=OFFSET_PURSUIT;}
  void flockingOff(){cohesion_off(); alignment_off(); separation_off(); wander_off();}

  bool is_flee_on(){return is_on(FLEE);}
  bool is_seek_on(){return is_on(SEEK);}
  bool is_arrive_on(){return is_on(ARRIVE);}
  bool is_wander_on(){return is_on(WANDER);}
  bool is_pursuit_on(){return is_on(PURSUIT);}
  bool is_evade_on(){return is_on(EVADE);}
  bool is_cohesion_on(){return is_on(COHESION);}
  bool is_separation_on(){return is_on(SEPARATION);}
  bool is_alignment_on(){return is_on(ALIGNMENT);}
  bool is_obstacle_avoidance_on(){return is_on(OBSTACLE_AVOIDANCE);}
  bool is_wall_avoidance_on(){return is_on(WALL_AVOIDANCE);}
  bool is_follow_path_on(){return is_on(FOLLOW_PATH);}
  bool is_interpose_on(){return is_on(INTERPOSE);}
  bool is_hide_on(){return is_on(HIDE);}
  bool is_offset_pursuit_on(){return is_on(OFFSET_PURSUIT);}

  void set_minimum_detection_box_length(const double length) {minimum_detection_box_length=length;}
  double get_minimum_detection_box_length()const{return minimum_detection_box_length;}
  double get_detection_box_length()const{return detection_box_length;}

  const std::vector<Vector2D>& get_feelers()const{return feelers;}

  void set_wander_jitter(const double jitter) {wander_jitter=jitter;}
  void set_wander_distance(const double distance) {wander_distance=distance;}
  void set_wander_radius(const double radius) {wander_radius=radius;}
  void set_wander_force_multiplier(const double force_multiplier) {wander_force_multiplier=force_multiplier;}

  double get_wander_jitter()const{return wander_jitter;}
  double get_wander_distance()const{return wander_distance;}
  double get_wander_radius()const{return wander_radius;}
  double get_wander_force_multiplier()const{return wander_force_multiplier;}

  double get_separation_weight()const{return weight_separation;}
  double get_alignment_weight()const{return weight_alignment;}
  double get_cohesion_weight()const{return weight_cohesion;}

  void set_probability_wander(const double probability){probability_wander=probability;}
  void set_probability_arrive(const double probability){probability_arrive=probability;}
  void set_probability_seek(const double probability){probability_seek=probability;}
  void set_probability_flee(const double probability){probability_flee=probability;}
  void set_probability_evade(const double probability){probability_evade=probability;}
  void set_probability_interpose(const double probability){probability_interpose=probability;}
  void set_probability_hide(const double probability){probability_hide=probability;}
  void set_probability_obstacle_avoidance(const double probability){probability_obstacle_avoidance=probability;}
  void set_probability_wall_avoidance(const double probability){probability_wall_avoidance=probability;}
  void set_probability_follow_path(const double probability){probability_follow_path=probability;}

  void set_weight_wander(const double weight){weight_wander=weight;}
  void set_weight_arrive(const double weight){weight_arrive=weight;}
  void set_weight_seek(const double weight){weight_flee=weight;}
  void set_weight_flee(const double weight){weight_flee=weight;}
  void set_weight_pursuit(const double weight){weight_pursuit=weight;}
  void set_weight_evade(const double weight){weight_evade=weight;}
  void set_weight_interpose(const double weight){weight_interpose=weight;}
  void set_weight_hide(const double weight){weight_hide=weight;}
  void set_weight_obstacle_avoidance(const double weight){weight_obstacle_avoidance=weight;}
  void set_weight_wall_avoidance(const double weight){weight_wall_avoidance=weight;}
  void set_weight_follow_path(const double weight){weight_follow_path=weight;}

  void set_deceleration(const int _deceleration) {
    switch (_deceleration) {
    case 1:
      deceleration = FAST;
      break;
    case 3:
      deceleration = SLOW;
      break;
    default:
      deceleration = NORMAL;
    }
  }
  void set_deceleration_tweaker(const double value) {deceleration_tweaker=value;}

  void set_use_evade_threat_range(const bool use) {use_evade_threat_range=use;}
  void set_evade_threat_range(const double range) {evade_threat_range=range;}

  void set_path_waypoint_seek_distance(const double distance) {
    path_waypoint_seek_distance=distance;
    path_waypoint_seek_distance_sq = path_waypoint_seek_distance * path_waypoint_seek_distance;
  }
  void set_path_predict_position(const double position) {path_predict_position=position;}
  void set_follow_path_force_multiplier(const double force_multiplier) {follow_path_force_multiplier=force_multiplier;}
  double get_follow_path_force_multiplier()const{return follow_path_force_multiplier;}

  void set_arrive_radius(const double radius) {arrive_radius = radius;}
};

#endif
