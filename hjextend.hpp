/*
    This is a template file for customization of Huaji Script.
    Put all Level_1 functions designed to be overloaded in HUAJISCRIPTBASE under private
    Write templates of Level_2 functions designed to be overloaded by derived class under protected
*/
#include "hjscript.hpp"

namespace hjex {
    class HJEXTEND : public hjbase::HUAJISCRIPTBASE {
        public:        
            HJEXTEND();
            ~HJEXTEND();

        protected:
            std::pair<int, bool> More_On_Command_Level_2(const const_itVecStr& command);
            
            std::pair<int, bool> More_On_Builtin_Function_Search_Level_2(const std::string& op);

            std::string More_On_Builtin_Function_Level_2(const std::string& op, int op_key, const const_itVecStr& vals);

            std::pair<std::string, bool> More_On_Names_Query_Level_2(const std::string& name);

            std::pair<std::string, bool> More_On_Slice_Operator_Level_2(const const_itVecStr& vals);

            void More_Cleanup_Level_2();

            void More_On_Config_Level_2(const std::string& item, const std::string& status);

            void More_On_Info_Level_2(const const_itVecStr& command);

            std::pair<bool, int> More_On_Check_If_GC_Required_Level_2(const std::string&  ref_val);

            void More_On_Delete_Object_Level_2(const std::string& ref_val, int type_code);

        private:

            std::pair<int, bool> More_On_Command_Level_1(const const_itVecStr& command);
            
            std::pair<int, bool> More_On_Builtin_Function_Search_Level_1(const std::string& op);

            std::string More_On_Builtin_Function_Level_1(const std::string& op, int op_key, const const_itVecStr& vals);

            std::pair<std::string, bool> More_On_Names_Query_Level_1(const std::string& name);

            std::pair<std::string, bool> More_On_Slice_Operator_Level_1(const const_itVecStr& vals);

            void More_Cleanup_Level_1();

            void More_On_Config_Level_1(const std::string& item, const std::string& status);

            void More_On_Info_Level_1(const const_itVecStr& command);

            std::pair<bool, int> More_On_Check_If_GC_Required_Level_1(const std::string&  ref_val);

            void More_On_Delete_Object_Level_1(const std::string& ref_val, int type_code);
    };
}