/* 在程序里找寻矩形 */ 

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_
#ifdef _CH_ 
#pragma package <opencv> 
#endif 
#ifndef _EiC 
#include "cv.h" 
#include "highgui.h"
#include <stdio.h> 
#include <math.h> 
#include <string.h>
#endif 
extern int thresh; 
extern IplImage* img;
extern IplImage* img0;
extern CvMemStorage* storage;
extern CvPoint pt[4]; 
extern const char* wndname; 
extern int g_slider_position;
#pragma once
//自己创建的全局变量，用于返回投影块的位置

extern CvPoint2D32f vertex[4];



// helper function: 
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2 
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 );
// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage 
CvSeq* findSquares4( IplImage* img, CvMemStorage* storage );
// the function draws all the squares in the image 
void drawSquares( IplImage* img, CvSeq* squares ); 


void on_trackbar( int a ); 
int find_rectangle(IplImage  * & g_image); 
#ifdef _EiC 
main(1,"squares.c"); 
#endif



#endif