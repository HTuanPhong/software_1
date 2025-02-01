#ifndef BASE_NAME_H
#define BASE_NAME_H

////////////////////////////////
// NOTE(phong): Dependencies

#include "base_type.h"


////////////////////////////////
// NOTE(phong): Clamps, Mins, Maxes

#define Min(A,B)      (((A)<(B))?(A):(B))
#define Max(A,B)      (((A)>(B))?(A):(B))
#define ClampTop(A,X) Min(A,X)
#define ClampBot(X,B) Max(X,B)
#define Clamp(A,X,B)  (((X)<(A))?(A):((X)>(B))?(B):(X))

////////////////////////////////
// NOTE(phong): Bit Twiddling

#define IsPow2OrZero(x) ((((x) - 1)&(x)) == 0)
#define IsPow2(x)       ((x)!=0 && IsPow2OrZero(x))

////////////////////////////////
// NOTE(phong): Constants

static f32 pi32 = 3.1415926535897f;


#endif //BASE_NAME_H