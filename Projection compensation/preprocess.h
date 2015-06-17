#ifndef _PREPROCESS_H_
#define _PREPROCESS_H_
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

extern int input_image(IplImage * & gray_image,IplImage * & color_image,CvCapture * cam_capture);  //input the images and made it into 1 channel pictures

extern int pre_process(IplImage * & gray_image,IplImage * & color_image,IplImage * & adjusted_image);

extern int pre_iteration(IplImage * & gray_image,IplImage * & color_image,IplImage * & adjusted_image);

#endif