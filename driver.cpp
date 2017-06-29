#include "shell.hpp"

int main(int argc, char * argv[])  {
    
    SHELL sh = SHELL();

    if(!argv[1]) {
        std::cout<<"Expect 1 argument, [shell] or a huaji script file"<<std::endl;

        return 0;
    }

    std::string option = argv[1];

    if(option=="shell") {

        sh.Commandline_Interface();

        return 0;
    }
    else {
        
        sh.Script_Interface(option);

        return 0;
    }
}