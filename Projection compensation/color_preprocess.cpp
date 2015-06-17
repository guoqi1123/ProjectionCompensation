#include "rectangle.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include "color_half_iteration.h"


int color_input_image(IplImage * & color_image,CvCapture * cam_capture)  //input the images and made it into 1 channel pictures
{
	IplImage * frame = cvQueryFrame(cam_capture);
	color_image = cvCloneImage(frame);
	if( !frame )return -1;
	return 0;
}


int color_pre_process(IplImage * & color_image,IplImage * & adjusted_image)
{
	CvSize pro_resolution;
	IplImage * image_for_rectangle = cvCloneImage(color_image);
	pro_resolution.width = 608;
	pro_resolution.height = 480;  //It is the resolution of the projector
	CvPoint2D32f dst[4];
	dst[0].x = 0;
	dst[0].y = 0;
	dst[1].x = 0;
	dst[1].y = 479;
	dst[2].x = 607;
	dst[2].y = 0;
	dst[3].x = 607;
	dst[3].y = 479;
	CvMat* warp_mat = cvCreateMat( 3, 3, CV_32FC1 );    //the transform matrix
	find_rectangle(image_for_rectangle);   //searched the edges and the vertex of the screen and return the vertex
	/*
	int width = vertex[1].x - vertex[0].x;
	int height = vertex[1].y - vertex[0].y;  
	CvSize pro_resolution;
	pro_resolution.width = 608;
	pro_resolution.height = 480;  //It is the resolution of the projector
	cvSetImageROI(gray_image,cvRect(vertex[0].x,vertex[0].y,width,height));  //change the ROI region to the projection screen
	adjusted_image = cvCreateImage(pro_resolution,gray_image->depth,gray_image->nChannels);
	cvResize(gray_image,adjusted_image,CV_INTER_LINEAR);  //resize the ROI region to fit the projection image
	*/
	adjusted_image = cvCreateImage(pro_resolution,color_image->depth,color_image->nChannels);
	cvGetPerspectiveTransform( vertex, dst, warp_mat );   
	cvWarpPerspective( color_image, adjusted_image, warp_mat );  
	cvSaveImage("camera_captured_screen.bmp",adjusted_image);  //adjusted_image will be used as the camera-captured image
	//cvResetImageROI(gray_image);
	return 0; 
}

int color_pre_calibration(IplImage * & color_image,IplImage * & adjusted_image)
{
	CvSize pro_resolution;
	IplImage * image_for_rectangle = cvCloneImage(color_image);
	pro_resolution.width = 608;
	pro_resolution.height = 480;  //It is the resolution of the projector
	CvPoint2D32f dst[4];
	dst[0].x = 0;
	dst[0].y = 0;
	dst[1].x = 0;
	dst[1].y = 479;
	dst[2].x = 607;
	dst[2].y = 0;
	dst[3].x = 607;
	dst[3].y = 479;
	CvMat* warp_mat = cvCreateMat( 3, 3, CV_32FC1 );    //the transform matrix
	adjusted_image = cvCreateImage(pro_resolution,color_image->depth,color_image->nChannels);
	cvGetPerspectiveTransform( vertex, dst, warp_mat );   
	cvWarpPerspective( color_image, adjusted_image, warp_mat );  
	cvSaveImage("camera_captured_screen.bmp",adjusted_image);  //adjusted_image will be used as the camera-captured image
	//cvResetImageROI(gray_image);
	return 0; 
}


