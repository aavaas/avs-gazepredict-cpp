#ifndef STATECLASS_H_INCLUDED
#define STATECLASS_H_INCLUDED

#include <deque>
#include <opencv2/core/core.hpp>

class substclass
{
public:
   cv::Rect headroi;
   cv::Rect leyeroi;
   cv::Rect reyeroi;
   cv::Rect noseroi;
};


class stateclass
{
public:
   std::deque<substclass> stateque;

   substclass cur;

   void pushsubstate();

};

#endif // STATECLASS_H_INCLUDED
