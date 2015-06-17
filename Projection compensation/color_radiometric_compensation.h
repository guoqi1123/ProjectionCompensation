#ifndef _COLOR_RADIOMETRIC_COMPENSATION_H_
#define _COLOR_RADIOMETRIC_COMPENSATION_H_
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

extern int color_compensation(IplImage * & basic_image);
extern short value_table[608][480][32][3];

#endif