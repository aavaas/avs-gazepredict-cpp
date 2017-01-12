//this class is for optimization and data transfer between modules

//if continious winks and blinks filter them and adjust values at runtime
   //possibly turn histograms on or off

//predict the position of eyes with help of previous data or one cascade only(nose)

//predict the sureness of face and eyes and turn haarcascades on/off at runtime

//use kalman filters to find smooth centres from haar cascades
#include "stateclass.h"

#include <iostream> // TODO remove at last


void stateclass::pushsubstate()
{
   stateque.push_front(cur);
   if (stateque.size()>5)
      stateque.pop_back();
}
