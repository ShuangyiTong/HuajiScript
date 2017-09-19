#lang scribble/manual

@(define-syntax-rule (samplemod . text) (codeblock . text))

@(define-syntax-rule (cr . text) (racketfont . text))

@(define-syntax-rule (ci . text) (racketvarfont . text))

@title[#:tag "oop-scope"]{OOP and Scope}

Huaji script provides extended support on namespace system to approximate OOP features. A namespace does not bind to 
any name in any variables scope, @ci{namespace-name} can be accessed anywhere in the script after it initialized.
An object uses the same implementation as namespace @litchar{std::map<std::string, std::string>}, but is a 
first-class value, and can be deleted. In Huaji script, there is no class, object are created using copy constructor on namespace.

@local-table-of-contents[]

@section[#:tag "init-command"]{init@ci{     command}}

@cr{init @ci{namespace-name};}

Create an empty namespace with the name @ci{namespace-name}.

@italic{Example}:

@samplemod|{
    init Vec2;
}|

Initialize a namespace called @cr{Vec2}.

@cr{init @ci{child-namespace-name parent-namespace};}

Use copy contructor @litchar{std::map::map} on @ci{parent-namespace} to create a new namespace, and bind it to @ci{child-namespace-name}

@italic{Example}:

@samplemod|{
    init 2D-pos Vec2;
}|

Initialize a namespace called @cr{2D-pos} copied from @cr{Vec2}. @cr{Vec2} is empty, so @cr{2D-pos} is empty too.

@section[#:tag "init-object"]{init@ci{     func}}

@cr{(init @ci{namespace-name})}

Return an object constructed by applying copy constructor on @ci{namespace-name}. It is identical internally,
but an object is a first-class value.

@italic{Example}:

@samplemod|{
    declare pointA = (init "Vec2");
}|

Notice here @cr{Vec2} is in quotation mark, because it is neither a @ci{name} nor a @ci{val}. Using quotation marks
make it a @ci{val}.

@section[#:tag "at" "@"]

@ci{name-in-namespace}@cr["@"]@ci{namespace-name}

Separator for accessing variables in a namespace.

@section[#:tag "mem-ref"]{mem-ref@ci{     func}}

@cr{(men-ref @ci{name-in-object-scope} @ci{object-name})}

Get the @ci{val} bind to the @ci{name-in-object-scope}.

@italic{Example}:

@samplemod|{
    init constant;
    declare pi@constant = 3.14, e@constant = 2.7;
    declare cnt_obj = (init "constant");

    e@constant => 2.7
    (mem-ref "pi" cnt_obj) => 3.14
}|

Notice here we have to use quotation mark agains, because "mem-ref" arguments get evaluated first, and it's not in the scope.