#lang scribble/manual

@(define-syntax-rule (samplemod . text) (codeblock . text))

@(define-syntax-rule (cr . text) (racketfont . text))

@(define-syntax-rule (ci . text) (racketvarfont . text))

@title[#:tag "basics"]{Basics}

Huaji scripts consisted of commands and are parsed dynamically. Besides @ci{command},
another two ast nodes are @ci{expr} and @ci{block}.

@ci{command} = @ci{(}@cr{ ...; || ...} @ci{block )}

All @ci{command}s should end with @(litchar ";") or @ci{block}.

@ci{block} = @cr{ { ... }}

All @ci{block}s should start with @(litchar "{") and end with @(litchar "}").

@ci{expr} = @ci{(}@cr{ (@ci{func} ...) || @ci{(name} or @ci{val) )}}

Expression can start with @(litchar "("), and has to end with @(litchar ")"), or just a @ci{name} or @ci{val}.

@table-of-contents[]

@section[#:tag "declare"]{declare@ci{    command}}
@(cr "declare") @(ci "name") @(cr ",")  @(ci "name") @(cr "=") @(ci "expr") @(cr ", ...;")

Declare names, use a comma to separate two names. 
If no expr is supplied, the name will be assigned a value of @(litchar "#:undefined") (e.g. @cr{hjbase::cnt::type_tag::UNDEFINED_TYPE}). 
Error will be signaled for name already declared.

@italic{Example}:

@samplemod|{
    declare a, b=1, c="value-of-c";
}|

@section[#:tag "mutate"]{mutate@ci{    command}}
@(cr "mutate") @(ci "name") @(cr "to") @(ci "expr")@(cr ";")

Mutate value bind to the name, one at a time. If the name has not declared yet, the interpreter will signal error.

@italic{Example}:

@samplemod|{
    mutate i to (+ i 1);
}|

@section[#:tag "print"]{print@ci{    command}}
@(cr "print") @(ci "flag (name") @(cr "or") @(ci "expr)")@cr{;}

@(ci "flag") = @cr{-}@cr{-nonl} @cr{|| -}@cr{-expr}

Print to console output. @cr{-}@cr{-nonl} disables printing new line character.
If users want to print @ci{expr}, it has to be led by @cr{-}@cr{-expr} flag.

@italic{Example}:

@samplemod|{
    declare file_name="doc/huaji/index.html", str1="6.9", str2="1.062";
    print "replacing string \"" str1 "\" with \"" str2 "\" in " file_name "...";

    OvO >>> replacing string "6.9" with "1.062" in doc/huaji/index.html...
}|

use @ci{expr}

@samplemod|{
    declare lst6="(        \"I have space\" 2 3)";
    print --expr (first lst6);

    OvO >>> I have space
}|

@section[#:tag "if"]{if@ci{    command}}
@cr{if} @ci{expr block}

Evaluate @ci{expr}, return value is considered true except @litchar{0}.
If return value is true, execute the @ci{block}.

@italic{Example}:

@samplemod|{
    if "1" {
        print "true";
    }

    OvO >>> true
}|

@section[#:tag "elif"]{elif@ci{    command}}
@cr{elif} @ci{expr block}

Same as @cr{if} command, but if the closest (last evaluated) @cr{if} or @cr{elif} condition is true,
this command is skipped.

@italic{Example}:

@samplemod|{
    if "0" {
        print "true";
    }
    elif 1 {
        print "false";
    }

    OvO >>> false;
}|

@section[#:tag "else"]{else@ci{    command}}
@cr{else} @ci{block}

Same as @cr{elif}, but no condition @ci{expr} required.

@italic{Example}:

@samplemod|{
    if (- 1 1) {
        print "true";
    }
    elif 0 {
        print "true";
    }
    else {
        print "false";
    }

    OvO >>> false;
}|

@section[#:tag "while"]{while@ci{    command}}
@cr{while} @ci{expr block}

Same as @cr{if}, but after executing the block, the interpreter will evaluate @ci{expr} again, until @ci{expr} evaluates to @litchar{0}.

@italic{Example}:

@samplemod|{
    declare x=0;
    while(< x 1000000) {
        mutate x to (+ x 1);
    }
    print "x=" x;

    OvO >>> x=1000000
}|

@section[#:tag "exec"]{exec@ci{    command}}
@cr{exec} @ci{(name} @cr{or} @ci{val)}@cr{;}

Call system shell command, implemented with @cr{system} defined in @racketmodname{stdlib.h}

@section[#:tag "arithemetic operators"]{Arithemetic Operators@ci{    func}}

When the interpreter applies numerical operations, it first converts strings to numerical values.
If a string has a decimal mark @litchar{.}, then the string will be converted to @litchar{double}, using @litchar{std::stod}.
Otherwise, the string will be converted to @litchar{long}, using @litchar{std::stol}.
The result will be converted back to string using @litchar{std::to_string}.

@cr{(+ @ci{numerical-val} ...)} 

Addition

@italic{Example}:

@samplemod|{
    (+ 123 456 789) => 1386
}|

@cr{(- @ci{numerical-val} ...)} 

Substraction

@italic{Example}:

@samplemod|{
    (- 123 456 789) => -1122
}|

@cr{(* @ci{numerical-val} ...)} 

Multiplication

@italic{Example}:

@samplemod|{
    (* 123 456 789) => 44253432
}|

@cr{(/ @ci{numerical-val} ...)} 

Division

@italic{Example}:

@samplemod|{
    (/ 123 456 789.0) => 0.000342
}|

@cr{(mod @ci{numerical-val} @ci{numerical-val})}

Modulo operation

@italic{Example}:

@samplemod|{
    (mod 44253435 456) => 3
}|

@section[#:tag "order relation"]{Order Relations@ci{    func}}

@cr{(<= @ci{numerical-val} ...)}

@cr{(>= @ci{numerical-val} ...)}

@cr{(< @ci{numerical-val} ...)}

@cr{(> @ci{numerical-val} ...)}

All these relation operators return string @litchar{1} if true, @litchar{0} if false.

@italic{Examples}:

@samplemod|{
    (>= 44253435 456 456) => 1

    (<= 123 122) => 0

    (> 123 122 124) => 0

    (< 1 2 3 4 5 6 7 8) => 1
}|

@section[#:tag "resolve"]{resolve@ci{  func}}

@cr{(resolve @ci{name})}

Resolve name.

@italic{Examples}:

@samplemod|{
    declare a=10;

    (resolve "a") => 10
}|

@section[#:tag "comments"]{Comments}

@cr{//}

Comment text on this line after this mark.

@cr{/* ... */}

Comment all text in this block.

@italic{Examples}:

@samplemod|{
    // line comments
    
    /*
        block comments
                        */
}|