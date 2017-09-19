#lang scribble/manual

@(define-syntax-rule (samplemod . text) (codeblock . text))

@(define-syntax-rule (cr . text) (racketfont . text))

@(define-syntax-rule (ci . text) (racketvarfont . text))

@title[#:tag "string"]{String}

Strings are the fundemental data type in Huaji script. @ci{val}s are strings, and they might link to
internal data structure. Huaji script provides built-in basic string functions to help user manipulate strings.

@section[#:tag "strapp"]{strapp@ci{     func}}

@cr{(strapp @ci{val} ...)}

@italic{Examples}:

Append strings.

@samplemod|{
    (strapp "append " "strings") => append strings
}|

@section[#:tag "strlen"]{strlen@ci{     func}}

@cr{(strlen @ci{val})}

Returns string length.

@italic{Examples}:

@samplemod|{
    (strlen "length ") => 7
}|

@section[#:tag "slice:string"]{slice:string@ci{     func}}

@cr{(slice @ci{val} @ci{numerical-val})}

Returns @ci{numerical-val}@superscript{th} @ci{char} in the string. Here @ci{char} means a string with only a single character.
If @ci{val} is a negative number, then the interpreter counts from back. @cr{-1} is the pos of the last @ci{char} in the string.

@italic{Examples}:

@samplemod|{
    (slice "abcde" 2) => c

    (slice "abcde" -1) => e
}|

@cr{(slice @ci{val} @ci{startpos-val} @ci{endpos-val})}

Returns a substring starts from @ci{startpos-val}, ends at @ci{endpos-val}. End point is excluded (e.g. [@ci{startpos-val}, @ci{endpos-val}))

@italic{Examples}:

@samplemod|{
    (slice "abcdefghijk" 2 5) => cde

    (slice "abcde" 0 -1) => abcde
}|