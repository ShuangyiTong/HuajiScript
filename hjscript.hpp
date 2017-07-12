#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <map>
#include <queue>


#define VALID_COMMAND_RETURN_CODE 1
#define INVALID_COMMAND_RETURN_CODE 0

/*  
    Comment this directive to improve performance
    Enable this will let itVecStr constructor check if iterator range is valid
*/
#define pVecStr_RANGE_SAFETY_CHECK

/*  
    class with const pointer based on std::vector<std::string>, 
    increasing performance by just passing two iterators
    instead of using new std::vector<std::string>(start_it, end_it);
    without changing interfaces like begin(), end(), at()
*/
class const_itVecStr {
    public:

        const_itVecStr(std::vector<std::string>::const_iterator from_begin, std::vector<std::string>::const_iterator from_end);
        ~const_itVecStr();

        std::vector<std::string>::const_iterator begin() const;
        std::vector<std::string>::const_iterator end() const;
        std::string front() const;
        std::string back() const;
        std::string at(int pos) const;
        int size() const;

    private:
        std::vector<std::string>::const_iterator this_begin;
        std::vector<std::string>::const_iterator this_end;
};



namespace hjbase {

    namespace cnt {
        namespace keywords {
            /*
                Changing these "keywors" may result in unexpected behavior of the tokenizer
                To customize these "keywords", make sure to get tokenizer (mainly Get_One_Token) working correctly
            */
            const std::string BLOCK_START = "{";
            const char cBLOCK_START = '{';
            const std::string BLOCK_END = "}";
            const char cBLOCK_END = '}';
            const std::string EXPR_START = "(";
            const char cEXPR_START = '(';
            const std::string EXPR_END = ")";
            const char cEXPR_END = ')';
            const std::string SQUARE_BRACKET_START = "[";
            const char cSQUARE_BRACKET_START = '[';
            const std::string SQUARE_BRACKET_END = "]";
            const char cSQUARE_BRACKET_END = ']';
            const std::string DELIMITER = ";";
            const char cDELIMITER = ';';
            const std::string SEPARATOR = ",";
            const char cSEPARATOR = ',';
            const char QUOTATION_MARK = '"';
            const char NOSUBST = '@';
            const std::string SLICER = ":";
            const char cSLICER = ':';
        }

        namespace operators {

            const std::string OP_ADD = "+";
            const std::string OP_MINUS = "-";
            const std::string OP_MULTIPLY = "*";
            const std::string OP_DIVISION = "/";
            const std::string OP_MOD = "%%";
            const std::string OP_LE = "<=";
            const std::string OP_GE = ">=";
            const std::string OP_LT = "<";
            const std::string OP_GT = ">";

            enum {
                eOP_ADD,
                eOP_MINUS,
                eOP_MULTIPLY,
                eOP_DIVISION,
                eOP_MOD,
                eOP_LE,
                eOP_GE,
                eOP_LT,
                eOP_GT,
            };

            const std::map<std::string, int> NUMERICAL_OPERATORS = {
                { OP_ADD, eOP_ADD },
                { OP_MINUS, eOP_MINUS },
                { OP_MULTIPLY, eOP_MULTIPLY },
                { OP_DIVISION, eOP_DIVISION },
                { OP_MOD, eOP_MOD },
                { OP_LE, eOP_LE },
                { OP_GE, eOP_GE },
                { OP_LT, eOP_LT },
                { OP_GT, eOP_GT },
            };

            const std::string OP_AND = "&";
            const std::string OP_OR = "|";
            const std::string OP_NOT = "!";
            const std::string OP_NE = "!=";
            const std::string OP_EQ = "=";
            const std::string OP_STRAPP = "strapp";
            const std::string OP_SLICE = "slice";

            enum {
                eOP_AND,
                eOP_OR,
                eOP_NOT,
                eOP_NE,
                eOP_EQ,
                eOP_STRAPP,
                eOP_SLICE,
            };

            const std::map<std::string, int> OTHER_BASIC_OPERATORS = {
                { OP_AND, eOP_AND },
                { OP_OR, eOP_OR },
                { OP_NOT, eOP_NOT },
                { OP_NE, eOP_NE },
                { OP_EQ, eOP_EQ },
                { OP_STRAPP, eOP_STRAPP },
                { OP_SLICE, eOP_SLICE },
            };
        }
        namespace commands_names {
            const std::string CMD_NAME_DECLARE = "var";
            const std::string CMD_NAME_MUTATE = "mutate";
            const std::string CMD_NAME_MUTATE_TO = "to";
            const std::string CMD_PRINT = "print";
            const std::string CMD_IF = "if";
            const std::string CMD_WHILE = "while";

            const std::string CONFIG_FLOAT_POINT_ENABLE = "fp_on";
            const std::string CONFIG_FLOAT_POINT_DISABLE = "fp_off";
            const std::string CONFIG_DEBUG_MODE_ENABLE = "debug_on";
            const std::string CONFIG_DEBUG_MODE_DISABLE = "debug_off";
            const std::string CONFIG_RAW_STRING_ENABLE = "rawstr_on";
            const std::string CONFIG_RAW_STRING_DISABLE = "rawstr_off";

            const std::string DEB_PRINT_GLOBAL_NAMES = "print_global_names";

            enum {
                eCMD_NAME_DECLARE,
                eCMD_NAME_MUTATE,
                eCMD_PRINT,
                eCMD_IF,
                eCMD_WHILE,
                eCONFIG_FLOAT_POINT_ENABLE,
                eCONFIG_FLOAT_POINT_DISABLE,
                eCONFIG_DEBUG_MODE_ENABLE,
                eCONFIG_DEBUG_MODE_DISABLE,
                eCONFIG_RAW_STRING_ENABLE,
                eCONFIG_RAW_STRING_DISABLE,
                eDEB_PRINT_GLOBAL_NAMES,
            };

            const std::map<std::string, int> HJBASE_CMD_SEARCH_TREE = {
                { CMD_NAME_DECLARE, eCMD_NAME_DECLARE },
                { CMD_NAME_MUTATE, eCMD_NAME_MUTATE },
                { CMD_PRINT, eCMD_PRINT },
                { CMD_IF, eCMD_IF },
                { CMD_WHILE, eCMD_WHILE },
                { CONFIG_FLOAT_POINT_ENABLE, eCONFIG_FLOAT_POINT_ENABLE },
                { CONFIG_FLOAT_POINT_DISABLE, eCONFIG_FLOAT_POINT_DISABLE },
                { CONFIG_DEBUG_MODE_ENABLE, eCONFIG_DEBUG_MODE_ENABLE },
                { CONFIG_DEBUG_MODE_DISABLE, eCONFIG_DEBUG_MODE_DISABLE },
                { CONFIG_RAW_STRING_ENABLE, eCONFIG_RAW_STRING_ENABLE },
                { CONFIG_RAW_STRING_DISABLE, eCONFIG_RAW_STRING_DISABLE },
                { DEB_PRINT_GLOBAL_NAMES, eDEB_PRINT_GLOBAL_NAMES },
            };
        }
        namespace common_msgs {
            const std::string SE_UNEXPECTED_TOKEN = "Syntax Error: Unexpceted token";
            const std::string SE_REQUIRE_MORE_TOKENS = "Syntax Error: Require more tokens";
            const std::string SE_MISSING_EXPR = "Syntax Error: Missing Expression";
            const std::string SE_UNABLE_TO_PROCESS_EXPR = "Syntax Error: Unable to process expression. Expression is empty or unknown operator";
            const std::string SE_ARITY_MISMATCH = "Syntax Error: Arity mismatch";

            const std::string IE_UNDEFINED_NAME = "Internal Error: Name undefined";
            const std::string IE_UNKNOWN = "Internal Error: Unknown error";
            const std::string IE_CONST_PVECSTR_OOR = "Internal Error: const_itVecStr out of range";

            const std::string NE_CONVERSION_FAILED = "Numerical Exception: Unable to convert to numerical value";
            const std::string NE_OUT_OF_RANGE = "Numerical Exception: Numerical value out of range";

            const std::string NAE_REDECLARE = "Name Error: Redeclaration of name";
            const std::string NAE_UNDEFINED = "Name Error: Resolving undefined name";

            const std::string AE_DVZ = "Arithemetic Exception: Divide by zero";

            const std::string DEB_COMMAND_START = "Executing command: ";
            const std::string DEB_COMMAND_END = "Command execution complete: ";
            const std::string DEB_EXPR_START = "Evaluating expression: ";
            const std::string DEB_EXPR_END = "Expression Evaluation complete: ";
        }
        namespace miscellaneous {
            const std::string NAME_THEADER = "Name";
            const std::string VAL_THEADER = "Val";
            const std::string HORIZONTAL_LINE = "--------------------";
            const std::string VERTICAL_LINE_WITH_SPACE_AFTER = "|  ";
            const std::string VERTICAL_LINE_WITH_SPACE_BOTH = "  |  ";
            const std::string VERTICAL_LINE_WITH_SPACE_BEFORE = "  |";

            const std::string BOOL_TRUE = "1";
            const std::string BOOL_FALSE = "0";
            const std::string INITIAL_VALUE = "NONE";

            const std::string CONSOLE_INFO_STRING = "Huaji Script Interpreter v1.0";
            const std::string COMMAND_LINE_PROMPT = "OvO >>> ";

            const std::string NONEWLINE = "--nonl";
            const std::string USE_EXPR_FROM_HERE = "--expr";
        }
        namespace type_tag {
            const std::string STRING_TAG = "#:string";
        }
    }

    namespace ufunc {

        void Signal_Error(const std::string& error_msg, const const_itVecStr* error_part);

        void Signal_Error(const std::string& error_msg, const std::string& error_part);

        void Print_IVSTR(const const_itVecStr* iVstr) ;

        void Highlight_Error_Part(const const_itVecStr* iVstr) ;

        bool Check_If_Float_Point(const const_itVecStr* vals);

        bool Starts_With(const std::string& this_str, const std::string& start_str);

        bool Is_Numerical(const std::string& this_str);

    }

    namespace except {

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
                    - free names memory
                    - free tokenizer memory
            */
            ~HUAJISCRIPTBASE();

            void Entry_Point();

        protected:

            virtual int More_On_Command_Level_1(const const_itVecStr* command_to_be_exectued);

            virtual std::string More_On_Expression_Level_1(const std::string& op, const const_itVecStr* vals);

            virtual std::string More_On_Names_Query_Level_1(const std::string& name);

            virtual std::string More_On_Slice_Operator_Level_1(const const_itVecStr* vals);

            virtual void More_Cleanup_Level_1();
            
            std::string Evaluate_Expression(const_itVecStr* p_vec_str);

            bool Find_And_Evaluate_Condition(const const_itVecStr* commands_block);

            void Block_Execution(const const_itVecStr* commands_block);

            void Declare_Name(const std::string& name, const std::string& val, std::map<std::string, std::string>* target_scope_names);

            void Mutate_Name(const std::string& name, const std::string& val, std::map<std::string, std::string>* target_scope_names);

            std::string Resolve_Name(const std::string& name, const std::map<std::string, std::string>* target_scope_names);

            void Cleanup_If_Exception_Thrown();
        
        private:

            template <typename T>
            std::string Numerical_Operation_Templated_Helper(const std::string& op, int op_key, int vals_size, int T_name, const const_itVecStr* vals);

            std::string Numerical_Operation(const std::string& op, int op_key, const const_itVecStr* vals);

            std::string Handle_Val(const std::string& name_or_val);

            void Print_Name_Map(const std::map<std::string, std::string>* names_map);

            std::string Other_Basic_Operation(const std::string& op, int op_key, const const_itVecStr* vals);
            
            std::string Basic_Operation(const std::string& op, const const_itVecStr* vals);

            void Print_Debug_Info(const std::string& info, int ast_depth_change, const const_itVecStr* node);

            void Print_Debug_Info(const std::string& info, int ast_depth_change, const std::string& node);

            std::string Indent_By_AST_Depth();

            int Collect_Tokens(const std::string& token);
            
            int Take_One_Token(std::vector<std::string>::const_iterator token_it);

            int Huaji_Command_Interpreter(const const_itVecStr* command_to_be_exectued);

            std::map<std::string, std::string>* names;

            class HUAJITOKENIZER* tokenizer;

            /*
                collect_status is used by Collect_Tokens to identify if a command is complete
                collect_length is used by Take_One_Token to understand how many tokens it had accepted.
            */
            int collect_status, collect_length, current_ast_depth;

            bool enable_float_point, enable_debug_mode, enable_raw_string, is_console;
    };
}