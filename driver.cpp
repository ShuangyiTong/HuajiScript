#include "hjscript.hpp"

int main(int argc, char * argv[])  {

    // file execution interface
    if(!argv[1]) {
        hjbase::HUAJISCRIPTBASE hjs = hjbase::HUAJISCRIPTBASE();
        hjs.Entry_Point();
        return 0;
    }

    // interactive command line interface
    std::string file_name = argv[1];
    hjbase::HUAJISCRIPTBASE hjs = hjbase::HUAJISCRIPTBASE(file_name);
    hjs.Entry_Point();
    return 0;
}