#ifndef RAY_H
#define RAY_H
#include "vec3.h"
struct ray{
    // origin and direction of ray
    vec3 o,d;
    ray(vec3 o_,vec3 d_) : o(o_) , d(d_){}
};
#endif