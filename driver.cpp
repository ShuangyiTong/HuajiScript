#include "hjscript.hpp"

using namespace hjbase_class;

int main(int argc, char * argv[])  {

    // file execution interface
    if(!argv[1]) {
        HUAJISCRIPTBASE hjs = HUAJISCRIPTBASE();
        hjs.Entry_Point();
        return 0;
    }

    // interactive command line interface
    std::string file_name = argv[1];
    HUAJISCRIPTBASE hjs = HUAJISCRIPTBASE(file_name);
    hjs.Entry_Point();
    return 0;
}