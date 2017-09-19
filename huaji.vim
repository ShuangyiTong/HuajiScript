" Vim syntax file
" Language: Huaji Script
" Maintainer: Shuangyi Tong s9tong@edu.uwaterloo.ca
" Latest Revision: 4 September 2017

" Not complete yet

if exists("b:current_syntax")
	finish
endif

" Commands
syn keyword huajiBaseCMD declare mutate to print if else while define init delete return source exec config info

" Integer with - +
syn match huajiNumber '\d\+'
syn match huajiNumber '[-+]\d\+'

" Floating point number with decimal 
syn match huajiNumber '[-+]\d\+\.\d*'

" Built-in functions
syn keyword builtinFunctions strapp strlen slice "list" "first" rest cons append resolve lambda "return-val" stack-pos init "mem-ref" lazy input output "read" "write"

" Quotation
syn keyword escapeChar \n \t \r \" contained
syn region huajiString start="\"" end="\"" contains=escapeChar

" No substitution block
syn region huajiNosubst start="#" end="#"

" Comments
syn match huajiInlineComment "//.*$"
syn region huajiBlockComment start="/\*" end="\*/" fold

" Env
syn region huajiEnv start="<" end=">" transparent 

" Expression
syn region huajiExpr start="(" end=")" fold transparent

" Commands Block
syn region huajiBlock start="{" end="}" fold transparent

let b:current_syntax = "huaji"

hi def link huajiBaseCMD Statement 
hi def link huajiNumber Number 
hi def link huajiString String
hi def link builtinFunctions Function 
hi def link escapeChar Character
hi def link huajiInlineComment Comment
hi def link huajiBlockComment Comment
hi def link huajiNosubst Type
