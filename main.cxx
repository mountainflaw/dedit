#include <dedit.hxx>
#include "obj.hxx"

std::string gWorkDirectory, gLevelFile;
bool gWindowSetup = false;

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
