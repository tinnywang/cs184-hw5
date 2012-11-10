/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix
// transformations for a stack (matransform) and to rightmultiply the
// top of a stack.  These functions are given to aid in setting up the
// transformations properly, and to use glm functions in the right way.
// Their use is optional in your program.

// The functions readvals and readfile do basic parsing.  You can of course
// rewrite the parser as you wish, but we think this basic form might be
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE.
// Read the other parts to get a context of what is going on.

/*****************************************************************************/

// Basic includes to get this file to work.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "Transform.h"

using namespace std ;
#include "variables.h"
#include "readfile.h"


// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, GLfloat * values) {
    mat4 transform = transfstack.top() ;
    vec4 valvec = vec4(values[0],values[1],values[2],values[3]) ;
    vec4 newval = valvec * transform ;
    for (int i = 0 ; i < 4 ; i++) values[i] = newval[i] ;
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) {
    mat4 &T = transfstack.top() ;
    // Right multiply M, but do this left to account for row/column major
    T = M * T ;
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.
bool readvals(stringstream &s, const int numvals, GLfloat * values) {
    for (int i = 0 ; i < numvals ; i++) {
        s >> values[i] ;
        if (s.fail()) {

            cout << "Failed reading value " << i << " will skip " << numvals <<"\n" ;
            return false ;
        }
    }
    return true ;
}

void readfile(const char * filename) {
    string str, cmd ;
    ifstream in ;
    in.open(filename) ;
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.
        // This is done using standard STL Templates
        stack <mat4> transfstack ;
        transfstack.push(mat4(1.0)) ;  // identity
        getline (in, str) ;
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines

                stringstream s(str) ;
                s >> cmd ;
                int i ;
                GLfloat values[10] ; // position and color for light, colors for others
                // Up to 10 params for cameras.
                bool validinput ; // validity of input

                // Lights
                if (cmd == "directional" || cmd == "point") {
                    if (numused == numLights) { // No more Lights
                        cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n" ;
                    } else {
                        validinput = readvals(s, 6, values) ; // Position/color for lts.
                        if (validinput) {
                            for (i = 0; i < 3; i++) {
                                lightposn[4 * numused + i] = values[i];
                            }
                            lightposn[4 * numused + 3] = cmd == "directional" ? 0 : 1;
                            for (i = 0; i < 3; i++) {
                                lightcolor[4 * numused + i] = values[i + 3];
                            }
                            lightcolor[4 * numused + 3] = 1.0; // alpha = 0
                            ++numused ;
                        }
                    }
                }
                else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values);
                    if (validinput)
                        for (i = 0; i < 3; i++) attenuation[i] = values[i];
                }
                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values) ; // colors
                    if (validinput) {
                        for (i = 0 ; i < 3 ; i++) ambient[i] = values[i];
                        ambient[3] = 1.0;
                    }
                }
                // Materials
                else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values) ;
                    if (validinput) {
                        for (i = 0 ; i < 3 ; i++) diffuse[i] = values[i] ;
                        diffuse[3] = 1.0;
                    }
                }
                else if (cmd == "specular") {
                    validinput = readvals(s, 3, values) ;
                    if (validinput) {
                        for (i = 0 ; i < 3 ; i++) specular[i] = values[i] ;
                        specular[3] = 1.0;
                    }
                }
                else if (cmd == "emission") {
                    validinput = readvals(s, 3, values) ;
                    if (validinput) {
                        for (i = 0 ; i < 3 ; i++) emission[i] = values[i] ;
                        emission[3] = 1.0;
                    }
                }
                else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values) ;
                    if (validinput) shininess = values[0] ;
                }
                // General
                else if (cmd == "size") {
                    validinput = readvals(s,2,values) ;
                    if (validinput) {
                        w = (int) values[0] ;
                        h = (int) values[1] ;
                    }
                }
                else if (cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    if (validinput) depth = values[0];
                }
                else if (cmd == "output") {
                    s >> output;
                }
                else if (cmd == "camera") {
                    validinput = readvals(s,10,values) ; // 10 values eye cen up fov
                    if (validinput) {
                        // YOUR CODE FOR HW 2 HERE
                        // Use all of values[0...9]
                        // You may need to use the upvector fn in Transform.cpp
                        // to set up correctly.
                        // Set eyeinit upinit center fovy in variables.h
                        eyeinit = glm::vec3(values[0], values[1], values[2]);
                        center = glm::vec3(values[3], values[4], values[5]);
                        upinit = glm::vec3(values[6], values[7], values[8]);
                        upinit = Transform::upvector(upinit, eyeinit - center);
                        fovy = values[9];
                    }
                }

                // Geometry
                else if (cmd == "maxverts") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        maxverts = values[0];
                        vertices = vector<glm::vec3>();
                    }
                } else if (cmd == "maxvertnorms") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        maxvertnorms = values[0];
                        vertices = vector<glm::vec3>();
                        normals = vector<glm::vec3>();
                    }
                } else if (cmd == "vertex") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        vertices.push_back(glm::vec3(values[0], values[1], values[2]));
                    }
                } else if (cmd == "vertexnormal") {
                    validinput = readvals(s, 6, values);
                    if (validinput) {
                        vertices.push_back(glm::vec3(values[0], values[1], values[2]));
                        normals.push_back(glm::vec3(values[3], values[4], values[5]));
                    }
                } else if (cmd == "sphere" || cmd == "tri" || cmd == "trinormal") {
                    if (numobjects == maxobjects) // No more objects
                        cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n" ;
                    else {
                        if (cmd == "sphere") {
                            validinput = readvals(s, 4, values);
                            if (validinput) {
                                Sphere obj(values[0], values[1], values[2], values[3]);
                                for (i = 0 ; i < 4 ; i++) {
                                  obj.ambient[i] = ambient[i] ;
                                  obj.diffuse[i] = diffuse[i] ;
                                  obj.specular[i] = specular[i] ;
                                  obj.emission[i] = emission[i] ;
                                }
                                obj.shininess = shininess ;
                                obj.transform = transfstack.top() ;
                                objects.push_back(obj);
                            }
                        } else if (cmd == "tri") {
                            validinput = readvals(s, 3, values);
                            if (validinput) {
                                Triangle obj(vertices[values[0]], vertices[values[1]], vertices[values[2]]);
                                for (i = 0 ; i < 4 ; i++) {
                                  obj.ambient[i] = ambient[i] ;
                                  obj.diffuse[i] = diffuse[i] ;
                                  obj.specular[i] = specular[i] ;
                                  obj.emission[i] = emission[i] ;
                                }
                                obj.shininess = shininess ;
                                obj.transform = transfstack.top() ;
                                objects.push_back(obj);
                            }
                        } else if (cmd == "trinormal") {
                            validinput = readvals(s, 6, values);
                            if (validinput) {
                                Triangle obj(vertices[values[0]], vertices[values[1]], vertices[values[2]],
                                             normals[values[0]], normals[values[1]], normals[values[2]]);
                                for (i = 0 ; i < 4 ; i++) {
                                  obj.ambient[i] = ambient[i] ;
                                  obj.diffuse[i] = diffuse[i] ;
                                  obj.specular[i] = specular[i] ;
                                  obj.emission[i] = emission[i] ;
                                }
                                obj.shininess = shininess ;
                                obj.transform = transfstack.top() ;
                                objects.push_back(obj);
                            }
                        }
                        ++numobjects;
                    }
                }
                // Transformations
                else if (cmd == "translate") {
                    validinput = readvals(s,3,values) ;
                    if (validinput) {
                        // YOUR CODE FOR HW 2 HERE.
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.
                        rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values) ;
                    if (validinput) {
                        // YOUR CODE FOR HW 2 HERE.
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.
                        rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values) ;
                    if (validinput) {
                        // YOUR CODE FOR HW 2 HERE.
                        // values[0..2] are the axis, values[3] is the angle.
                        // You may want to normalize the axis (or in Transform::rotate)
                        // See how the stack is affected, as above.
                        vec3 axis = glm::vec3(values[0], values[1], values[2]);
                        rightmultiply(glm::mat4(Transform::rotate(values[3], axis)), transfstack);
                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform")
                    transfstack.push(transfstack.top()) ;
                else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1)
                        cerr << "Stack has no elements.  Cannot Pop\n" ;
                    else transfstack.pop() ;
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n" ;
                }
            }
            getline (in, str) ;
        }

        // Set up initial position for eye, up and amount
        // As well as booleans

        eye = eyeinit ;
        up = upinit ;
        amount = 5;
        sx = sy = 1.0 ; // scales in x and y
        tx = ty = 0.0 ; // translation in x and y
        useGlu = false; // don't use the glu perspective/lookat fns

    }
    else {
        cerr << "Unable to Open Input Data File " << filename << "\n" ;
        throw 2 ;
    }

}
