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

    public:
    Fast3DParser(const std::string &p, bool g);

    bool parse() {
        return true;
    }
};

Fast3DParser::Fast3DParser(const std::string &p, bool g) {
    path = p;
    geometryLayout = g;
}

void render_level() {
    for (uint32_t i = 0; i < dlVector.size(); i++) {
        dlVector[i].render();
    }
}

/**
 * Entry point for the graphics thread. Runs until the program is exited.
 */

void gfx_main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "dedit", NULL, NULL);

    if (window == NULL) {
        std::cout << "failed to create glfw context" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    gWindowSetup = true; /* tell the ui thread that the window setup phase is finished */

    while (1) {
        render_level();
    }
}
