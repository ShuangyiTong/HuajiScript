#pragma once

#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <map>
#include <utility>
#include <queue>

/*  
    Comment this directive to improve performance
    Enable this will let itVecStr constructor check if iterator range is valid
*/
#define itVecStr_RANGE_SAFETY_CHECK

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
            const char NOSUBST = '&';
            const std::string SLICER = ":";
            const char cSLICER = ':';
            const std::string LIST_DIRECT = "`";
            const char cLIST_DIRECT = '`';
            const char cLIST_TO_EVALUATE = '\'';
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
            const std::string OP_STRLEN = "strlen";
            const std::string OP_SLICE = "slice";
            const std::string OP_LIST = "list";
            const std::string OP_FIRST = "first";
            const std::string OP_REST = "rest";
            const std::string OP_CONS = "cons";
            const std::string OP_APPEND = "append";
            const std::string OP_RESOLVE = "resolve";

            enum {
                eOP_AND,
                eOP_OR,
                eOP_NOT,
                eOP_NE,
                eOP_EQ,
                eOP_STRAPP,
                eOP_STRLEN,
                eOP_SLICE,
                eOP_LIST,
                eOP_FIRST,
                eOP_REST,
                eOP_CONS,
                eOP_APPEND,
                eOP_RESOLVE,
            };

            const std::map<std::string, int> OTHER_BASIC_OPERATORS = {
                { OP_AND, eOP_AND },
                { OP_OR, eOP_OR },
                { OP_NOT, eOP_NOT },
                { OP_NE, eOP_NE },
                { OP_EQ, eOP_EQ },
                { OP_STRAPP, eOP_STRAPP },
                { OP_STRLEN, eOP_STRLEN },
                { OP_SLICE, eOP_SLICE },
                { OP_LIST, eOP_LIST },
                { OP_FIRST, eOP_FIRST },
                { OP_REST, eOP_REST },
                { OP_CONS, eOP_CONS },
                { OP_APPEND, eOP_APPEND },
                { OP_RESOLVE, eOP_RESOLVE },
            };
        }
        namespace builtin_func {
            const std::string FUNC_CHECK_EXPECT = "check-expect";

            enum {
                eFUNC_CHECK_EXPECT,
            };

            const std::map<std::string, int> HJBASE_BUILTIN_FUNC = {
                { FUNC_CHECK_EXPECT, eFUNC_CHECK_EXPECT },
            };
        }
        namespace commands_names {
            const std::string CMD_NAME_DECLARE = "declare";
            const std::string CMD_NAME_MUTATE = "mutate";
            const std::string CMD_NAME_MUTATE_TO = "to";
            const std::string CMD_PRINT = "print";
            const std::string CMD_IF = "if";
            const std::string CMD_WHILE = "while";
            const std::string CMD_DEFINE = "define";
            const std::string CMD_RETURN = "return";

            const std::string CONFIG_FLOAT_POINT_ENABLE = "fp_on";
            const std::string CONFIG_FLOAT_POINT_DISABLE = "fp_off";
            const std::string CONFIG_DEBUG_MODE_ENABLE = "debug_on";
            const std::string CONFIG_DEBUG_MODE_DISABLE = "debug_off";
            const std::string CONFIG_RAW_STRING_ENABLE = "rawstr_on";
            const std::string CONFIG_RAW_STRING_DISABLE = "rawstr_off";
            const std::string DEB_PRINT_NAMES = "print_names";

            const std::string EVALUATE = "(";

            enum {
                eCMD_NAME_DECLARE,
                eCMD_NAME_MUTATE,
                eCMD_PRINT,
                eCMD_IF,
                eCMD_WHILE,
                eCMD_DEFINE,
                eCMD_RETURN,
                eCONFIG_FLOAT_POINT_ENABLE,
                eCONFIG_FLOAT_POINT_DISABLE,
                eCONFIG_DEBUG_MODE_ENABLE,
                eCONFIG_DEBUG_MODE_DISABLE,
                eCONFIG_RAW_STRING_ENABLE,
                eCONFIG_RAW_STRING_DISABLE,
                eDEB_PRINT_NAMES,
                eEVALUATE,
            };

            const std::map<std::string, int> HJBASE_CMD_SEARCH_TREE = {
                { CMD_NAME_DECLARE, eCMD_NAME_DECLARE },
                { CMD_NAME_MUTATE, eCMD_NAME_MUTATE },
                { CMD_PRINT, eCMD_PRINT },
                { CMD_IF, eCMD_IF },
                { CMD_WHILE, eCMD_WHILE },
                { CMD_DEFINE, eCMD_DEFINE },
                { CMD_RETURN, eCMD_RETURN },
                { CONFIG_FLOAT_POINT_ENABLE, eCONFIG_FLOAT_POINT_ENABLE },
                { CONFIG_FLOAT_POINT_DISABLE, eCONFIG_FLOAT_POINT_DISABLE },
                { CONFIG_DEBUG_MODE_ENABLE, eCONFIG_DEBUG_MODE_ENABLE },
                { CONFIG_DEBUG_MODE_DISABLE, eCONFIG_DEBUG_MODE_DISABLE },
                { CONFIG_RAW_STRING_ENABLE, eCONFIG_RAW_STRING_ENABLE },
                { CONFIG_RAW_STRING_DISABLE, eCONFIG_RAW_STRING_DISABLE },
                { DEB_PRINT_NAMES, eDEB_PRINT_NAMES },
                { EVALUATE, eEVALUATE },
            };
        }
        namespace common_msgs {
            const std::string SE_UNEXPECTED_TOKEN = "Syntax Error: Unexpceted token";
            const std::string SE_REQUIRE_MORE_TOKENS = "Syntax Error: Require more tokens";
            const std::string SE_MISSING_EXPR = "Syntax Error: Missing Expression";
            const std::string SE_UNABLE_TO_PROCESS_EXPR = "Syntax Error: Unable to process expression. Expression is empty or unknown operator";
            const std::string SE_ARITY_MISMATCH = "Syntax Error: Arity mismatch";
            const std::string SE_UNRECOGNISED_FLAG = "Syntax Error: Unrecognised flag";

            const std::string IE_UNDEFINED_NAME = "Internal Error: Name undefined";
            const std::string IE_UNKNOWN = "Internal Error: Unknown error, contact developers for more information";
            const std::string IE_CONST_ITVECSTR_OOR = "Internal Error: const_itVecStr out of range";
            const std::string IE_INSERTION_FAILURE = "Internal Error: Failed to insert name into targeted map, contact developers for more information";
            const std::string IE_INTERNAL_QUERY_FAILED = "Internal Error: Internal query failed";
            const std::string IE_ACCESSING_UNDEFINED_STACK_FRAME = "Internal Error: Accessing undefined stack frame";

            const std::string NE_CONVERSION_FAILED = "Numerical Exception: Unable to convert to numerical value";
            const std::string NE_OUT_OF_RANGE = "Numerical Exception: Numerical value out of range";

            const std::string NAE_REDECLARE = "Name Error: Redeclaration of name";
            const std::string NAE_UNDEFINED = "Name Error: Resolving undefined name";

            const std::string AE_DVZ = "Arithemetic Exception: Divide by zero";

            const std::string SLE_OUT_OF_RANGE = "Slice out of range";

            const std::string SEF_NOT_ALLOWED = "Side Effects Error: This function has side effects, please allow side effects to execute this function";
            const std::string SEF_REQUIRED = "Side Effects Error: Functions with side effects required here";

            const std::string TE_NOT_LIST = "Type Error: Not a list";
            const std::string TE_NOT_FUNC = "Type Error: Not a function";

            const std::string LE_EMPTY = "List Error: Unable to perform operation on empty list";
            const std::string LE_INCOMPLETE = "List Error: Incomplete list";

            const std::string DEB_MODE_ON = "Debug Mode On...";
            const std::string DEB_MODE_OFF = "Debug Mode Off...";
            const std::string DEB_COMMAND_START = "Executing command: ";
            const std::string DEB_COMMAND_END = "Command execution complete: ";
            const std::string DEB_EXPR_START = "Evaluating expression: ";
            const std::string DEB_EXPR_END = "Expression Evaluation complete: ";
            const std::string DEB_TEST_RES_HEAD = "Test on ";
            const std::string DEB_TEST_RES_PASS = "passed.";
            const std::string DEB_TEST_RES_FAIL = "failed.";
            const std::string DEB_TEST_RES_EXPECTED = "Expected - ";
            const std::string DEB_TEST_RES_OUTPUT = "Output - ";
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

            const std::string CONSOLE_INFO_STRING = "Huaji Script Interpreter v1.05";
            const std::string COMMAND_LINE_PROMPT = "OvO >>> ";

            const std::string UNDEFINED_NAME = "__undefined__";

            const std::string NONEWLINE = "--nonl";
            const std::string USE_EXPR_FROM_HERE = "--expr";
            const std::string OVERLOADING_FLAG = "--overload";
            const std::string STACK_FLAG = "--stack";
            const std::string NAMESPACE_FLAG = "--namespace";
        }
        namespace type_tag {
            const std::string STRING_TAG = "#:string";
            const std::string LIST_TAG = "#:list";
            const std::string FUNC_TAG = "#:func";
            const std::string UNDEFINED_TYPE = "#:undefined";
        }
    }

    namespace ufunc {

        void Signal_Error(const std::string& error_msg, const const_itVecStr& error_part);

        void Signal_Error(const std::string& error_msg, const std::string& error_part);

        void Print_IVSTR(const const_itVecStr& iVstr) ;

        void Highlight_Error_Part(const const_itVecStr& iVstr) ;

        bool Check_If_Float_Point(const const_itVecStr& vals);

        std::pair<int, int> Get_First_Element_Pos(const std::string& this_list);

        std::string Format_List_String(const std::string& original_lst);

        bool Starts_With(const std::string& this_str, const std::string& start_str);

        bool Is_Numerical(const std::string& this_str);

        bool Quotation_Rquired_For_List_Elem(const std::string& this_str);

        std::string Convert_Ptr_To_Str(void* ptr);

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

        class SIDE_EFFECTS_EXCEPTION : public HUAJIBASE_EXCEPTION {
            virtual const char* what() const noexcept {
                return "side effects not allowed";
            }
        };

        class TYPE_EXCEPTION : public HUAJIBASE_EXCEPTION {            
            virtual const char* what() const noexcept {
                return "type mismatch";
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
        const TYPE_EXCEPTION type_except;
        const EVALUATION_EXCEPTION eval_except;
        const SYNTAX_EXCEPTION syntax_except;
        const NAME_EXCEPTION name_except;
        const TOKEN_EXCEPTION token_except;
        const SIDE_EFFECTS_EXCEPTION side_effects_except;
    }

    class LISTFORMATTER {
        public:

            LISTFORMATTER();
            ~LISTFORMATTER();

            int Take_One_Char(char cur_char);

            std::string formatted_lst;

        private:
            int list_depth;
            bool prev_space, is_in_list_quotation;
    };

    class HUAJITOKENIZER {
        public:

            HUAJITOKENIZER(std::string& file_name);
            HUAJITOKENIZER();
            ~HUAJITOKENIZER();

            std::string Get_One_Token();
            bool enable_raw_string;
        
        private:
            
            void Constructor_Helper();
            std::istream *source;
            std::queue<std::string> *token_queue;
            int list_depth;
            bool is_cin, is_in_quotation, is_in_block_comment, is_in_line_comment,
            is_in_nosubst, is_in_square_bracket, is_in_list;
            class LISTFORMATTER* lst_formatter;
    };

    class FUNC {
        public:
            
            FUNC(bool with_side_effects, const const_itVecStr& body_iVstr, 
                 const std::map<std::string, std::string>* stack_map, const std::list<std::string>* arg_names);
            ~FUNC();

            std::map<std::string, std::string>* Get_Stack_Frame_Template_Copy_On_Heap() const;
            std::list<std::string> Get_Var_Names() const;
            const_itVecStr Get_Fbody();
            bool has_side_effects;

        private:
            const_itVecStr* fbody;
            const std::vector<std::string>* fbody_container;
            const std::list<std::string>* var_names;
            const std::map<std::string, std::string>* stack_frame_template;
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

            std::pair<int, bool> More_On_Command_Level_1(const const_itVecStr& command);

            std::pair<std::string, bool> More_On_Expression_Level_1(const std::string& op, const const_itVecStr& vals);

            std::pair<std::string, bool> More_On_Names_Query_Level_1(const std::string& name);

            std::pair<std::string, bool> More_On_Slice_Operator_Level_1(const const_itVecStr& vals);

            void More_Cleanup_Level_1();
            
            std::string Evaluate_Expression(const const_itVecStr& expr);
            
            std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
            Get_Expr_Position_Iterators(const const_itVecStr& iVstr);

            std::string Find_And_Evaluate_Expression(const const_itVecStr& iVstr);

            int Block_Execution(const const_itVecStr& commands_block);

            void Declare_Name(const std::string& name, const std::string& val);

            void Mutate_Name(const std::string& name, const std::string& val);

            std::string Resolve_Name(const std::string& name);

            void Cleanup_If_Exception_Thrown(int reverted_ast_depth=0);

            void Side_Effects_Guard(std::string func_name_with_side_effects=cnt::miscellaneous::UNDEFINED_NAME);
        
        private:
        
            std::vector<std::pair<std::string, std::string>> General_Name_Declaring_Syntax_Parser(const const_itVecStr& iVstr, bool from_func_call=false);

            void Push_Stack_Frame(std::map<std::string, std::string>* preset_stack_frame);

            void Pop_Stack_Frame();

            template <typename T>
            std::string Numerical_Operation_Templated_Helper(const std::string& op,
                                                             int op_key, int vals_size, int T_name, const const_itVecStr& vals);

            std::string Numerical_Operation(const std::string& op, int op_key, const const_itVecStr& vals);

            std::string Handle_Val(const std::string& name_or_val);

            void Print_Name_Map(const std::map<std::string, std::string>* names_map);

            std::string Other_Basic_Operation(const std::string& op, int op_key, const const_itVecStr& vals);
            
            std::string Apply_Function(const std::string& op, const const_itVecStr& vals);

            void Print_Debug_Info(const std::string& info, int ast_depth_change, const const_itVecStr& node);

            void Print_Debug_Info(const std::string& info, int ast_depth_change, const std::string& node);

            std::string Indent_By_AST_Depth();

            int Collect_Tokens(const std::string& token);
            
            int Take_One_Token(std::vector<std::string>::const_iterator token_it);

            int Huaji_Command_Interpreter(const const_itVecStr& command);

            std::vector<std::map<std::string, std::string>*>* names_stack;

            void Constructor_Helper();

            std::map<std::string, FUNC*>* func_pool;

            std::string temp_return_val;

            HUAJITOKENIZER* tokenizer;

            /*
                collect_status is used by Collect_Tokens to identify if a command is complete
                collect_length is used by Take_One_Token to understand how many tokens it had accepted.
            */
            int collect_status, collect_length, current_ast_depth;

            bool enable_float_point, enable_debug_mode, enable_raw_string, is_console, allow_side_effects;
    };
}