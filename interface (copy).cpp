#include "interface.h"


int interfaceclass::start(void) //interface background loop ie not for user
{
   double waitperiod=1000.0/framerate;
   double waittime;
   while(currentkey!=globalendkey)
      {
         double ticks = (double)cv::getTickCount(); //no of ticks till now

//------------------------------------------------------------------------------
         //key handling for all the windows
         keytoid=currentkey-48;
         if (currentkey>0) //if there is a key press
         {
            if (keytoid>0 and keytoid<=noofwin) //if it is number key
               {
                  getwindow_p(keytoid)->pause();
               }
            else if (currentkey==globalpausekey and activewin!=0)
               {
                  getwindow_p(activewin)->pause();
               }
            else if (currentkey==globaldelkey and activewin!=0)
               {
                  deletewindow(activewin);
               }
         }



         //goto main loop
         mainloop(currentkey);


         //redraw windows
         for (int tid=1; tid<=noofwin; tid++)
            {
               getwindow_p(tid)->redraw();
            }

//------------------------------------------------------------------------------
         ticks = ((double)cv::getTickCount() - ticks)/cv::getTickFrequency();//"Time passed in seconds: "
         waittime=waitperiod - (ticks*1000);
         if (waittime<1) waittime=waitperiod; //if it took longer than a framerate, skip a frame
         //std::cout<<waitperiod<<" "<<ticks<<" "<<" "<<(waitperiod - (ticks*1000.0))<<" "<<waittime<<std::endl;

         currentkey=cv::waitKey(waittime);
         if (noofwin==0) return -1;  //exit out of loop if no window
      }

}

int interfaceclass::newwindow(char* name,int val)
{
   if ((noofwin+1)<winmax_val)
      {
         noofwin++;
         if (!val)
         {
            cv::namedWindow(name,CV_WINDOW_NORMAL);
         }
         else{
            cv::namedWindow(name,CV_WINDOW_AUTOSIZE);
         }
         //std::cout<<"about to be created id"<<noofwin<<std::endl;
         winarray[noofwin] = new windowclass(noofwin,name);
         cv::setMouseCallback(name,mousehandler,(&(winarray[noofwin]->id)));
         return 0;
      }
}
int interfaceclass::deletewindow(int id)
{
   if (id<=noofwin and id>0)
      {
         cv::destroyWindow(winarray[id]->name);
         delete winarray[id];
         for(int i=id; i<noofwin; i++)
            {
               winarray[i]=winarray[i+1];
               winarray[i]->id--;
               std::cout<<"del loop"<<i<<std::endl;
            }
         noofwin--;
      }
   //for(int a=1; a<=noofwin;a++) winarray[a]->showme();
}

windowclass* interfaceclass::getwindow_p(int a)
{
   if (a<1) return 0;
   return winarray[a];
}


void interfaceclass::mousehandler (int event, int x, int y, int, void*id)
{
   if (event==CV_EVENT_LBUTTONDOWN)
      {
         activewin=*(static_cast<int *>(id));
         std::cout<<"id:"<<activewin<<" x:"<<x<<" y:"<<y<<std::endl; //show active window and xy pos // TODO remove at last
      }
}


void interfaceclass::setframerate(int a)
{
   framerate=a;
}


void interfaceclass::setendkey(char a)
{
   globalendkey=a;
}

void interfaceclass::setpausekey(char ch)
{
   globalpausekey=ch;
}

void interfaceclass::setdelkey(char ch)
{
   globaldelkey=ch;
}

int interfaceclass::getwebcam()
{
   cv::VideoCapture tmp(0);
   source =tmp;
   if(!source.isOpened())
      {
         std::cout << "ERROR: Could not open camera" << std::endl;
         return 11;
      }
   return 0;
}

void interfaceclass::getframe(cv::Mat &avsMainMat)
{
   source >> avsMainMat;
   if( avsMainMat.empty() )
      {
         std::cout << "ERROR: no frame captured!" << std::endl;
      }
   cv::flip(avsMainMat, avsMainMat, 1);
}


void interfaceclass::setwinmatrix(int id,cv::Mat* avsMat)
{
   winarray[id]->setmatrix(avsMat) ;
}






