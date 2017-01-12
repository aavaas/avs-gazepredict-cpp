#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "windowclass.h"

#include <iostream>


void mainloop(char);//import main.cpp's active main loop

const int MAXWINDOWSNO =5;


class interfaceclass
{
private:
   const int winmax_val;
   int noofwin;

   int currentkey;
   int keytoid; //for keyboard key to id
   int globalendkey;
   char globalpausekey;
   char globaldelkey;
   int framerate;

   cv::VideoCapture source;

   windowclass* winarray[MAXWINDOWSNO+1]; //max windows  [5]no 0f windows

public:
   static int activewin; //current actie window id

   interfaceclass():winmax_val(MAXWINDOWSNO+1)
   {
      noofwin=0;
      currentkey=-1;
      globalendkey=27;
      globalpausekey='p';
      globaldelkey=27;
      framerate=15;
   }


   int start(); //for initiating the main loop
   void setendkey(char a=27);
   void setpausekey(char ch='p');
   void setdelkey(char ch='d');


   void setframerate(int); //framerate

   int newwindow(char* name,int val=0);// TODO name, flags( resize, auto )
   int deletewindow(int); //delete window (window id)
   windowclass* getwindow_p(int); //get window pointer(id)
   void setwinmatrix(int,cv::Mat*); //assin the matrix the window uses to update itself

   //detect active window and pause accordingly

   static void mousehandler (int event, int x, int y, int, void*);

   // TODO have mouse draw and select functions
   // TODO have trackbar window  interface

   int getwebcam(); //initialize the default webcam
   void getframe(cv::Mat&); //get a webcam frame into the matrix passed


   ~interfaceclass()
   {
      cv::destroyAllWindows();

      //std::cout<<"start delete"<<std::endl;
      for(int a=1; a<=noofwin;a++) delete winarray[a];
      //std::cout<<"enddelete"<<std::endl;
   }
};



#endif // INTERFACE_H_INCLUDED
