#ifndef WINDOWCLASS_H_INCLUDED
#define WINDOWCLASS_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream> //test
#include<cstring>

class windowclass
{
public:
   int id;
   char* name;
   bool active;
   cv::Mat* framematrix;

public:
   windowclass(int no,char *nm):id(no)
   {
      std::cout<<"window created and id is:"<<id<<std::endl;
      name=new char[std::strlen(nm)];
      std::strcpy(name,nm);
      active=1;
   }

   void setmatrix(cv::Mat*);
   void redraw();
   void showdetails();


   int pause();


   ~windowclass()
   {
      delete[] name;
   }
};

#endif // WINDOWCLASS_H_INCLUDED
