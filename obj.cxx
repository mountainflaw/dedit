#include <dedit.hxx>

struct Sm64Area {
    uint8_t index;
    std::string music;
    std::string collision;
    std::string collisionType;
    std::string geo;
    std::string macro;
    std::string rooms;
};

struct Sm64Object {
    uint8_t type; /* basically, anything written is an object to the editor */
    int16_t pos[3]; /* warps: id level area */
    int16_t rot[3]; /* warps: node*/
    uint32_t bParam; /* warps: unused filler */
    uint8_t area;
    uint8_t jump;
    bool acts[7];
    std::string other1; /* model id or rom start */
    std::string other2; /* bhv or rom end*/
};

std::vector<Sm64Object> gObjList;

class ObjHandler {
    private:
    std::string file;
    uint16_t currCharPos = 0;

    std::string get_whitespace(uint8_t w) {
        std::string wt = "";
        for (uint8_t i = 0; i < w; i++) {
            wt += " ";
        }
        return wt;
    }

    void parseArea();
    void parseJump(const std::string &jump, uint8_t a);

    /** Sets matrix based off of obj properties */
    void setMtx(Sm64Object obj, int16_t mtx[4][4]) {
        mtx[0][3] = obj.pos[0];
        mtx[1][3] = obj.pos[1];
        mtx[2][3] = obj.pos[2];
        mtx[3][3] = 1;
    }

    public:
    ObjHandler(const std::string &f);
    void init();
    void write();
};

ObjHandler::ObjHandler(const std::string &f) {
    file = f;
}

#define NUM_OBJ_TYPES 14

static const std::string objTypes[] = {
    "area_end",
    "area",
    "object",
    "warp_node",
    "macro_objects",
    "rooms",
    "set_background_music",
    "terrain_type",
    "show_dialog",
    "load_mio0_texture",
    "load_mio0",
    "load_raw",
    "load_model_from_geo",
    "mario_pos",
    "jump_link"
};

enum ObjectTypes {
    OBJ_AREA_END,
    OBJ_AREA,
    OBJ_OBJECT,
    OBJ_WARP_NODE,
    OBJ_MACRO_OBJECTS,
    OBJ_ROOMS,
    OBJ_SET_BACKGROUND_MUSIC,
    OBJ_TERRAIN_TYPE,
    OBJ_SHOW_DIALOG,
    OBJ_LOAD_MIO0_TEXTURE,
    OBJ_LOAD_MIO0,
    OBJ_LOAD_RAW,
    OBJ_LOAD_MODEL_FROM_GEO,
    OBJ_MARIO_POS,
    OBJ_JUMP
};

void ObjHandler::parseJump(const std::string &jump, uint8_t a) {
    lvlScript.open(file, std::fstream::in);
    std::string line;
    bool entry = false;
    while (getline(lvlScript, line)) {
        if (!entry && line == jump + ":") {
            entry = true;
            continue;
        } else {
            for (uint16_t i = 0; i < line.length(); i++) {
                if (!determined) {
                    for (uint8_t o = 0; o < NUM_OBJ_TYPES; o++) { /* parse level cmd */
                        if (line[i] != ' ' && line.substr(i, objTypes[o].length()) == objTypes[o]) {
                            std::cout << objTypes[o] << " set" << std::endl;
                            //std::cout << objTypes[o].length() << std::endl;
                            determined = true;
                            i += objTypes[o].length();
                            type = o;
                            continue;
                        }
                    }
                } else if (line[i] != ' ') { /* parse arguments */
                    uint8_t distance = 0;
                    while (line[i + distance] != ',' && i + distance < line.length()) {
                        distance++;
                    }
                    argCounter++;
                    std::cout << "argument " << line.substr(i, distance) << std::endl;
                    i += distance;
                }
            }
        }
    }
}

void ObjHandler::parseMain() {
}

/** Initialize level script. Reads and parses level script and stores the output (if valid) into gObjectList. */
void ObjHandler::init() {
    std::string line;
    std::fstream lvlScript;
    uint8_t type;
    std::string arguments[10];

    lvlScript.open(file, std::fstream::in);

    if (lvlScript.is_open()) {
        bool entryPoint = false;
        while (getline(lvlScript, line)) {
            uint8_t argCounter = 0;
            bool determined = false;
            uint8_t currArea;

            std::regex regex(R"(\/\*.*?\*\/)"); /* strip comments */
            line = std::regex_replace(line, regex, "");

            for (uint16_t i = 0; i < line.length(); i++) {
                if (!determined) {
                    for (uint8_t o = 0; o < NUM_OBJ_TYPES; o++) { /* parse level cmd */
                        if (line[i] != ' ' && line.substr(i, objTypes[o].length()) == objTypes[o]) {
                            std::cout << objTypes[o] << " set" << std::endl;
                            //std::cout << objTypes[o].length() << std::endl;
                            determined = true;
                            i += objTypes[o].length();
                            type = o;
                            continue;
                        }
                    }
                } else if (line[i] != ' ') { /* parse arguments */
                    uint8_t distance = 0;
                    while (line[i + distance] != ',' && i + distance < line.length()) {
                        distance++;
                    }
                    argCounter++;
                    std::cout << "argument " << line.substr(i, distance) << std::endl;
                    i += distance;
                }
            }
            std::cout << std::endl;

        }
        lvlScript.close();
    }
}

void ObjHandler::write() {
    for (uint8_t i = 0; i < 8; i++) {
    }
}
