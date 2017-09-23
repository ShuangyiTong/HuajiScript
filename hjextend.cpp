#include "hjextend.hpp"

using namespace hjbase::ufunc;
using namespace hjbase::cnt::common_msgs;

hjex::HJEXTEND::HJEXTEND()
    : hjbase::HUAJISCRIPTBASE() {
}

hjex::HJEXTEND::~HJEXTEND() {}

std::pair<int, bool> hjex::HJEXTEND::More_On_Command_Level_1(const const_itVecStr& command) {
    return More_On_Command_Level_2(command);
}

std::pair<int, bool> hjex::HJEXTEND::More_On_Command_Level_2(const const_itVecStr& command) {
    return std::pair<int, bool>(-1, false);
}

std::pair<int, bool> hjex::HJEXTEND::More_On_Builtin_Function_Search_Level_1(const std::string& op) {
    return More_On_Builtin_Function_Search_Level_2(op);
}

std::pair<int, bool> hjex::HJEXTEND::More_On_Builtin_Function_Search_Level_2(const std::string& op) {
    return std::pair<int, bool>(-1, false);
}

std::string hjex::HJEXTEND::More_On_Builtin_Function_Level_1(const std::string& op, int op_key, const const_itVecStr& vals) {
    return More_On_Builtin_Function_Level_2(op, op_key, vals);
}

std::string hjex::HJEXTEND::More_On_Builtin_Function_Level_2(const std::string& op, int op_key, const const_itVecStr& vals) {
    throw hjbase::except::huaji_except;
}

std::pair<std::string, bool> hjex::HJEXTEND::More_On_Names_Query_Level_1(const std::string& name) {
    return More_On_Names_Query_Level_2(name);
}

std::pair<std::string, bool> hjex::HJEXTEND::More_On_Names_Query_Level_2(const std::string& name) {
    return std::pair<std::string, bool>(std::string(), false);
}

std::pair<std::string, bool> hjex::HJEXTEND::More_On_Slice_Operator_Level_1(const const_itVecStr& vals) {
    return More_On_Slice_Operator_Level_2(vals);
}

std::pair<std::string, bool> hjex::HJEXTEND::More_On_Slice_Operator_Level_2(const const_itVecStr& vals) {
    return std::pair<std::string, bool>(std::string(), false);
}

void hjex::HJEXTEND::More_Cleanup_Level_1() {
    return More_Cleanup_Level_2();
}

void hjex::HJEXTEND::More_Cleanup_Level_2() {
    return;
}

void hjex::HJEXTEND::More_On_Config_Level_1(const std::string& item, const std::string& status) {
    return More_On_Config_Level_1(item, status);
}

void hjex::HJEXTEND::More_On_Config_Level_2(const std::string& item, const std::string& status) {
    Signal_Error(IE_UNDEFINED_NAME, item);
    return;
}

void hjex::HJEXTEND::More_On_Info_Level_1(const const_itVecStr& command) {
    return More_On_Info_Level_1(command);
}

void hjex::HJEXTEND::More_On_Info_Level_2(const const_itVecStr& command) {
    Signal_Error(IE_UNDEFINED_NAME, command.at(1));
    return;
}

std::pair<bool, int> hjex::HJEXTEND::More_On_Check_If_GC_Required_Level_1(const std::string&  ref_val) {
    return More_On_Check_If_GC_Required_Level_2(ref_val);
}

std::pair<bool, int> hjex::HJEXTEND::More_On_Check_If_GC_Required_Level_2(const std::string&  ref_val) {
    return std::pair<bool, int>(false, -1);
}

void hjex::HJEXTEND::More_On_Delete_Object_Level_1(const std::string& ref_val, int type_code) {
    return More_On_Delete_Object_Level_2(ref_val, type_code);
}

void hjex::HJEXTEND::More_On_Delete_Object_Level_2(const std::string& ref_val, int type_code) {
    Signal_Error(IE_NOT_DELETABLE, ref_val);
    return;
}