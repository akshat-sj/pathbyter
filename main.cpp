#include <stdlib.h>
#include <cstdint>
#include <iostream>
#include "include/vec3.h"
#include "include/ray.h"
#include "include/sphere.h"
#include "include/helper.h"
#include "include/image.h"
int main(){
    int w =1024;
    int h =768;
    int samps = 10; //1600 spp (can modify)
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
    double is = 1.0/samps;
    int progwidth =50;
    std::cout<<"Rendering Progress: [";
// dynamic scheduling 
#pragma omp parallel for schedule(dynamic, 1) private(r)
    // loop over rows and columns
    for(int y=0;y<h;y++){
#pragma omp critical
        {
        fprintf(stderr, "\r Rendering");
        int percent = static_cast<int>(100.0 * y / (h - 1));
        int progleng = static_cast<int>(static_cast<double>(percent) / 100 * progwidth);
        std::cout << "[";
        for (int i = 0; i < progleng; i++) {
            std::cout << "=";
        }
        std::cout << ">";
        for (int i = progleng + 1; i < progwidth; i++) {
            std::cout << " ";
        }
        std::cout << "] " << percent << "%";
        }
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
    writeppm("image.ppm", c.data(), w, h);
}
