#include "hjscript.hpp"

int main(int argc, char * argv[])  {

    if(!argv[1]) {
        HUAJISCRIPTBASE hjs = HUAJISCRIPTBASE();
        hjs.Entry_Point();
        return 0;
    }

    std::string file_name = argv[1];
    HUAJISCRIPTBASE hjs = HUAJISCRIPTBASE(file_name);
    hjs.Entry_Point();
    return 0;
}