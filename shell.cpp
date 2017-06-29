#include "shell.hpp"

SHELL::SHELL() {
    hjscript = new HUAJISCRIPTBASE;
}

SHELL::~SHELL() {
    delete hjscript;
}

void SHELL::Commandline_Interface() {
    std::string token;
    std::cout<<std::endl<<"Huaji script v1.0"<<std::endl<<std::endl;
    std::cout<<"O_O >>> ";
    std::vector<std::string> *command = new std::vector<std::string>;
    while(std::cin>>token) {
        if(token=="quit") {
            break;
        }
        int status = hjscript->Take_One_Token(token, command);
        if(status==VALID_COMMAND_RETURN_CODE) {
            std::cout<<"Internal error, command ignored."<<std::endl;
            std::cout<<"O_O >>> ";
        }
        if(!status){
            std::cout<<"O_O >>> ";
        }
    }
    delete command;
    
}

void SHELL::Script_Interface(std::string script_name) {
    std::string token;
    std::ifstream script_file(script_name);

    int status;
    std::vector<std::string> *command = new std::vector<std::string>;
    while(script_file>>token) {
        if(token=="quit") {
            break;
        }
        status = hjscript->Take_One_Token(token, command);
        if(status==VALID_COMMAND_RETURN_CODE) {
            std::cout<<"Internal error, command ignored."<<std::endl;
        }
    }

    if(status==-1) {
        std::cout<<"Incomplete Command"<<std::endl;
    }
    delete command;

    return;
}
