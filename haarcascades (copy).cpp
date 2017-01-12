//universal haarcascades for face and eyes working together
#include "haarcascades.h"

int avshaarclass::loadcascade(cv::CascadeClassifier &CC,char* name)
{
   if( !CC.load(name))
      {
         std::cerr << "ERROR: Could not open: "<<name<<std::endl;
         return -1;
      }
}


int avshaarclass::setmatrix(cv::Mat *tmp)
{
   mathaar=tmp;
}


int avshaarclass::findface(cv::Mat *m)
{
   vface.clear();
   cvtColor( *mathaar, *mathaar, CV_BGR2GRAY );
   //equalizeHist( *mathaar, *mathaar);  //  @todo optional on/off

   face_cascade.detectMultiScale(
      *mathaar,
      vface,
      1.3,
      2,
      CV_HAAR_SCALE_IMAGE,
      cv::Size(250, 250));
}

int avshaarclass::findeyes(cv::Mat *m)
{
   if(!vface.size()) return 0; //if no face
   cv::Rect tmpeyeroi(
      vface[0].x+vface[0].width*0.1,
      vface[0].y+vface[0].height*0.2,
      vface[0].width*0.8,
      vface[0].height/2.7);
   eyeroi=tmpeyeroi;

   ////do this only at eye ball
   //float sigma = 0.005 * eyeroi.width;
   //GaussianBlur( eyeroi, eyeroi, cv::Size( 0, 0 ), sigma);

   vlefteye.clear();
   vrighteye.clear();

   cv::Rect leyeroi(eyeroi);
   leyeroi.width/=2;


   cv::Mat faceroi=(*mathaar)(leyeroi);
   equalizeHist( faceroi, faceroi );//equalize histogram for left eye

   eyes_cascade.detectMultiScale(
      faceroi,
      vlefteye,
      1.3,
      2,
      CV_HAAR_SCALE_IMAGE,
      cv::Size(70, 70));

   if (vlefteye.size()>0)
   {
   g_leyeroi=cv::Rect(eyeroi.x+vlefteye[0].x,
                      eyeroi.y+vlefteye[0].y,
                      vlefteye[0].width,
                      vlefteye[0].height);
   }
//-------------------------------------------------------------
   cv::Rect reyeroi(leyeroi);
   reyeroi.x+=eyeroi.width/2;
   faceroi=(*mathaar)(reyeroi);

   equalizeHist( faceroi, faceroi );//equalize histogram for right eye

   eyes_cascade.detectMultiScale(
      faceroi,
      vrighteye,
      1.3,
      2,
      CV_HAAR_SCALE_IMAGE,
      cv::Size(70, 70));

   if(vrighteye.size()>0)
   {
   g_reyeroi=cv::Rect(eyeroi.x+vrighteye[0].x+eyeroi.width/2,
                                  eyeroi.y+vrighteye[0].y,
                                  vrighteye[0].width,
                                  vrighteye[0].height);
   }
}

int avshaarclass::findnose(cv::Mat *m)
{
   if(!vface.size()) return 0; //if noface

   cv::Rect tmpnoseroi(
      vface[0].x+vface[0].width/4,
      vface[0].y+vface[0].height/3,
      vface[0].width/2,
      vface[0].height/2);
   noseroi=tmpnoseroi;

   vnose.clear();

   cv::Mat faceroi=(*mathaar)(noseroi);
   nose_cascade.detectMultiScale(
      faceroi,
      vnose,
      1.3,
      2,
      CV_HAAR_SCALE_IMAGE,
      cv::Size(70, 70));

}

void avshaarclass::drawfound(cv::Mat *m)
{
   if(!m) m=mathaar;
   for( int i = 0; i < vface.size(); i++ )
      {
         cv::rectangle(*m, vface[i], cv::Scalar(255,255,255));//big face region

         //middle line
         rectangle(*m,
                   cv::Rect(vface[0].x+vface[0].width/2,
                            vface[0].y,
                            2,
                            vface[0].height),
                   cv::Scalar(255,255,255));


         cv::rectangle(*m, noseroi, cv::Scalar(255,255,255));//nose region

         if (eyeroi.width>0)
            rectangle(*m, eyeroi, cv::Scalar(255,255,255)); //two eyes region

         if (vlefteye.size()>0)
            {
               rectangle(*m,g_leyeroi,cv::Scalar(255,255,255));
               circle(*m,
                      cv::Point(eyeroi.x+vlefteye[0].x+vlefteye[0].width/2,
                                eyeroi.y+vlefteye[0].y+vlefteye[0].height/2),
                      15,
                      cv::Scalar(255,255,255));
            }
         if (vrighteye.size()>0)
            {
               rectangle(*m,g_reyeroi,cv::Scalar(255,255,255));
               circle(*m,
                      cv::Point(eyeroi.x+eyeroi.width/2+vrighteye[0].x+vrighteye[0].width/2,
                                eyeroi.y+vrighteye[0].y+vrighteye[0].height/2),
                      15,
                      cv::Scalar(255,255,255));
            }

         if (vnose.size()>0)
            {
               rectangle(*m,
                         cv::Rect(noseroi.x+vnose[0].x,
                                  noseroi.y+vnose[0].y,
                                  vnose[0].width,
                                  vnose[0].height),
                         cv::Scalar(255,255,255));
            }

      }
}

int avshaarclass::haarblinkstate(bool flag)
{
   if (vface.size()>0)
      {
         if (vlefteye.size()==0 and vrighteye.size()==0)
            {
               blinkstate=1;
               if (flag) std::cout<<"blink"<<std::endl;
            }
         else if (vlefteye.size()==0)
            {
               blinkstate=2;
               if (flag) std::cout<<"l-wink"<<std::endl;
            }
         else if (vrighteye.size()==0)
            {
               blinkstate=3;
               if (flag) std::cout<<"R-wink"<<std::endl;
            }
      }
}


void avshaarclass::updstatebyhaar(stateclass &state)
{
   if (vface.size()>0) state.cur.headroi=vface[0];
   if (vnose.size()>0) state.cur.noseroi=vnose[0];
   if (vlefteye.size()>0) state.cur.leyeroi=g_leyeroi;
   if (vrighteye.size()>0) state.cur.reyeroi=g_reyeroi;

   state.pushsubstate();
}
