#include "cv.h"
#include "highgui.h"
#include "rectangle.h"
#include "preprocess.h"
#include <stdio.h>

int add_weighted_matrix(IplImage * & a_image,IplImage * & b_image,IplImage * & c_image,IplImage * & d_image, float alpha)  //c + alpha * (b - a) = d
{
	CvScalar temp_a,temp_b,temp_c,temp_d;
	int temp_value;
	for (int x = 0; x < 608; x ++)
	{
		for (int y = 0; y < 480; y ++)
		{
			temp_a = cvGet2D(a_image,y,x);
			temp_b = cvGet2D(b_image,y,x);  //the first variation is y!!!!!!
			temp_c = cvGet2D(c_image,y,x);
			temp_value = temp_c.val[0] + alpha * (temp_b.val[0] - temp_a.val[0]);
			if(temp_value < 0)temp_value = 0;
			if(temp_value >= 256)temp_value = 255;
			temp_d.val[0] = temp_value;
			cvSet2D(d_image,y,x,temp_d);
		}
	}
	return 0;
}

int alpha_iteration(IplImage * & pro_image,IplImage * & adjusted_image,double alpha)  //as the resolution of the projector is 608*480,
	//while the resolution of camera-captured graph is poorer than 608*480,we demand that the background to be continuous
{
	 
	IplImage * pro_image1 = cvCloneImage(pro_image);  //pro_image1 is used to save the iterated result of pro_image
	add_weighted_matrix(adjusted_image,pro_image,pro_image,pro_image1,alpha);  //equal to the beneath code
	//cvAddWeighted(pro_image,1,sub_image,alpha,0,pro_image1);  //we use 0.5 here as alpha
	pro_image = pro_image1;  //finish iteration
	return 0;
}

double get_average(IplImage * tar_image,CvPoint & lu_point, CvPoint & rd_point)
{
	int width = (rd_point.x) - (lu_point.x),height =(rd_point.y)  - (lu_point.y);
	int i = lu_point.x, j = lu_point.y, dest_i = rd_point.x, dest_j = rd_point.y;
	CvScalar temp;   //temp is used to save the infomation of each pixel
	double sum = 0;
	if(width < 0){
		width = -width;
		i = rd_point.x;
		dest_i = lu_point.x;
	}
	if(height < 0){
		height = -height;
		j = rd_point.y;
		dest_j = lu_point.y;
	}  //make the program robust
	int n = width * height;  //get the total number of pixels
	for(i; i < dest_i;i ++)
	{
		for (j;j < dest_j;j ++)
		{
			temp = cvGet2D(tar_image,i,j);
			sum = temp.val[0] + sum;
		}
	}
	return sum/n;   //return the average of a picture
}

double get_variation(IplImage * tar_image,CvPoint & lu_point,CvPoint & rd_point)
{
	double aver = get_average(tar_image,lu_point,rd_point);
	//calculate the variation of a picture
	int width = (rd_point.x) - (lu_point.x),height =(rd_point.y)  - (lu_point.y);
	int i = lu_point.x, j = lu_point.y, dest_i = rd_point.x, dest_j = rd_point.y;
	CvScalar temp;   //temp is used to save the infomation of each pixel
	double sum = 0;
	if(width < 0){
		width = -width;
		i = rd_point.x;
		dest_i = lu_point.x;
	}
	if(height < 0){
		height = -height;
		j = rd_point.y;
		dest_j = lu_point.y;
	}  //make the program robust
	int n = width * height;  //get the total number of pixels
	for(i; i < dest_i;i ++)
	{
		for (j;j < dest_j;j ++)
		{
			temp = cvGet2D(tar_image,i,j);
			sum = (temp.val[0]-aver) * (temp.val[0]-aver)+ sum;
		}
	}
	return sum/n;   //return the average of a picture
}

int Half_Iteration(IplImage * & basic_image)
{
	//cvNamedWindow("camera_gray",CV_WINDOW_AUTOSIZE);   //the graph captured by the camera with 1 channel
	CvCapture * cam_capture = cvCreateCameraCapture(-1);  //get the graph captured by camera
	IplImage * frame, * gray_image, * adjusted_image, * pro_image, * color_image; 
	input_image(gray_image,color_image,cam_capture);
	cvSaveImage("gray_image.bmp",gray_image);  //for debug
	pre_process(gray_image,color_image,adjusted_image);    //finish the pre_process£¬adjusted image is the cam_image

	pro_image = cvCreateImage(cvGetSize(basic_image),basic_image->depth,1);
	cvCvtColor(basic_image,pro_image,CV_BGR2GRAY);  //create the projected image for iteration, it must be 1 channel picture
	cvSaveImage("pro_image.bmp",pro_image);   //save a version before iteration begin


	// The ending judgment should be placed here!!!!!!!!!!
	while(1)
	{
		alpha_iteration(pro_image,adjusted_image,0.5);
		cvSaveImage("pro_image.bmp",pro_image);
		input_image(gray_image,color_image,cam_capture);
		pre_iteration(gray_image,color_image,adjusted_image);  //finish the pre_process£¬adjusted image is the cam_image

	}
	return 0;
}