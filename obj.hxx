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
    OBJ_RETURN,
    OBJ_NONE
};
