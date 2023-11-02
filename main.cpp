#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
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
struct ray{
    // origin and direction of ray
    vec3 o,d;
    ray(vec3 o_,vec3 d_) : o(o_) , d(d_){}
};
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
vec3 Cen(50,-20,-860);
 sphere spheres[] = {//Scene: radius, position, emission, color, material 
   sphere(1e5, vec3( 1e5+1,40.8,81.6), vec3(),vec3(0,.75,.0),DIFF),//Left 
   sphere(1e5, vec3(-1e5+99,40.8,81.6),vec3(),vec3(.75,0,0),DIFF),//Rght 
   sphere(1e5, vec3(50,40.8, 1e5),     vec3(),vec3(.75,.75,.75),DIFF),//Back 
   sphere(1e5, vec3(50,40.8,-1e5+170), vec3(),vec3(),           DIFF),//Frnt 
   sphere(1e5, vec3(50, 1e5, 81.6),    vec3(),vec3(.75,.75,.75),DIFF),//Botm 
   sphere(1e5, vec3(50,-1e5+81.6,81.6),vec3(),vec3(.75,.75,.75),DIFF),//Top 
   sphere(16.5,vec3(27,16.5,47),       vec3(),vec3(1,1,1)*.999, SPEC),//Mirr 
    sphere(16.5,vec3(73,43,78),       vec3(),vec3(1,1,1)*.999, REFR),//Glas 
   sphere(600, vec3(50,681.6-.27,81.6),vec3(12,12,12),  vec3(), DIFF) //Lite 
 }; 

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
int main(){
    int w =1024;
    int h =768;
    int samps = 30;
    //numbers chosen have an artistic base to make scene look pretty
    ray cam(vec3(50,52,295.6),normalize(vec3(0,-0.042612,-1)));
    double a_r = double(w)/double(h);
    double vfov = 0.502643;
    double fovscale = 2*tan(0.5*vfov);
    // gettting the camera vectors
    vec3 cx = vec3(a_r,0,0)*fovscale;
    vec3 cy = normalize(cross(cx,cam.d))*fovscale;
    vec3 r;
    std::vector<vec3> c(w * h, vec3(0.0, 0.0, 0.0));
// dynamic scheduling 
#pragma omp parallel for schedule(dynamic, 1) private(r)
    // loop over rows and columns
    int is = 1.0/samps;
    for(int y=0;y<h;y++){
        fprintf(stderr, "\rendering (%d spp) %5.2f%%", samps * 4, 100.0*y / (h - 1));
        for(unsigned short x = 0;x < w;x++){
            // looping over the samples
            for(int sy=0,i=(h-y-1)*w+x;sy<2;sy++){
               for(int sx=0;sx<2;sx++,r=vec3(0.0,0.0,0.0)){
                for(int s = 0;s<samps;s++){
                    double r1 = 2*random();
                    double r2 = 2*random();
                    // doing a tent filter
                    double dx = r1<1?sqrt(r1) - 1:1-sqrt(2-r1);
                    double dy = r2<1?sqrt(r2) - 1:1-sqrt(2-r2);
                    vec3 d = cx*(((((sx +.5 + dx)/2) + x)/w) - 0.5)
                            +cy*(((((sy +.5 + dy)/2) + y)/h) - 0.5)
                            +cam.d;
                    r= r+ radiance(ray(cam.o + d*140,normalize(d)),0)*(is);
                }
                c[i]=c[i] + vec3(clamp(r.x),clamp(r.y),clamp(r.z))*0.25;
            } 
            }
        }
    }
    FILE *f = fopen("image.ppm","w");
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i = 0; i < w*h; i++){
		fprintf(f, "%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
    }
}
