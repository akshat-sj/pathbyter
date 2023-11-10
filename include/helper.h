#ifndef HELPER_H
#define HELPER_H
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include <cstdint>
#include <random>
#define MAX_DEPTH 10
#define PI 3.1415926535897932384626433832795
// implemented an xorshift to generate random float numbers between 0 and 1 faster
static uint64_t state = std::random_device{}();
double random(){
    state ^= state>>12;
    state ^= state<<25;
    state ^= state>>27;
    uint64_t rndb = state*2685821657736338717ull;
    return static_cast<double>(rndb)/static_cast<double>(UINT64_MAX);
}


inline double clamp(double x){
    return x<0?0:x>1?1:x;
}
// gamma correction
inline int toInt(double x){
    return int(pow(clamp(x),1/2.2)*255.0 + 0.5);
}
inline bool intersect(const ray &r,double &t,int &id){
    double n = sizeof(spheres)/sizeof(sphere);
    double inf=t=std::numeric_limits<double>::infinity();
    double eps= 1e-4;
    double d;
    for(int i = int(n);i--;){
        if((d=spheres[i].intersect(r))>eps && (d<t)){
            id = i;
            t = d;
        }
    }
    return t<inf;
}
//grayscale encoding 
double luma(const vec3& color){
    return dot(color,vec3(0.2126f,0.7152f,0.0722f));
}
vec3 radiance(const ray& r,int depth){
    if(depth>MAX_DEPTH){
        return vec3(0.0,0.0,0.0);
    }
    int id = 0;
    double t;
    if(!intersect(r,t,id)){
        return vec3(0.0,0.0,0.0);
    }
    const sphere &obj= spheres[id];
    vec3 x = r.o + r.d*t;
    vec3 n = obj.normal(x);
    vec3 n1 = dot(n,r.d)<0?n:n*-1.0;
    vec3 alb = obj.c;
    double russianroulette = luma(alb);
    if(++depth>5){
        if(random()<russianroulette){
            alb/=russianroulette;
        }else{
            return obj.e;
        }
    }
    if(obj.refl==SPEC){
        //reflect ray about normal
        return obj.e + alb*radiance(ray(x,r.d-n*2*dot(n,r.d)),depth);
    }else if(obj.refl==DIFF){
        double r = random();
        //getting the azimuthal angle
        double phi = 2*PI*random();
        // use an inverse pdf to get sintheta and costheta
        double sintheta = sqrt(r);
        double costheta = sqrt(1-r);
        // making an vector co-ordinate system
        vec3 w =n1;
        vec3 u = normalize(cross(fabs(w.x) > .1 ? vec3(0, 1, 0) : vec3(1, 0, 0), w));
        vec3 v = cross(w,u);
        //changing co-ordinate systems from sphereical to our vector co-ordinate system
        vec3 d = normalize(u*cos(phi)*sintheta + v*sin(phi)*sintheta + w*costheta);
        return obj.e + alb*radiance(ray(x,d),depth);
    }
    ray reflray(x,r.d-n*2*dot(n,r.d));
    bool into = (dot(n,n1) >0); // this checks whether light ray is inside sphere or not
    double na = 1.0;
    double nt = 1.5;
    double nnt = into?na/nt:nt/na;
    double costheta = dot(r.d,n1); // angle of reflectance
    // now we calculate angle of incidence by squaring snell's law and moving terms around
    double costheta1sqr = 1 - nnt * nnt*(1 - costheta * costheta);
    if(costheta1sqr<0){
        return obj.e + alb*radiance(reflray,depth);
    }
    // just getting direction of redlected ray
    vec3 tdir = normalize(r.d*nnt - n * ((into ? 1 : -1)*(costheta*costheta + sqrt(costheta1sqr))));
    //schlick's approximation 
    //Rθ = R0 + ( 1 - cos θ )5 ( 1 - R0 )
    double a = nt - na;
    double b = na+ nt;
    double R0 = (a*a/(b*b));
    double costheta1 = dot(tdir,n);
    double c = 1 - (into?-costheta:costheta1);
    double Re = R0 + (1-R0)*c*c*c*c*c;
    // how much of the incident light is refracted
    double Tr = 1 - Re;
    double P = 0.25 + 0.5*Re;
    double RP = Re/P;
    double TP = Tr/(1-P);
    // if recursion to deep ignore it and just do russian roulette to choose
    // refraction or reflection else
    // we do both reflaction and refraction in parts
    if(depth<3){
    return obj.e + alb*(radiance(reflray, depth)*Re + radiance(ray(x, tdir), depth)*Tr); 
    }else{
        if(random()<P){
            return obj.e + alb * radiance(reflray, depth)*RP;
        }else{
           return obj.e + alb * radiance(ray(x, tdir), depth)*TP; 
        }
    }

}
#endif