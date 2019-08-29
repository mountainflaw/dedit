#include <dedit.hxx>

enum GbiTypes { VTX, TRI1, TRI2, SETGEO, CLEARGEO };

struct Vtx {
    int16_t pos[3];
    int16_t tc[2];
    uint8_t col[4];
};

class DisplayList {
    public:
    void render() {
        return;
    }
};

std::vector<DisplayList> dlVector;

class Fast3DParser {
    private:
    bool geometryLayout;
    std::string path;

    /**
     * this is the method that actually parses the gbi list
     */

    bool parse_gbi(const std::string &f) {
        return true;
    }

    public:
    Fast3DParser(const std::string &p, bool g);

    /**
     * parsing front-end
     */

    bool parse() {

        /*
         * parse geometry layout for displaylist references first, then try to
         * find display lists.
         *
         * assumptions this makes:
         * 1.) it assumes the file structure is <script.s location>/areas/x/geo.s
         * 2.) it assumes there is only one area
         */

        if (geometryLayout) {
        } else {
            parse_gbi(path);
        }
        return true;
    }
};

Fast3DParser::Fast3DParser(const std::string &p, bool g) {
    path = p;
    geometryLayout = g;
}

/**
 * Cycles through display list objects and executes them.
 */

void gfx_render_level() {
    for (uint32_t i = 0; i < dlVector.size(); i++) {
        dlVector[i].render();
    }
}

/* gui */
uint8_t col[3] = { 128, 0, 255 };

/* gl window stuff */

void gfx_resize_window(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * Entry point for the graphics thread. Runs until the program is exited,
 * and is the main handler for the window.
 */

void gfx_main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "dedit", NULL, NULL);

    if (window == NULL) {
        std::cout << "failed to create glfw context" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, gfx_resize_window);
    glfwSwapInterval(120);
    gWindowSetup = true;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(col[CR] / 255.0f, col[CG] / 255.0f, col[CB] / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        gfx_render_level();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}
