#lang scribble/manual

@(define-syntax-rule (samplemod . text) (codeblock . text))

@(define-syntax-rule (cr . text) (racketfont . text))

@(define-syntax-rule (ci . text) (racketvarfont . text))

@title[#:tag "list"]{List}

Built-in list is just a string, and all functions related to it are string operations. List members are separated by 
space. Use quotation mark can group list elements. @litchar{(...)} represents sublist. After applying any list functions,
the string gets formatted using @litchar{hjbase::LISTFORMMATER}. Applying list functions to all strings that satisfy
list format is valid. 

In @litchar{lib/std}, we provide another list implementation using lambda.

In all examples in this list section, previous executed code has effects on later code.

@section[#:tag "list-construction"]{List Construction@ci{     syntax sugar}}

@cr{`(...) => (...)}

Construct a list without substitution or evaluation.

@italic{Example}:

@samplemod|{
    declare lst1=`(1   2 3 4 5 6), lst2=`( 1 2 4   5 6   "a b   c"ab   );

    lst1 => (1 2 3 4 5 6)

    lst2 => (1 2 4 5 6 "a b   c" ab)
}|

@cr{’(...) => (list ...)}

Apply function @cr{list}

@italic{Example}:

@samplemod|{
    declare lst3='(1 (+ 1 1) (+ 1 2) (+ 2 3)), 
            lst4='("abc123"   4562 (* 3 4.5) (strapp "abc " "efg"));

    lst3 => (1 2 3 5)

    lst4 => (abc123 4562 13.500000 "abc efg")
}|

@section[#:tag "list-func"]{list@ci{     func}}

@cr{(list ...)}

Evaluate @cr{...}, then construct list.

@italic{Example}:

@samplemod|{
    declare lst5=(list (list (+ 1 1)));

    lst5 => ((2))
}|

@section[#:tag "first"]{first@ci{     func}}

@cr{(first @ci{list-val} ...)}

Returns the first element in the list if only one @ci{list-val} supplied. Returns a list of all first elements in @ci{list-val}s if more than
one @ci{list-val}s provided.

@italic{Examples}:

@samplemod|{
    (first (first lst5)) => 2

    declare lst6="(        \"I have space\" 2 3)";

    (first lst1 lst2 lst3 lst4 lst5 lst6) => (1 1 1 abc123 (2) "I have space")
}|

@cr{lst1, lst2 ...}'s definition are reused here

@section[#:tag "rest"]{rest@ci{     func}}

@cr{(rest @ci{list-val} ...)}

Returns the rest of the list except the first element. Same as @cr{first}, can return a list of rest lists if more than one @ci{list-val}s supplied.

@italic{Examples}:

@samplemod|{
    (rest lst5) => ()

    (rest lst6) => (2 3)

    (rest lst1 lst2 lst3 lst4 lst5 lst6)
    => ((2 3 4 5 6) (2 4 5 6 "a b   c" ab) (2 3 5) (4562 13.500000 "abc efg") () (2 3))
}|

@section[#:tag "cons"]{cons@ci{     func}}

@cr{(cons @ci{val list-val})}

Add one element to the front of the list.

@italic{Examples}:

@samplemod|{
    (cons 10 lst1) => (10 1 2 3 4 5 6)

    (cons " I   have spaces " lst6) => (" I   have spaces " "I have space" 2 3)

    (cons lst1 lst2) => ((1 2 3 4 5 6) 1 2 4 5 6 "a b   c" ab)
}|

@section[#:tag "append"]{append@ci{     func}}

@cr{(append @ci{list-val ...})}

Append lists.

@italic{Example}:

@samplemod|{
    (append lst1 lst2 lst3 lst4 lst5 lst6)
    => (1 2 3 4 5 6 1 2 4 5 6 "a b   c" ab 1 2 3 5 abc123 4562 13.500000 "abc efg" (2) "I have space" 2 3)
}|
