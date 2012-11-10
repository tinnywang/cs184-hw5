/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.
// Since all files need access to it, we define EXTERN as either blank or
// extern, depending on if included in the main program or not.
#include <vector>
#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"

#ifdef MAINPROGRAM
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up

#ifdef MAINPROGRAM
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point
int w = 600, h = 400 ; // width and height
int depth = 5;
float fovy = 90.0 ; // For field of view
#else
EXTERN vec3 eyeinit ;
EXTERN vec3 upinit ;
EXTERN vec3 center ;
EXTERN int w, h ;
EXTERN float fovy ;
EXTERN int depth ;
#endif

EXTERN string output;

EXTERN bool useGlu; // Toggle use of "official" opengl/glm transform vs user
EXTERN GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
static enum {view, translate, scale} transop ; // which operation to transform
EXTERN float sx, sy ; // the scale in x and y
EXTERN float tx, ty ; // the translation in x and y

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10 ;
EXTERN GLfloat lightposn [4*numLights] ; // Light Positions
EXTERN GLfloat lightcolor[4*numLights] ; // Light Colors
EXTERN GLfloat lightransf[4*numLights] ; // Lights transformed by modelview
EXTERN int numused ;                     // How many lights are used
EXTERN GLfloat attenuation[3] ;
EXTERN GLfloat ambient[4] ;

// Materials (read from file)
// With multiple objects, these are colors for each.
EXTERN GLfloat diffuse[4] ;
EXTERN GLfloat specular[4] ;
EXTERN GLfloat emission[4] ;
EXTERN GLfloat shininess ;

// For multiple objects, read from a file.
const int maxobjects = 1000 ;
EXTERN int numobjects ;
EXTERN std::vector<Object> objects ;

// Variables for specifying geometry
EXTERN vector<vec3> vertices;
EXTERN vector<vec3> normals;
EXTERN int maxvertnorms;
EXTERN int maxverts;

// Variables to set uniform params for lighting fragment shader
EXTERN GLuint lightcol ;
EXTERN GLuint lightpos ;
EXTERN GLuint numusedcol ;
EXTERN GLuint enablelighting ;
EXTERN GLuint ambientcol ;
EXTERN GLuint diffusecol ;
EXTERN GLuint specularcol ;
EXTERN GLuint emissioncol ;
EXTERN GLuint shininesscol ;

