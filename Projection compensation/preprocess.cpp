#include "rectangle.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>



int input_image(IplImage * & gray_image,IplImage * & color_image,CvCapture * cam_capture)  //input the images and made it into 1 channel pictures
{
	IplImage * frame = cvQueryFrame( cam_capture );
	color_image = cvCloneImage(frame);
	if( !frame )return -1;
	gray_image = cvCreateImage(cvGetSize(frame),frame->depth,1); 
	cvCvtColor(frame, gray_image, CV_BGR2GRAY);  //gray_image saved the 1 channel picture from the camera
	return 0;
}


int pre_process(IplImage * & gray_image,IplImage * & color_image,IplImage * & adjusted_image)
{
	CvSize pro_resolution;
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
	find_rectangle(color_image);   //searched the edges and the vertex of the screen and return the vertex
	adjusted_image = cvCreateImage(pro_resolution,gray_image->depth,gray_image->nChannels);
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
	cvGetPerspectiveTransform( vertex, dst, warp_mat );   
	cvWarpPerspective( gray_image, adjusted_image, warp_mat );  
	cvSaveImage("camera_captured_screen.bmp",adjusted_image);  //adjusted_image will be used as the camera-captured image
	//cvResetImageROI(gray_image);
	return 0; 
}

int pre_iteration(IplImage * & gray_image,IplImage * & color_image,IplImage * & adjusted_image)
{
	CvSize pro_resolution;
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
	find_rectangle(color_image);   //searched the edges and the vertex of the screen and return the vertex
	adjusted_image = cvCreateImage(pro_resolution,gray_image->depth,gray_image->nChannels);
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
	cvGetPerspectiveTransform( vertex, dst, warp_mat );   
	cvWarpPerspective( gray_image, adjusted_image, warp_mat );  
	cvSaveImage("camera_captured_screen.bmp",adjusted_image);  //adjusted_image will be used as the camera-captured image
	//cvResetImageROI(gray_image);
	return 0; 
}
