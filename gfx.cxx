/**
 * Handles the GUI and F3D renderer.
 */

#include <dedit.hxx>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

extern std::vector<std::string> gLevelDirectories;

enum GbiTypes { VTX, TRI1, TRI2, SETGEO, CLEARGEO };

struct Vtx {
    int16_t pos[3];
    int16_t tc[2];
    uint8_t col[4];
};

/* renderer */

static int16_t sVtxBuffer[64] = {0}; /* support for up to f3dex rej */
static int16_t sVtxBufferColors[64] = {0};

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

/* renderer */

/* gui */

/* dummy data */
static uint8_t col[3] = { 128, 0, 255 };
static int32_t pos[3] = { 0 };
static bool actCheckbox[7] = {false};
static uint8_t currAreaIndex = 1;
static char areaMusic[8][128] = { "SEQ_GRASS", "SEQ_GRASS", "SEQ_GRASS", "SEQ_GRASS", "SEQ_GRASS", "SEQ_GRASS", "SEQ_GRASS", "SEQ_GRASS" };
static char bParam[4][3];
static char modelId[97];

std::string selectedObject = "Nothing selected";
const static char hexChars[24] = "abcdefABCDEF1234567890 ";

static bool gfx_gui_check_byte(char b) {
    for (uint8_t i = 0; i < 24; i++) {
        std::cout << "comparing " << b << " vs " << hexChars[i] << std::endl;
        if (b == hexChars[i]) {
            return true;
        }
    }
    return false; /* not a valid hex character */
}

static void set_bparams_to_char(char* bparam) {
    if (!gfx_gui_check_byte(bparam[0])) {
        bparam[0] = ' ';
    }

    if (!gfx_gui_check_byte(bparam[1])) {
        bparam[1] = ' ';
    }
}

/* gui rendering */

static void gfx_render_gui_level_editor() {
    ImGui::Begin("Level");

    /* levels */
    static const char* currentLevel = gLevelDirectories[0].c_str();
    if (ImGui::BeginCombo("Level", currentLevel)) {
        for (uint8_t i = 0; i < gLevelDirectories.size(); i++) {
            if (ImGui::Selectable(gLevelDirectories[i].c_str(), currentLevel == gLevelDirectories[i].c_str())) {
                currentLevel = gLevelDirectories[i].c_str();
            }
        }
        ImGui::EndCombo();
    }

    /* areas */
    static const char* areaList[] = { "Area 1", "Area 2", "Area 3", "Area 4", "Area 5", "Area 6", "Area 7", "Area 8" };
    static const char* currentArea = areaList[0];
    if (ImGui::BeginCombo("Area", currentArea)) {
        for (uint8_t i = 0; i < 8; i++) {
            if (ImGui::Selectable(areaList[i], currentArea == areaList[i])) {
                currentArea = areaList[i];
            }
        }
        ImGui::EndCombo();
    }
    /* music */
    ImGui::InputText("Music", areaMusic[currAreaIndex - 1], IM_ARRAYSIZE(areaMusic[0]));

    ImGui::Button("Load"); ImGui::SameLine();
    ImGui::Button("Write");
    ImGui::End();
}

/* selection editor */

/* obj editor */
static void gfx_render_gui_selected_obj() {
    ImGui::Begin(selectedObject.c_str());

    if (!actCheckbox[6]) {
        ImGui::Checkbox("Act 1", &actCheckbox[0]); ImGui::SameLine();
        ImGui::Checkbox("Act 2", &actCheckbox[1]);
        ImGui::Checkbox("Act 3", &actCheckbox[2]); ImGui::SameLine();
        ImGui::Checkbox("Act 4", &actCheckbox[3]);
        ImGui::Checkbox("Act 5", &actCheckbox[4]); ImGui::SameLine();
        ImGui::Checkbox("Act 6", &actCheckbox[5]);
    }

    if (actCheckbox[0] && actCheckbox[1] && actCheckbox[2] && actCheckbox[3] && actCheckbox[4] && actCheckbox[5]) {
        for (uint8_t i = 0; i < 6; i++) {
            actCheckbox[i] = false;
        }
        actCheckbox[6] = true;
    }

    ImGui::Checkbox("All acts", &actCheckbox[6]);

    ImGui::InputInt("X Position", &pos[0]);
    ImGui::InputInt("Y Position", &pos[1]);
    ImGui::InputInt("Z Position", &pos[2]);

    ImGui::PushItemWidth(30);
    ImGui::InputText("BParam 1", bParam[0], IM_ARRAYSIZE(bParam[0])); ImGui::SameLine();
    ImGui::InputText("BParam 2", bParam[1], IM_ARRAYSIZE(bParam[0]));
    ImGui::InputText("BParam 3", bParam[2], IM_ARRAYSIZE(bParam[0])); ImGui::SameLine();
    ImGui::InputText("BParam 4", bParam[3], IM_ARRAYSIZE(bParam[0]));

    ImGui::PopItemWidth();
    ImGui::InputText("Model ID", modelId, IM_ARRAYSIZE(modelId));

    /* don't let values overflow for s16 */
    for (uint8_t i = 0; i < 3; i++) {
        if (pos[i] > 32767) {
            pos[i] = 32767;
        }

        if (pos[i] < -32768) {
            pos[i] = -32768;
        }
    }

    ImGui::Button("Apply");
    ImGui::End();
}

static void gfx_render_gui_selection_editor() {
    gfx_render_gui_selected_obj();
}

/* gl window stuff */

void gfx_resize_window(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * Entry point for the graphics thread. Runs until the program is exited,
 * and is the main handler for the window.
 */

/* TODO: Move the UI out of this and into their own functions */

void gfx_main() {
    const char* glsl_version = "#version 150";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); /* DO NOT CHANGE THIS */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); /* DO NOT CHANGE THIS */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 800, "dedit", NULL, NULL);

    if (window == NULL) {
        fprintf(stderr, "failed to initialize GLFW!\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (glewInit()) {
            fprintf(stderr, "Failed to initialize OpenGL loader!\n");
            glfwTerminate();
            return;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, gfx_resize_window);
    gWindowSetup = true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();

    io_set_lvl_dirs();

    while (!glfwWindowShouldClose(window)) {
        for (uint8_t i = 0; i < 4; i++) {
            set_bparams_to_char(bParam[i]);
        }

        glfwPollEvents();
        gfx_render_level();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        gfx_render_gui_level_editor();
        gfx_render_gui_selection_editor();

        glClearColor(col[CR] / 255.0f, col[CG] / 255.0f, col[CB] / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        dlVector.clear();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}
