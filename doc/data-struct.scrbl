#lang scribble/manual

@(define-syntax-rule (samplemod . text) (codeblock . text))

@(define-syntax-rule (cr . text) (racketfont . text))

@(define-syntax-rule (ci . text) (racketvarfont . text))

@title[#:tag "data-struct"]{Built-in Data Structures}

@local-table-of-contents[]

@section[#:tag "slice"]{slice@ci{     syntax sugar}}

@ci{slice} is designed to be a common method to multiple data structures. Huaji script provides a syntax sugar for @ci{slice} function.

@ci{name}@cr{[@ci{1st-expr}:@ci{2nd-expr} ...]}

@cr{=> (slice @ci{name 1st-expr 2nd-expr} ...)}

@italic{Example}:

@samplemod|{
    declare str="123456";

    str[0:-1] => 123456
}|

@include-section["string.scrbl"]
@include-section["list.scrbl"]
@include-section["array.scrbl"]