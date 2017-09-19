#include "hjscript.hpp"

int main(int argc, char* argv[])  {

    hjbase::HUAJISCRIPTBASE hjs = hjbase::HUAJISCRIPTBASE();

    // command line interface
    if(!argv[1]) {
        hjs.Entry_Point();
        return 0;
    }

    // file execution interface
    std::string file_name = argv[1];
    hjs.Entry_Point(file_name);
    return 0;
}