#ifndef BASE_MATH_H
#define BASE_MATH_H

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Dependencies

#include "base_type.h"


////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Clamps, Mins, Maxes

#define Min(A,B)      (((A)<(B))?(A):(B))
#define Max(A,B)      (((A)>(B))?(A):(B))
#define ClampTop(A,X) Min(A,X)
#define ClampBot(X,B) Max(X,B)
#define Clamp(A,X,B)  (((X)<(A))?(A):((X)>(B))?(B):(X))

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Bit Twiddling

#define IsPow2OrZero(x) ((((x) - 1)&(x)) == 0)
#define IsPow2(x)       ((x)!=0 && IsPow2OrZero(x))

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Constants

#define PI 3.1415926535897f

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Units

#define KB(n)  (((Size)(n)) << 10)
#define MB(n)  (((Size)(n)) << 20)
#define GB(n)  (((Size)(n)) << 30)
#define TB(n)  (((Size)(n)) << 40)
#define Thousand(n)   ((n)*1000)
#define Million(n)    ((n)*1000000)
#define Billion(n)    ((n)*1000000000)


#endif //BASE_MATH_H