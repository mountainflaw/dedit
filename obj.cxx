#include <dedit.hxx>

struct Sm64Object {
    uint8_t type; /* basically, anything written is an object to the editor */
    int16_t pos[3];
    int16_t rot[3];
    uint32_t bParam;
    uint8_t area;
    bool acts[7];
    std::string other1; /* model id or rom start */
    std::string other2; /* bhv or rom end*/
};

std::vector<Sm64Object> gObjList;

class ObjHandler {
    std::string file;
    uint16_t currCharPos = 0;

    std::string get_whitespace(uint8_t w) {
        std::string wt = "";
        for (uint8_t i = 0; i < w; i++) {
            wt += " ";
        }
        return wt;
    }

    /** Sets matrix based off of obj properties */
    void set_mtx(Sm64Object obj, int16_t mtx[4][4]) {
        mtx[0][3] = obj.pos[0];
        mtx[1][3] = obj.pos[1];
        mtx[2][3] = obj.pos[2];
        mtx[3][3] = 1;
    }

    public:
    ObjHandler(const std::string &f);
    void init();
};

ObjHandler::ObjHandler(const std::string &f) {
    file = f;
}

#define NUM_OBJ_TYPES 14

static const std::string objTypes[] = {
    "area",
    "area_end",
    "object",
    "warp_node",
    "macro_objects",
    "rooms",
    "set_background_music",
    "terrain_type",
    "show_dialog",
    "load_mio0",
    "load_mio0_texture",
    "load_raw",
    "load_model_from_geo",
    "mario_pos"
};

enum ObjectTypes {
    OBJ_AREA,
    OBJ_AREA_END,
    OBJ_OBJECT,
    OBJ_WARP_NODE,
    OBJ_MACRO_OBJECTS,
    OBJ_ROOMS,
    OBJ_SET_BACKGROUND_MUSIC,
    OBJ_TERRAIN_TYPE,
    OBJ_SHOW_DIALOG,
    OBJ_LOAD_MIO0,
    OBJ_LOAD_MIO0_TEXTURE,
    OBJ_LOAD_RAW,
    OBJ_LOAD_MODEL_FROM_GEO,
    OBJ_MARIO_POS
};

/** Initialize level script. Reads and parses level script and stores the output (if valid) into gObjectList. */
void ObjHandler::init() {
    std::string line;
    std::fstream lvlScript;
    uint8_t type;
    std::string arguments[10];

    lvlScript.open(file, std::fstream::in);

    if (lvlScript.is_open()) {
        while (getline(lvlScript, line)) {
            uint8_t argCounter = 0;
            std::string unparsedLine;
            bool determined = false;
            uint8_t currArea;

            /* strip comments here pls */

            for (uint16_t i = 0; i < line.length(); i++) {
                if (!determined) {
                    for (uint8_t o = 0; o < NUM_OBJ_TYPES; o++) { /* parse level cmd */
                        if (line[i] != ' ' && line.substr(i, objTypes[o].length()) == objTypes[o]) {
                            std::cout << objTypes[o] << " set" << std::endl;
                            //std::cout << objTypes[o].length() << std::endl;
                            determined = true;
                            i += objTypes[o].length();
                            continue;
                        }
                    }
                } else if (line[i] != ' ') { /* parse arguments */
                    uint8_t distance = 0;
                    while (line[i + distance] != ',' && i + distance < line.length()) {
                        distance++;
                    }
                    std::cout << "argument " << line.substr(i, distance) << std::endl;
                    i += distance;
                }
            }
            std::cout << std::endl;
        }
        lvlScript.close();
    }
}

/*void ObjHandler::write() {
    for (uint16_t i = 0; i < gObjList.size(); i++) {
    }
}*/
