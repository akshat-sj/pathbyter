#ifndef SPHERE_H
#define SPHERE_H
#include "vec3.h"
#include "ray.h"
enum refl_t{DIFF,SPEC,REFR}; //types of material used 
struct sphere{
    double rad;
    vec3 p,e,c;
    refl_t refl;
    sphere(double rad_,vec3 p_,vec3 e_,vec3 c_, refl_t refl_):
        rad(rad_),p(p_),e(e_),c(c_),refl(refl_){}
    double intersect(const ray &r) const{
        double t;
        vec3 op = p - r.o;
        double b = dot(op,r.d);
        double det = b*b - dot(op,op) + rad*rad;
        double ans;
        if(det<0){
            return -1;
        }else{
            ans = sqrt(det);
        }
        // choose between entry and exit points
        double ir = (t = b -ans)>0?t:b+ans;
        return ir;

    }
    vec3 normal(const vec3& v) const{
        return ((v-p)/rad);
    }
};

inline sphere spheres[] = {
   sphere(1e5, vec3( 1e5+1,40.8,81.6), vec3(),vec3(0,.75,.0),DIFF),
   sphere(1e5, vec3(-1e5+99,40.8,81.6),vec3(),vec3(.75,0,0),DIFF),
   sphere(1e5, vec3(50,40.8, 1e5),     vec3(),vec3(.75,.75,.75),DIFF),
   sphere(1e5, vec3(50,40.8,-1e5+170), vec3(),vec3(),DIFF),
   sphere(1e5, vec3(50, 1e5, 81.6),    vec3(),vec3(.75,.75,.75),DIFF),
   sphere(1e5, vec3(50,-1e5+81.6,81.6),vec3(),vec3(.75,.75,.75),DIFF),
   sphere(16.5,vec3(27,16.5,47),       vec3(),vec3(1,1,1)*.999, SPEC),
    sphere(16.5,vec3(73,43,62),       vec3(),vec3(1,1,1)*.999, REFR), 
   sphere(600, vec3(50,681.6-.27,81.6),vec3(12,12,12),  vec3(), DIFF)
 }; 

#endif