// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Tazrian Khan, 2008.
//
// First added:  2008-09-27
// Last changed: 2008-10-06
//------------------------------------------------------------------------------
#ifndef DEFS_H
#define DEFS_H
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <ctime>
#include <math.h>

//------------------------------------------------------------------------------
const float PIOVER180 = 0.017453292519943295769236907684886;
const float SRAY_EPS = 0.0;
const float NEAREST_VPOINT_DIST = 0.1;
//const float SRAY_INFINITY = 2000.0;
const float SRAY_INFINITY = 10000.0;
const float SRAY_NEG_INFINITY = -1000.0;
const int SRAY_MAX_LEVEL = 50;
const float ONE_OVER_RANDMAX = 1.0f / (RAND_MAX + 1.0f);

const int SRAY_SUPERSAMPLING_COEF = 4;
const float ONE_OVER_SRAY_SUPERSAMPLING_COEF = 1.0f / SRAY_SUPERSAMPLING_COEF;
const float SRAY_SQRT_SUPERSAMPLING_COEF = sqrt(1.0f * SRAY_SUPERSAMPLING_COEF);
const float ONE_OVER_SQRT_SRAY_SUPERSAMPLING_COEF = 1.0f / SRAY_SQRT_SUPERSAMPLING_COEF;
//------------------------------------------------------------------------------
enum eColorOffset { OFFSET_RED = 0, OFFSET_GREEN, OFFSET_BLUE, OFFSET_MAX };
enum eShapeType { ST_UNDEFINED = 0, ST_SPHERE = 1 };
//------------------------------------------------------------------------------
typedef unsigned char COLOR;

//#define RES_ROW 480
//#define RES_COL 640

//#define RES_ROW 800
//#define RES_COL 1280

#define RES_ROW 800
#define RES_COL 1280

//#define max(x,y) x > y ? x : y
//#define min(x,y) x < y ? x : y
//------------------------------------------------------------------------------
/* Expected that 'srand((unsigned)time(0))' is already called */
inline float pseudo(float l, float h)
{
   float r = h-l;
   return l + r * rand() * ONE_OVER_RANDMAX;
}
//------------------------------------------------------------------------------
inline float InvSqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int *) &x;
	i = 0x5F3759DF - (i >> 1);
	x = * (float *) &i;
	x = x * (1.5f - xhalf * x * x);
	return x; 
}
//------------------------------------------------------------------------------

inline void StrToVector3f(char str[], float lv3f[])
{
   sscanf(str, "(%f,%f,%f)", &lv3f[0], &lv3f[1], &lv3f[2]);
}
//------------------------------------------------------------------------------
#endif // DEFS_H
//------------------------------------------------------------------------------
