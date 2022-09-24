#include "path.h"
#include "../misc/utils.h"
#include "../2d/transformations.h"

std::vector<Vector2D> Path::create_random_path(int number_of_waypoints,
                                             double minimum_x,
                                             double minimum_y,
                                             double maximim_x,
                                             double maximim_y)
{
    waypoints.clear();
    /*

      DOES NOTHING AS NEEDS TO CATER FOR REGISTERED PATHS


    double middle_x = (maximim_x+minimum_x)/2.0;
    double middle_y = (maximim_y+minimum_y)/2.0;

    double smaller = fmin(middle_x, middle_y);

    double spacing = TwoPi/(double)number_of_waypoints;

    for (int i=0; i<number_of_waypoints; ++i)
    {
      double RadialDist = RandInRange(smaller*0.2f, smaller);

      Vector2D temp(RadialDist, 0.0f);

      Vec2DRotateAroundOrigin(temp, i*spacing);

      temp.x += middle_x; temp.y += middle_y;

      waypoints.push_back(temp);
    }

    current_waypoint = waypoints.at(0);

    return waypoints;
  */
  return waypoints;
}
