#include "findcen.h"

#include <iostream> // TODO remove at last
#include "opencv2/highgui/highgui.hpp" // TODO remove
#include <vector>

void findcenclass::setmatrix(cv::Mat *m)
{
   matcen=m;
}
//---------------------------------------------------------------------------


//4
bool kEnableWeight = false;
float kWeightDivisor = 150.0;
void testPossibleCentersFormula(int x, int y, unsigned char weight,double gx, double gy, cv::Mat &out)
{
   // for all possible centers
   for (int cy = 0; cy < out.rows; ++cy)
      {
         double *Or = out.ptr<double>(cy);
         for (int cx = 0; cx < out.cols; ++cx)
            {
               if (x == cx && y == cy)
                  {
                     continue;
                  }
               // create a vector from the possible center to the gradient origin
               double dx = x - cx;
               double dy = y - cy;
               // normalize d
               double magnitude = sqrt((dx * dx) + (dy * dy));
               dx = dx / magnitude;
               dy = dy / magnitude;
               double dotProduct = dx*gx + dy*gy;
               dotProduct = std::max(0.0,dotProduct);
               // square and multiply by the weight
               if (kEnableWeight)
                  {
                     Or[cx] += dotProduct * dotProduct * (weight/kWeightDivisor);
                  }
               else
                  {
                     Or[cx] += dotProduct * dotProduct;
                  }
            }
      }
}

//3
double computeDynamicThreshold(const cv::Mat &mat, double stdDevFactor)
{
   cv::Scalar stdMagnGrad, meanMagnGrad;
   cv::meanStdDev(mat, meanMagnGrad, stdMagnGrad);
   double stdDev = stdMagnGrad[0] / sqrt(mat.rows*mat.cols);
   return stdDevFactor * stdDev + meanMagnGrad[0];
}


//2
cv::Mat matrixMagnitude(const cv::Mat &matX, const cv::Mat &matY)
{
   cv::Mat mags(matX.rows,matX.cols,CV_64F);
   for (int y = 0; y < matX.rows; ++y)
      {
         const double *Xr = matX.ptr<double>(y), *Yr = matY.ptr<double>(y);
         double *Mr = mags.ptr<double>(y);
         for (int x = 0; x < matX.cols; ++x)
            {
               double gX = Xr[x], gY = Yr[x];
               double magnitude = sqrt((gX * gX) + (gY * gY));
               Mr[x] = magnitude;
            }
      }
   return mags;
}

//1
cv::Mat computeMatXGradient(const cv::Mat &mat)
{
   cv::Mat out(mat.rows,mat.cols,CV_64F);

   for (int y = 0; y < mat.rows; ++y)
      {
         const uchar *Mr = mat.ptr<uchar>(y);
         double *Or = out.ptr<double>(y);

         Or[0] = Mr[1] - Mr[0];
         for (int x = 1; x < mat.cols - 1; ++x)
            {
               Or[x] = (Mr[x+1] - Mr[x-1])/2.0;
            }
         Or[mat.cols-1] = Mr[mat.cols-1] - Mr[mat.cols-2];
      }

   return out;
}


void findcenclass::findcentre(char id,cv::Rect roi)
{
   if (roi.x)
      {
         //TODO not necessary as there is below [blur factor times width ]
         //GaussianBlur( *matcen, *matcen, cv::Size( 0, 0 ), (0.005*roi.width));

         cv::Mat gradientX = computeMatXGradient(*matcen);
         cv::Mat gradientY = computeMatXGradient((*matcen).t()).t(); //TODO these two with laplacian
         cv::Mat mags = matrixMagnitude(gradientX, gradientY); // TODO with builtin function

         double kGradientThreshold = 50.0; // TODO Global
         //double gradientThresh = kGradientThreshold; //TODO for speedup
         //double gradientThresh = 0;
         double gradientThresh = computeDynamicThreshold(mags, kGradientThreshold);//compute the threshold

         for (int y = 0; y < (*matcen).rows; ++y)//normalize
            {
               double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
               const double *Mr = mags.ptr<double>(y);
               for (int x = 0; x < (*matcen).cols; ++x)
                  {
                     double gX = Xr[x], gY = Yr[x];
                     double magnitude = Mr[x];
                     if (magnitude > gradientThresh)
                        {
                           Xr[x] = gX/magnitude;
                           Yr[x] = gY/magnitude;
                        }
                     else
                        {
                           Xr[x] = 0.0;
                           Yr[x] = 0.0;
                        }
                  }
            }

         //-- Create a blurred and inverted image for weighting
         cv::Mat weight;
         int kWeightBlurSize = 5;
         GaussianBlur( *matcen , weight, cv::Size( kWeightBlurSize, kWeightBlurSize ), 0, 0 );
         for (int y = 0; y < weight.rows; ++y)
            {
               unsigned char *row = weight.ptr<unsigned char>(y);
               for (int x = 0; x < weight.cols; ++x)
                  {
                     row[x] = (255 - row[x]);
                  }
            }

         //Running the algotithm
         cv::Mat outSum = cv::Mat::zeros((*matcen).rows,(*matcen).cols,CV_64F);

         // for each possible center
         for (int y = 0; y < weight.rows; ++y)
            {
               const unsigned char *Wr = weight.ptr<unsigned char>(y);
               const double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
               for (int x = 0; x < weight.cols; ++x)
                  {
                     double gX = Xr[x], gY = Yr[x];
                     if (gX == 0.0 && gY == 0.0)
                        {
                           continue;
                        }
                     testPossibleCentersFormula(x, y, Wr[x], gX, gY, outSum);
                  }
            }

         // scale all the values down, basically averaging them
         double numGradients = (weight.rows*weight.cols);
         cv::Mat out;
         outSum.convertTo(out, CV_32F,1.0/numGradients);
         cv::imshow("deb", out );

         //-- Find the maximum point
         cv::Point maxP;
         double maxVal;
         cv::minMaxLoc(out, NULL,&maxVal,NULL,&maxP);



         leftpupil=maxP;
      }
}




//-----------------------------------------------------------------------------
void findcenclass::drawfound(cv::Mat *m)
{
   if(!m) m=matcen;

   circle(*m, leftpupil, 3, 1234);


}
