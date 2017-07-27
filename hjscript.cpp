#include "hjscript.hpp"

#define INTERNAL_ERROR_RETURN_CODE 1
#define SUCCESSFULLY_EXECUTED_RETURN_CODE 0
#define REQUIRE_MORE_TOKENS_RETURN_CODE -1

#define REQUIRE_MORE_TOKENS_FOR_COMPLETE_COMMAND_RETURN_CODE 0
#define COMMAND_COMPLETED_RETURN_CODE 1

#define FORMATTING_COMPLETE 1
#define REQUIRE_MORE_CHAR_FOR_COMPLETE_LST 0

#define TYPE_DOUBLE 0
#define TYPE_LONG 1

#define INCREASE_AST_DEPTH 1
#define DECREASE_AST_DEPTH -1
#define AST_DEPTH_UNCHANGED 0

using namespace hjbase::cnt::keywords;
using namespace hjbase::cnt::operators;
using namespace hjbase::cnt::common_msgs;
using namespace hjbase::cnt::miscellaneous;
using namespace hjbase::cnt::commands_names;
using namespace hjbase::cnt::type_tag;
using namespace hjbase::ufunc;
using namespace hjbase::except;

void hjbase::ufunc::Print_IVSTR(const const_itVecStr* iVstr) {
    for(std::vector<std::string>::const_iterator it=iVstr->begin();it!=iVstr->end();++it) {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

void hjbase::ufunc::Highlight_Error_Part(const const_itVecStr* iVstr) {
    for(std::vector<std::string>::const_iterator it=iVstr->begin();it!=iVstr->end();++it) {
        std::cout<<std::string((*it).size(), '~')<<" ";
    }
    std::cout<<std::endl;
}

void hjbase::ufunc::Signal_Error(const std::string& error_msg, const const_itVecStr* error_part) {
    std::cerr<<error_msg<<std::endl;
    std::cout<<"    ";
    Print_IVSTR(error_part);
    std::cout<<"    ";
    Highlight_Error_Part(error_part);
}

void hjbase::ufunc::Signal_Error(const std::string& error_msg, const std::string& error_part) {
    std::cerr<<error_msg<<std::endl;
    std::cout<<"    "<<error_part<<std::endl;
}

bool hjbase::ufunc::Check_If_Float_Point(const const_itVecStr* vals) {
    // iterate over vals vector
    for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
        /* 
            just try to find a dot to determine if it is a float number or not,
            note this is not enough to make sure it is a valid numerical value.
            But we will not check it here.
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

std::pair<int, int> hjbase::ufunc::Get_First_Element_Pos(const std::string& this_list) {
    if(this_list.front()!=cEXPR_START||this_list.back()!=cEXPR_END) {
        Signal_Error(TE_NOT_LIST, this_list);
        throw eval_except;
    }
    // the previous implicitly checked this_list.size() must be at least 2
    if(this_list.size()==2) {
        Signal_Error(LE_EMPTY, this_list);
        throw eval_except;
    }
    bool is_quotation = false;
    bool is_sublist = false;
    // Get the first position which is not a space after cEXPR_START
    int start_pos = 1;
    for(;start_pos<this_list.size()-1;++start_pos) {
        if(this_list[start_pos]!=' ') {
            if(this_list[start_pos]==cEXPR_START) {
                is_sublist = true;
            }
            if(this_list[start_pos]==QUOTATION_MARK) {
                is_quotation = true;
            }
            break;
        }
    }
    for(std::string::const_iterator it=this_list.begin()+start_pos+1;it!=this_list.end();++it) {
        if(is_quotation) {
            if(*it==QUOTATION_MARK) {
                // include QUOTATION mark
                return std::pair<int, int>(start_pos, it-this_list.begin()-start_pos+1);
            }
        }
        else if(is_sublist) {
            if(*it==cEXPR_END) {
                // include cEXPR_START and cEXPR_END
                return std::pair<int, int>(start_pos, it-this_list.begin()-start_pos+1);
            }
        }
        else {
            if(*it==' ') {
                // exclude space
                return std::pair<int, int>(start_pos, it-this_list.begin()-start_pos);
            }
        }
    }
    if(is_quotation||is_sublist) {
        Signal_Error(LE_INCOMPLETE, this_list);
        throw eval_except;
    }
    // From previous condition, start_pos must be less or equal to this_list.size()-1
    if(start_pos==this_list.size()-1) {
        Signal_Error(LE_EMPTY, this_list);
    }
    return std::pair<int, int>(start_pos, this_list.size()-start_pos-1);
}

std::string hjbase::ufunc::Format_List_String(const std::string& original_lst) {                    
    // construct LISTFORMATTER object
    LISTFORMATTER lst_formatter;
    for(std::string::const_iterator it=original_lst.begin();it!=original_lst.end()-1;++it) {
        if(lst_formatter.Take_One_Char(*it)==FORMATTING_COMPLETE) {
            Signal_Error(TE_NOT_LIST, original_lst);
            throw eval_except;
        }
    }
    if(lst_formatter.Take_One_Char(original_lst.back())!=FORMATTING_COMPLETE) {
        Signal_Error(TE_NOT_LIST, original_lst);
        throw eval_except;
    }
    return lst_formatter.formatted_lst;
}

inline bool hjbase::ufunc::Starts_With(const std::string& this_str, const std::string& start_str) {
    if(this_str.substr(0,start_str.size())==start_str) {
        return true;
    }
    return false;
}

inline bool hjbase::ufunc::Is_Numerical(const std::string& this_str) {
    for(std::string::const_iterator it=this_str.begin();it!=this_str.end();++it) {
        if(!isdigit(*it)&&(*it)!='.'&&(*it)!='-') {
            return false;
        }
    }
    return true;
}

inline bool hjbase::ufunc::Quotation_Rquired_For_List_Elem(const std::string& this_str) {
    if(this_str.front()==cEXPR_START&&this_str.back()==cEXPR_END) {
        return false;
    }
    bool ret_flag = false;
    for(std::string::const_iterator it=this_str.begin();it!=this_str.end();++it) {
         if(*it==' ') {
            ret_flag = true;
            break;
        }
    }
    return ret_flag;
}

const_itVecStr::const_itVecStr(std::vector<std::string>::const_iterator from_begin, std::vector<std::string>::const_iterator from_end)
    : this_begin (from_begin)
    , this_end (from_end) {
    #ifdef itVecStr_RANGE_SAFETY_CHECK
        if(this_begin>this_end) {
            Signal_Error(IE_CONST_ITVECSTR_OOR, std::string());
            throw huaji_except;
        }
    #endif
}

const_itVecStr::~const_itVecStr() {}

std::vector<std::string>::const_iterator const_itVecStr::begin() const {
    return this_begin;
}

std::vector<std::string>::const_iterator const_itVecStr::end() const {
    return this_end;
}

std::string const_itVecStr::front() const {
    return *(this_begin);
}

std::string const_itVecStr::back() const {
    return *(this_end-1);
}

std::string const_itVecStr::at(int pos) const {
    return *(this_begin+pos);
}

int const_itVecStr::size() const {
    return  this_end-this_begin;
}

hjbase::LISTFORMATTER::LISTFORMATTER()
    // Not allowed to have space after EXPR_START
    : prev_space (true) 
    , list_depth (0) 
    , is_in_list_quotation (false) {
    formatted_lst = EXPR_START;
}

hjbase::LISTFORMATTER::~LISTFORMATTER() {}

int hjbase::LISTFORMATTER::Take_One_Char(char cur_char) {
    if(is_in_list_quotation) {
        formatted_lst.push_back(cur_char);
        if(cur_char==QUOTATION_MARK) {
            is_in_list_quotation = false;
            formatted_lst.push_back(' ');
            prev_space = true;
        }
    }
    else {
        switch (cur_char) {
            case cEXPR_END: {
                // Replace last char with cEXPR_END if last char is a space
                if(formatted_lst.back()==' ') {
                    formatted_lst[formatted_lst.size()-1] = cEXPR_END;
                }
                else {
                    formatted_lst.push_back(cEXPR_END);
                }
                // not in any other sublist
                if(!list_depth) {
                    return FORMATTING_COMPLETE;
                }
                // in other sublist, just decrease list_depth
                else {
                    --list_depth;
                }
                break;
            }
            case cEXPR_START: {
                formatted_lst.push_back(cEXPR_START);
                // Not allowed to have space after EXPR_START
                prev_space = true;
                ++list_depth;
                break;
            }
            case QUOTATION_MARK: {
                // add a space if no prev_space
                if(!prev_space) {
                    formatted_lst.push_back(' ');
                }
                formatted_lst.push_back(cur_char);
                is_in_list_quotation = true;
                break;
            }
            default: {
                if(cur_char==' ') {
                    if(!prev_space) {
                        prev_space = true;
                        formatted_lst.push_back(cur_char);
                    }
                }
                else {
                    formatted_lst.push_back(cur_char);
                }
            }
        }
        // Reset prev_space if cur_char is not a space
        if(cur_char!=' '&&cur_char!=cEXPR_START) {
            prev_space = false;
        }
    }
    return REQUIRE_MORE_CHAR_FOR_COMPLETE_LST;
}

void hjbase::HUAJITOKENIZER::Constructor_Helper() {
    is_in_quotation = false;
    is_in_block_comment = false;
    is_in_line_comment = false;
    is_in_square_bracket = false;
    is_in_nosubst = false;
    is_in_list = false;
    enable_raw_string = false;
    token_queue = new std::queue<std::string>;
    lst_formatter = nullptr;
}

hjbase::HUAJITOKENIZER::HUAJITOKENIZER(std::string file_name) 
    : is_cin (false) {
    source = new std::ifstream(file_name);
    Constructor_Helper();
}

hjbase::HUAJITOKENIZER::HUAJITOKENIZER() 
    : is_cin (true) {
    source = &(std::cin);
    Constructor_Helper();
}

hjbase::HUAJITOKENIZER::~HUAJITOKENIZER() {
    if(!is_cin) {
        delete source;
    }
    if(lst_formatter) {
        delete lst_formatter;
    }
    delete token_queue;
}

std::string hjbase::HUAJITOKENIZER::Get_One_Token() {
    std::string token;
    if(token_queue->size()) {
        token = token_queue->front();
        token_queue->pop();
        return token;
    }
    try {
        while(1) {
            char cur_char = source->get();
            if(cur_char==std::char_traits<char>::eof()) {
                // is_in_list store token in lst_formatter object, so token.size() won't work here.
                if(token.size()||is_in_list) {
                    Signal_Error(SE_REQUIRE_MORE_TOKENS, token);
                }
                throw token_except;
            }
            // already in quotation mark
            else if(is_in_quotation) {
                if(cur_char==QUOTATION_MARK) {
                    is_in_quotation = false;
                    if(enable_raw_string) {
                        return token;
                    }
                    else {
                        return token.insert(0, STRING_TAG);
                    }
                }
                // characters substitution
                else if(cur_char=='\\') {
                    char next_char = source->get();
                    switch (next_char) {
                        case 'n': {
                            token.push_back('\n');
                            break;
                        }
                        case 'r': {
                            token.push_back('\r');
                            break;
                        }
                        case 't': {
                            token.push_back('\t');
                            break;
                        }
                        default:
                            token.push_back(next_char);
                    }
                }
                else {
                    token.push_back(cur_char);
                }
            }
            else if(is_in_list) {
                if(!lst_formatter) {
                    lst_formatter = new LISTFORMATTER();
                }
                if(lst_formatter->Take_One_Char(cur_char)==FORMATTING_COMPLETE) {
                    std::string ret_lst = lst_formatter->formatted_lst;
                    delete lst_formatter;
                    // add a LIST_TAG
                    ret_lst.insert(0, LIST_TAG);
                    lst_formatter = nullptr;
                    is_in_list = false;
                    return ret_lst;
                }
            }
            // in no substitution block
            else if(is_in_nosubst) {
                if(cur_char==NOSUBST) {
                    is_in_nosubst = false;
                    return token;
                }
            }
            // already in block comment
            else if(is_in_block_comment) {
                if(cur_char=='*') {
                    if(source->get()=='/') {
                        is_in_block_comment = false;
                    }
                }
            }
            // inline comment, exit comment mode if a endline char found
            else if(is_in_line_comment) {
                if(cur_char=='\n') {
                    is_in_line_comment = false;
                }
            }
            else {
                char next_char = source->peek();
                switch (cur_char) {
                    case '/': {
                        if(next_char=='*') {
                            source->get();
                            is_in_block_comment = true;
                            if(token.size()) {
                                return token;
                            }
                        }
                        else if(next_char=='/') {
                            source->get();
                            is_in_line_comment = true;
                            if(token.size()) {
                                return token;
                            }
                        }
                        else {
                            token.push_back(cur_char);
                        }
                        break;
                    }
                    case '<': {
                        if(next_char=='=') {
                            token.push_back(cur_char);
                            token.push_back(source->get());
                        }
                        else {
                            token.push_back(cur_char);
                        }
                        break;
                    }
                    case '>': {
                        if(next_char=='=') {
                            token.push_back(cur_char);
                            token.push_back(source->get());
                        }
                        else {
                            token.push_back(cur_char);
                        }
                        break;
                    }
                    case '!': {
                        if(next_char=='=') {
                            token.push_back(cur_char);
                            token.push_back(source->get());
                        }
                        else {
                            token.push_back(cur_char);
                        }
                        break;
                    }
                    case '=': {
                        if(token.size()) {
                            token_queue->push(std::string(1, cur_char));
                            return token;
                        }
                        return "=";
                    }
                    case cLIST_TO_EVALUATE: {
                        if(next_char==cEXPR_START) {
                            source->get();
                            if(token.size()) {
                                token_queue->push(EXPR_START);
                                token_queue->push(OP_LIST);
                                return token;
                            }
                            else {
                                token_queue->push(OP_LIST);
                                return EXPR_START;
                            }
                        }
                        else {
                            token.push_back(cLIST_TO_EVALUATE);
                        }
                    }
                    case cLIST_DIRECT: {
                        if(next_char==cEXPR_START) {
                            source->get();
                            is_in_list = true;
                            if(token.size()) {
                                return token;
                            }
                        }
                        else {
                            return LIST_DIRECT;
                        }
                        break;
                    }
                    case cSLICER: {
                        if(is_in_square_bracket&&token.size()) {
                            return token;
                        }
                        else {
                            if(token.size()) {
                                token_queue->push(SLICER);
                                return token;
                            }
                            else {
                                return SLICER;
                            }
                        }
                    }
                    case QUOTATION_MARK: {
                        is_in_quotation = true;
                        if(token.size()) {
                            return token;
                        }
                        break;
                    }
                    case NOSUBST: {
                        is_in_nosubst = true;
                        if(token.size()) {
                            return token;
                        }
                        break;
                    }
                    case cSQUARE_BRACKET_START: {
                        if(token.size()) {
                            is_in_square_bracket = true;
                            token_queue->push(OP_SLICE);
                            token_queue->push(token);
                            return EXPR_START;
                        }
                        else {
                            return SQUARE_BRACKET_START;
                        }
                    }
                    case cSQUARE_BRACKET_END: {
                        if(token.size()) {
                            if(is_in_square_bracket) {
                                token_queue->push(EXPR_END);
                                is_in_square_bracket = false;
                            }
                            else {
                                token_queue->push(SQUARE_BRACKET_END);
                            }
                            return token;
                        }
                        else {
                            if(is_in_square_bracket) {
                                is_in_square_bracket = false;
                                return EXPR_END;
                            }
                            else {
                                return SQUARE_BRACKET_END;
                            }
                        }
                    }
                    case cBLOCK_START: {
                        if(token.size()) {
                            token_queue->push(std::string(1, cur_char));
                            return token;
                        }
                        return BLOCK_START;
                    }
                    case cBLOCK_END: {
                        if(token.size()) {
                            token_queue->push(std::string(1, cur_char));
                            return token;
                        }
                        return BLOCK_END;
                    }
                    case cEXPR_START: {
                        if(token.size()) {
                            token_queue->push(std::string(1, cur_char));
                            return token;
                        }
                        return EXPR_START;
                    }
                    case cEXPR_END: {
                        if(token.size()) {
                            token_queue->push(std::string(1, cur_char));
                            return token;
                        }
                        return EXPR_END;
                    }
                    case cSEPARATOR: {
                        if(token.size()) {
                            token_queue->push(std::string(1, cur_char));
                            return token;
                        }
                        return SEPARATOR;
                    }
                    case cDELIMITER: {
                        if(token.size()) {
                            token_queue->push(std::string(1, cur_char));
                            return token;
                        }
                        return DELIMITER;
                    }
                    default: {
                        if(std::isspace(cur_char)) {
                            if(token.size()) {
                                return token;
                            }
                            // ignore if token is empty
                        }
                        else {
                            token.push_back(cur_char);
                        }
                    }
                }
            }
        }
    }
    catch (const std::ios_base::failure &e) {
        std::cout<<e.what()<<std::endl;
        throw token_except;
    }
}

void hjbase::HUAJISCRIPTBASE::Constructor_Helper() {
    collect_status = 0;
    collect_length = 0;
    enable_float_point = true;
    enable_debug_mode = false;
    current_ast_depth = 0;
    enable_raw_string = false;
}

hjbase::HUAJISCRIPTBASE::HUAJISCRIPTBASE(std::string file_name) 
    : is_console (false) {
    names = new std::map<std::string, std::string>;
    tokenizer = new HUAJITOKENIZER(file_name);
    Constructor_Helper();
}

hjbase::HUAJISCRIPTBASE::HUAJISCRIPTBASE() 
    : is_console (true) {
    names = new std::map<std::string, std::string>;
    tokenizer = new HUAJITOKENIZER();
    Constructor_Helper();
}

hjbase::HUAJISCRIPTBASE::~HUAJISCRIPTBASE() {
    delete names;
    delete tokenizer;
}

void hjbase::HUAJISCRIPTBASE::Cleanup_If_Exception_Thrown() {
    current_ast_depth = AST_DEPTH_UNCHANGED;
    More_Cleanup_Level_1();
    return;
}

void hjbase::HUAJISCRIPTBASE::More_Cleanup_Level_1() {
    return;
}

std::string hjbase::HUAJISCRIPTBASE::Indent_By_AST_Depth() {
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

void hjbase::HUAJISCRIPTBASE::Print_Debug_Info(const std::string& info, int ast_depth_change, const const_itVecStr* node) {
    if(enable_debug_mode) {
        if(ast_depth_change<0) {
            current_ast_depth += ast_depth_change;
        }
        std::cout<<Indent_By_AST_Depth()<<info;
        Print_IVSTR(node);
        if(ast_depth_change>0) {
            current_ast_depth += ast_depth_change;
        }
    }
}

void hjbase::HUAJISCRIPTBASE::Print_Debug_Info(const std::string& info, int ast_depth_change, const std::string& node) {
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

void hjbase::HUAJISCRIPTBASE::Print_Name_Map(const std::map<std::string, std::string>* names_map) {
    // Table Head
    std::cout<<HORIZONTAL_LINE<<std::endl;
    std::cout<<VERTICAL_LINE_WITH_SPACE_AFTER<<NAME_THEADER<<VERTICAL_LINE_WITH_SPACE_BOTH<<VAL_THEADER<<VERTICAL_LINE_WITH_SPACE_BEFORE<<std::endl;
    std::cout<<HORIZONTAL_LINE<<std::endl;
    // Table body
    for(std::map<std::string, std::string>::const_iterator it=names_map->begin();it!=names_map->end();++it) {
        std::cout<<VERTICAL_LINE_WITH_SPACE_AFTER<<it->first<<VERTICAL_LINE_WITH_SPACE_BOTH<<it->second<<VERTICAL_LINE_WITH_SPACE_BEFORE<<std::endl;
        std::cout<<HORIZONTAL_LINE<<std::endl;
    }
}

int hjbase::HUAJISCRIPTBASE::Collect_Tokens(const std::string& token) {

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

int hjbase::HUAJISCRIPTBASE::Take_One_Token(std::vector<std::string>::const_iterator token_it) {

    ++collect_length;

    int command_complete = Collect_Tokens(*token_it);

    if(command_complete) {

        const_itVecStr* cmd = new const_itVecStr(token_it-collect_length+1, token_it+1);

        // Reset collect_length, and start_it will be set to new start as well.
        collect_length = 0;

        // Build const_itVecStr object for execution
        int execution_status = Huaji_Command_Interpreter(cmd);

        delete cmd;

        // Return status
        if(execution_status==INVALID_COMMAND_RETURN_CODE) {
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

void hjbase::HUAJISCRIPTBASE::Entry_Point() {
    std::vector<std::string>* command = new std::vector<std::string>;
    std::string token;
    if(is_console) {
        std::cout<<CONSOLE_INFO_STRING<<std::endl;
        std::cout<<COMMAND_LINE_PROMPT;
        while(1) {
            try {
                token = tokenizer->Get_One_Token();
                command->push_back(token);
            }
            catch (const TOKEN_EXCEPTION& token_except) {    
                if (collect_length!=0) {
                    Signal_Error(SE_REQUIRE_MORE_TOKENS, token);
                }
                delete command;
                return;
            }
            if(Take_One_Token(command->end()-1)!=REQUIRE_MORE_TOKENS_RETURN_CODE) {
                command->clear();
                std::cout<<COMMAND_LINE_PROMPT;
            }
        }
    }
    else {
        while(1) {
            try {
                token = tokenizer->Get_One_Token();
                command->push_back(token);
            }
            catch (const TOKEN_EXCEPTION& token_except) {                
                if (collect_length!=0) {
                    Signal_Error(SE_REQUIRE_MORE_TOKENS, token);
                }
                delete command;
                return;
            }
            if(Take_One_Token(command->end()-1)!=REQUIRE_MORE_TOKENS_RETURN_CODE) {
                command->clear();
            }
        }
    }
}

void hjbase::HUAJISCRIPTBASE::Declare_Name(const std::string& name, const std::string& val, std::map<std::string, std::string> *target_scope_names) {
    // insert return a std::pair, with second element specifying if successfully inserted.
    if((target_scope_names->insert(std::pair<std::string, std::string>(name, val))).second==false) {
        Signal_Error(NAE_REDECLARE, name);
        throw name_except;
    }
    return;
}

void hjbase::HUAJISCRIPTBASE::Mutate_Name(const std::string& name, const std::string& val, std::map<std::string, std::string> *target_scope_names) {
    // Get the iterator pointing to name
    std::map<std::string, std::string>::iterator res_it = target_scope_names->find(name);
    // Check if name is in the map, if not res_it will be std::map::end()
    if(res_it!=target_scope_names->end()) {
        res_it->second = val;
    }
    else {
        // Mutate a name that is not declared is forbidden
        Signal_Error(NAE_UNDEFINED, name);
        throw name_except;
    }
    return;
}

std::string hjbase::HUAJISCRIPTBASE::Resolve_Name(const std::string& name, const std::map<std::string, std::string> *target_scope_names) {
    std::string val;
    try {
        val = target_scope_names->at(name);
    }
    catch (const std::out_of_range& oor) {
        Signal_Error(NAE_UNDEFINED, name);
        throw name_except;
    }
    return val;
}

void hjbase::HUAJISCRIPTBASE::Block_Execution(const const_itVecStr* commands_block) {

    int start_exec_pos = 0;

    // Find execution position, where the first BLOCK_START appears
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
        We already know currently collect_status = collect_length = 0, 
        so use a nested Take_One_Token here won't cause problems.
        check for end block token is ignored, as we iterate to end()-1
    */
    for(std::vector<std::string>::const_iterator it=commands_block->begin()+start_exec_pos+1;it!=commands_block->end()-1;++it) {
        Take_One_Token(it);
    }

    // force reset collect_length, it is possible to have positive collect_length
    if (collect_length!=0) {
        Signal_Error(SE_REQUIRE_MORE_TOKENS, commands_block);
        collect_length = 0;
        collect_status = 0;
    }
}

bool hjbase::HUAJISCRIPTBASE::Find_And_Evaluate_Condition(const const_itVecStr* commands_block) {        
    /* 
        Find first block start token, send it for evlauation
        Notice here we also don't check expression tokens,
        as we iterate from begin() + 2 (first one must be a command name)
    */
    const_itVecStr* expr = nullptr;
    for(std::vector<std::string>::const_iterator it=commands_block->begin()+2;it!=commands_block->end()-1;++it) {
        if(*it==BLOCK_START) {
            expr = new const_itVecStr(commands_block->begin()+1, it);
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
    catch (const HUAJIBASE_EXCEPTION& huaji_except){
        // cleanup
        delete expr;
        // throw again
        throw huaji_except;
    }

    delete expr;

    if(val!=BOOL_FALSE) {
        return true;
    }

    return false;

}

int hjbase::HUAJISCRIPTBASE::Huaji_Command_Interpreter(const const_itVecStr* command) {

    Print_Debug_Info(DEB_COMMAND_START, INCREASE_AST_DEPTH, command);

    // Only a DELIMITER presented
    if(command->size()==1) {
        return VALID_COMMAND_RETURN_CODE;
    }

    std::string cmd_str = command->at(0);

    std::map<std::string, int>::const_iterator cmd_key_it = HJBASE_CMD_SEARCH_TREE.find(cmd_str);
    int cmd_key;
    if(cmd_key_it!=HJBASE_CMD_SEARCH_TREE.end()) {
        cmd_key = cmd_key_it->second;
    }
    else {
        return More_On_Command_Level_1(command);
    }

    switch (cmd_key) {
    // Variable declare
        case eCMD_NAME_DECLARE: {

            if(command->size() < 3) {
                Signal_Error(SE_REQUIRE_MORE_TOKENS, command);
                Cleanup_If_Exception_Thrown();
                return VALID_COMMAND_RETURN_CODE;
            }

            std::string name = command->at(1);
            int declared = 0;

            const_itVecStr* expr = nullptr;
            std::string val;

            try {
                for(std::vector<std::string>::const_iterator it=command->begin()+2;it!=command->end()-1;++it) {
                    // Assign VAL when declare
                    if(*it==OP_EQ) {
                        std::vector<std::string>::const_iterator expr_begin_it = it+1;
                        // right expr not provided
                        if(expr_begin_it==command->end()-1) {
                            Signal_Error(SE_UNEXPECTED_TOKEN, command);
                            throw syntax_except;
                        }
                        std::vector<std::string>::const_iterator expr_end_it = it+2;
                        for(it=expr_end_it;it!=command->end()-1;++it) {
                            // expr_end_it should be it + 1 as it is excluded
                            expr_end_it = it;
                            if(*it==SEPARATOR) {
                                break;
                            }
                        }
                        if(it==command->end()-1) {
                            /*
                                Here is means it is at same point at DELIMITER, 
                                so we need to assign expr_end_it = it (because this is not executed yet) first,
                                and more importantly decrease it by 1 as ++it in outter loop will bring it==command->end()
                                and cause undefined behaviour.
                            */
                            expr_end_it = it;
                            --it;
                        }
                        /* 
                            Create new ast node by allocate new const_itVecStr object
                            evaluate and free memory, if exception thrown in evaluation
                            expcetion will be caught and memory will be freed
                        */
                        expr = new const_itVecStr(expr_begin_it, expr_end_it);
                        val = Evaluate_Expression(expr);
                        delete expr;
                        expr = nullptr;

                        // Declare name in global name map names, and flag as declared.
                        Declare_Name(name, val, names);
                        declared = 1;
                    }
                    else if(*it==SEPARATOR) {
                        if(declared) {
                            Signal_Error(SE_UNEXPECTED_TOKEN, command);
                            throw syntax_except;
                        }
                        Declare_Name(name, INITIAL_VALUE, names);
                        declared = 1;
                    }
                    // must be a name
                    else {
                        if(!declared) {
                            Signal_Error(SE_UNEXPECTED_TOKEN, command);
                            throw syntax_except;
                        }
                        name = *it;
                        declared = 0;
                    }
                }

                // declared is 0 means still remaining name undeclared
                if(!declared) {
                    Declare_Name(name, INITIAL_VALUE, names);
                }
            }
            catch (const HUAJIBASE_EXCEPTION& huaji_except){
                if(expr) {
                    delete expr;
                }
                Cleanup_If_Exception_Thrown();
                return VALID_COMMAND_RETURN_CODE;
            }
            break;
        }

        case eCMD_NAME_MUTATE: {
            if(command->size() < 5) {
                Signal_Error(SE_REQUIRE_MORE_TOKENS, command);
                Cleanup_If_Exception_Thrown();
                return VALID_COMMAND_RETURN_CODE;
            }
            if(command->at(2)!=CMD_NAME_MUTATE_TO) {
                Signal_Error(SE_UNEXPECTED_TOKEN, command);
                Cleanup_If_Exception_Thrown();
                return VALID_COMMAND_RETURN_CODE;
            }
            std::string name = command->at(1);
            const_itVecStr* expr = new const_itVecStr(command->begin()+3, command->end()-1);
            std::string val;
            try {
                val = Evaluate_Expression(expr);
                Mutate_Name(name, val, names);
            }
            catch (const HUAJIBASE_EXCEPTION& huaji_except){
                delete expr;
                Cleanup_If_Exception_Thrown();
                return VALID_COMMAND_RETURN_CODE;
            }
            delete expr;
            break;
        }

        // Print command 
        case eCMD_PRINT: {
            bool nl_flag = true;
            for(std::vector<std::string>::const_iterator it=command->begin()+1;it!=command->end()-1;++it) {
                if((*it)==NONEWLINE&&nl_flag) {
                    nl_flag = false;
                }
                else if((*it)==USE_EXPR_FROM_HERE&&it!=command->end()-2) {
                    const_itVecStr* expr = new const_itVecStr(it+1,command->end()-1);
                    try {
                        std::cout<<Evaluate_Expression(expr);
                    }
                    catch (const HUAJIBASE_EXCEPTION& huaji_except) {
                        delete expr;
                        Cleanup_If_Exception_Thrown();
                        return VALID_COMMAND_RETURN_CODE;
                    }
                    delete expr;
                    break;
                }
                else {
                    try {
                        std::cout<<Handle_Val(*it);
                    }
                    catch (const HUAJIBASE_EXCEPTION& huaji_except) {                
                        Cleanup_If_Exception_Thrown();
                        return VALID_COMMAND_RETURN_CODE;
                    }
                }
            }
            if(nl_flag) {
                std::cout<<std::endl;
            }
            break;
        }

        // If command
        case eCMD_IF: {

            bool condition = false;

            try {
                condition = Find_And_Evaluate_Condition(command);
            }
            catch (const HUAJIBASE_EXCEPTION& huaji_except){
                Cleanup_If_Exception_Thrown();
                return VALID_COMMAND_RETURN_CODE;
            }

            if(condition) {
                Block_Execution(command);
            }
            break;
        }

        // While command
        case eCMD_WHILE: {

            bool condition = false;

            try {
                condition = Find_And_Evaluate_Condition(command);
            }
            catch (const HUAJIBASE_EXCEPTION& huaji_except){
                Cleanup_If_Exception_Thrown();
                return VALID_COMMAND_RETURN_CODE;
            }

            while(condition) {
                Block_Execution(command);
                try {
                    condition = Find_And_Evaluate_Condition(command);
                }
                catch (const HUAJIBASE_EXCEPTION& huaji_except){
                    Cleanup_If_Exception_Thrown();
                    return VALID_COMMAND_RETURN_CODE;
                }
            }
            break;
        }

        case eDEB_PRINT_GLOBAL_NAMES: {
            Print_Name_Map(names);
            break;
        }

        case eDEB_CHECK_EXPECT: {
            
        }

        case eCONFIG_FLOAT_POINT_ENABLE: {
            enable_float_point = true;
            break;
        }

        case eCONFIG_FLOAT_POINT_DISABLE: {
            enable_float_point = false;
            break;
        }

        case eCONFIG_RAW_STRING_ENABLE: {
            enable_raw_string = true;
            tokenizer->enable_raw_string = true;
            break;
        }

        case eCONFIG_RAW_STRING_DISABLE: {
            enable_raw_string = false;
            tokenizer->enable_raw_string = false;
            break;
        }

        case eCONFIG_DEBUG_MODE_ENABLE: {
            enable_debug_mode = true;
            current_ast_depth = 0;
            std::cout<<"Debug Mode on..."<<std::endl;
            return VALID_COMMAND_RETURN_CODE;
        }

        case eCONFIG_DEBUG_MODE_DISABLE: {
            enable_debug_mode = false;
            current_ast_depth = 0;
            std::cout<<"Debug Mode off..."<<std::endl;
            return VALID_COMMAND_RETURN_CODE;
        }

        case eEVALUATE: {
            const_itVecStr* expr = new const_itVecStr(command->begin()+1,command->end()-2);
            try {
                std::cout<<Evaluate_Expression(expr);
            }
            catch (const HUAJIBASE_EXCEPTION& huaji_except) {
                delete expr;
                Cleanup_If_Exception_Thrown();
                return VALID_COMMAND_RETURN_CODE;
            }
            delete expr;
            break;
        }

        default: {
            Signal_Error(IE_UNDEFINED_NAME, command);
            return INVALID_COMMAND_RETURN_CODE;
        }

    }

    Print_Debug_Info(DEB_COMMAND_END, DECREASE_AST_DEPTH, command);
    return VALID_COMMAND_RETURN_CODE;
}

int hjbase::HUAJISCRIPTBASE::More_On_Command_Level_1(const const_itVecStr* command) {
    Signal_Error(IE_UNDEFINED_NAME, command);
    return INVALID_COMMAND_RETURN_CODE;
}

std::string hjbase::HUAJISCRIPTBASE::Evaluate_Expression(const_itVecStr* expr) {

    Print_Debug_Info(DEB_EXPR_START, INCREASE_AST_DEPTH, expr);
    
    std::string ans_val;

    if(!expr) {
        Signal_Error(SE_UNABLE_TO_PROCESS_EXPR, expr);
        throw eval_except;
    }

    else if(expr->size()==1) {
        ans_val = Handle_Val(expr->at(0));
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
        std::string op = expr->at(1);

        /* 
            Start to wrap arguments and record position in the following vectors.
            The iterators are stored in ordered pairs, even though the we're using flat data structure - vector.
        */
        std::vector<std::vector<std::string>::const_iterator> pos_its;

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

                    pos_its.push_back(it);
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
                    pos_its.push_back(it+1);
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
                    pos_its.push_back(it);
                    pos_its.push_back(it+1);
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
            Create a std::vector<std::string>* vals_container to store evaluated expression first,
            later we will construct a const_itVecStr object based on this vals_container
            and pass it to other processing functions
        */
        std::vector<std::string>* vals_container = new std::vector<std::string>;
        for(int i=0;i<pos_its.size();i+=2) {
            const_itVecStr* arg = new const_itVecStr(pos_its.at(i), pos_its.at(i+1));
            try {
                vals_container->push_back(Evaluate_Expression(arg));
            }
            catch (const HUAJIBASE_EXCEPTION& huaji_except) {
                delete arg;
                delete vals_container;
                throw huaji_except;
            }
            delete arg;
        }

        // Construct const_itVecStr for evaluation
        const_itVecStr* vals = new const_itVecStr(vals_container->begin(), vals_container->end());

        // Evaluate Expression
        try {
            ans_val = Basic_Operation(op, vals);
        }
        catch (const HUAJIBASE_EXCEPTION& huaji_except) {
            delete vals;
            delete vals_container;
            throw huaji_except;
        }
        delete vals;
        delete vals_container;
    }
    Print_Debug_Info(DEB_EXPR_END, DECREASE_AST_DEPTH, ans_val);
    
    return ans_val;
}

template <typename T>
std::string hjbase::HUAJISCRIPTBASE::Numerical_Operation_Templated_Helper(const std::string& op, int op_key, int vals_size, int T_name, const const_itVecStr* vals) {
    // Initialize numerical value array
    T* numerical_vals = new T[vals_size];
    T numerical_ans_val = 0;

    bool direct_string_return = false;
    std::string direct_return_string;

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
                delete [] numerical_vals;
                Signal_Error(IE_UNDEFINED_NAME, std::to_string(T_name));
                throw eval_except;
            }
        }
        // Handle Conversion error
        catch (const std::invalid_argument& ia) {
            delete [] numerical_vals;
            Signal_Error(NE_CONVERSION_FAILED, *it);
            throw eval_except;
        }
        catch (const std::out_of_range& oor) {
            delete [] numerical_vals;
            Signal_Error(NE_OUT_OF_RANGE, *it);
            throw eval_except;
        }
    }

    try {
        switch (op_key) {
            case eOP_ADD: {
                for(int i=0;i<vals_size;++i) {
                    numerical_ans_val += numerical_vals[i];
                }
                break;
            }
            case eOP_MINUS: {
                numerical_ans_val = numerical_vals[0];
                for(int i=1;i<vals_size;++i) {
                    numerical_ans_val -= numerical_vals[i];
                }
                break;
            }
            case eOP_MULTIPLY: {
                numerical_ans_val = numerical_vals[0];
                for(int i=1;i<vals_size;++i) {
                    numerical_ans_val *= numerical_vals[i];
                }
                break;
            }
            case eOP_DIVISION: {
                numerical_ans_val = numerical_vals[0];
                for(int i=1;i<vals_size;++i) {
                    if(numerical_vals[i]==0) {
                        Signal_Error(AE_DVZ, vals);
                        throw eval_except;
                    }
                    numerical_ans_val /= numerical_vals[i];
                }
                break;
            }
            case eOP_MOD: {
                if(vals_size!=2) {
                    Signal_Error(SE_ARITY_MISMATCH, vals);
                    throw eval_except;
                }
                else {
                    if(static_cast<long>(numerical_vals[1])==0) {
                        Signal_Error(AE_DVZ, vals);
                        throw eval_except;
                    }
                    /*
                        As this template also applys to double, we have to make compiler happy by add casting.
                        Although it sounds reasonable to signal error when performing modulo expression with 
                        floating point number, I still want to let it possible by converted to integer.
                    */
                    return std::to_string(static_cast<long>(numerical_vals[0])%static_cast<long>(numerical_vals[1]));
                }
            }
            case eOP_LE: {
                direct_string_return = true;
                direct_return_string = BOOL_TRUE;
                for(int i=1;i<vals_size;++i) {
                    // I feel this is clearer, although use '>' might save time
                    if(!(numerical_vals[i-1]<=numerical_vals[i])){
                        direct_return_string = BOOL_FALSE;
                        break;
                    }
                }
                break;
            }
            case eOP_GE: {
                direct_string_return = true;
                direct_return_string = BOOL_TRUE;
                for(int i=1;i<vals_size;++i) {
                    if(!(numerical_vals[i-1]>=numerical_vals[i])){
                        direct_return_string = BOOL_FALSE;
                        break;
                    }
                }
                break;
            }
            case eOP_LT: {
                direct_string_return = true;
                direct_return_string = BOOL_TRUE;
                for(int i=1;i<vals_size;++i) {
                    if(!(numerical_vals[i-1]<numerical_vals[i])){
                        direct_return_string = BOOL_FALSE;
                        break;
                    }
                }
                break;
            }
            case eOP_GT: {
                direct_string_return = true;
                direct_return_string = BOOL_TRUE;
                for(int i=1;i<vals_size;++i) {
                    if(!(numerical_vals[i-1]>numerical_vals[i])){
                        direct_return_string = BOOL_FALSE;
                        break;
                    }
                }
                break;
            }
            default: {
                Signal_Error(IE_UNKNOWN, op);
                throw huaji_except;
            }
        }
    }
    catch (const HUAJIBASE_EXCEPTION& huaji_except) {
        delete [] numerical_vals;
        throw huaji_except;
    }
    catch (...) {
        delete [] numerical_vals;
        Signal_Error(IE_UNKNOWN, vals);
        throw eval_except;
    }
    delete [] numerical_vals;

    if(direct_string_return) {
        return direct_return_string;
    }

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
template std::string hjbase::HUAJISCRIPTBASE::Numerical_Operation_Templated_Helper<long>(const std::string&, int, int, int, const const_itVecStr*);
template std::string hjbase::HUAJISCRIPTBASE::Numerical_Operation_Templated_Helper<double>(const std::string&, int, int, int, const const_itVecStr*);

std::string hjbase::HUAJISCRIPTBASE::Numerical_Operation(const std::string& op, int op_key, const const_itVecStr* vals) {
    int vals_size = vals->size();
    // we allow vals to be empty in some cases, but none of numerical operation can have no vals
    if(!vals_size) {
        Signal_Error(SE_ARITY_MISMATCH, op);
        throw syntax_except;
    }
    std::string ans_val;
    /*
        Determine to use float point calculation or integer, this process might be slow,
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
            ans_val = Numerical_Operation_Templated_Helper<double>(op, op_key, vals_size, TYPE_DOUBLE, vals);
            return ans_val;
        }
    }
    // explicitly specify using long template
    ans_val = Numerical_Operation_Templated_Helper<long>(op, op_key, vals_size, TYPE_LONG, vals);
    return ans_val;
}

std::string hjbase::HUAJISCRIPTBASE::More_On_Slice_Operator_Level_1(const const_itVecStr* vals) {
    throw fnld_except;
}

std::string hjbase::HUAJISCRIPTBASE::Other_Basic_Operation(const std::string& op, int op_key, const const_itVecStr* vals) {
    if(!vals->size()) {
        Signal_Error(SE_ARITY_MISMATCH, op);
        throw syntax_except;
    }
    try {
        switch (op_key) {
            case eOP_AND: {
                for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
                    if(*it==BOOL_FALSE) {
                        return BOOL_FALSE;
                    }
                }
                return BOOL_TRUE;
            }        
            case eOP_OR: {
                for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
                    if(*it!=BOOL_FALSE) {
                        return BOOL_TRUE;
                    }
                }
                return BOOL_FALSE;
            }
            case eOP_NOT: {
                if(vals->size()!=1) {
                    Signal_Error(SE_ARITY_MISMATCH, vals);
                    throw eval_except;
                }
                if(vals->front()!=BOOL_FALSE) {
                    return BOOL_TRUE;
                }
                return BOOL_FALSE;
            }
            case eOP_EQ: {
                for(std::vector<std::string>::const_iterator it=vals->begin()+1;it!=vals->end();++it) {
                    if(*(it-1)!=*it) {
                        return BOOL_FALSE;
                    }
                }
                return BOOL_TRUE;
            }
            case eOP_NE: {
                for(std::vector<std::string>::const_iterator it_first=vals->begin();it_first!=vals->end()-1;++it_first) {
                    for(std::vector<std::string>::const_iterator it_second=vals->begin()+1;it_second!=vals->end();++it_second) {
                        if(*it_first==*it_second) {
                            return BOOL_FALSE;
                        }
                    }
                }
                return BOOL_TRUE;
            }
            case eOP_STRAPP: {
                std::string ans_val;
                for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
                    ans_val.append(*it);
                }
                return ans_val;
            }
            case eOP_STRLEN: {
                if(vals->size()!=1) {
                    Signal_Error(SE_ARITY_MISMATCH, op);
                    throw eval_except;
                }
                return std::to_string(vals->at(0).size());
            }
            case eOP_SLICE: {
                /* 
                    use exception handling mechanism to allow extension on slice operation,
                    although it might be faster to just use a return code, instead of catching a exception
                */
                try {
                    return More_On_Slice_Operator_Level_1(vals);
                }
                catch (const FUNCTION_NOT_OVERLOADED_YET& fnld_except) {
                    int vals_size = vals->size();
                    // Require at least two arguments
                    if(vals_size>1) {
                        std::string to_be_sliced = vals->at(0);
                        // get slice starting position
                        int slice_start = std::stoi(vals->at(1));
                        // three arguments indicate an ending position also provided
                        if(vals_size==3) {
                            int slice_end = std::stoi(vals->at(2));
                            if(slice_end<0) {
                                slice_end = to_be_sliced.size()+slice_end+1;
                            }
                            int slice_length = slice_end-slice_start;
                            if(slice_length<=0) {
                                Signal_Error(SLE_OUT_OF_RANGE, vals);
                                throw eval_except;
                            }
                            return to_be_sliced.substr(slice_start, slice_length);
                        }
                        // two arguments, no ending position provided, return that starting position char
                        else if(vals_size==2) {
                            return std::string(1, to_be_sliced.at(slice_start));
                        }
                        else {
                            Signal_Error(SE_ARITY_MISMATCH, op);
                            throw syntax_except;
                        }
                    }
                    else {
                        Signal_Error(SE_ARITY_MISMATCH, op);
                        throw syntax_except;
                    }
                }
            }
            case eOP_LIST: {
                std::string ret_list = EXPR_START;
                for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
                    // Add quotation mark if *it has space
                    if(Quotation_Rquired_For_List_Elem(*it)) {
                        ret_list.push_back(QUOTATION_MARK);
                        ret_list.append(*it);
                        ret_list.push_back(QUOTATION_MARK);
                    }
                    else {
                        ret_list.append(*it);
                    }
                    ret_list.push_back(' ');
                }
                // The last char is a space, but we want it to be cEXPR_END
                ret_list[ret_list.size()-1] = cEXPR_END;
                return ret_list;
            }
            case eOP_FIRST: {
                /*
                    Check how many vals are there, if more than one val, return a list
                    consists of all first elements in each list. Exception will be thrown if 
                    any list is empty. Else return the first member of the list
                */
                int vals_size = vals->size();
                if(vals_size==1) {
                    std::string ret_elem;
                    std::string this_list = vals->at(0);
                    std::pair<int, int> first_elem_pos = Get_First_Element_Pos(this_list);
                    ret_elem = this_list.substr(first_elem_pos.first, first_elem_pos.second);
                    if(Quotation_Rquired_For_List_Elem(ret_elem)) {
                        ret_elem.insert(ret_elem.begin(), QUOTATION_MARK);
                        ret_elem.push_back(QUOTATION_MARK);
                        return ret_elem;
                    }
                    return ret_elem;
                }
                else if(vals_size>1) {
                    std::string ret_list = EXPR_START;
                    for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
                        std::pair<int, int> first_elem_pos = Get_First_Element_Pos(*it);
                        std::string to_append = (*it).substr(first_elem_pos.first, first_elem_pos.second);
                        if(Quotation_Rquired_For_List_Elem(to_append)) {
                            ret_list.push_back(QUOTATION_MARK);
                            ret_list.append(to_append);
                            ret_list.push_back(QUOTATION_MARK);
                        }
                        else {
                            ret_list.append(to_append);
                        }
                        ret_list.push_back(' ');
                    }
                    ret_list[ret_list.size()-1] = cEXPR_END;
                    return ret_list;
                }
                else {
                    Signal_Error(SE_ARITY_MISMATCH, op);
                    throw syntax_except;
                }
            }
            case eOP_REST: {
                if(vals->size()==1) {
                    // Need to exclude last quotation mark, so pass include_last_quotaion=true
                    std::pair<int, int> first_pos = Get_First_Element_Pos(vals->at(0));
                    return Format_List_String(vals->at(0).substr(first_pos.first+first_pos.second, std::string::npos));
                }
                else if(vals->size()>1) {
                    std::string ret_list = EXPR_START;
                    for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
                        std::pair<int, int> first_pos = Get_First_Element_Pos(*it);
                        ret_list.append(Format_List_String((*it).substr(first_pos.first+first_pos.second, std::string::npos)));
                        ret_list.push_back(' ');
                    }
                    ret_list[ret_list.size()-1] = cEXPR_END;
                    return ret_list;
                }
                else {
                    Signal_Error(SE_ARITY_MISMATCH, op);
                    throw syntax_except;
                }
            }
            case eOP_CONS: {
                if(vals->size()!=2) {
                    Signal_Error(SE_ARITY_MISMATCH, op);
                    throw syntax_except;
                }
                // Put elem to cons at list front
                std::string ret_list;                       
                if(Quotation_Rquired_For_List_Elem(vals->at(0))) {
                    ret_list.push_back(QUOTATION_MARK);
                    ret_list.append(vals->at(0));
                    ret_list.push_back(QUOTATION_MARK);
                }
                /* 
                    Append second arg to ret_list, note here we don't check if second arg
                    is a valid list or not, if not, exception will thrown in Format_List_String
                */
                ret_list.push_back(' ');
                ret_list.append(vals->at(1).substr(1, std::string::npos));

                return Format_List_String(ret_list);
            }
            case eOP_APPEND: {
                std::string ret_list;
                for(std::vector<std::string>::const_iterator it=vals->begin();it!=vals->end();++it) {
                    if((*it).front()!=cEXPR_START&&(*it).back()!=cEXPR_END) {
                        Signal_Error(TE_NOT_LIST, *it);
                        throw eval_except;
                    }
                    // Rip a head and a tail append to ret_list
                    ret_list.append((*it).substr(1, (*it).size()-2));
                    // Add a space
                    ret_list.push_back(' ');
                }
                // change last space to cEXPR_END
                ret_list[ret_list.size()-1] = cEXPR_END;
                return Format_List_String(ret_list);
            }
            case eOP_RESOLVE: {
                if(vals->size()!=1) {
                    Signal_Error(SE_ARITY_MISMATCH, vals);
                    throw syntax_except;
                }

                return Handle_Val(vals->at(0));
            }
            default: {
                Signal_Error(IE_UNKNOWN, op);
                throw huaji_except;
            }
        }
    }
    catch (const HUAJIBASE_EXCEPTION& huaji_except) {
        throw huaji_except;
    }
    catch (const std::invalid_argument& ia) {
        Signal_Error(NE_CONVERSION_FAILED, vals);
        throw eval_except;
    }
    catch (const std::out_of_range& oor) {
        Signal_Error(NE_OUT_OF_RANGE, vals);
        throw eval_except;
    }
    catch (...) {
        Signal_Error(IE_UNKNOWN, vals);
        throw eval_except;
    }
    // no match clause, must be a bug
    Signal_Error(IE_UNDEFINED_NAME, op);
    throw huaji_except;
}

std::string hjbase::HUAJISCRIPTBASE::More_On_Expression_Level_1(const std::string& op, const const_itVecStr* vals) {
    Signal_Error(SE_UNABLE_TO_PROCESS_EXPR, op);
    throw eval_except;
}

std::string hjbase::HUAJISCRIPTBASE::Basic_Operation(const std::string& op, const const_itVecStr* vals) {
    try {
        std::map<std::string, int>::const_iterator op_key_it = NUMERICAL_OPERATORS.find(op);
        // Numerical_operators
        if(op_key_it!=NUMERICAL_OPERATORS.end()) {
            return Numerical_Operation(op, op_key_it->second, vals);
        }
        // Other basic operations
        op_key_it = OTHER_BASIC_OPERATORS.find(op);
        if(op_key_it!=OTHER_BASIC_OPERATORS.end()) {
            return Other_Basic_Operation(op, op_key_it->second, vals);
        }
        else {
            return More_On_Expression_Level_1(op, vals);
        }
    }
    catch (const HUAJIBASE_EXCEPTION& huaji_except) {
        throw huaji_except;
    }
    // catch all exceptions
    catch (...) {
        Signal_Error(IE_UNKNOWN, vals);
        throw huaji_except;
    }
}

std::string hjbase::HUAJISCRIPTBASE::More_On_Names_Query_Level_1(const std::string& name) {
    throw name_except;
}

std::string hjbase::HUAJISCRIPTBASE::Handle_Val(const std::string& name_or_val) {
    if(enable_raw_string) {
        if(name_or_val.size()>1) {
            if(name_or_val.front()=='$'&&name_or_val.back()=='$') {
                std::string name = name_or_val.substr(1,name_or_val.size()-2);
                try {
                    return More_On_Names_Query_Level_1(name);
                }
                catch (const NAME_EXCEPTION& name_except) {
                    return Resolve_Name(name, names);
                }
            }
        }
        return name_or_val;
    }
    else {
        if(Starts_With(name_or_val, STRING_TAG)) {
            return name_or_val.substr(STRING_TAG.size(),std::string::npos);
        }
        else if(Starts_With(name_or_val, LIST_TAG)) {
            return name_or_val.substr(LIST_TAG.size(),std::string::npos);
        }
        else if(Is_Numerical(name_or_val)) {
            return name_or_val;
        }
        else {
            try {
                return More_On_Names_Query_Level_1(name_or_val);
            }
            catch (const NAME_EXCEPTION& name_except) {
                return Resolve_Name(name_or_val, names);
            }
        }
    }
}