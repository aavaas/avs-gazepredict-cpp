#include "windowclass.h"


void windowclass::setmatrix(cv::Mat* avsMat)
{
   framematrix=avsMat;
}


void windowclass::redraw()
{
   if(active) cv::imshow(name,*framematrix);
}

int windowclass::pause()
{
   active=!active;
}

void windowclass::showdetails()
{
   std::cout<<"id is:"<<id<<std::endl;
   std::cout<<"name is:"<<name<<std::endl;
}


