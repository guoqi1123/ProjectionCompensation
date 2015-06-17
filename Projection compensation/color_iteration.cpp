#include "cv.h"
#include "highgui.h"
#include "rectangle.h"
#include "color_preprocess.h"
#include <stdio.h>

int color_adjust(IplImage * & src_image, IplImage * & dst_image)
{
	IplImage* imgChannel[4] = { 0, 0, 0, 0 };  
	dst_image= cvCreateImage( cvGetSize( src_image ), 8, 3 );  
	for(int i = 0; i < 4; i ++)
	{
		imgChannel[i] = cvCreateImage( cvGetSize( src_image ), 8, 1 );
	}
	cvSplit( src_image, imgChannel[0], imgChannel[1], imgChannel[2], NULL );  
	for(int i = 0; i < 4; i++ )  
	{    
		 cvEqualizeHist( imgChannel[i], imgChannel[i] );  
	}   
	cvMerge( imgChannel[0], imgChannel[1], imgChannel[2], NULL, dst_image );  
	return 0;
}

int color_add_weighted_matrix(IplImage * & a_image,IplImage * & b_image,IplImage * & c_image,IplImage * & d_image, float alpha)  //c + alpha * (b - a) = d
{
	CvScalar temp_a,temp_b,temp_c,temp_d;
	int temp_value0,temp_value1,temp_value2;
	for (int x = 0; x < 608; x ++)
	{
		for (int y = 0; y < 480; y ++)
		{
			temp_a = cvGet2D(a_image,y,x);
			temp_b = cvGet2D(b_image,y,x);
			temp_c = cvGet2D(c_image,y,x);
			temp_value0 = temp_c.val[0] + alpha * (temp_b.val[0] - temp_a.val[0]);
			temp_value1 = temp_c.val[1] + alpha * (temp_b.val[1] - temp_a.val[1]);
			temp_value2 = temp_c.val[2] + alpha * (temp_b.val[2] - temp_a.val[2]);
			if(temp_value0 < 0)temp_value0 = 0;
			if(temp_value0 >= 256)temp_value0 = 255;
			if(temp_value1 < 0)temp_value1 = 0;
			if(temp_value1 >= 256)temp_value1 = 255;
			if(temp_value1 < 0)temp_value2 = 0;
			if(temp_value1 >= 256)temp_value2 = 255;
			temp_d.val[0] = temp_value0;
			temp_d.val[1] = temp_value1;
			temp_d.val[2] = temp_value2;
			cvSet2D(d_image,y,x,temp_d);
		}
	}
	return 0;
}

int color_alpha_iteration(IplImage * & pro_image,IplImage * & adjusted_image,double alpha)  //as the resolution of the projector is 608*480,
	//while the resolution of camera-captured graph is poorer than 608*480,we demand that the background to be continuous
{

	IplImage * pro_image1 = cvCloneImage(pro_image);  //pro_image1 is used to save the iterated result of pro_image
	color_add_weighted_matrix(adjusted_image,pro_image,pro_image,pro_image1,alpha);  //equal to the beneath code
	//cvAddWeighted(pro_image,1,sub_image,alpha,0,pro_image1);  //we use 0.5 here as alpha
	pro_image = pro_image1;  //finish iteration
	return 0;
}

int color_Half_Iteration(IplImage * & basic_image)
{
	//cvNamedWindow("camera_gray",CV_WINDOW_AUTOSIZE);   //the graph captured by the camera with 1 channel
	
	CvCapture * cam_capture = cvCreateCameraCapture(-1);  //get the graph captured by camera
	IplImage  * adjusted_image, * pro_image, * color_image, * dst_image, * diag_image;

	color_input_image(color_image,cam_capture);
	cvSaveImage("color_image.bmp",color_image);  //for debug

	color_pre_process(color_image,adjusted_image);    //finish the pre_process£¬adjusted image is the cam_image

	color_adjust(basic_image,diag_image);
	cvSaveImage("diag_image.bmp",diag_image);
	pro_image = cvCloneImage(basic_image);
	cvSaveImage("pro_image.bmp",pro_image);   //save a version before iteration begin

	// The ending judgment should be placed here!!!!!!!!!!
	double min_value = 0,max_value = 0,range = 1;
	while(1)
	{
		color_alpha_iteration(pro_image,adjusted_image,0.5);
		color_adjust(pro_image,dst_image);
		cvSaveImage("pro_image.bmp",pro_image);
		cvSaveImage("dst_image.bmp",dst_image);
		color_input_image(color_image,cam_capture);
		color_pre_process(color_image,adjusted_image);  //finish the pre_process£¬adjusted image is the cam_image
	}
	return 0;
}