#include <dedit.hxx>

struct Sm64Area {
    uint8_t index;
    std::string music;
    std::string collision;
    std::string terrain;
    std::string geo;
    std::string macro;
    std::string rooms;
};

struct Sm64Object {
    uint8_t type;
    int16_t pos[3];
    int16_t rot[3];
    uint32_t bParam;
    uint8_t area;
    bool acts[7];
    std::string other1; /* model id */
    std::string other2; /* bhv */
};

struct Sm64RomLoad {
    uint8_t type;
    uint8_t segment;
    std::string romStart;
    std::string romEnd;
};

std::vector<Sm64Object> gObjList;
std::vector<Sm64RomLoad> gRomLoadList;
struct Sm64Area gAreaList[8];
bool gAreaUsed[8] = {false};

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

    void parseLevel(bool jump, const std::string &label);
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

#define NUM_OBJ_TYPES 16

static std::string arguments[10];

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
    "jump_link",
    "return"
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
    OBJ_JUMP,
    OBJ_RETURN
};

std::string currAreaMusic = "";
std::string currAreaCollision = "";
std::string currAreaTerrain = "";
std::string currAreaGeo = "";
std::string currAreaRooms = "";
std::string currAreaMacro = "";
int16_t currAreaDialog[2] = {-1, -1};
int16_t marioPos[5] = {1, 0, 0, 0, 0};

uint8_t currAreaIndex;
bool inArea = false;

void ObjHandler::parseLevel(bool jump, const std::string &label) {
    std::string line;
    std::fstream lvlScript;
    uint8_t type;

    lvlScript.open(file, std::fstream::in);

    if (lvlScript.is_open()) {
        bool entryPoint = false;
        while (getline(lvlScript, line)) { /* TODO: replace this with a setup that can seek */
            uint8_t argCounter = 0;
            bool determined = false;

            std::regex regex(R"(\/\*.*?\*\/)"); /* strip comments */
            line = std::regex_replace(line, regex, "");

            for (uint16_t i = 0; i < line.length(); i++) { /* extract data from line */
                if (!determined) {
                    for (uint8_t o = 0; o < NUM_OBJ_TYPES; o++) { /* parse level cmd */
                        if (line[i] != ' ' && line.substr(i, objTypes[o].length()) == objTypes[o]) {
                            std::cout << objTypes[o] << " set" << std::endl;
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
                    arguments[argCounter] = line.substr(i, distance);
                    std::cout << "argument " << line.substr(i, distance) << std::endl;
                    i += distance;
                }

                switch (type) {
                case OBJ_AREA_END: /* adds previous area to area vector */
                    gAreaList[currAreaIndex] = {currAreaIndex, currAreaMusic, currAreaCollision, currAreaTerrain, currAreaGeo, currAreaMacro, currAreaRooms};
                    gAreaUsed[currAreaIndex] = true;
                    inArea = false;
                    break;
                case OBJ_AREA: /* initialize area */
                      currAreaIndex = std::stoi(arguments[0]);
                      currAreaGeo = arguments[1];
                      inArea = true;
                      break;
                case OBJ_OBJECT:
                      break;
                case OBJ_WARP_NODE:
                      break;
                case OBJ_MACRO_OBJECTS: currAreaMacro = arguments[0]; break;
                case OBJ_ROOMS:
                    currAreaRooms = arguments[0];
                    break;
                case OBJ_SET_BACKGROUND_MUSIC: currAreaMusic = arguments[0]; break;
                case OBJ_TERRAIN_TYPE: currAreaTerrain = arguments[0]; break;
                case OBJ_SHOW_DIALOG:
                    currAreaDialog[0] = std::stoi(arguments[0]);
                    currAreaDialog[1] = std::stoi(arguments[1]);
                    break;
                case OBJ_LOAD_MIO0_TEXTURE:
                    gRomLoadList.push_back({OBJ_LOAD_MIO0_TEXTURE, (uint8_t)std::stoi(arguments[0]), arguments[1], arguments[2]});
                    break;
                case OBJ_LOAD_MIO0:
                    gRomLoadList.push_back({OBJ_LOAD_MIO0, (uint8_t)std::stoi(arguments[0]), arguments[1], arguments[2]});
                    break;
                case OBJ_LOAD_RAW:
                    gRomLoadList.push_back({OBJ_LOAD_RAW, (uint8_t)std::stoi(arguments[0]), arguments[1], arguments[2]});
                    break;
                case OBJ_LOAD_MODEL_FROM_GEO:
                      break;
                case OBJ_MARIO_POS:
                      marioPos[0] = std::stoi(arguments[0]);
                      marioPos[1] = std::stoi(arguments[1]);
                      marioPos[2] = std::stoi(arguments[2]);
                      marioPos[3] = std::stoi(arguments[3]);
                      marioPos[4] = std::stoi(arguments[4]);
                      break;
                case OBJ_JUMP: /* squashes calls to area if local jump, preserved if global */
                      break;
                case OBJ_RETURN:
                      return;
                }
            }
            std::cout << std::endl;

        }
        lvlScript.close();
    }
}

/** Initialize level script. Reads and parses level script and stores the output (if valid) into gObjectList. */
void ObjHandler::init() {
    parseLevel(false, "");

}

void ObjHandler::write() {
    for (uint8_t i = 0; i < 8; i++) {
    }
}
