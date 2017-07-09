#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <map>
#include <queue>

#include "config.hpp"

namespace hjbase_ufunc {

    void Signal_Error(const std::string &error_msg, const std::vector<std::string> *error_part);

    void Signal_Error(const std::string &error_msg, const std::string &error_part);

    void Print_Vector_String(const std::vector<std::string> *vector_string);

    void Highlight_Error_Part(const std::vector<std::string> *vector_string);

    bool Check_If_Float_Point(const std::vector<std::string> *vals);

    bool Starts_With(const std::string &this_str, const std::string &start_str);

    bool Is_Numerical(const std::string &this_str);

}

namespace hjbase_except {

    class HUAJIBASE_EXCEPTION : public std::exception {
        virtual const char* what() const noexcept {
            return "Huaji Interpreter Unspecified Exception";
        }
    };

    class TOKEN_EXCEPTION : public std::exception {
        virtual const char* what() const noexcept {
            return "Thrown by tokenizer";
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
            return "Name Undefined or redeclared";
        }
    };

    const HUAJIBASE_EXCEPTION huaji_except;
    const EVALUATION_EXCEPTION eval_except;
    const SYNTAX_EXCEPTION syntax_except;
    const NAME_EXCEPTION name_except;
    const TOKEN_EXCEPTION token_except;
}

namespace hjbase_class {
    class HUAJITOKENIZER {
        public:

            HUAJITOKENIZER(std::string file_name);
            HUAJITOKENIZER();
            ~HUAJITOKENIZER();

            std::string Get_One_Token();
            bool enable_raw_string;
        
        private:

            std::istream *source;
            std::queue<std::string> *token_queue;
            bool is_cin, is_in_quotation, is_in_block_comment, is_in_line_comment, is_in_nosubst, is_in_square_bracket;
    };


    class HUAJISCRIPTBASE {

        public:

            HUAJISCRIPTBASE(std::string file_name);
            HUAJISCRIPTBASE();
            /*
                Destructor
                    - release names memory
                    - release tokenizer memory
            */
            ~HUAJISCRIPTBASE();

            void Entry_Point();

        protected:

            virtual int More_On_Command_Level_1(const std::vector<std::string> *command_to_be_exectued);

            virtual std::string More_On_Expression_Level_1(const std::string &op, const std::vector<std::string> *vals);

            virtual std::string More_On_Names_Query_Level_1(const std::string &name);

            virtual std::string More_On_Slice_Operator_Level_1(const std::vector<std::string> *vals);

            virtual void More_Cleanup_Level_1();
            
            std::string Evaluate_Expression(const std::vector<std::string> *expr);

            bool Find_And_Evaluate_Condition(const std::vector<std::string> *commands_block);

            void Block_Execution(const std::vector<std::string> *commands_block);

            void Declare_Name(const std::string &name, const std::string &val, std::map<std::string, std::string> *target_scope_names);

            void Mutate_Name(const std::string &name, const std::string &val, std::map<std::string, std::string> *target_scope_names);

            std::string Resolve_Name(const std::string &name, const std::map<std::string, std::string> *target_scope_names);

            void Cleanup_If_Exception_Thrown();
        
        private:
            /*
                Although there are ways to detect T type automatically, but pass a integer representing type
                feels easier to me
            */
            template <typename T>
            std::string Numerical_Operation_Templated_Helper(const std::string &op, int vals_size, int T_name, const std::vector<std::string> *vals);

            std::string Numerical_Operation(const std::string &op, const std::vector<std::string> *vals);

            std::string Handle_Val(const std::string &name_or_val);

            void Print_Name_Map(const std::map<std::string, std::string> *names_map);

            std::string Other_Basic_Operation(const std::string &op, const std::vector<std::string> *vals);
            
            std::string Basic_Operation(const std::string &op, const std::vector<std::string> *vals);

            void Print_Debug_Info(const std::string &info, int ast_depth_change, const std::vector<std::string> *node);

            void Print_Debug_Info(const std::string &info, int ast_depth_change, const std::string &node);

            std::string Indent_By_AST_Depth();

            int Collect_Tokens(const std::string &token);

            /*
                Arguments:
                    token : current last token in command vector string
                    command : pointer to command vector string (created by the caller function and free by the called function), 
                        will be mutated (cleared) if command is completed and executed.
                Return code is same as Fake_Tokenizer
            */
            int Take_One_Token(const std::string &token, std::vector<std::string> *command);

            int Huaji_Command_Interpreter(const std::vector<std::string> *command_to_be_exectued);

            std::map<std::string, std::string> *names;

            class HUAJITOKENIZER *tokenizer;

            int collect_status, current_ast_depth;

            bool enable_float_point, enable_debug_mode, enable_raw_string, is_console;
    };
}