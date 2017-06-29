#pragma once

#include "hjscript.hpp"

class SHELL {
public:
    SHELL();
    ~SHELL();

    void Commandline_Interface();
    void Script_Interface(std::string script_name);

private:
    HUAJISCRIPTBASE *hjscript;
};