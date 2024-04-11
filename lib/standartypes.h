/*=============================================================================
 * Copyright (c) 2018 rafaelkrause
 *
 * @Script: nctypedef.h
 * @Author: rafaelkrause
 * @Email: rafaelkrause@gmail.com
 * @Create At: 2018-06-11 16:25:00
 * @Last Modified By: rafaelkrause
 * @Last Modified At: 2018-06-01 17:08:32
 * @Description: Declare All Base Types Definitions (bools,int etc..)
 *============================================================================*/
#ifndef _NC_STANDAR_TYPES_DEF_
#define _NC_STANDAR_TYPES_DEF_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
	#include <cfloat>
#else
	#include "float.h"
#endif 

#include <stdbool.h>
#include <math.h>


 // Bool type
 //--------------------------------------------
typedef unsigned char       bool_t;

//#define false	0
//#define true	1

//Bytes Type
//----------------------------------------------
typedef unsigned char       byte_t;
typedef unsigned short      word_t;
typedef unsigned int        dword_t;

// Int types
//---------------------------------------------
typedef unsigned short      uint_t;
typedef short               int_t;
typedef unsigned int        udint_t;
typedef int                 dint_t;
typedef unsigned long       ulint_t;
typedef long                lint_t;
#define IEC_INT_MAX         32767
#define IEC_INT_MIN         -32768
#define IEC_DINT_MAX        2147483647
#define IEC_DINT_MIN        -2147483648
#define IEC_LINT_MAX        9223372036854775807
#define IEC_LINT_MIN        -9223372036854775808

// Float types
//---------------------------------------------
typedef float               real_t;
typedef double              lreal_t;
#define IEC_FLOAT_MAX           FLT_MAX
#define IEC_FLOAT_MIN           FLT_MIN
#define IEC_FLOAT_EPSLON        1E-5
#define IEC_DOUBLE_MAX          1E+37
#define IEC_DOUBLE_MIN          1E-37
#define IEC_DOUBLE_EPSLON       1E-5

// String type
//---------------------------------------------
#define     STR_LEN         255
typedef struct _string { char c[STR_LEN]; } string_t;

// TRIG
//--------------------------------------------
#define IEC_PI			3.14159265358979323846
#define IEC_HALF_PI		1.57079632679489661923
#define IEC_QUARTER_PI	0.78539816339744830962
#define IEC_2_PI		6.28318530717958647692

// Vector types
//---------------------------------------------

// Vector 2d Float (Usually For x y Operation)
typedef struct _Vec2f {
	real_t	x;
	real_t	y;
}Vec2f_t;

// Vector 3d Float (Usually For x y z Operation)
typedef struct _Vec3f {
	real_t	x;
	real_t	y;
	real_t	z;
}Vec3f_t;

// Vector 4d Float (Usually For Colors Operation)
typedef struct _Vec4f {
	real_t	x;
	real_t	y;
	real_t	z;
	real_t	w;
}Vec4f_t;

// Vector 2d Double (Usually For x y Operation)
typedef struct _Vec2d {
	lreal_t	x;
	lreal_t	y;
}Vec2d_t;

// Vector 3d Double (Usually For x y z Operation)
typedef struct _Vec3d {
	lreal_t	x;
	lreal_t	y;
	lreal_t	z;
}Vec3d_t;

// Vector 4d Double (Usually For Colors Operation)
typedef struct _Vec4d {
	lreal_t	x;
	lreal_t	y;
	lreal_t	z;
	lreal_t	w;
}Vec4d_t;

//Vector Calculations
// dot product (3D) which allows vector operations in arguments
#define Vec3f_dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define Vec3f_norm(v)     sqrt(Vec3f_dot(v,v))     // norm = length of  vector



// Bit Operations
//-----------------------------------------------
#define iec_max(a,b) (((a) > (b)) ? (a) : (b))
#define iec_min(a,b) (((a) < (b)) ? (a) : (b))

//#define max(a,b) \
//   ({ __typeof__ (a) _a = (a); \
//       __typeof__ (b) _b = (b); \
//     _a > _b ? _a : _b; })
//
//#define min(a,b) \
//   ({ __typeof__ (a) _a = (a); \
//       __typeof__ (b) _b = (b); \
//     _a < _b ? _a : _b; })



//#define isequal_position_vector(a,b) !(memcmp(a, b, sizeof(float)*N_AXIS))

// Bit field and masking macros
#define bitmask(n) (1 << n)
#define bitSet(data,bitmask) ((data) |= (bitmask))
#define bitReset(data,bitmask) ((data) &= ~(bitmask))
#define bit_isTrue(data,bitmask) ((data & bitmask) != 0)
#define bit_isFalse(data,bitmask) ((data & bitmask) == 0)
#define bitisTRUE(data,n) (data & (1<<n))
#define set_bit(data,n,value) if(value){data |= 1 << n;}else{data &= ~(1 << n);}

#define IEC_SQ( x ) (x*x) 

static inline real_t IEC_fx(double A, double B, double C, double x) 
{
	return -(A * x + C) / B;
}

static inline real_t IEC_fy(double A, double B, double C, double y) 
{
	return -(B * y + C) / A;
}

static float round_2d(float var)
{
	// 37.66666 * 100 =3766.66 
	// 3766.66 + .5 =3767.16    for rounding off value 
	// then type cast to int so value is 3767 
	// then divided by 100 so the value converted into 37.67 
	double value = (int)(var * 100 + .5);
	return (float)value / 100;
}

static double lreal_trunc(double d)
{
	return (d > 0) ? floor(d) : ceil(d);
}

static bool_t nearlyEqual(float a, float b, float epsilon) {

	//got from https://floating-point-gui.de/errors/comparison/

	real_t absA = fabs(a);
	real_t absB = fabs(b);
	real_t diff = fabs(a - b);

	if (a == b) { // shortcut, handles infinities
		return 1;
	}
	else if (a == 0 || b == 0 || (absA + absB < IEC_FLOAT_MIN)) {
		// a or b is zero or both are extremely close to it
		// relative error is less meaningful here
		return diff < (epsilon * IEC_FLOAT_MIN);
	}
	else { // use relative error
		return (fabs(diff) < epsilon);
	}
}

static bool_t IEC_IsEqual(real_t a, real_t b, real_t error) 
{
	real_t absA = fabs(a);
	real_t absB = fabs(b);
	real_t diff = fabs(a - b);

	if (a == b) 
		return 1;
	else 
		return (fabs(diff) < error);
}

#ifndef inline
#define  inline __inline
#endif // !inline

#ifdef __cplusplus
}
#endif

#endif