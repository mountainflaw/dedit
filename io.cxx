#include <dedit.hxx>

std::vector<std::string> gLevelDirectories;

bool io_is_directory(const std::string &dir) {
    return std::filesystem::is_directory(dir);
}

bool io_exists(const std::string &p)
{ return std::filesystem::exists(p); }

std::string io_get_file_name(const std::string &p)
{ return std::filesystem::path(p).filename().string(); }

void io_set_lvl_dirs() {
    for (auto& f: std::filesystem::directory_iterator("/home/xor/romes/sm64/srcwork/levels")) {
        if (io_is_directory(f.path())) {
            gLevelDirectories.push_back(io_get_file_name(f.path().string()));
        }
    }
}
