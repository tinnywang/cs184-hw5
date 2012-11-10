/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "Raytrace.h"
#include "shaders.h"
#include "Transform.h"
#include <FreeImage.h>
#include "UCB/grader.h"

using namespace std ;

// Main variables in the program.
#define MAINPROGRAM
#include "variables.h"
#include "readfile.h" // prototypes for readfile.cpp  

void saveScreenshot(FIBITMAP* img, string fname) {
    std::cout << "Saving screenshot: " << fname << "\n";
    FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
}

void init() {
    attenuation[0] = 1;
    attenuation[1] = 0;
    attenuation[2] = 0;
    ambient[0] = 0.2;
    ambient[1] = 0.2;
    ambient[2] = 0.2;
    ambient[3] = 1;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: transforms scenefile \n";
        exit(-1);
    }

    FreeImage_Initialise();
    init();
    readfile(argv[1]) ;
    
    FIBITMAP* bitmap = FreeImage_Allocate(w, h, bpp);
    float fovx = fovy * static_cast<float>(w/h);

    Raytrace rt;
    rt.raytrace(eye, center, up, fovx, fovy, w, h, bitmap);

    saveScreenshot(bitmap, "temp.png");
    FreeImage_DeInitialise();
    return 0;
}
