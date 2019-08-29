#include <dedit.hxx>

struct Sm64Object {
    uint8_t type; /* basically, anything written is an object to the editor */
    int16_t pos[3];
    int16_t rot[3];
    uint32_t bParam;
};

std::vector<Sm64Object> objList;

static std::string find_from_whitespace(const std::string &s) {
    return "";
}

/**
 * Parses level script file to find objects
 */

void obj_init() {
    std::cout << find_from_whitespace("");

    std::string line;
    std::fstream lvlScript;

    lvlScript.open(gLevelFile, std::fstream::in);

    if (lvlScript.is_open()) {
        while (getline(lvlScript, line)) {
            std::cout << line << std::endl;
        }
        lvlScript.close();
    }
}

void obj_write() {
}
