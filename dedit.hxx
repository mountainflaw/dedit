#include <iostream>
#include <regex>
#include <vector>
#include <stdio.h>
#include <string>
#include <math.h>
#include <pthread.h>
#include <thread>
#include <fstream>
#include <filesystem>

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

void gfx_main();
void obj_init();
bool io_is_directory(const std::string &dir);
bool io_exists(const std::string &dir);
void io_set_lvl_dirs();

extern bool gWindowSetup;
