#lang scribble/manual

@(define-syntax-rule (samplemod . text) (codeblock . text))

@(define-syntax-rule (cr . text) (racketfont . text))

@(define-syntax-rule (ci . text) (racketvarfont . text))

@title[#:tag "array-title"]{Array}

Array is implemented with @litchar{std::vector<std::string>}. 

In all examples in this array section, previous executed code has effects on later code.

@section[#:tag "array-new"]{array-new@ci{     func}}

@cr{(array-new)}

Returns a new, empty @ci{array-val}

@italic{Example}:

@samplemod|{
    declare test-array=(array-new);
}|

@section[#:tag "array-size"]{array-size@ci{     func}}

@cr{(array-size @ci{array-val})}

Returns size of @ci{array-val}.

@italic{Example}:

@samplemod|{
    (array-size test-array) => 0
}|

@section[#:tag "array-ref"]{array-ref@ci{     func}}

@cr{(array-ref @ci{array-val numerical-val})}

Returns @ci{numerical-val}@superscript{th} element in @ci{array-val}

@section[#:tag "array"]{array@ci{     command}}

@cr{array push_back @ci{array-expr} @ci{elem-expr};}

Push one element at the back of @ci{array-val} evaluated from the first @ci{array-expr}, implemented with @litchar{std::vector::push_back}.

@italic{Example}:

@samplemod|{
    array push_back test-array (+ 5 5);

    (array-ref test-array 0) => 10
}|

@cr{array insert @ci{array-expr} @ci{pos-expr} @ci{elem-expr};}

Insert one element in front of @ci{pos}, evaluated from @ci{pos-expr}, into @ci{array-val}, evaluated from the first @ci{array-expr}.
Implemented with @litchar{std::vector::insert}.

@italic{Example}:

@samplemod|{
    array insert test-array (- 1 1) (+ 5 2);

    (array-ref test-array 0) => 7
}|

@cr{array mutate  @ci{array-expr} @ci{pos-expr} @ci{elem-expr};}

Mutate value stored at @ci{pos} in @ci{array-val} to @ci{val}, evaluated from @ci{elem-expr}.

@italic{Example}:

@samplemod|{
    array mutate test-array 0 5;

    (array-ref test-array 0) => 5
}|

@cr{array erase @ci{array-expr} @ci{pos-expr};}

Erase @ci{pos}@superscript{th} element in @ci{array-val}, evaluated from the first @ci{array-expr}.
Implemented with @litchar{std::vector::erase}.

@italic{Example}:

@samplemod|{
    array erase test-array (- 1 1);

    (array-ref test-array 0) => 10
}|

@cr{array pop_back @ci{array-expr};}

Pop last element out. Implemented with @litchar{std::vector::pop_back}.

@italic{Example}:

@samplemod|{
    (array-size test-array) => 1

    array pop_back test-array;

    (array-size test-array) => 0
}|