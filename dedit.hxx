#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <math.h>
#include <pthread.h>
#include <thread>
#include <fstream>

/* opengl */
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

/* enums */

enum Axis { AXX, AXY, AXZ };
enum Rot { RX, RY, RZ };
enum Col { CR, CG, CB, CA };

/* externs / prototypes */

extern std::string gWorkDirectory, gLevelFile;

/* entry points*/
void gfx_main();

void obj_init();

extern bool gWindowSetup;
