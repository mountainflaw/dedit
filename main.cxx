#include <dedit.hxx>

std::string gWorkDirectory, gLevelFile;
bool gWindowSetup = false;

class ObjHandler {
    public:
    ObjHandler(const std::string &f);
    void init();
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "err" << std::endl;
        exit(1);
    }

    gLevelFile = argv[1];

    ObjHandler objInstance(gLevelFile);
    objInstance.init();
    gfx_main();
}
