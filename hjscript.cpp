#include "hjscript.hpp"

using namespace huaji_script_base_constant::keywords;
using namespace huaji_script_base_constant::common_msgs;
using namespace huaji_script_base_exception;

HUAJISCRIPTBASE::HUAJISCRIPTBASE() 
    : collect_status (0)
    , enable_float_point (true)
    , enable_debug_mode (false)
    , current_ast_depth (0)
    , variables (NULL) {
}

HUAJISCRIPTBASE::~HUAJISCRIPTBASE() {
    for(struct VAR *temp=variables;temp;) {
        struct VAR *to_be_deleted = temp;
        temp=temp->next;
        delete to_be_deleted;
    }
}

void HUAJISCRIPTBASE::Cleanup_If_Exception_Thrown() {
    current_ast_depth = 0;
    More_Cleanup_Level_1();
    return;
}

void HUAJISCRIPTBASE::More_Cleanup_Level_1() {
    return;
}

void HUAJISCRIPTBASE::Print_Vector_String(const std::vector<std::string> *vector_string) {
    for(std::vector<std::string>::const_iterator it=vector_string->begin();it!=vector_string->end();++it) {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

void HUAJISCRIPTBASE::Highlight_Error_Part(const std::vector<std::string> *vector_string) {
    for(std::vector<std::string>::const_iterator it=vector_string->begin();it!=vector_string->end();++it) {
        for(int i=0;i<=(*it).size();++i) {
            std::cout<<HIGHLIGHT_NOTATION;
        }
    }
    std::cout<<std::endl;
}

void HUAJISCRIPTBASE::Signal_Error(std::string error_msg, const std::vector<std::string> *error_part) {
    std::cerr<<error_msg<<std::endl;
    std::cout<<"    ";
    Print_Vector_String(error_part);
    std::cout<<"    ";
    Highlight_Error_Part(error_part);
}

void HUAJISCRIPTBASE::Signal_Error(std::string error_msg, std::string error_part) {
    std::cerr<<error_msg<<std::endl;
    std::cout<<"    "<<error_part<<std::endl;
}

std::string HUAJISCRIPTBASE::Indent_By_AST_Depth() {
    if(current_ast_depth>=0) {
        std::string indent;
        for(int i=0;i<current_ast_depth;++i) {
            indent.append("|   ");
        }
        return indent;
    }
    else {
        Signal_Error(IE_UNKNOWN, "Negative AST Depth");
        current_ast_depth = 0;
    }
    return std::string();
}

void HUAJISCRIPTBASE::Print_Debug_Info(std::string info, int ast_depth_change, const std::vector<std::string> *node) {
    if(enable_debug_mode) {
        if(ast_depth_change<0) {
            current_ast_depth += ast_depth_change;
        }
        std::cout<<Indent_By_AST_Depth()<<info;
        Print_Vector_String(node);
        if(ast_depth_change>0) {
            current_ast_depth += ast_depth_change;
        }
    }
}

void HUAJISCRIPTBASE::Print_Debug_Info(std::string info, int ast_depth_change, const std::string &node) {
    if(enable_debug_mode) {
        if(ast_depth_change<0) {
            current_ast_depth += ast_depth_change;
        }
        std::cout<<Indent_By_AST_Depth()<<info<<node<<std::endl;
        if(ast_depth_change>0) {
            current_ast_depth += ast_depth_change;
        }
    }
}

int HUAJISCRIPTBASE::Collect_Tokens(std::string token) {

    // Increase collect_status by 1 as it is an left brace
    if(token==BLOCK_START) {
        ++collect_status;
        return REQUIRE_MORE_TOKENS_FOR_COMPLETE_COMMAND_RETURN_CODE;
    } 

    // Decrease collect_status as some previous left brace is closed
    if(token==BLOCK_END) {
        --collect_status;

        // No more left braces
        if(!collect_status) {
            return COMMAND_COMPLETED_RETURN_CODE;
        }

        // prevent if someone put extra close brace
        if(collect_status<0) {
            // reset collect_status
            collect_status = 0;
            Signal_Error(SE_UNEXPECTED_TOKEN, token);
        }

        // left braces still left
        else {
            return REQUIRE_MORE_TOKENS_FOR_COMPLETE_COMMAND_RETURN_CODE;
        }
    }

    // end of command keyword, check if it is inside some brace
    if(token==DELIMITER) {
        if(!collect_status) {
            return COMMAND_COMPLETED_RETURN_CODE;
        }
        else {
            return REQUIRE_MORE_TOKENS_FOR_COMPLETE_COMMAND_RETURN_CODE;
        }
    }

    // Require more tokens for tokens other than 3 above
    else {
        return REQUIRE_MORE_TOKENS_FOR_COMPLETE_COMMAND_RETURN_CODE;
    }
}

int HUAJISCRIPTBASE::Take_One_Token(std::string token, std::vector<std::string> *command) {

    command->push_back(token);

    int command_complete = Collect_Tokens(token);

    if(command_complete) {

        // Command complete, send for execution
        int execution_status = Huaji_Command_Interpreter(command);

        // Clear command vector string for later execution
        command->clear();

        // Return status
        if(!execution_status) {
            return INTERNAL_ERROR_RETURN_CODE;
        }
        else {
            return SUCCESSFULLY_EXECUTED_RETURN_CODE;
        }
    }
    else {
        return REQUIRE_MORE_TOKENS_RETURN_CODE;
    }
}

void HUAJISCRIPTBASE::Variable_Assignment(std::string var, std::string val) {

    for(struct VAR *temp=variables;temp;temp=temp->next) {

        // Variable already exists, update value
        if(temp->var==var) {
            temp->val = val;
            return;
        }
    }

    // Variable not found, insert to front
    struct VAR *new_var = new VAR;
    new_var->var = var;
    new_var->val = val;
    new_var->next = variables;
    variables = new_var;

    return;
}

std::string HUAJISCRIPTBASE::Variable_Query(std::string var) {
    
    for(struct VAR *temp=variables;temp;temp=temp->next) {
        if(temp->var==var) {
            return temp->val;
        }
    }

    // Variable not found, throw exception
    throw name_except;

}

void HUAJISCRIPTBASE::Block_Execution(const std::vector<std::string> *commands_block) {

    int start_exec_pos = 0;

    // Find execution position, where the first { appears
    for(std::vector<std::string>::const_iterator it=commands_block->begin()+1;it!=commands_block->end()-1;++it) {
        if(*it==BLOCK_START) {
            start_exec_pos = it-commands_block->begin();
            break;
        }
    }

    if(!start_exec_pos) {
        return;
    }
    
    /*
        This is very slow, but robust. 
        As I can't find any better way to do this, I decide to leave it for now.
    */
    std::vector<std::string> *command = new std::vector<std::string>;

    /*
        We already know currently collect_status = 0, 
        so use Take_One_Token here won't cause any problem.
        check for end block token is ignored, as we iterate to end()-1
    */
    for(std::vector<std::string>::const_iterator it=commands_block->begin()+start_exec_pos+1;it!=commands_block->end()-1;++it) {
        Take_One_Token(*it,command);
    }
    delete command;

    // force reset collect_status, it is possible to have positive collect status
    if (collect_status!=0) {
        Signal_Error(SE_REQUIRE_MORE_TOKENS, commands_block);
        collect_status = 0;
    }
}

bool HUAJISCRIPTBASE::Find_And_Evaluate_Condition(const std::vector<std::string> *commands_block) {        
    /* 
        Find first block start token, send it for evlauation
        Notice here we also don't check expression tokens,
        as we iterate from begin() + 2 (first one must be a command name)
    */
    std::vector<std::string> *expr=NULL;
    for(std::vector<std::string>::const_iterator it=commands_block->begin()+2;it!=commands_block->end()-1;++it) {
        if(*it==BLOCK_START) {
            expr = new std::vector<std::string>(commands_block->begin()+1, it);
            break;
        }
    }

    if(!expr) {
        Signal_Error(SE_MISSING_EXPR, commands_block);
        throw syntax_except;
    }
    
    // Handle expression
    std::string val;
    try {
        val = Evaluate_Expression(expr);
    }
    catch (const HUAJIBASE_EXCEPTION& hj_base_except){
        // cleanup
        delete expr;
        // throw again
        throw hj_base_except;
    }

    delete expr;

    if(val!="0") {
        return true;
    }

    return false;

}

int HUAJISCRIPTBASE::Huaji_Command_Interpreter(const std::vector<std::string> *command_to_be_executed) {

    Print_Debug_Info(DEB_COMMAND_START, INCREASE_AST_DEPTH, command_to_be_executed);

    std::string command = command_to_be_executed->front();

    // Variable assignment command
    if(command == CMD_VAR_ASSIGN) {

        if(command_to_be_executed->size() < 4) {
            Signal_Error(SE_REQUIRE_MORE_TOKENS, command_to_be_executed);
            Cleanup_If_Exception_Thrown();
            return VALID_COMMAND_RETURN_CODE;
        }

        std::string var = *(command_to_be_executed->begin()+1);

        std::vector<std::string> *expr = new std::vector<std::string>(command_to_be_executed->begin()+2, command_to_be_executed->end()-1);

        std::string val;
        try {
            val = Evaluate_Expression(expr);
        }
        catch (const HUAJIBASE_EXCEPTION& hj_base_except){
            delete expr;
            Cleanup_If_Exception_Thrown();
            return VALID_COMMAND_RETURN_CODE;
        }

        delete expr;

        Variable_Assignment(var, val);
    }

    // Print command 
    else if(command == CMD_PRINT) {
        for(std::vector<std::string>::const_iterator it=command_to_be_executed->begin()+1;it!=command_to_be_executed->end()-1;++it) {
            std::cout<<May_Need_Dereference(*it);    
        }
        std::cout<<std::endl;
    }

    // If command
    else if(command == CMD_IF) {

        bool condition = false;

        try {
            condition = Find_And_Evaluate_Condition(command_to_be_executed);
        }
        catch (const HUAJIBASE_EXCEPTION& hj_base_except){
            Cleanup_If_Exception_Thrown();
            return VALID_COMMAND_RETURN_CODE;
        }

        if(condition) {
            Block_Execution(command_to_be_executed);
        }
    }

    // While command
    else if(command == CMD_WHILE) {

        bool condition = false;

        try {
            condition = Find_And_Evaluate_Condition(command_to_be_executed);
        }
        catch (const HUAJIBASE_EXCEPTION& hj_base_except){
            Cleanup_If_Exception_Thrown();
            return VALID_COMMAND_RETURN_CODE;
        }

        while(condition) {
            Block_Execution(command_to_be_executed);
            try {
                condition = Find_And_Evaluate_Condition(command_to_be_executed);
            }
            catch (const HUAJIBASE_EXCEPTION& hj_base_except){
                Cleanup_If_Exception_Thrown();
                return VALID_COMMAND_RETURN_CODE;
            }
        }
    }

    else if(command == CONFIG_FLOAT_POINT_ENABLE) {
        enable_float_point = true;
    }

    else if(command == CONFIG_FLOAT_POINT_DISABLE) {
        enable_float_point = false;
    }

    else if(command == CONFIG_DEBUG_MODE_ENABLE) {
        enable_debug_mode = true;
        current_ast_depth = 0;
        std::cout<<"Debug Mode on..."<<std::endl<<std::endl;
        return VALID_COMMAND_RETURN_CODE;
    }

    else if(command == CONFIG_DEBUG_MODE_DISABLE) {
        enable_debug_mode = false;
        current_ast_depth = 0;
        std::cout<<"Debug Mode off..."<<std::endl<<std::endl;
        return VALID_COMMAND_RETURN_CODE;
    }

    else {
        return More_Command_Level_1(command_to_be_executed);
    }

    Print_Debug_Info(DEB_COMMAND_END, DECREASE_AST_DEPTH, command);
    return VALID_COMMAND_RETURN_CODE;
}

int HUAJISCRIPTBASE::More_Command_Level_1(const std::vector<std::string> *command_to_be_executed) {
    Signal_Error(IE_UNDEFINED_NAME, command_to_be_executed);
    return INVALID_COMMAND_RETURN_CODE;
}

std::string HUAJISCRIPTBASE::Evaluate_Expression(const std::vector<std::string> *expr) {

    Print_Debug_Info(DEB_EXPR_START, INCREASE_AST_DEPTH, expr);
    
    std::string ans_val;

    if(!expr) {
        Signal_Error(SE_UNABLE_TO_PROCESS_EXPR, expr);
        throw eval_except;
    }

    else if(expr->size()==1) {
        ans_val = May_Need_Dereference((*expr)[0]);
    }

    else {

        // Check if braces are presented
        if(expr->front()!=EXPR_START||expr->back()!=EXPR_END) {
            Signal_Error(SE_UNEXPECTED_TOKEN, expr);
            throw syntax_except;
        }

        /*
            After above checks, the expression must be expr = EXPR_START op expr ... EXPR_END
            Detect operator
        */
        std::string op = (*expr)[1];

        /* 
            Start to wrap arguments and record position in the following vectors.
            The positions are stored in ordered pairs, even though the we're using flat data structure - vector.
        */
        std::vector<int> pos;

        int num_of_args = 0;

        // Record if the symbol is inside braces
        int num_of_unclosed_braces = 0;

        /* 
            Loop all tokens inside the EXPR_START, note we have recorded operator symbol so we start at begin()+2
            end()-1 must be a EXPR_END. Though we don't check here, in Collect_Tokens it has been implicitly checked.
        */
        for(std::vector<std::string>::const_iterator it=expr->begin()+2;it!=expr->end()-1;++it) {

            // Handling nested expression
            if(*it==EXPR_START) {

                // If this is already at top level
                if(num_of_unclosed_braces==0) {

                    pos.push_back(it-expr->begin());
                }

                // Add unclosed braces
                ++num_of_unclosed_braces;
            }
            else if(*it==EXPR_END) {
                --num_of_unclosed_braces;

                if(num_of_unclosed_braces==0) {

                    /* 
                        Here we add 1 because we will use std::vector constructor
                        , which does not include end iterator
                    */
                    pos.push_back(it-expr->begin()+1);
                    ++num_of_args;
                }

                // Prevent too many right braces
                if(num_of_unclosed_braces<0) {
                    Signal_Error(SE_UNEXPECTED_TOKEN, expr);
                    throw syntax_except;
                }
            }
            // Handling val
            else {

                // At top level and is not EXPR_START or EXPR_END then it must be a val
                if(num_of_unclosed_braces==0) {
                    pos.push_back(it-expr->begin());
                    pos.push_back(it-expr->begin()+1);
                    ++num_of_args;
                }
            }
        }

        // Throw error if there are unclosed EXPR_START
        if(num_of_unclosed_braces!=0) {
            Signal_Error(SE_REQUIRE_MORE_TOKENS, expr);
            throw syntax_except;
        }

        /*
            As we don't have a parser, we have to create ast dynamically.
            Create a vector of strings to store evaluated expression.
        */
        std::vector<std::string> *vals = new std::vector<std::string>;
        std::vector<std::string> *arg = NULL;
        for(int i=0;i<pos.size();i+=2) {
            arg = new std::vector<std::string>(expr->begin()+pos[i], expr->begin()+pos[i+1]);
            try {
                vals->push_back(Evaluate_Expression(arg));
            }
            catch (const HUAJIBASE_EXCEPTION& hj_base_except) {
                delete arg;
                delete vals;
                throw hj_base_except;
            }
            delete arg;
        }

        // Evaluate Expression
        try {
            ans_val = Basic_Operation(op, vals);
        }
        catch (const HUAJIBASE_EXCEPTION& hj_base_except) {
            delete vals;
            throw hj_base_except;
        }
        delete vals;
    }
    Print_Debug_Info(DEB_EXPR_END, DECREASE_AST_DEPTH, ans_val);
    
    return ans_val;
}

bool HUAJISCRIPTBASE::Check_If_Float_Point(const std::vector<std::string> *vals) {
    // iterate over vals vector
    for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
        /* 
            just try to find dot to determine if it is a float number or not,
            note this is not enough to make sure it is a valid numerical value.
            But we will not check it here, as finally std::stod will throw exception if it is not.
        */
        std::size_t found = (*it).find_first_of('.');
        if(found!=std::string::npos) {
            // std::string::npos is returned if not found, here we know it founds '.'
            return true;
        }
    }
    // '.' not found
    return false;
}

template <typename T>
std::string HUAJISCRIPTBASE::Numerical_Operation_Templated_Helper(std::string op, int vals_size, int T_name, const std::vector<std::string> *vals) {
    // Initialize numerical value array
    T *numerical_vals = new T[vals_size];
    T numerical_ans_val = 0;
    std::string ans_val;
    for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
        try {
            if(T_name == TYPE_DOUBLE) {
                numerical_vals[it-vals->begin()] = std::stod(*it);
            }
            else if(T_name == TYPE_LONG) {
                numerical_vals[it-vals->begin()] = std::stol(*it);
            }
            else {
                Signal_Error(IE_UNDEFINED_NAME, std::to_string(T_name));
                throw eval_except;
            }
        }
        // Handle Conversion error
        catch (const std::invalid_argument& ia) {
            delete [] numerical_vals;
            Signal_Error(NE_CONVERSION_FAILED, ia.what());
            throw eval_except;
        }
        catch (const std::out_of_range& oor) {
            delete [] numerical_vals;
            Signal_Error(NE_OUT_OF_RANGE, oor.what());
            throw eval_except;
        }
    }

    // val initialization complete, perform native calculation
    try {
        if(op==OP_ADD) {
            for(int i=0;i<vals_size;++i) {
                numerical_ans_val += numerical_vals[i];
            }
        }
        else if(op==OP_MINUS) {
            numerical_ans_val = numerical_vals[0];
            for(int i=1;i<vals_size;++i) {
                numerical_ans_val -= numerical_vals[i];
            }
        }
        else if(op==OP_MULTIPLY) {
            for(int i=0;i<vals_size;++i) {
                numerical_ans_val *= numerical_vals[i];
            }
        }
        else if(op==OP_DIVISION) {
            numerical_ans_val = numerical_vals[0];
            for(int i=1;i<vals_size;++i) {
                if(numerical_vals[i]==0) {
                    Signal_Error(AE_DVZ, vals);
                    throw eval_except;
                }
                numerical_ans_val /= numerical_vals[i];
            }
        }
        else if(op==OP_MOD) {
            if(vals_size!=2) {
                Signal_Error(SE_ARITY_MISMATCH, vals);
                throw eval_except;
            }
            else {
                if(long(numerical_vals[1])==0) {
                    Signal_Error(AE_DVZ, vals);
                    throw eval_except;
                }
                /*
                    As this template also applys to double, we have to make compiler happy by add casting.
                    Although it sounds reasonable to signal error when performing modulo expression with 
                    floating point number, I still want to let it possible by converted to integer.
                */
                return std::to_string(long(numerical_vals[0])%long(numerical_vals[1]));
            }
        }
        else if(op==OP_LE) {
            for(int i=1;i<vals_size;++i) {
                // I feel this is clearer, although use '>' might save time
                if(!(numerical_vals[i-1]<=numerical_vals[i])){
                    return BOOL_FALSE;
                }
            }
            return BOOL_TRUE;
        }
        else if(op==OP_GE) {
            for(int i=1;i<vals_size;++i) {
                if(!(numerical_vals[i-1]>=numerical_vals[i])){
                    return BOOL_FALSE;
                }
            }
            return BOOL_TRUE;
        }
        else if(op==OP_LT) {
            for(int i=1;i<vals_size;++i) {
                if(!(numerical_vals[i-1]<numerical_vals[i])){
                    return BOOL_FALSE;
                }
            }
            return BOOL_TRUE;
        }
        else if(op==OP_GT) {
            for(int i=1;i<vals_size;++i) {
                if(!(numerical_vals[i-1]>numerical_vals[i])){
                    return BOOL_FALSE;
                }
            }
            return BOOL_TRUE;
        }
    }
    catch (const HUAJIBASE_EXCEPTION& hj_base_except) {
        delete [] numerical_vals;
        throw hj_base_except;
    }
    catch (...) {
        delete [] numerical_vals;
        Signal_Error(IE_UNKNOWN, vals);
        throw eval_except;
    }
    delete [] numerical_vals;

    // If the result is numerical, convert back to string, and catch all exceptions during conversion.
    try {
        ans_val = std::to_string(numerical_ans_val);
    }
    catch (...) {
        Signal_Error(NE_CONVERSION_FAILED, vals);
        throw huaji_except;
    }
    return ans_val;
}

// Explicit template instantiation
template std::string HUAJISCRIPTBASE::Numerical_Operation_Templated_Helper<long>(std::string, int, int, const std::vector<std::string> *vals);
template std::string HUAJISCRIPTBASE::Numerical_Operation_Templated_Helper<double>(std::string, int, int, const std::vector<std::string> *vals);

std::string HUAJISCRIPTBASE::Numerical_Operation(std::string op, const std::vector<std::string> *vals) {
    int vals_size = vals->size();
    // we allow vals to be empty in some cases, but none of numerical operation can have no vals
    if(!vals_size) {
        Signal_Error(SE_ARITY_MISMATCH, op);
        throw syntax_except;
    }
    std::string ans_val;
    /*
        Determine to use float point calculation or integer, this process is slow,
        so we decide to add an option for users to disable float point
    */
    if(enable_float_point) {
        // Check if a dot in all val.
        if(Check_If_Float_Point(vals)) {
            /* 
                Now we have an array of numerical oprands, apply templated calculation function.
                As it is impossible for compiler to deduce which version of template to use,
                we need to explicitly specify template type
            */
            ans_val = Numerical_Operation_Templated_Helper<double>(op, vals_size, TYPE_DOUBLE, vals);
            return ans_val;
        }
    }
    // explicitly specify using long template
    ans_val = Numerical_Operation_Templated_Helper<long>(op, vals_size, TYPE_LONG, vals);
    return ans_val;
}

std::string HUAJISCRIPTBASE::Other_Basic_Operation(std::string op, const std::vector<std::string> *vals) {
    if(!vals->size()) {
        Signal_Error(SE_ARITY_MISMATCH, op);
        throw syntax_except;
    }
    try {
        if(op==OP_AND) {
            for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
                if(*it==BOOL_FALSE) {
                    return BOOL_FALSE;
                }
            }
            return BOOL_TRUE;
        }        
        if(op==OP_OR) {
            for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
                if(*it!=BOOL_FALSE) {
                    return BOOL_TRUE;
                }
            }
            return BOOL_FALSE;
        }
        if(op==OP_NOT) {
            if(vals->size()!=1) {
                Signal_Error(SE_ARITY_MISMATCH, vals);
                throw eval_except;
            }
            if(vals->front()!=BOOL_FALSE) {
                return BOOL_TRUE;
            }
            return BOOL_FALSE;
        }
        if(op==OP_EQ) {
            for(std::vector<std::string>::const_iterator it=vals->begin()+1;it!=vals->end();++it) {
                if(*(it-1)!=*it) {
                    return BOOL_FALSE;
                }
            }
            return BOOL_TRUE;
        }
        if(op==OP_NE) {
            for(std::vector<std::string>::const_iterator it_first=vals->begin();it_first!=vals->end()-1;++it_first) {
                for(std::vector<std::string>::const_iterator it_second=vals->begin()+1;it_second!=vals->end();++it_second) {
                    if(*it_first==*it_second) {
                        return BOOL_FALSE;
                    }
                }
            }
            return BOOL_TRUE;
        }
    }
    catch (const HUAJIBASE_EXCEPTION& hj_base_except) {
        throw hj_base_except;
    }
    catch (...) {
        Signal_Error(IE_UNKNOWN, vals);
        throw eval_except;
    }
    // no match clause, must be a bug
    Signal_Error(IE_UNDEFINED_NAME, op);
    throw huaji_except;
}

std::string HUAJISCRIPTBASE::More_Expression_Level_1(std::string op, const std::vector<std::string> *vals) {
    Signal_Error(SE_UNABLE_TO_PROCESS_EXPR, op);
    throw eval_except;
}

std::string HUAJISCRIPTBASE::Basic_Operation(std::string op, const std::vector<std::string> *vals) {
    std::string ans_val;
    try {
        if (NUMERICAL_OPERATORS.find(op)!=std::string::npos) {
            /* 
                Check if operator is numerical operator, if is its, apply numerical operation.
                For strings can't be converted to numbers, throw eval_except;
            */
            ans_val = Numerical_Operation(op, vals);
        }
        // Other basic operations
        else if (OTHER_BASIC_OPERATORS.find(op)!=std::string::npos) {
            Other_Basic_Operation(op, vals);
        }
        else {
            More_Expression_Level_1(op, vals);
        }
    }
    catch (const HUAJIBASE_EXCEPTION& hj_base_except) {
        throw hj_base_except;
    }
    // catch all exceptions
    catch (...) {
        Signal_Error(IE_UNKNOWN, vals);
        throw huaji_except;
    }
    return ans_val;
}

std::string HUAJISCRIPTBASE::May_Need_Dereference(std::string to_be_dereferenced) {
    if(to_be_dereferenced.size()>1) {
        if(to_be_dereferenced.front()=='$'&&to_be_dereferenced.back()=='$') {
            std::string dereferenced = to_be_dereferenced.substr(1,to_be_dereferenced.size()-2);
            try {
                return Variable_Query(dereferenced);
            }
            catch (...) {                        
                std::cout<<"Variable "<<dereferenced<<" not found"<<std::endl;
                std::cout<<"         "<<std::string(dereferenced.size(),'~')<<std::endl;
                return std::string();
            }
        }

        // Spacial characters substitution
        if(to_be_dereferenced.front()=='\\'&&to_be_dereferenced.back()=='s') {
            return " ";
        }

        return to_be_dereferenced;
    }
    else {
        return to_be_dereferenced;
    }
}