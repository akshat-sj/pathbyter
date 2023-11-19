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
### Changing 
The samples per pixel of the image will be directly proportional to the quality of the image as well as the time taken for it to render. You can customize the width and height of our viewport as well.
We can change all this in the main function of main.cpp
```c++
    int w =1024;
    int h =768;
    int samps = 400; //1600 spp (can modify)
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
## Renders made at

### Cornell Box
<img width="236" alt="man" src="https://github.com/akshat-sj/razwire/blob/main/assets/man.png?raw=true">

### gun 

<img width="818" alt="gun" src="https://github.com/akshat-sj/razwire/blob/main/assets/gun.png?raw=true">


## Built With

* [C++](https://isocpp.org/) - the programming language used


## Acknowledgments


