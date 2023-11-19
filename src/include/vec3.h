#ifndef VEC3_H
#define VEC3_H
#include <math.h>
struct vec3
{
    double x,y,z;
    vec3():x(0.0),y(0.0),z(0.0){}
    vec3(double x,double y,double z):x(x),y(y),z(z){}
    vec3 operator-() const{
        return vec3(-x,-y,-z);
    }
    vec3& operator+=(const vec3& t){
        x+=t.x;
        y+=t.y;
        z+=t.z;
        return *this;
    }
    vec3& operator-=(const vec3& t){
        x-=t.x;
        y-=t.y;
        z-=t.z;
        return *this;
    }
    vec3& operator*=(const vec3& t){
        x*=t.x;
        y*=t.y;
        z*=t.z;
        return *this;
    }
    vec3& operator*=(double s){
        x*=s;
        y*=s;
        z*=s;
        return *this;
    }
    vec3& operator/=(const vec3& t){
        x/=t.x;
        y/=t.y;
        z/=t.z;
        return *this;
    }
    vec3& operator/=(double s){
        double is = 1.0/s;
        x*=is;
        y*=is;
        z*=is;
        return *this;
    }
    friend vec3 operator+(const vec3& a,const vec3& b){
        return vec3(a.x+b.x,a.y+b.y,a.z+b.z);
    }
    friend vec3 operator-(const vec3& a,const vec3& b){
        return vec3(a.x-b.x,a.y-b.y,a.z-b.z);
    }
    friend vec3 operator*(const vec3& a,const vec3& b){
        return vec3(a.x*b.x,a.y*b.y,a.z*b.z);
    }
    friend vec3 operator*(const vec3& a,double s){
        return vec3(a.x*s,a.y*s,a.z*s);
    }
    friend vec3 operator*(double s,const vec3& b){
        return b*s;
    }
    friend vec3 operator/(const vec3& a,const vec3& b){
        return vec3(a.x/b.x,a.y/b.y,a.z/b.z);
    }
    friend vec3 operator/(const vec3& a,double s){
        return a*(1.0/s);
    }
    friend vec3 operator/(double s,const vec3& a){
        return a*(1.0/s);
    }
    friend double dot(const vec3& a,const vec3& b){
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
    friend vec3 cross(const vec3& a, const vec3& b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
    }
    friend vec3 normalize(const vec3& v) {
        double length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        return v / length;
    }
};
#endif