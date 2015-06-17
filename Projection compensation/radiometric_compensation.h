#ifndef _RADIOMETRIC_COMPENSATION_H_
#define _RADIOMETRIC_COMPENSATION_H_
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

extern int compensation(IplImage * & basic_image);

extern short value_table[608][480][32][3];  //construct a look-up table function

#endif