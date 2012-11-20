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
#include "Transform.h"
#include <FreeImage.h>

using namespace std ;

// Main variables in the program.
#define MAINPROGRAM
#include "variables.h"
#include "readfile.h" // prototypes for readfile.cpp  
#include "Raytrace.h"

void saveScreenshot(FIBITMAP* img, string fname) {
    std::cout << "Saving screenshot: " << fname << "\n";
    FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
}

void init() {
    attenuation = glm::vec3(1, 0, 0);
    ambient = glm::vec4(0.2, 0.2, 0.2, 1);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: transforms scenefile \n";
        exit(-1);
    }

    FreeImage_Initialise();
    init();
    output = "temp.png";
    readfile(argv[1]) ;
    FIBITMAP* bitmap = FreeImage_Allocate(w, h, bpp);
    float fovx = fovy * static_cast<float>(w)/h;

    Raytrace rt;
    rt.raytrace(eye, center, up, fovx, fovy, w, h, bitmap, depth);

    saveScreenshot(bitmap, output);
    FreeImage_DeInitialise();

    for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
        delete *it;
    }
    return 0;
}
