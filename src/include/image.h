#ifndef IMAGE_H
#define IMAGE_H
#include "vec3.h"
#include "helper.h"
#include <fstream>
#include <iostream>
inline void writeppm(const char* filename, const vec3* pixels,int width,int height){
    FILE *f = fopen(filename,"w");
    fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
    for (int i = 0; i < width*height; i++){
		fprintf(f, "%d %d %d ", toInt(pixels[i].x), toInt(pixels[i].y), toInt(pixels[i].z));
    }
}
#endif