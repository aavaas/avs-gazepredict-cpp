#ifndef FINDCEN_H_INCLUDED
#define FINDCEN_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "stateclass.h"


class findcenclass
{
private:
   cv::Mat *matcen;

   cv::Point leftpupil;
   cv::Point rightpupil;

public:
   void setmatrix(cv::Mat *);

   void findcentre(char id,cv::Rect roi);

   void drawfound(cv::Mat (*m)=0);


};

#endif // FINDCEN_H_INCLUDED
