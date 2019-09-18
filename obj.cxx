#include <dedit.hxx>
#include "obj.hxx"

std::vector<Sm64Object> gObjList;

ObjHandler::ObjHandler(const std::string &f) {
    file = f;
}

#define NUM_OBJ_TYPES 16

static const std::string objTypes[] = {
    "end_area",
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
    "mario_pos"
    "jump_link",
    "return"
};

static const uint8_t objTypesMaxArgs[2][4] = {
};

/** Initialize level script. Reads and parses level script and stores the output (if valid) into gObjectList. */
void ObjHandler::init() {
    std::string line;
    std::fstream lvlScript;
    std::string arguments[10];

    lvlScript.open(file, std::fstream::in);

    if (lvlScript.is_open()) {
        bool entryPoint = false;
        while (getline(lvlScript, line)) {
            uint8_t argCounter = 0;
            bool determined = false;
            int8_t currArea = -1;
            uint8_t type = OBJ_NONE;

            std::regex regex(R"(\/\*.*?\*\/)");
            line = std::regex_replace(line, regex, ""); /* strip objects */

            if (line.find("glabel level_bob_entry") != std::string::npos) {
                std::cout << "entry!" << std::endl;
                entryPoint = true;
            }
            if (entryPoint) {
                for (uint16_t i = 0; i < line.length(); i++) {
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
                        arguments[argCounter] = line.substr(i, distance);
                        argCounter++;
                        std::cout << "argument " << line.substr(i, distance) << std::endl;
                        i += distance;
                    }
                }
                std::cout << std::endl;
            }

            /* execute command */
            switch (type) {
                case OBJ_RETURN:
                return;
            }

        }
        lvlScript.close();
    }
}

/*void ObjHandler::write() {
    for (uint16_t i = 0; i < gObjList.size(); i++) {
    }
}*/
