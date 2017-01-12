#ifndef HAARCASCADES_H_INCLUDED
#define HAARCASCADES_H_INCLUDED

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "stateclass.h"

#include <iostream>
#include<vector>



class avshaarclass
{
private:
   cv::Mat *mathaar;

   char *name[3];
   int minsize;
   float scalefactor;
   int noofneigh;

   cv::CascadeClassifier face_cascade;
   cv::CascadeClassifier eyes_cascade;
   cv::CascadeClassifier nose_cascade;

   cv::Rect eyeroi; //dumb roi from head rectangle
   cv::Rect noseroi;

   cv::Rect g_leyeroi;//in global cocordinates
   cv::Rect g_reyeroi;



   // @todo only have single rectangle here from found haar
   std::vector<cv::Rect> vface;
   std::vector<cv::Rect> vlefteye;
   std::vector<cv::Rect> vrighteye;
   std::vector<cv::Rect> vnose;

   int blinkstate;

public:


   avshaarclass()
   {
      scalefactor=1.3;
      noofneigh=3;

      name[0]="res/haarcascade_frontalface_alt.xml";
      name[1]="res/haarcascade_eye.xml";
      name[2]="res/haarcascade_mcs_nose.xml";

      loadcascade(face_cascade,name[0]);
      loadcascade(eyes_cascade,name[1]);
      loadcascade(nose_cascade,name[2]);
   }


   int loadcascade(cv::CascadeClassifier &,char*);


   int setmatrix(cv::Mat *); // TODO convert colour here also pass the roi

   //TODO change to take the ROI
   int findface(cv::Mat (*m)=0); //TODO add parameter to set the eyes and nose roi
   int findeyes(cv::Mat (*m)=0); //TODO change to get the predefined roi
   int findnose(cv::Mat (*m)=0);

   void drawfound(cv::Mat (*m)=0);
   int haarblinkstate(bool flag=0); //1 for display output

   void updstatebyhaar(stateclass &state);
   // TODO : updhaarbystate
};



#endif // HAARCASCADES_H_INCLUDED
