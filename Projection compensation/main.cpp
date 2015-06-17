#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include "color_half_iteration.h"
#include "color_radiometric_compensation.h"
#include "half_iteration.h"
#include "radiometric_compensation.h"
int camera_test()
{
	cvNamedWindow("camera_test",CV_WINDOW_AUTOSIZE);
	CvCapture * camera = cvCreateCameraCapture(-1);
	IplImage * frame;
	while (1)
	{
		frame = cvQueryFrame(camera);
		cvShowImage("camera_test",frame);
		char c = cvWaitKey(10);    
		if(c == 27)break;
	}
	cvReleaseCapture( & camera);
	cvDestroyWindow("camera_test");
	return 0;
}
int main()
{
	camera_test();
	IplImage * basic_image = cvLoadImage("basic_image.bmp");
	color_Half_Iteration(basic_image);
	color_compensation(basic_image);
	return 0;
}