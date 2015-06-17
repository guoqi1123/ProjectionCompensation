#ifndef _COLOR_PREPROCESS_H_
#define _COLOR_PREPROCESS_H_
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

extern int color_input_image(IplImage * & color_image,CvCapture * cam_capture);  //input the images and made it into 1 channel pictures

extern int color_pre_process(IplImage * & color_image,IplImage * & adjusted_image);

extern int color_pre_calibration(IplImage * & color_image,IplImage * & adjusted_image);

#endif