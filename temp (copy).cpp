
float kPostProcessThreshold = 0.97;

cv::Point findEyeCenter(cv::Mat &face,cv::Rect eye)
{
   //scaledownregion to 80/some
   // draw eye region
   //-- Find the gradient
   //-- Normalize and threshold the gradient
      // compute all the magnitudes
      //compute the threshold


   //-- Create a blurred and inverted image for weighting

   //-- Run the algorithm!

   // for each possible center
   //std::cout<<"Eye Size:"<<outSum.cols<<" "<<outSum.rows<<std::endl;

   //-- Flood fill the edges
   if(kEnablePostProcess)
      {
         cv::Mat floodClone;
         //double floodThresh = computeDynamicThreshold(out, 1.5);
         double floodThresh = maxVal * kPostProcessThreshold;
         cv::threshold(out, floodClone, floodThresh, 0.0f, cv::THRESH_TOZERO);
         if(kPlotVectorField)
            {
               //plotVecField(gradientX, gradientY, floodClone);
               //imwrite("eyeFrame.png",eyeROIUnscaled);
            }
         cv::Mat mask = floodKillEdges(floodClone);
         //imshow(debugWindow + " Mask",mask);

         //imshow(debugWindow,out);
         // redo max
         cv::minMaxLoc(out, NULL,&maxVal,NULL,&maxP,mask);
      }
   return unscalePoint(maxP,eye);
}
