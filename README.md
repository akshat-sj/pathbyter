# pathbyter

A self-contained pathtracer written in c++ for physically based rendering using Monte Carlo methods for realistic light simulation and image rendering.

## Getting Started
### Setting up the quality
The samples per pixel of the image will be directly proportional to the quality of the image as well as the time taken for it to render. You can customize the width and height of our viewport as well.
We can change all this in the main function of main.cpp
```c++
    int w =1024;
    int h =768;
    int samps = 400; //1600 spp (can modify)
```
### Changing the scene
You can edit the scene as well, the default scene is that of a cornell box. The scene is located in the sphere.h header file
```c++
    //object(radius,position,emission,color,material type)
    sphere(16.5,vec3(27,16.5,47),vec3(0,0,0),vec3(0.99,0.99,0.99), SPEC),
```

### Executing program
Compile the program using the makefile
```console
make
```
If the compilation is successful, execute the main program
```console
main
```
## Renders made using pathbyter(rendered at 2400 spp)

### Scene with spheres of various types
<img width="512" alt="man" src="https://github.com/akshat-sj/pathbyter/blob/main/assets/balls.png?raw=true">

### Cornell Box 

<img width="512" alt="gun" src="https://github.com/akshat-sj/pathbyter/blob/main/assets/cornellbox.png?raw=true">


## Built With

* [C++](https://isocpp.org/) - the programming language used


## Acknowledgments
* [Physically Based Rendering](https://pbrt.org/)
* [Scratchapixel](https://www.scratchapixel.com/)
* [Ohio State Notes](https://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/reflection_refraction.pdf)
* [Xorshift PRNG](https://en.wikipedia.org/wiki/Xorshift)
* [smallpt slides by Dr. David Cline](https://drive.google.com/file/d/0B8g97JkuSSBwUENiWTJXeGtTOHFmSm51UC01YWtCZw/view?resourcekey=0-BSMES1GmPEtIIcv6EgBTjw)

