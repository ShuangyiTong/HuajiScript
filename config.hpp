#pragma once


// language base perference settings
namespace hjbase_cnt {
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

#define INTERNAL_ERROR_RETURN_CODE 1
#define SUCCESSFULLY_EXECUTED_RETURN_CODE 0
#define REQUIRE_MORE_TOKENS_RETURN_CODE -1

#define REQUIRE_MORE_TOKENS_FOR_COMPLETE_COMMAND_RETURN_CODE 0
#define COMMAND_COMPLETED_RETURN_CODE 1

#define VALID_COMMAND_RETURN_CODE 1
#define INVALID_COMMAND_RETURN_CODE 0

#define TYPE_DOUBLE 0
#define TYPE_LONG 1

#define INCREASE_AST_DEPTH 1
#define DECREASE_AST_DEPTH -1
#define AST_DEPTH_UNCHANGED 0