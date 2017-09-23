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
#include <set>
#include <utility>
#include <queue>

/*  
    Comment this directive to improve performance
    Enable this will let itVecStr constructor check if iterator range is valid
*/
#define itVecStr_RANGE_SAFETY_CHECK

/*
    Check example/gc_error.hjs to see what will happen if using this directive
*/
// #define SIGNAL_UNABLE_TO_DELETE

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

        /*
            Defines a strict less relation on Set S = 
                {string s \lVert \forall x\in S, \forall y\in S, Starts_With(x, y)==Starts_With(y, x)==false}
        */
        class Starts_With_Less_Predicate {
            public:
                bool operator() (const std::string& x, const std::string& y) const;
        };

        void Iterate_Over_Names_Map(const std::map<std::string, std::string>* names,
            const void (*f)(const std::string&, const std::string&));

    }

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
            const std::string ENV_START = "<";
            const char cENV_START = '<';
            const std::string ENV_END = ">";
            const char cENV_END = '>';
            const std::string DELIMITER = ";";
            const char cDELIMITER = ';';
            const std::string SEPARATOR = ",";
            const char cSEPARATOR = ',';
            const char QUOTATION_MARK = '"';
            const char NOSUBST = '#';
            const char STACK_SEPARATOR = '&';
            const char NAMESPACE_SEPARATOR = '@';
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
            const std::string OP_MOD = "mod";
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

            const std::string FUNC_AND = "&";
            const std::string FUNC_OR = "|";
            const std::string FUNC_NOT = "!";
            const std::string FUNC_NE = "!=";
            const std::string FUNC_EQ = "=";
            const std::string FUNC_STRAPP = "strapp";
            const std::string FUNC_STRLEN = "strlen";
            const std::string FUNC_SLICE = "slice";
            const std::string FUNC_LIST = "list";
            const std::string FUNC_FIRST = "first";
            const std::string FUNC_REST = "rest";
            const std::string FUNC_CONS = "cons";
            const std::string FUNC_APPEND = "append";
            const std::string FUNC_RESOLVE = "resolve";
            const std::string FUNC_LAMBDA = "lambda";
            const std::string FUNC_GETRET = "return-val";
            const std::string FUNC_STACK_POS = "stack-pos";
            const std::string FUNC_INIT = "init";
            const std::string FUNC_MEMVAR = "mem-ref";
            const std::string FUNC_LAZY = "lazy";
            const std::string FUNC_IS = "input";
            const std::string FUNC_OS = "output";
            const std::string FUNC_READ = "read";
            const std::string FUNC_WRITE = "write";
            const std::string FUNC_ARRAY_NEW = "array-new";
            const std::string FUNC_ARRAY_REF = "array-ref";
            const std::string FUNC_ARRAY_SIZE = "array-size";

            enum {
                eFUNC_AND,
                eFUNC_OR,
                eFUNC_NOT,
                eFUNC_NE,
                eFUNC_EQ,
                eFUNC_STRAPP,
                eFUNC_STRLEN,
                eFUNC_SLICE,
                eFUNC_LIST,
                eFUNC_FIRST,
                eFUNC_REST,
                eFUNC_CONS,
                eFUNC_APPEND,
                eFUNC_RESOLVE,
                eFUNC_GETRET,
                eFUNC_STACK_POS,
                eFUNC_INIT,
                eFUNC_MEMVAR,
                eFUNC_MEMFUNC,
                eFUNC_LAZY,
                eFUNC_IS,
                eFUNC_OS,
                eFUNC_READ,
                eFUNC_WRITE,
                eFUNC_ARRAY_NEW,
                eFUNC_ARRAY_REF,
                eFUNC_ARRAY_SIZE,
            };

            const std::map<std::string, int> BUILTIN_FUNCTIONS = {
                { FUNC_AND, eFUNC_AND },
                { FUNC_OR, eFUNC_OR },
                { FUNC_NOT, eFUNC_NOT },
                { FUNC_NE, eFUNC_NE },
                { FUNC_EQ, eFUNC_EQ },
                { FUNC_STRAPP, eFUNC_STRAPP },
                { FUNC_STRLEN, eFUNC_STRLEN },
                { FUNC_SLICE, eFUNC_SLICE },
                { FUNC_LIST, eFUNC_LIST },
                { FUNC_FIRST, eFUNC_FIRST },
                { FUNC_REST, eFUNC_REST },
                { FUNC_CONS, eFUNC_CONS },
                { FUNC_APPEND, eFUNC_APPEND },
                { FUNC_RESOLVE, eFUNC_RESOLVE },
                { FUNC_GETRET, eFUNC_GETRET },
                { FUNC_STACK_POS, eFUNC_STACK_POS },
                { FUNC_INIT, eFUNC_INIT },
                { FUNC_MEMVAR, eFUNC_MEMVAR },
                { FUNC_LAZY, eFUNC_LAZY },
                { FUNC_IS, eFUNC_IS },
                { FUNC_OS, eFUNC_OS },
                { FUNC_READ, eFUNC_READ },
                { FUNC_WRITE, eFUNC_WRITE },
                { FUNC_ARRAY_NEW, eFUNC_ARRAY_NEW },
                { FUNC_ARRAY_REF, eFUNC_ARRAY_REF },
                { FUNC_ARRAY_SIZE, eFUNC_ARRAY_SIZE },
            };
        }
        namespace commands_names {
            const std::string CMD_NAME_DECLARE = "declare";
            const std::string CMD_NAME_MUTATE = "mutate";
            const std::string CMD_NAME_MUTATE_TO = "to";
            const std::string CMD_PRINT = "print";
            const std::string CMD_IF = "if";
            const std::string CMD_ELSE = "else";
            const std::string CMD_ELIF = "elif";
            const std::string CMD_WHILE = "while";
            const std::string CMD_DEFINE = "define";
            const std::string CMD_INIT = "init";
            const std::string CMD_DELETE = "delete";
            const std::string CMD_RETURN = "return";
            const std::string CMD_SOURCE = "source";
            const std::string CMD_EXEC = "exec";
            const std::string CMD_ARRAY = "array";

            const std::string CMD_CONFIG = "config";
            const std::string CONFIG_DEBUG = "debug";
            const std::string CONFIG_FLOAT_POINT = "fp";
            const std::string CONFIG_RAW_STRING = "rawstr";
            const std::string CONFIG_GC = "gc";
            const std::string CONFIG_SIDE_EFFECTS = "side-effects";
            const std::string STATUS_ON = "on";
            
            const std::string CMD_INFO = "info";
            const std::string INFO_NAME = "name";
            const std::string INFO_FUNC_POOL = "func_pool";
            const std::string INFO_NAMESPACE_POOL = "namespace_pool";
            const std::string INFO_OBJECT_POOL = "object_pool";
            const std::string INFO_OBJECT_STACK = "object_stack";
            const std::string INFO_LAZY_POOL = "lazy_pool";
            const std::string INFO_ISTREAM_POOL = "istream_pool";
            const std::string INFO_OSTREAM_POOL = "ostream_pool";
            const std::string INFO_ARRAY_POOL = "array_pool";

            const std::string EVALUATE = "(";

            enum {
                eCMD_NAME_DECLARE,
                eCMD_NAME_MUTATE,
                eCMD_PRINT,
                eCMD_IF,
                eCMD_ELSE,
                eCMD_ELIF,
                eCMD_WHILE,
                eCMD_DEFINE,
                eCMD_INIT,
                eCMD_DELETE,
                eCMD_RETURN,
                eCMD_SOURCE,
                eCMD_EXEC,
                eCMD_ARRAY,
                eCMD_CONFIG,
                eCMD_INFO,
                eEVALUATE,
            };

            const std::map<std::string, int> HJBASE_CMD_SEARCH_TREE = {
                { CMD_NAME_DECLARE, eCMD_NAME_DECLARE },
                { CMD_NAME_MUTATE, eCMD_NAME_MUTATE },
                { CMD_PRINT, eCMD_PRINT },
                { CMD_IF, eCMD_IF },
                { CMD_ELSE, eCMD_ELSE },
                { CMD_ELIF, eCMD_ELIF },
                { CMD_WHILE, eCMD_WHILE },
                { CMD_DEFINE, eCMD_DEFINE },
                { CMD_INIT, eCMD_INIT },
                { CMD_DELETE, eCMD_DELETE },
                { CMD_RETURN, eCMD_RETURN },
                { CMD_SOURCE, eCMD_SOURCE },
                { CMD_ARRAY, eCMD_ARRAY },
                { CMD_EXEC, eCMD_EXEC },
                { CMD_CONFIG, eCMD_CONFIG },
                { CMD_INFO, eCMD_INFO },
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
            const std::string IE_NOT_DELETABLE = "Internal Error: Unable to delete";

            const std::string IOE_FILE_NOT_EXISTS = "IO Error: File not exist, or unable to open";
            const std::string IOE_IOSBASE_FAILURE = "IO Error: ios_base failure";
            const std::string IOE_ALREADY_IN_CIN = "IO Error: Cannot source console input when using console input";

            const std::string NE_CONVERSION_FAILED = "Numerical Exception: Unable to convert to numerical value";
            const std::string NE_OUT_OF_RANGE = "Numerical Exception: Numerical value out of range";

            const std::string NAE_REDECLARE = "Name Error: Redeclaration of name";
            const std::string NAE_UNDEFINED = "Name Error: Resolving undefined name";

            const std::string AE_DVZ = "Arithemetic Exception: Divide by zero";

            const std::string SLE_OUT_OF_RANGE = "Slice out of range";

            const std::string SEF_NOT_ALLOWED = "Side Effects Error: This function has side effects, please allow side effects to execute this function";
            const std::string SEF_REQUIRED = "Side Effects Error: Functions with side effects required here";

            const std::string GCE_DELETE_NOT_ALLOWED = "Garbage Collector Error: Cannot use delete command when garbage collector is enabled";

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
        namespace type_tag {
            const std::string STRING_TAG = "#:string";
            const int STRING_TAG_SIZE = 8;
            const std::string LIST_TAG = "#:list";
            const int LIST_TAG_SIZE = 6;
            const std::string FUNC_TAG = "#:func";
            const std::string OBJECT_TAG = "#:object";
            const std::string LAZY_TAG = "#:lazy";
            const std::string ISTREAM_TAG = "#:istream";
            const std::string OSTREAM_TAG = "#:ostream";
            const std::string ARRAY_TAG = "#:array";
            const std::string RBT_TAG = "#:RBt";
            const std::string EOF_TAG = "#:eof";
            const std::string UNDEFINED_TYPE = "#:undefined";
            const std::string FLAG_TAG = "--";

            enum {
                FUNC_TAG_CODE,
                OBJECT_TAG_CODE,
                LAZY_TAG_CODE,
                ISTREAM_TAG_CODE,
                OSTREAM_TAG_CODE,
                ARRAY_TAG_CODE,
            };

            /*
                We ensure the set EXTRA_DATA_STRUCTURE_REQUIRED_TYPE_TREE satisfies
                {string s \lVert \forall x\in S, \forall y\in S, Starts_With(x, y)==Starts_With(y, x)==false}
                Then, the relation Starts_With_Less_Predicate is a valid strict less relation on this set
            */
            const std::map<std::string, int, hjbase::ufunc::Starts_With_Less_Predicate>
                EXTRA_DATA_STRUCTURE_REQUIRED_TYPE_TREE = {
                { FUNC_TAG, FUNC_TAG_CODE },
                { OBJECT_TAG, OBJECT_TAG_CODE },
                { LAZY_TAG, LAZY_TAG_CODE },
                { ISTREAM_TAG, ISTREAM_TAG_CODE },
                { OSTREAM_TAG, OSTREAM_TAG_CODE },
                { ARRAY_TAG, ARRAY_TAG_CODE },
            };
        }
        namespace miscellaneous {
            const std::string NAME_THEADER = "Name";
            const std::string VAL_THEADER = "Val";
            const std::string HORIZONTAL_LINE = "--------------------";
            const std::string VERTICAL_LINE_WITH_SPACE_AFTER = "|  ";
            const std::string VERTICAL_LINE_WITH_SPACE_BOTH = "  |  ";
            const std::string VERTICAL_LINE_WITH_SPACE_BEFORE = "  |";

            const std::string EMPTY_STRING_OBJECT = std::string();

            const std::string BOOL_TRUE = "1";
            const std::string BOOL_FALSE = "0";

            const std::string CONSOLE_INFO_STRING = "Huaji Script Interpreter v1.062";
            const std::string COMMAND_LINE_PROMPT = "OvO >>> ";

            const std::string UNDEFINED_NAME = "__undefined__";
            const std::string TEMP_RET_VAL_NAME = "temp&ret";
            const std::string OBJECT_SCOPE_REF = "self";

            // Used in CMD_PRINT
            const std::string NONEWLINE = "--nonl";
            const std::string USE_EXPR_FROM_HERE = "--expr";
            // Used in CMD_DEFINE and FUNC_LAMBDA
            const std::string OVERLOADING_FLAG = "--overload";
            const std::string LAZY_FLAG = "--lazy";
            // Used in CMD_INFO
            const std::string STACK_FLAG = "--stack";
            const std::string NAMESPACE_FLAG = "--namespace";
            // Used in CMD_SOURCE
            const std::string FORCE_FLAG = "--force";
            const std::string CONSOLE_FLAG = "console";
            // Used in FUNC_OS
            const std::string APP_FLAG = "--append";
            // Used in FUNC_READ
            const std::string GETCHAR_FLAG = "--getchar";
            const std::string UNGETCHAR_FLAG = "--unget";
            const std::string GETLINE_FLAG = "--getline";
            const std::string TOEND_FLAG = "--to-end";
            // Used in CMD_ARRAY
            const std::string MUTATE_FLAG = "mutate";
            const std::string ERASE_FLAG = "erase";
            const std::string PUSH_BACK_FLAG = "push_back";
            const std::string POP_BACK_FLAG = "pop_back";
            const std::string INSERT_FLAG = "insert";
            
            const std::string CIN_PTRSTR = "#:consolein";
            const std::string COUT_PTRSTR = "#:consoleout";
        }
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

            HUAJITOKENIZER();
            ~HUAJITOKENIZER();

            std::string Get_One_Token();
            bool enable_raw_string;
            std::istream* source;
        
        private:
            std::queue<std::string>* token_queue;
            int list_depth;
            bool is_in_quotation, is_in_block_comment, is_in_line_comment,
            is_in_nosubst, is_in_square_bracket, is_in_list;
            class LISTFORMATTER* lst_formatter;
    };

    class FUNC {
        public:
            
            FUNC(bool with_side_effects, const const_itVecStr& body_iVstr, 
                 const std::map<std::string, std::string>* stack_map, const std::list<std::string>* arg_names, bool lazy=false);
            ~FUNC();

            std::map<std::string, std::string>* Get_Stack_Frame_Template_Copy_On_Heap() const;
            const std::map<std::string, std::string>* Get_Stack_Frame_Template() const;
            std::list<std::string> Get_Var_Names() const;
            const_itVecStr Get_Fbody();
            bool has_side_effects, is_lazy;

        private:
            const_itVecStr* fbody;
            const std::vector<std::string>* fbody_container;
            const std::list<std::string>* var_names;
            const std::map<std::string, std::string>* stack_frame_template;
    };


    class HUAJISCRIPTBASE {

        public:

            HUAJISCRIPTBASE();
            /*
                Destructor
                    - free names_stack memory
                    - free tokenizer memory
                    - free func_pool
            */
            ~HUAJISCRIPTBASE();

            void Entry_Point(std::string filename=cnt::miscellaneous::EMPTY_STRING_OBJECT);

        protected:

            virtual std::pair<int, bool> More_On_Command_Level_1(const const_itVecStr& command);

            virtual std::pair<int, bool> More_On_Builtin_Function_Search_Level_1(const std::string& op);

            virtual std::string More_On_Builtin_Function_Level_1(const std::string& op, int op_key, const const_itVecStr& vals);

            virtual std::pair<std::string, bool> More_On_Names_Query_Level_1(const std::string& name);

            virtual std::pair<std::string, bool> More_On_Slice_Operator_Level_1(const const_itVecStr& vals);

            virtual void More_Cleanup_Level_1();

            virtual void More_On_Config_Level_1(const std::string& item, const std::string& status);

            virtual void More_On_Info_Level_1(const const_itVecStr& command);

            virtual std::pair<bool, int> More_On_Check_If_GC_Required_Level_1(const std::string&  ref_val);

            virtual void More_On_Delete_Object_Level_1(const std::string& ref_val, int type_code);
            
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

            int Get_Stack_Frame_Pos(const std::string& req_str);

            std::vector<std::string>* Get_Array_Object(const std::string& arrat_ptrStr);
        
        private:

            int FUNC_Paring_Helper(std::map<std::string, std::string>* stack_map, std::list<std::string>* arg_names, const const_itVecStr& iVstr,
                                   std::vector<std::string>::const_iterator& func_def_begin, std::vector<std::string>::const_iterator& func_def_end);

            void Array_Processing_Helper(const const_itVecStr& iVstr);

            std::pair<std::string*, std::map<std::string, std::string>*> Resolve_Namespace(const std::string& name);
        
            std::vector<std::pair<std::string, std::string>> General_Name_Declaring_Syntax_Parser(const const_itVecStr& iVstr, bool from_func_call=false);

            void Push_Stack_Frame(std::map<std::string, std::string>* preset_stack_frame);

            void Pop_Stack_Frame();

            void Push_Object_Into_Stack(const std::string& object_ptrStr);

            void Pop_Object_From_Stack();

            template <typename T>
            std::string Numerical_Operation_Templated_Helper(const std::string& op,
                                                             int op_key, int vals_size, int T_name, const const_itVecStr& vals);

            std::string Numerical_Operation(const std::string& op, int op_key, const const_itVecStr& vals);

            std::string Handle_Val(const std::string& name_or_val);

            void Print_Name_Map(const std::map<std::string, std::string>* names_map);

            template <typename ptr>
            void Print_Internal_Pointer_Map(const typename std::map<std::string, ptr>* internal_map);

            std::string Builtin_Function(const std::string& op, int op_key, const const_itVecStr& vals);
            
            std::string Apply_Function(FUNC* func_ptr, const const_itVecStr& vals);

            void Print_Debug_Info(const std::string& info, int ast_depth_change, const const_itVecStr& node);

            void Print_Debug_Info(const std::string& info, int ast_depth_change, const std::string& node);

            std::string Indent_By_AST_Depth();

            int Collect_Tokens(const std::string& token);
            
            int Take_One_Token(std::vector<std::string>::const_iterator token_it);

            int Huaji_Command_Interpreter(const const_itVecStr& command);

            std::pair<bool, int> Check_If_GC_Required(const std::string& ref_val);
            
            void GC_New_Record(const std::string& name, const std::string& val);
                
            void GC_Delete_Record(const std::string& name, const std::string& val);

            void GC_New_MultiRecords(const std::map<std::string, std::string>* names);

            void GC_Delete_MultiRecords(const std::map<std::string, std::string>* names);

            void GC_Delete_MultiRecords(const std::vector<std::pair<std::string, std::string>>& names);
            
            void Delete_Func_Object(const std::string& func_ptrStr);

            void Delete_Object_Object(const std::string& object_ptrStr);

            void Delete_Lazy_Object(const std::string& lazy_ptrStr);

            void Delete_IS_Object(const std::string& is_ptrStr);

            void Delete_OS_Object(const std::string& os_ptrStr);

            void Delete_Array_Object(const std::string& array_ptrStr);

            void Delete_Internal_Object(const std::string& ref_val, int type_code=-1);

            /*
                RBt
                    key - val that needs to be deleted
                    value - Link List
                            - list of names point to the val

                Indeed, there is no need to use a link list to record names,
                a integer is enough for GC to do the job. But it will be difficult for
                debugging, and as we allow disable GC, users may misuse delete
                and cause wired bugs after turing GC back on. Provide name info might be useful.
            */
            std::map<std::string, std::list<std::string>>* gc_records;

            std::vector<std::map<std::string, std::string>*>* names_stack;

            std::vector<std::map<std::string, std::string>*>* object_stack;

            std::map<std::string, std::map<std::string, std::string>*>* namespace_pool;

            std::map<std::string, std::map<std::string, std::string>*>* object_pool;

            std::map<std::string, FUNC*>* func_pool;

            std::map<std::string, const_itVecStr*>* lazy_expr_pool;

            std::string temp_return_val;

            HUAJITOKENIZER* tokenizer;

            std::vector<bool>* is_console_stack;

            std::vector<std::istream*>* input_stream_stack;

            std::set<std::string>* sourced_history;

            std::map<std::string, std::istream*>* istream_pool;

            std::map<std::string, std::ostream*>* ostream_pool;

            std::map<std::string, std::vector<std::string>*>* array_pool;

            /*
                collect_status is used by Collect_Tokens to identify if a command is complete
                collect_length is used by Take_One_Token to understand how many tokens it had accepted.
            */
            int collect_status, collect_length, current_ast_depth;

            bool enable_float_point, enable_debug_mode, enable_raw_string, allow_side_effects, enable_gc, last_if_cond;
    };
}