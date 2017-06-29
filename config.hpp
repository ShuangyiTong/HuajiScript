#pragma once


// language base perference settings
namespace huaji_script_base_constant {
    namespace keywords {
        const std::string HIGHLIGHT_NOTATION = "~";
        const std::string BLOCK_START = "{";
        const std::string BLOCK_END = "}";
        const std::string EXPR_START = "(";
        const std::string EXPR_END = ")";
        const std::string DELIMITER = ";";
        const std::string COMMENT = "#";

        const std::string CMD_VAR_ASSIGN = "set";
        const std::string CMD_PRINT = "print";
        const std::string CMD_IF = "if";
        const std::string CMD_WHILE = "while";
        const std::string CMD_EXEC = "exec";

        const std::string CONFIG_FLOAT_POINT_ENABLE = "fp_on";
        const std::string CONFIG_FLOAT_POINT_DISABLE = "fp_off";
        const std::string CONFIG_DEBUG_MODE_ENABLE = "debug_on";
        const std::string CONFIG_DEBUG_MODE_DISABLE = "debug_off";

        const std::string NUMERICAL_OPERATORS = "+ -  * / mod <= >= < >";
        const std::string OTHER_BASIC_OPERATORS = "& | ! != =";

        const std::string OP_ADD = "+";
        const std::string OP_MINUS = "-";
        const std::string OP_MULTIPLY = "*";
        const std::string OP_DIVISION = "/";
        const std::string OP_MOD = "mod";
        const std::string OP_LE = "<=";
        const std::string OP_GE = ">=";
        const std::string OP_LT = "<";
        const std::string OP_GT = ">";

        const std::string OP_AND = "&";
        const std::string OP_OR = "|";
        const std::string OP_NOT = "!";
        const std::string OP_NE = "!=";
        const std::string OP_EQ = "=";

        const std::string BOOL_TRUE = "1";
        const std::string BOOL_FALSE = "0";
    }
    namespace common_msgs {
        const std::string SE_UNEXPECTED_TOKEN = "Syntax Error: Unexpceted token";
        const std::string SE_REQUIRE_MORE_TOKENS = "Syntax Error: Require more tokens";
        const std::string SE_MISSING_EXPR = "Syntax Error: Missing Expression";
        const std::string SE_UNABLE_TO_PROCESS_EXPR = "Syntax Error: Unable to process expression. Expression is empty or unknown operator";
        const std::string SE_ARITY_MISMATCH = "Syntax Error: Arity mismatch";

        const std::string IE_UNDEFINED_NAME = "Internal Error: Name undefined";
        const std::string IE_UNKNOWN = "Internal Error: Unknown error";

        const std::string NE_CONVERSION_FAILED = "Numerical Exception: Unable to convert numerical value";
        const std::string NE_OUT_OF_RANGE = "Numerical Exception: Numerical value out of range";

        const std::string AE_DVZ = "Arithemetic Exception: Divide by zero";

        const std::string DEB_COMMAND_START = "Executing command: ";
        const std::string DEB_COMMAND_END = "Command execution complete: ";
        const std::string DEB_EXPR_START = "Evaluating expression: ";
        const std::string DEB_EXPR_END = "Expression Evaluation complete: ";
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