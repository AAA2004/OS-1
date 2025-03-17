#include <iostream>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

void list_files(const string& dir_path) {
    DIR* dir = opendir(dir_path.c_str());
    if (!dir) {
        cerr << "Error: " << dir_path << " is not a valid directory." << endl;
        return;
    }

    cout << "Contents of directory: " << dir_path << endl;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_name[0] != '.') {  // Ignore hidden files
            cout << entry->d_name << endl;
        }
    }
    cout << endl;
    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <directory1> [directory2] ..." << endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        list_files(argv[i]);
    }

    return 0;
}
