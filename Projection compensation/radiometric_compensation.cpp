#include "cv.h"
#include "highgui.h"
#include "rectangle.h"
#include <stdio.h>
#include "preprocess.h"
#include "color_radiometric_compensation.h"
using namespace std;


int calibration()
{
	CvSize stand_size;
	CvScalar set_color,each_pixel;
	stand_size.width = 608;
	stand_size.height = 480;
	IplImage * cali_image = cvCreateImage(stand_size,8,1);
	CvCapture * capture = cvCreateCameraCapture(-1);  //get the graph captured by camera
	IplImage * gray_image, * adjusted_image;
	for(int i = 0;i < 256;i ++)
	{
		set_color.val[0] = i;  //set the lightness of each pixel
		cvSet(cali_image,set_color);  //set the calibration image
		cvSaveImage("Calibration.bmp",cali_image);
		//input_image(gray_image,capture);
	//	pre_process(gray_image,adjusted_image);  //We will focus on the adjusted image then
		for(int x = 0; x < 608;x ++)
		{
			for (int y = 0; y < 480;y ++)
			{
				each_pixel = cvGet2D(adjusted_image,x,y);  //get teh lightness of each pixel 
				value_table[x][y][i][0] = each_pixel.val[0];  //and save it to the value_table
			}
		}
	}  //after which we will have this value_table
	return 0;
}

int get_closest(CvScalar & pixel_value,int x, int y)
{
	int closest_value = value_table[x][y][0][0],flag = 0;
	int ab_distance = abs(closest_value - pixel_value.val[0]);  //find closest value
	for (int i = 1 ; i < 256; i ++)  //if we use binary search, we can save more time!!!!!!!!!
	{
		if(abs(value_table[x][y][i][0]-pixel_value.val[0]) < ab_distance){
			closest_value = value_table[x][y][i][0];
			flag = i;  //flag saved the sequence number of the closest value
			ab_distance = abs(closest_value - pixel_value.val[0]);
		}
	}
	return flag;
}

int compensation(IplImage * & basic_image)
{
	IplImage * compen_image = cvCloneImage(basic_image);
	CvScalar pixel_value;
	for (int x = 0;x < 608;x ++)
	{
		for (int y = 0; y < 480; y ++)
		{
			pixel_value = cvGet2D(basic_image,y,x);  //get the lightness of the given pixel
			pixel_value.val[0] = get_closest(pixel_value,x,y);
			cvSet2D(compen_image,y,x,pixel_value);
		}
	}
	cvSaveImage("compensated_image.bmp",compen_image);  //save the compensated image
	return 0;
}
