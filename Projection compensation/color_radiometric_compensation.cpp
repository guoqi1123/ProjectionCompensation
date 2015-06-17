#include "cv.h"
#include "highgui.h"
#include "rectangle.h"
#include <stdio.h>
#include "preprocess.h"
#include "color_preprocess.h"
#include "iostream"
#include "conio.h"
using namespace std;

short value_table [608][480][32][3];  //construct a look-up table function

int cam_judge(IplImage * & color_image,IplImage * & adjusted_image,CvCapture * cam_capture)
{
	
	CvScalar mean;
	double num_mean = 0,num_pre_mean = 0,range = 200;
	while(1)
	{
		for(int i = 0; i < 3; i ++)   //read 10 pictures and get the average
		{
		color_input_image(color_image,cam_capture);
		color_pre_calibration(color_image,adjusted_image);
		mean = cvAvg(adjusted_image);
		num_mean += (mean.val[0])*(mean.val[0])+(mean.val[1])*(mean.val[1])+(mean.val[2])*(mean.val[2]);
		}
		num_mean /= 10;
		if (abs(num_mean - num_pre_mean) > range && num_pre_mean != 0)break;
		else num_pre_mean = num_mean;
		cvReleaseImage( & color_image);
		cvReleaseImage( & adjusted_image);
	}
	return 0;
}

int color_calibration(CvCapture * & cam_capture)
{
	CvSize stand_size;
	CvScalar set_color,each_pixel;
	stand_size.width = 608;
	stand_size.height = 480;
	IplImage * cali_image = cvCreateImage(stand_size,8,3);
	CvCapture * capture = cvCreateCameraCapture(-1);  //get the graph captured by camera
	IplImage * color_image, * adjusted_image;
	color_input_image(color_image,cam_capture);
	color_pre_process(color_image,adjusted_image);
	set_color.val[0] = 0;  //set the lightness of each pixel
	set_color.val[1] = 0;
	set_color.val[2] = 0;
	cvSet(cali_image,set_color);  //set the calibration image
	cvSaveImage("Calibration.bmp",cali_image);
	getch();
	for(int j = 0;j < 3; j ++)
	{
		for(int i = 0;i < 32;i ++)
		{
		color_input_image(color_image,cam_capture);
		color_pre_calibration(color_image,adjusted_image);
		set_color.val[0] = (j == 0) * i * 8;  //set the lightness of each pixel
		set_color.val[1] = (j == 1) * i * 8;
		set_color.val[2] = (j == 2) * i * 8;
		cvSet(cali_image,set_color);  //set the calibration image
		cvSaveImage("Calibration.bmp",cali_image);
//		cam_judge(color_image,adjusted_image,cam_capture);
//		getch();
		for(int x = 0; x < 608;x ++)
		{
			for (int y = 0; y < 480;y ++)
			{
				each_pixel = cvGet2D(adjusted_image,y,x);  //get teh lightness of each pixel 
				value_table[x][y][i][0] = each_pixel.val[0];  //and save it to the value_table
				value_table[x][y][i][1] = each_pixel.val[1];  //and save it to the value_table
				value_table[x][y][i][2] = each_pixel.val[2];  //and save it to the value_table
				
			}
		}
		cout<<i<<endl;
		cvReleaseImage( & color_image);
		cvReleaseImage( & adjusted_image);
		}  //after which we will have this value_table
	}
	return 0;
}

CvScalar color_get_closest(CvScalar & pixel_value,int x, int y)
{
	int closest_value[3],flag[3] = {0};
	CvScalar pro_pixel;
	closest_value[0] = value_table[x][y][0][0];
	closest_value[1] = value_table[x][y][0][1];
	closest_value[2] = value_table[x][y][0][2];
	int ab_distance[3];
	ab_distance[0] = abs(closest_value[0] - pixel_value.val[0] - 10); 
	ab_distance[1] = abs(closest_value[1] - pixel_value.val[1] - 10);
	ab_distance[2] = abs(closest_value[2] - pixel_value.val[2] - 10);  //find closest value
	for (int i = 1 ; i < 32; i ++)  //if we use binary search, we can save more time!!!!!!!!!
	{
		if(abs(value_table[x][y][i][0]-pixel_value.val[0] -10) < ab_distance[0]){
			closest_value[0] = value_table[x][y][i][0];
			flag[0] = i;  //flag saved the sequence number of the closest value
			ab_distance[0] = abs(closest_value[0] - pixel_value.val[0]);
		}
		if(abs(value_table[x][y][i][1]-pixel_value.val[0] -10) < ab_distance[1]){
			closest_value[1] = value_table[x][y][i][1];
			flag[1] = i;  //flag saved the sequence number of the closest value
			ab_distance[1] = abs(closest_value[1] - pixel_value.val[0]);
		}
		if(abs(value_table[x][y][i][2]-pixel_value.val[0] -10) < ab_distance[2]){
			closest_value[2] = value_table[x][y][i][2];
			flag[2] = i;  //flag saved the sequence number of the closest value
			ab_distance[2] = abs(closest_value[2] - pixel_value.val[0]);
		}
	}
	pro_pixel.val[0] = flag[0];
	pro_pixel.val[1] = flag[1];
	pro_pixel.val[2] = flag[2];
	return pro_pixel;
}

int color_compensation(IplImage * & basic_image)
{
	IplImage * compen_image = cvCloneImage(basic_image);
	CvScalar pixel_value;
	CvCapture * cam_capture = cvCreateCameraCapture(-1);
	color_calibration(cam_capture);
	while(1)
	{
		basic_image = cvLoadImage("basic_image.bmp");
	for (int x = 0;x < 608;x ++)
	{
		for (int y = 0; y < 480; y ++)
		{
			pixel_value = cvGet2D(basic_image,y,x);  //get the lightness of the given pixel
			pixel_value = color_get_closest(pixel_value,x,y);
			cvSet2D(compen_image,y,x,pixel_value);
		}
	}
	cvSaveImage("compensated_image.bmp",compen_image);  //save the compensated image
	getch();
	}
	return 0;
}
