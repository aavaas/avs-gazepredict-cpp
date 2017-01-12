#include "interface.h"
#include "haarcascades.h"
#include "findcen.h"

#include "stateclass.h"

//stateclass state;

interfaceclass interface;
int interfaceclass::activewin=0;// NOTE doesnt work when kept at interface.h why?

avshaarclass avshaar;
findcenclass findcen;
stateclass mainstate;

cv::Mat avsMainMat;
cv::Mat avsTestMat;

int main() //static part of interface;
{
   if (interface.getwebcam()) return 111;

   interface.setframerate(6);
   interface.setendkey(27); //27 esc 32 spc
   interface.setpausekey('p');
   interface.setdelkey('d');

   //main window
   interface.newwindow("Main",1);
   interface.setwinmatrix(1,&avsMainMat);
//   cv::createTrackbar( "opt1", "Main", 0, 255,0);
//   cv::createTrackbar( "opt2", "Main", 0, 255,0);
//   cv::createTrackbar( "opt3", "Main", 0, 255,0);
//      int val1,val2,val3;
//      val1=cv::getTrackbarPos("opt1", "Main");
//      val2=cv::getTrackbarPos("opt2", "Main");
//      val3=cv::getTrackbarPos("opt3", "Main");



   //eye window
//   avsTestMat=cv::imread("res/1.jpg");
//   interface.newwindow("eyel",0);
//   interface.setwinmatrix(2,&avsTestMat);

   //debug window
   cv::namedWindow("deb",CV_WINDOW_NORMAL);

   cv::waitKey();
   interface.start();
   return 0;
}

void mainloop(char key) //no cv::waitKey here
{
   interface.getframe(avsMainMat);



   avshaar.setmatrix(&avsMainMat);
   avshaar.findface();
   //avshaar.findnose();
   avshaar.findeyes();
   avshaar.haarblinkstate(1);

   avshaar.updstatebyhaar(mainstate);


   if(mainstate.cur.leyeroi.x)   // If there is left eye found
   {
   avsTestMat=avsMainMat(mainstate.cur.leyeroi);
   findcen.setmatrix(&avsTestMat);
   findcen.findcentre('l',mainstate.cur.leyeroi);
   findcen.drawfound(&avsTestMat);
   }

   avshaar.drawfound();//is destructive to matrix thus give another matrix to draw or draw at last



   //std::cout<<int(key)<<"main loop pass"<<std::endl;
}
