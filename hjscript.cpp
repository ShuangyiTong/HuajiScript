#include "hjscript.hpp"

// Used by hjbase::HUAJISCRIPTBASE::Take_One_Token(std::vector<std::string>::const_iterator)
#define REQUIRE_MORE_TOKENS_RETURN_CODE -1

// Used by hjbase::HUAJISCRIPTBASE::Collect_Tokens(const std::string&)
#define REQUIRE_MORE_TOKENS_FOR_COMPLETE_COMMAND_RETURN_CODE 0
#define COMMAND_COMPLETED_RETURN_CODE 1

// Used by hjbase::HUAJISCRIPTBASE::Huaji_Command_Interpreter(const const_itVecStr&)
#define EXIT_RETURN_CODE 2
#define VALID_COMMAND_RETURN_CODE 1
#define INVALID_COMMAND_RETURN_CODE 0

// Used by hjbase::LISTFORMATTER::Take_One_Char(char)
#define FORMATTING_COMPLETE 1
#define REQUIRE_MORE_CHAR_FOR_COMPLETE_LST 0

#define TYPE_DOUBLE 0
#define TYPE_LONG 1

#define INCREASE_AST_DEPTH 1
#define DECREASE_AST_DEPTH -1

using namespace hjbase::cnt::keywords;
using namespace hjbase::cnt::operators;
using namespace hjbase::cnt::common_msgs;
using namespace hjbase::cnt::miscellaneous;
using namespace hjbase::cnt::commands_names;
using namespace hjbase::cnt::type_tag;
using namespace hjbase::ufunc;
using namespace hjbase::except;

void hjbase::ufunc::Print_IVSTR(const const_itVecStr& iVstr) {
    for(std::vector<std::string>::const_iterator it=iVstr.begin();it<iVstr.end();++it) {
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

void hjbase::ufunc::Highlight_Error_Part(const const_itVecStr& iVstr) {
    for(std::vector<std::string>::const_iterator it=iVstr.begin();it<iVstr.end();++it) {
        std::cout<<std::string((*it).size(), '~')<<" ";
    }
    std::cout<<std::endl;
}

void hjbase::ufunc::Signal_Error(const std::string& error_msg, const const_itVecStr& error_part) {
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

bool hjbase::ufunc::Check_If_Float_Point(const const_itVecStr& vals) {
    // iterate over vals vector
    for(std::vector<std::string>::const_iterator it=vals.begin();it<vals.end();++it) {
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

std::string hjbase::ufunc::Convert_Ptr_To_Str(void* ptr) {
    std::stringstream ss;
    ss<<ptr;
    return ss.str();
}

std::pair<int, int> hjbase::ufunc::Get_First_Element_Pos(const std::string& this_list) {
    if(this_list.front()!=cEXPR_START||this_list.back()!=cEXPR_END) {
        Signal_Error(TE_NOT_LIST, this_list);
        throw type_except;
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
    for(std::string::const_iterator it=this_list.begin()+start_pos+1;it<this_list.end();++it) {
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
    for(std::string::const_iterator it=original_lst.begin();it<original_lst.end()-1;++it) {
        if(lst_formatter.Take_One_Char(*it)==FORMATTING_COMPLETE) {
            Signal_Error(TE_NOT_LIST, original_lst);
            throw type_except;
        }
    }
    if(lst_formatter.Take_One_Char(original_lst.back())!=FORMATTING_COMPLETE) {
        Signal_Error(TE_NOT_LIST, original_lst);
        throw type_except;
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
    for(std::string::const_iterator it=this_str.begin();it<this_str.end();++it) {
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
    if(this_str.front()==QUOTATION_MARK&&this_str.back()==QUOTATION_MARK) {
        return false;
    }
    bool ret_flag = false;
    for(std::string::const_iterator it=this_str.begin();it<this_str.end();++it) {
         if(*it==' ') {
            ret_flag = true;
            break;
        }
    }
    return ret_flag;
}

const_itVecStr::const_itVecStr(std::vector<std::string>::const_iterator from_begin,
                               std::vector<std::string>::const_iterator from_end)
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

inline std::vector<std::string>::const_iterator const_itVecStr::begin() const {
    return this_begin;
}

inline std::vector<std::string>::const_iterator const_itVecStr::end() const {
    return this_end;
}

inline std::string const_itVecStr::front() const {
    return *(this_begin);
}

inline std::string const_itVecStr::back() const {
    return *(this_end-1);
}

inline std::string const_itVecStr::at(int pos) const {
    return *(this_begin+pos);
}

inline int const_itVecStr::size() const {
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

hjbase::HUAJITOKENIZER::HUAJITOKENIZER(std::string& file_name) 
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
                        break;
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
    catch (const std::ios_base::failure& e) {
        std::cout<<e.what()<<std::endl;
        throw token_except;
    }
}

hjbase::FUNC::FUNC(bool with_side_effects, const const_itVecStr& body_iVstr, 
                   const std::map<std::string, std::string>* stack_map, const std::list<std::string>* arg_names)
    // use std::vector::vector range constructor
    : fbody_container (new std::vector<std::string>(body_iVstr.begin(), body_iVstr.end()))
    /* 
        now fbody should built on fbody_container, as the container for body_iVstr is 
        allocated on stack by hjbase::HUAJISCRIPTBASE::Entry_Point and will be cleared
        once the root command execution completed
    */
    , stack_frame_template(stack_map)
    , has_side_effects (with_side_effects)
    , var_names (arg_names) {
    fbody = new const_itVecStr(fbody_container->begin(), fbody_container->end());
}

hjbase::FUNC::~FUNC() {
    delete fbody_container;
    delete fbody;
    delete stack_frame_template;
    delete var_names;
}

inline std::map<std::string, std::string>* 
hjbase::FUNC::Get_Stack_Frame_Template_Copy_On_Heap() const {
    return new std::map<std::string, std::string>(*stack_frame_template);
}

inline std::list<std::string> hjbase::FUNC::Get_Var_Names() const {
    return *var_names;
}

inline const_itVecStr hjbase::FUNC::Get_Fbody() {
    return *fbody;
}

void hjbase::HUAJISCRIPTBASE::Constructor_Helper() {
    names_stack = new std::vector<std::map<std::string, std::string>*>;
    Push_Stack_Frame(new std::map<std::string, std::string>);
    func_pool = new std::map<std::string, FUNC*>;
    collect_status = 0;
    collect_length = 0;
    enable_float_point = true;
    enable_debug_mode = false;
    current_ast_depth = 0;
    enable_raw_string = false;
    allow_side_effects = true;
}

hjbase::HUAJISCRIPTBASE::HUAJISCRIPTBASE(std::string file_name) 
    : is_console (false) {
    tokenizer = new HUAJITOKENIZER(file_name);
    Constructor_Helper();
}

hjbase::HUAJISCRIPTBASE::HUAJISCRIPTBASE() 
    : is_console (true) {
    tokenizer = new HUAJITOKENIZER();
    Constructor_Helper();
}

hjbase::HUAJISCRIPTBASE::~HUAJISCRIPTBASE() {
    for(std::vector<std::map<std::string, std::string>*>::iterator it=names_stack->begin();it<names_stack->end();++it) {
        delete *it;
    }
    delete names_stack;
    for(std::map<std::string, FUNC*>::iterator it=func_pool->begin();it!=func_pool->end();++it) {
        delete (*it).second;
    }
    delete func_pool;
    delete tokenizer;
}

std::vector<std::pair<std::string, std::string>> 
hjbase::HUAJISCRIPTBASE::General_Name_Declaring_Syntax_Parser(const const_itVecStr& iVstr, bool from_func_call) {
    // Initialize return vector
    std::vector<std::pair<std::string, std::string>> ret_vec;

    if(!(iVstr.size())) {
        return ret_vec;
    }

    // Here we don't check the size of iVstr, as this is what caller function should do
    std::string name = iVstr.at(0);
    bool name_already_used = false;

    for(std::vector<std::string>::const_iterator it=iVstr.begin()+1;it<iVstr.end();++it) {
        // Assign val when declare
        if(*it==OP_EQ) {
            std::vector<std::string>::const_iterator expr_begin_it = it+1;
            // expr_begin_it needs to be smaller than end()
            if(expr_begin_it>=iVstr.end()) {
                Signal_Error(SE_UNEXPECTED_TOKEN, iVstr);
                throw syntax_except;
            }
            std::vector<std::string>::const_iterator expr_end_it = it+2;
            for(it=expr_end_it;it<iVstr.end();++it) {
                // expr_end_it should be it + 1 as it is excluded
                expr_end_it = it;
                if(*it==SEPARATOR) {
                    break;
                }
            }
            if(it==iVstr.end()) {
                // Here is means it already went pass last token in iVstr, so we need to assign expr_end_it = it,
                expr_end_it = it;
            }
            // Create new ast node by construct new const_itVecStr object
            const_itVecStr expr = const_itVecStr(expr_begin_it, expr_end_it);
            std::string val;
            val = Evaluate_Expression(expr);
            // Push into return vector
            ret_vec.push_back(std::pair<std::string, std::string>(name, val));
            name_already_used = true;
        }                    
        else if(*it==SEPARATOR) {
            if(name_already_used) {
                Signal_Error(SE_UNEXPECTED_TOKEN, iVstr);
                throw syntax_except;
            }
            if(from_func_call) {
                // Here name should be val, we add this option to reduce code, as the syntax is the same, but this way of naming variable is incorrect.
                ret_vec.push_back(std::pair<std::string, std::string>(UNDEFINED_TYPE, Handle_Val(name)));
            }
            else {
                ret_vec.push_back(std::pair<std::string, std::string>(name, UNDEFINED_TYPE));
            }
            name_already_used = true;
        }                    
        // must be a name
        else {
            if(!name_already_used) {
                Signal_Error(SE_UNEXPECTED_TOKEN, iVstr);
                throw syntax_except;
            }
            name = *it;
            name_already_used = false;
        }
    }
    // Last name not pushed into vector
    if(!name_already_used) {
        if(from_func_call) {
            ret_vec.push_back(std::pair<std::string, std::string>(UNDEFINED_TYPE, Handle_Val(name)));
        }
        else {
            ret_vec.push_back(std::pair<std::string, std::string>(name, UNDEFINED_TYPE));
        }
    }

    return ret_vec;
}

void hjbase::HUAJISCRIPTBASE::Cleanup_If_Exception_Thrown(int reverted_ast_depth) {
    current_ast_depth = reverted_ast_depth;
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

void hjbase::HUAJISCRIPTBASE::Print_Debug_Info(const std::string& info, int ast_depth_change, const const_itVecStr& node) {
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
    std::cout<<VERTICAL_LINE_WITH_SPACE_AFTER<<NAME_THEADER<<VERTICAL_LINE_WITH_SPACE_BOTH
    <<VAL_THEADER<<VERTICAL_LINE_WITH_SPACE_BEFORE<<std::endl;
    std::cout<<HORIZONTAL_LINE<<std::endl;
    // Table body
    for(std::map<std::string, std::string>::const_iterator it=names_map->begin();it!=names_map->end();++it) {
        std::cout<<VERTICAL_LINE_WITH_SPACE_AFTER<<it->first<<VERTICAL_LINE_WITH_SPACE_BOTH
        <<it->second<<VERTICAL_LINE_WITH_SPACE_BEFORE<<std::endl;
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

        const_itVecStr cmd = const_itVecStr(token_it-collect_length+1, token_it+1);

        // Reset collect_length, and start_it will be set to new start as well.
        collect_length = 0;

        // Build const_itVecStr object for execution
        int execution_status = Huaji_Command_Interpreter(cmd);

        // Return status
        return execution_status;
    }
    else {
        return REQUIRE_MORE_TOKENS_RETURN_CODE;
    }
}

void hjbase::HUAJISCRIPTBASE::Entry_Point() {
    std::vector<std::string> command;
    std::string token;
    if(is_console) {
        std::cout<<CONSOLE_INFO_STRING<<std::endl;
        std::cout<<COMMAND_LINE_PROMPT;
    }
    while(1) {
        try {
            token = tokenizer->Get_One_Token();
            command.push_back(token);
        }
        catch (const TOKEN_EXCEPTION& e) {    
            if (collect_length!=0) {
                Signal_Error(SE_REQUIRE_MORE_TOKENS, token);
            }
            return;
        }
        int ret_code = Take_One_Token(command.end()-1);
        if(ret_code!=REQUIRE_MORE_TOKENS_RETURN_CODE) {
            command.clear();
            if(ret_code==EXIT_RETURN_CODE) {
                return;
            }
            if(is_console) {
                std::cout<<COMMAND_LINE_PROMPT;
            }
        }
    }
}

inline void hjbase::HUAJISCRIPTBASE::Push_Stack_Frame(std::map<std::string, std::string>*
                                                      preset_stack_frame) {
    names_stack->push_back(preset_stack_frame);
}

inline void hjbase::HUAJISCRIPTBASE::Pop_Stack_Frame() {
    delete names_stack->back();
    names_stack->pop_back();
}

inline void hjbase::HUAJISCRIPTBASE::Side_Effects_Guard(std::string func_name_with_side_effects) {
    if(!allow_side_effects) {
        Signal_Error(SEF_NOT_ALLOWED, func_name_with_side_effects);
        throw side_effects_except; 
    }
}

void hjbase::HUAJISCRIPTBASE::Declare_Name(const std::string& name, const std::string& val) {
    // Declare name in top stack
    std::map<std::string, std::string>* target_scope_names = names_stack->back();

    // insert return a std::pair, with second element specifying if successfully inserted.
    if((target_scope_names->insert(std::pair<std::string, std::string>(name, val))).second==false) {
        Signal_Error(NAE_REDECLARE, name);
        throw name_except;
    }
    return;
}

void hjbase::HUAJISCRIPTBASE::Mutate_Name(const std::string& name, const std::string& val) {
    // Mutate name in top stack
    std::map<std::string, std::string>* target_scope_names = names_stack->back();

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

std::string hjbase::HUAJISCRIPTBASE::Resolve_Name(const std::string& name) {
    // Mutate name in top stack
    std::map<std::string, std::string>* target_scope_names = names_stack->back();
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

int hjbase::HUAJISCRIPTBASE::Block_Execution(const const_itVecStr& commands_block) {

    int start_exec_pos = -1;

    // Find execution position, where the first BLOCK_START appears
    for(std::vector<std::string>::const_iterator it=commands_block.begin();
        it<commands_block.end();++it) {
        if(*it==BLOCK_START) {
            start_exec_pos = it-commands_block.begin();
            break;
        }
    }

    if(start_exec_pos==-1) {
        Signal_Error(SE_REQUIRE_MORE_TOKENS, commands_block);
        return VALID_COMMAND_RETURN_CODE;
    }

    /*
        We already know currently collect_status = collect_length = 0, 
        so use a nested Take_One_Token here won't cause problems.
        check for end block token is ignored, as we iterate to end()-1
    */
    for(std::vector<std::string>::const_iterator it=commands_block.begin()+start_exec_pos+1;
        it<commands_block.end()-1;++it) {
        if(Take_One_Token(it)==EXIT_RETURN_CODE) {
            return EXIT_RETURN_CODE;
        }
    }

    // force reset collect_length, it is possible to have positive collect_length
    if (collect_length!=0) {
        Signal_Error(SE_REQUIRE_MORE_TOKENS, commands_block);
        collect_length = 0;
        collect_status = 0;
    }
    return VALID_COMMAND_RETURN_CODE;
}

std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
hjbase::HUAJISCRIPTBASE::Get_Expr_Position_Iterators(const const_itVecStr& iVstr) {    
    std::vector<std::string>::const_iterator start_it = iVstr.end();
    std::vector<std::string>::const_iterator end_it = iVstr.end();
    int expr_depth = 0;
    for(std::vector<std::string>::const_iterator it=iVstr.begin();it<iVstr.end();++it) {
        if(*it==EXPR_START) {
            if(!expr_depth) {
                start_it = it;
            }
            ++expr_depth;
        }
        else if(*it==EXPR_END) {
            --expr_depth;
            if(start_it!=iVstr.end()&&!expr_depth) {
                end_it = it+1;
                break;
            }
        }
    }

    if(start_it==iVstr.end()) {
        Signal_Error(SE_MISSING_EXPR, iVstr);
        throw syntax_except;
    }

    return std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
               (start_it, end_it);
}

std::string hjbase::HUAJISCRIPTBASE::Find_And_Evaluate_Expression(const const_itVecStr& iVstr) {

    std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator> ret_pair
        = Get_Expr_Position_Iterators(iVstr);

    const_itVecStr expr = const_itVecStr(ret_pair.first, ret_pair.second);
    
    // Handle expression
    std::string val;
    val = Evaluate_Expression(expr);
    return val;
}

int hjbase::HUAJISCRIPTBASE::Huaji_Command_Interpreter(const const_itVecStr& command) {

    Print_Debug_Info(DEB_COMMAND_START, INCREASE_AST_DEPTH, command);

    // Get CMD side
    int command_size = command.size();

    // Only a DELIMITER presented
    if(command_size==1) {
        return VALID_COMMAND_RETURN_CODE;
    }

    // record current ast_depth, used in Cleanup_If_Exception_Thrown
    int reverted_ast_depth = current_ast_depth-1;

    std::string cmd_str = command.at(0);

    std::map<std::string, int>::const_iterator cmd_key_it = HJBASE_CMD_SEARCH_TREE.find(cmd_str);
    int cmd_key = -1;
    if(cmd_key_it!=HJBASE_CMD_SEARCH_TREE.end()) {
        cmd_key = cmd_key_it->second;
    }
    else {
        std::pair<int, bool> ret_pair = More_On_Command_Level_1(command);
        if(ret_pair.second) {
            Print_Debug_Info(DEB_COMMAND_END, DECREASE_AST_DEPTH, command);
            return ret_pair.first;
        }
        else {

            // FUNC execution
            if(command_size<4) {
                Signal_Error(SE_REQUIRE_MORE_TOKENS, command);
                return VALID_COMMAND_RETURN_CODE;
            }

            std::string func_ptrStr;
            try {
                func_ptrStr = Resolve_Name(cmd_str);
            }
            catch (const HUAJIBASE_EXCEPTION& e) {
                Signal_Error(IE_UNDEFINED_NAME, command);
                return INVALID_COMMAND_RETURN_CODE;
            }

            FUNC* func_ptr = nullptr;
            std::map<std::string, FUNC*>::const_iterator func_ptr_it = func_pool->find(func_ptrStr);
            if(func_ptr_it==func_pool->end()) {
                Signal_Error(IE_INTERNAL_QUERY_FAILED, func_ptrStr);
                return VALID_COMMAND_RETURN_CODE;
            }
            else {
                func_ptr = (*func_ptr_it).second;
            }

            // Halt Execution if it has no side effects
            if(!(func_ptr->has_side_effects)) {
                Signal_Error(SEF_REQUIRED, cmd_str);
                return VALID_COMMAND_RETURN_CODE;
            }

            // Check syntax and get parsed names for generating stack_map
            if(command.at(1)!=EXPR_START&&command.at(command_size-2)!=EXPR_END) {
                Signal_Error(SE_UNEXPECTED_TOKEN, command);
                return VALID_COMMAND_RETURN_CODE;
            }
            std::vector<std::pair<std::string, std::string>> parsed_names;
            try {
                parsed_names = General_Name_Declaring_Syntax_Parser(const_itVecStr(command.begin()+2, command.end()-2), true);
            }
            catch (const HUAJIBASE_EXCEPTION& e) {
                Cleanup_If_Exception_Thrown(reverted_ast_depth);
                return VALID_COMMAND_RETURN_CODE;
            }

            // Allocate new stack and pushed to names_stack
            std::map<std::string, std::string>* preset_stack_frame
                = func_ptr->Get_Stack_Frame_Template_Copy_On_Heap();
            std::list<std::string> var_names = func_ptr->Get_Var_Names();
            if(var_names.size()<parsed_names.size()) {
                Signal_Error(SE_ARITY_MISMATCH, command);
                return VALID_COMMAND_RETURN_CODE;
            }
            for(std::vector<std::pair<std::string, std::string>>::const_iterator
                name_it=parsed_names.begin();name_it<parsed_names.end();++name_it) {
                if((*name_it).first==UNDEFINED_TYPE) {
                    (*preset_stack_frame)[var_names.front()] = (*name_it).second;
                    var_names.pop_front();
                }
                else {
                    if((preset_stack_frame->find((*name_it).first))==preset_stack_frame->end()) {
                        Signal_Error(NAE_UNDEFINED, (*name_it).first);
                        return VALID_COMMAND_RETURN_CODE;
                    }
                    (*preset_stack_frame)[(*name_it).first] = (*name_it).second;
                    // Not accessing this itrator any more, this should be safe
                    var_names.remove((*name_it).first);
                }
            }
            Push_Stack_Frame(preset_stack_frame);

            Block_Execution(func_ptr->Get_Fbody());

            Pop_Stack_Frame();

            Print_Debug_Info(DEB_COMMAND_END, DECREASE_AST_DEPTH, command);

            return VALID_COMMAND_RETURN_CODE;
        }
    }

    switch (cmd_key) {
        // Variable declare
        case eCMD_NAME_DECLARE: {

            if(command_size<3) {
                Signal_Error(SE_REQUIRE_MORE_TOKENS, command);
                return VALID_COMMAND_RETURN_CODE;
            }
            // Create a const_itVecStr object for names parsing
            const_itVecStr names_iVstr = const_itVecStr(command.begin()+1,command.end()-1);
            try {
                std::vector<std::pair<std::string, std::string>> parsed_names 
                    = General_Name_Declaring_Syntax_Parser(names_iVstr);

                for(std::vector<std::pair<std::string, std::string>>::const_iterator
                    name_pair_it=parsed_names.begin();
                    name_pair_it<parsed_names.end();++name_pair_it) {
                    Declare_Name((*name_pair_it).first, (*name_pair_it).second);
                }
            }
            catch (const HUAJIBASE_EXCEPTION& e) {
                Cleanup_If_Exception_Thrown(reverted_ast_depth);
                return VALID_COMMAND_RETURN_CODE;
            }
            break;
        }

        case eCMD_NAME_MUTATE: {
            if(command_size<5) {
                Signal_Error(SE_REQUIRE_MORE_TOKENS, command);
                return VALID_COMMAND_RETURN_CODE;
            }
            if(command.at(2)!=CMD_NAME_MUTATE_TO) {
                Signal_Error(SE_UNEXPECTED_TOKEN, command);
                return VALID_COMMAND_RETURN_CODE;
            }
            std::string name = command.at(1);
            const_itVecStr expr = const_itVecStr(command.begin()+3, command.end()-1);
            std::string val;
            try {
                val = Evaluate_Expression(expr);
                Mutate_Name(name, val);
            }
            catch (const HUAJIBASE_EXCEPTION& e){
                Cleanup_If_Exception_Thrown(reverted_ast_depth);
                return VALID_COMMAND_RETURN_CODE;
            }
            break;
        }

        // Print command 
        case eCMD_PRINT: {
            bool nl_flag = true;
            for(std::vector<std::string>::const_iterator it=command.begin()+1;it<command.end()-1;++it) {
                if((*it)==NONEWLINE&&nl_flag) {
                    nl_flag = false;
                }
                else if((*it)==USE_EXPR_FROM_HERE&&it!=command.end()-2) {
                    const_itVecStr expr = const_itVecStr(it+1,command.end()-1);
                    try {
                        std::cout<<Evaluate_Expression(expr);
                    }
                    catch (const HUAJIBASE_EXCEPTION& e) {
                        Cleanup_If_Exception_Thrown(reverted_ast_depth);
                        return VALID_COMMAND_RETURN_CODE;
                    }
                    break;
                }
                else {
                    try {
                        std::cout<<Handle_Val(*it);
                    }
                    catch (const HUAJIBASE_EXCEPTION& e) {
                        Cleanup_If_Exception_Thrown(reverted_ast_depth);
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
                condition = Find_And_Evaluate_Expression(command)!=BOOL_FALSE;
            }
            catch (const HUAJIBASE_EXCEPTION& e){
                Cleanup_If_Exception_Thrown(reverted_ast_depth);
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
                condition = Find_And_Evaluate_Expression(command)!=BOOL_FALSE;
            }
            catch (const HUAJIBASE_EXCEPTION& e){
                Cleanup_If_Exception_Thrown(reverted_ast_depth);
                return VALID_COMMAND_RETURN_CODE;
            }

            while(condition) {
                Block_Execution(command);
                try {
                    condition = Find_And_Evaluate_Expression(command)!=BOOL_FALSE;
                }
                catch (const HUAJIBASE_EXCEPTION& e){
                    Cleanup_If_Exception_Thrown(reverted_ast_depth);
                    return VALID_COMMAND_RETURN_CODE;
                }
            }
            break;
        }

        case eCMD_DEFINE: {
            if(command_size<7) {
                Signal_Error(SE_REQUIRE_MORE_TOKENS, command);
                return VALID_COMMAND_RETURN_CODE;
            }

            int option_offset = 0;
            bool overloaded = false;
            if(command.at(1)==OVERLOADING_FLAG) {
                option_offset = 1;
                overloaded = true;
            }
            std::string func_name;
            bool with_side_effects = true;
            std::vector<std::string>::const_iterator func_def_begin = command.end();
            std::vector<std::string>::const_iterator func_def_end = command.end();
            std::list<std::string>* arg_names = new std::list<std::string>;
            std::map<std::string, std::string>* stack_map = new std::map<std::string, std::string>;

            // begin with EXPR_START, treat as a no side effects function
            if(command.at(1+option_offset)==EXPR_START) {
                std::vector<std::string>::const_iterator func_arg_end_it = command.end();
                func_name = command.at(2+option_offset);
                with_side_effects = false;
                for(std::vector<std::string>::const_iterator arg_name_it=command.begin()+3+option_offset;
                    arg_name_it<command.end();++arg_name_it) {
                    if(*arg_name_it==EXPR_START) {
                        Signal_Error(SE_UNEXPECTED_TOKEN, command);
                        return VALID_COMMAND_RETURN_CODE;
                    }
                    if(*arg_name_it==EXPR_END) {
                        func_arg_end_it = arg_name_it;
                        break;
                    }
                    arg_names->push_back(*arg_name_it);
                }
                if(func_arg_end_it+1>=command.end()) {
                    Signal_Error(SE_REQUIRE_MORE_TOKENS, command);
                    return VALID_COMMAND_RETURN_CODE;
                }
                func_def_begin = func_arg_end_it+1;
                // Generate stack_map using arg_names
                for(std::list<std::string>::const_iterator arg_name_it=arg_names->begin();
                    arg_name_it!=arg_names->end();++arg_name_it) {
                    if((stack_map->insert(std::pair<std::string, std::string>
                        (*arg_name_it, UNDEFINED_TYPE)))
                        .second==false) {
                        Signal_Error(NAE_REDECLARE, *arg_name_it);
                        return VALID_COMMAND_RETURN_CODE;
                    }
                }
                func_def_end = command.end()-1;
            }
            else if(command.at(2+option_offset)==EXPR_START) {
                func_name = command.at(1+option_offset);
                std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator> ret_pair;
                try {
                    ret_pair = Get_Expr_Position_Iterators(command);
                }
                catch (const HUAJIBASE_EXCEPTION& e) {
                    Cleanup_If_Exception_Thrown(reverted_ast_depth);
                    return VALID_COMMAND_RETURN_CODE;
                }
                
                func_def_begin = ret_pair.second;

                // remove EXPR_START and EXPR_END
                ++(ret_pair.first);
                --(ret_pair.second);

                // Send to name declaring parser
                std::vector<std::pair<std::string, std::string>> parsed_names
                    = General_Name_Declaring_Syntax_Parser(const_itVecStr(ret_pair.first, ret_pair.second));
                
                // Configure stack_map and arg_names
                for(std::vector<std::pair<std::string, std::string>>::const_iterator name_pair_it=parsed_names.begin();
                    name_pair_it<parsed_names.end();++name_pair_it) {
                    (*stack_map)[(*name_pair_it).first] = (*name_pair_it).second;
                    arg_names->push_back((*name_pair_it).first);
                }
            }
            else {
                Signal_Error(SE_UNEXPECTED_TOKEN, command);
                return VALID_COMMAND_RETURN_CODE;
            }

            // Build FUNC object
            FUNC* func_ptr = new FUNC(with_side_effects, const_itVecStr(func_def_begin, func_def_end), stack_map, arg_names);
            // Create unique identifier for FUNC to store in func_pool
            std::string func_ptrStr = FUNC_TAG + Convert_Ptr_To_Str(func_ptr) + func_name;

            try {
                if(overloaded) {
                    Mutate_Name(func_name, func_ptrStr);
                }
                else {
                    Declare_Name(func_name, func_ptrStr);
                }
            }
            catch (const HUAJIBASE_EXCEPTION& e) {
                delete func_ptr;
                Cleanup_If_Exception_Thrown(reverted_ast_depth);
                return VALID_COMMAND_RETURN_CODE;
            }

            if(!(func_pool->insert(std::pair<std::string, FUNC*>(func_ptrStr, func_ptr))).second) {
                Signal_Error(IE_INSERTION_FAILURE, func_ptrStr);
                return VALID_COMMAND_RETURN_CODE;
            }

            break;
        }

        case eCMD_RETURN: {
            try {
                const_itVecStr expr = const_itVecStr(command.begin()+1,command.end()-2);
                temp_return_val = Evaluate_Expression(expr);
                return EXIT_RETURN_CODE;
            }
            catch (const HUAJIBASE_EXCEPTION& e) {
                Cleanup_If_Exception_Thrown(reverted_ast_depth);
                return VALID_COMMAND_RETURN_CODE;
            }
        }

        case eDEB_PRINT_NAMES: {
            if(command_size<4) {
                Print_Name_Map(names_stack->back());
            }
            else {
                std::string option = command.at(1);
                std::string option_arg;
                if(command.at(2)==EXPR_START) {
                    try {
                        option_arg = Find_And_Evaluate_Expression(command);
                    }
                    catch (const HUAJIBASE_EXCEPTION& e) {
                        Cleanup_If_Exception_Thrown(reverted_ast_depth);
                        return VALID_COMMAND_RETURN_CODE;
                    }
                }
                else {
                    option_arg = command.at(2);
                }

                if(option==STACK_FLAG) {
                    // 0 is the bottom of the stack
                    int current_frame_depth = names_stack->size();
                    int requested_frame_depth;
                    try {
                        requested_frame_depth = stoi(option_arg);
                    }
                    catch (const std::logic_error) {
                        Signal_Error(NE_CONVERSION_FAILED, command);
                        Cleanup_If_Exception_Thrown();
                        return VALID_COMMAND_RETURN_CODE;
                    }
                    if(requested_frame_depth<0) {
                        requested_frame_depth = current_frame_depth + requested_frame_depth;
                    }
                    if(requested_frame_depth>=current_frame_depth||requested_frame_depth<0) {
                        Signal_Error(IE_ACCESSING_UNDEFINED_STACK_FRAME, option_arg);
                        return VALID_COMMAND_RETURN_CODE;
                    }
                    Print_Name_Map(names_stack->at(requested_frame_depth));
                }
                else if(option==NAMESPACE_FLAG) {
                    std::cout<<"Not implemented yet";
                }
                else {
                    Signal_Error(SE_UNRECOGNISED_FLAG, option);
                    return VALID_COMMAND_RETURN_CODE;
                }
            }
            break;
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
            std::cout<<DEB_MODE_ON<<std::endl;
            return VALID_COMMAND_RETURN_CODE;
        }

        case eCONFIG_DEBUG_MODE_DISABLE: {
            enable_debug_mode = false;
            current_ast_depth = 0;
            std::cout<<DEB_MODE_OFF<<std::endl;
            return VALID_COMMAND_RETURN_CODE;
        }

        case eEVALUATE: {
            const_itVecStr expr = const_itVecStr(command.begin()+1,command.end()-2);
            try {
                Evaluate_Expression(expr);
            }
            catch (const HUAJIBASE_EXCEPTION& e) {
                Cleanup_If_Exception_Thrown(reverted_ast_depth);
                return VALID_COMMAND_RETURN_CODE;
            }
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

std::pair<int,bool> hjbase::HUAJISCRIPTBASE::More_On_Command_Level_1(const const_itVecStr& command) {
    return std::pair<int, bool>(-1, false);
}

std::string hjbase::HUAJISCRIPTBASE::Evaluate_Expression(const const_itVecStr& expr) {

    Print_Debug_Info(DEB_EXPR_START, INCREASE_AST_DEPTH, expr);
    
    std::string ans_val;

    if(!expr.size()) {
        Signal_Error(SE_UNABLE_TO_PROCESS_EXPR, expr);
        throw eval_except;
    }

    else if(expr.size()==1) {
        ans_val = Handle_Val(expr.at(0));
    }

    else {

        // Check if braces are presented
        if(expr.front()!=EXPR_START||expr.back()!=EXPR_END) {
            Signal_Error(SE_UNEXPECTED_TOKEN, expr);
            throw syntax_except;
        }

        /*
            After above checks, the expression must be expr = EXPR_START op expr ... EXPR_END
            We also allow op to be a expr
        */
        std::string op = expr.at(1);
        int op_expr_offset = 2;
        if(op==EXPR_START) {
            std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
                op_expr_it_pair = Get_Expr_Position_Iterators(const_itVecStr(expr.begin()+1, expr.end()-1));
            
            op = Evaluate_Expression(const_itVecStr(op_expr_it_pair.first, op_expr_it_pair.second));
            op_expr_offset = op_expr_it_pair.second - expr.begin();
        }

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
        for(std::vector<std::string>::const_iterator it=expr.begin()+op_expr_offset;it<expr.end()-1;++it) {

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
            const_itVecStr arg = const_itVecStr(pos_its.at(i), pos_its.at(i+1));
            try {
                vals_container->push_back(Evaluate_Expression(arg));
            }
            catch (const HUAJIBASE_EXCEPTION& e) {
                delete vals_container;
                throw e;
            }
        }

        // Construct const_itVecStr for evaluation
        const_itVecStr vals = const_itVecStr(vals_container->begin(), vals_container->end());

        // Evaluate Expression
        try {
            ans_val = Apply_Function(op, vals);
        }
        catch (const HUAJIBASE_EXCEPTION& e) {
            delete vals_container;
            throw e;
        }
        delete vals_container;
    }
    Print_Debug_Info(DEB_EXPR_END, DECREASE_AST_DEPTH, ans_val);
    
    return ans_val;
}

template <typename T> std::string
hjbase::HUAJISCRIPTBASE::Numerical_Operation_Templated_Helper(const std::string& op, int op_key, int vals_size, int T_name, const const_itVecStr& vals) {
    // Initialize numerical value array
    T* numerical_vals = new T[vals_size];
    T numerical_ans_val = 0;

    bool direct_string_return = false;
    std::string direct_return_string;

    std::string ans_val;
    for(std::vector<std::string>::const_iterator it=vals.begin();it<vals.end();++it) {
        try {
            if(T_name == TYPE_DOUBLE) {
                numerical_vals[it-vals.begin()] = std::stod(*it);
            }
            else if(T_name == TYPE_LONG) {
                numerical_vals[it-vals.begin()] = std::stol(*it);
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
                    return std::to_string(static_cast<long>(numerical_vals[0])
                                         %static_cast<long>(numerical_vals[1]));
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
    catch (const HUAJIBASE_EXCEPTION& e) {
        delete [] numerical_vals;
        throw e;
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
template std::string
hjbase::HUAJISCRIPTBASE::Numerical_Operation_Templated_Helper<long>(const std::string&, int, int, int, const const_itVecStr&);
template std::string
hjbase::HUAJISCRIPTBASE::Numerical_Operation_Templated_Helper<double>(const std::string&, int, int, int, const const_itVecStr&);

std::string hjbase::HUAJISCRIPTBASE::Numerical_Operation(const std::string& op, int op_key, const const_itVecStr& vals) {
    int vals_size = vals.size();
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

std::pair<std::string, bool>
hjbase::HUAJISCRIPTBASE::More_On_Slice_Operator_Level_1(const const_itVecStr& vals) {
    return std::pair<std::string, bool>(std::string(), false);
}

std::string
hjbase::HUAJISCRIPTBASE::Other_Basic_Operation(const std::string& op, int op_key, const const_itVecStr& vals) {
    if(!vals.size()) {
        Signal_Error(SE_ARITY_MISMATCH, op);
        throw syntax_except;
    }
    try {
        switch (op_key) {
            case eOP_AND: {
                for(std::vector<std::string>::const_iterator it=vals.begin();it<vals.end();++it) {
                    if(*it==BOOL_FALSE) {
                        return BOOL_FALSE;
                    }
                }
                return BOOL_TRUE;
            }        
            case eOP_OR: {
                for(std::vector<std::string>::const_iterator it=vals.begin();it<vals.end();++it) {
                    if(*it!=BOOL_FALSE) {
                        return BOOL_TRUE;
                    }
                }
                return BOOL_FALSE;
            }
            case eOP_NOT: {
                if(vals.size()!=1) {
                    Signal_Error(SE_ARITY_MISMATCH, vals);
                    throw eval_except;
                }
                if(vals.front()!=BOOL_FALSE) {
                    return BOOL_TRUE;
                }
                return BOOL_FALSE;
            }
            case eOP_EQ: {
                for(std::vector<std::string>::const_iterator it=vals.begin()+1;it<vals.end();++it) {
                    if(*(it-1)!=*it) {
                        return BOOL_FALSE;
                    }
                }
                return BOOL_TRUE;
            }
            case eOP_NE: {
                for(std::vector<std::string>::const_iterator it_first=vals.begin();it_first!=vals.end()-1;++it_first) {
                    for(std::vector<std::string>::const_iterator it_second=vals.begin()+1;it_second!=vals.end();++it_second) {
                        if(*it_first==*it_second) {
                            return BOOL_FALSE;
                        }
                    }
                }
                return BOOL_TRUE;
            }
            case eOP_STRAPP: {
                std::string ans_val;
                for(std::vector<std::string>::const_iterator it=vals.begin();it<vals.end();++it) {
                    ans_val.append(*it);
                }
                return ans_val;
            }
            case eOP_STRLEN: {
                if(vals.size()!=1) {
                    Signal_Error(SE_ARITY_MISMATCH, op);
                    throw eval_except;
                }
                return std::to_string(vals.at(0).size());
            }
            case eOP_SLICE: {                
                std::pair<std::string, bool> ret_pair =  More_On_Slice_Operator_Level_1(vals);
                if(ret_pair.second) {
                    return ret_pair.first;
                }
                else {
                    int vals_size = vals.size();
                    // Require at least two arguments
                    if(vals_size>1) {
                        std::string to_be_sliced = vals.at(0);
                        // get slice starting position
                        int slice_start = std::stoi(vals.at(1));
                        // three arguments indicate an ending position also provided
                        if(vals_size==3) {
                            int slice_end = std::stoi(vals.at(2));
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
                for(std::vector<std::string>::const_iterator it=vals.begin();it<vals.end();++it) {
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
                int vals_size = vals.size();
                if(vals_size==1) {
                    std::string ret_elem;
                    std::string this_list = vals.at(0);
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
                    for(std::vector<std::string>::const_iterator it=vals.begin();it<vals.end();++it) {
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
                if(vals.size()==1) {
                    // Need to exclude last quotation mark, so pass include_last_quotaion=true
                    std::pair<int, int> first_pos = Get_First_Element_Pos(vals.at(0));
                    return Format_List_String(vals.at(0).substr(first_pos.first+first_pos.second, std::string::npos));
                }
                else if(vals.size()>1) {
                    std::string ret_list = EXPR_START;
                    for(std::vector<std::string>::const_iterator it=vals.begin();it<vals.end();++it) {
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
                if(vals.size()!=2) {
                    Signal_Error(SE_ARITY_MISMATCH, op);
                    throw syntax_except;
                }
                // Put elem to cons at list front
                std::string ret_list;                       
                if(Quotation_Rquired_For_List_Elem(vals.at(0))) {
                    ret_list.push_back(QUOTATION_MARK);
                    ret_list.append(vals.at(0));
                    ret_list.push_back(QUOTATION_MARK);
                }
                else {
                    ret_list = vals.at(0);
                }
                /* 
                    Append second arg to ret_list, note here we don't check if second arg
                    is a valid list or not, if not, exception will thrown in Format_List_String
                */
                ret_list.push_back(' ');
                ret_list.append(vals.at(1).substr(1, std::string::npos));

                return Format_List_String(ret_list);
            }
            case eOP_APPEND: {
                std::string ret_list;
                for(std::vector<std::string>::const_iterator it=vals.begin();it<vals.end();++it) {
                    if((*it).front()!=cEXPR_START&&(*it).back()!=cEXPR_END) {
                        Signal_Error(TE_NOT_LIST, *it);
                        throw type_except;
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
                if(vals.size()!=1) {
                    Signal_Error(SE_ARITY_MISMATCH, vals);
                    throw syntax_except;
                }

                return Handle_Val(vals.at(0));
            }
            default: {
                Signal_Error(IE_UNKNOWN, op);
                throw huaji_except;
            }
        }
    }
    catch (const HUAJIBASE_EXCEPTION& e) {
        throw eval_except;
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

std::pair<std::string, bool>
hjbase::HUAJISCRIPTBASE::More_On_Expression_Level_1(const std::string& op, const const_itVecStr& vals) {
    return std::pair<std::string, bool>(std::string(), false);
}

std::string hjbase::HUAJISCRIPTBASE::Apply_Function(const std::string& op, const const_itVecStr& vals) {
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
        std::pair<std::string, bool> ret_pair = More_On_Expression_Level_1(op, vals);
        if(ret_pair.second) {
            return ret_pair.first;
        }
    }
    // FUNC execution
    std::string func_ptrStr = Resolve_Name(op);

    FUNC* func_ptr = nullptr;
    std::map<std::string, FUNC*>::const_iterator func_ptr_it = func_pool->find(func_ptrStr);
    if(func_ptr_it==func_pool->end()) {
        Signal_Error(IE_INTERNAL_QUERY_FAILED, func_ptrStr);
        throw huaji_except;
    }
    else {
        func_ptr = (*func_ptr_it).second;
    }

    // Invoke Side_Effects_Guard if the function has side effects
    if(func_ptr->has_side_effects) {
        Side_Effects_Guard(op);
    }

    // Allocate new stack and pushed to names_stack
    std::map<std::string, std::string>* preset_stack_frame
        = func_ptr->Get_Stack_Frame_Template_Copy_On_Heap();
    std::list<std::string> var_names = func_ptr->Get_Var_Names();
    if(var_names.size()!=vals.size()) {
        Signal_Error(SE_ARITY_MISMATCH, vals);
        throw syntax_except;
    }

    // Variable substitution
    int i = 0;
    for(std::list<std::string>::const_iterator name_it=var_names.begin();
        name_it!=var_names.end();++name_it) {
        (*preset_stack_frame)[*name_it] = vals.at(i);
        ++i;
    }

    std::string return_val;

    Push_Stack_Frame(preset_stack_frame);

    if(func_ptr->has_side_effects) {
        if(Block_Execution(func_ptr->Get_Fbody())==EXIT_RETURN_CODE) {
            return_val = temp_return_val;
        }
        else {
            return_val = UNDEFINED_TYPE;
        }
    }
    else {
        return_val = Evaluate_Expression(func_ptr->Get_Fbody());
    }

    Pop_Stack_Frame();

    return return_val;
}

std::pair<std::string, bool> 
hjbase::HUAJISCRIPTBASE::More_On_Names_Query_Level_1(const std::string& name) {
    return std::pair<std::string, bool>(std::string(), false);
}

std::string hjbase::HUAJISCRIPTBASE::Handle_Val(const std::string& name_or_val) {
    if(enable_raw_string) {
        if(name_or_val.size()>1) {
            if(name_or_val.front()=='$'&&name_or_val.back()=='$') {
                std::string name = name_or_val.substr(1,name_or_val.size()-2);
                std::pair<std::string, bool> ret_pair =  More_On_Names_Query_Level_1(name);
                if(ret_pair.second) {
                    return ret_pair.first;
                }
                else {
                    return Resolve_Name(name);
                }
            }        
            else if(Starts_With(name_or_val, LIST_TAG)) {
                return name_or_val.substr(LIST_TAG.size(),std::string::npos);
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
            std::pair<std::string, bool> ret_pair =  More_On_Names_Query_Level_1(name_or_val);
            if(ret_pair.second) {
                return ret_pair.first;
            }
            else {
                return Resolve_Name(name_or_val);
            }
        }
    }
}