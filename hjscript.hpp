#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <cstdlib>

#include "config.hpp"

struct VAR {
    std::string var;
    std::string val;
    struct VAR *next;
};

class HUAJIBASE_EXCEPTION : public std::exception {
    virtual const char* what() const noexcept {
        return "Huaji Interpreter Unspecified Exception";
    }
};

class EVALUATION_EXCEPTION : public HUAJIBASE_EXCEPTION {
    virtual const char* what() const noexcept {
        return "Evaluation Exception";
    }
};

class SYNTAX_EXCEPTION : public HUAJIBASE_EXCEPTION {
    virtual const char* what() const noexcept {
        return "Syntax Error";
    }
};

class NAME_EXCEPTION : public HUAJIBASE_EXCEPTION {
    virtual const char* what() const noexcept {
        return "Undefined name";
    }
};

namespace huaji_script_base_exception {
    const HUAJIBASE_EXCEPTION huaji_except;
    const EVALUATION_EXCEPTION eval_except;
    const SYNTAX_EXCEPTION syntax_except;
    const NAME_EXCEPTION name_except;
}

class HUAJISCRIPTBASE {

    public:

        HUAJISCRIPTBASE();

        /*
            Destructor
                - release variables memory
        */
        ~HUAJISCRIPTBASE();

        /*
            Arguments:
                token : current last token in command vector string
                command : pointer to command vector string (created by the caller function and free by the called function), 
                    will be mutated (cleared) if command is completed and executed.
            Return Code:
                0 - normal
                1 - unknown command
                -1 - waiting for more tokens
        */
        int Take_One_Token(std::string token, std::vector<std::string> *command);

    protected:

        virtual int More_Command_Level_1(const std::vector<std::string> *command_to_be_exectued);

        virtual std::string More_Expression_Level_1(std::string op, const std::vector<std::string> *vals);

        virtual void More_Cleanup_Level_1();
        
        std::string Evaluate_Expression(const std::vector<std::string> *expr);

        void Signal_Error(std::string error_msg, const std::vector<std::string> *error_part);

        void Signal_Error(std::string error_msg, std::string error_part);

        std::string May_Need_Dereference(std::string to_be_dereferenced);

        bool Find_And_Evaluate_Condition(const std::vector<std::string> *commands_block);

        void Block_Execution(const std::vector<std::string> *commands_block);

        void Variable_Assignment(std::string var, std::string val);

        std::string Indent_By_AST_Depth();

        bool Check_If_Float_Point(const std::vector<std::string> *vals);

        void Cleanup_If_Exception_Thrown();

        void Print_Debug_Info(std::string info, int ast_depth_change, const std::vector<std::string> *node);

        void Print_Debug_Info(std::string info, int ast_depth_change, const std::string &node);
    
    private:
        /*
            Although there are ways to detect T type automatically, but pass a integer representing type
            feels easier to me
        */
        template <typename T>
        std::string Numerical_Operation_Templated_Helper(std::string op, int vals_size, int T_name, const std::vector<std::string> *vals);

        std::string Numerical_Operation(std::string op, const std::vector<std::string> *vals);

        std::string Other_Basic_Operation(std::string op, const std::vector<std::string> *vals);
        
        std::string Basic_Operation(std::string op, const std::vector<std::string> *vals);

        int Collect_Tokens(std::string token);

        int Huaji_Command_Interpreter(const std::vector<std::string> *command_to_be_exectued);

        std::string Variable_Query(std::string var);

        void Print_Vector_String(const std::vector<std::string> *vector_string);

        void Highlight_Error_Part(const std::vector<std::string> *vector_string);

        struct VAR *variables;

        int collect_status, current_ast_depth;

        bool enable_float_point, enable_debug_mode;
};