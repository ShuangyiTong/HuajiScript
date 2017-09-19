#lang scribble/manual

@(define-syntax-rule (samplemod . text) (codeblock . text))

@(define-syntax-rule (cr . text) (racketfont . text))

@(define-syntax-rule (ci . text) (racketvarfont . text))

@title[#:tag "function"]{Functions}

Functions are first class values in Huaji script. A function is a string with its value as the key, maps to
a @litchar{hjbase::FUNC} object pointer. In Huaji script, functions are divided into two types based on with 
or without side effects.

@local-table-of-contents[]

@section[#:tag "define"]{define@ci{    command}}

@cr{define @ci{flag func-name}(@ci{name}, @ci{name}=@ci{expr} ...) @ci{block}}

Defines a function with side effects

@cr{define @ci{flag} (@ci{func-name name} ...) <@ci{env}> @ci{expr};}

Defines a function without side effects

@(ci "flag") =  @cr{-}@cr{-overload} @cr{|| -}@cr{-lazy}

@cr{-}@cr{-overload} means the function is previously defined, and now it will be overloaded. Internally, if @cr{-}@cr{-overload}
flag is not provided, the interpreter use the same function as in @cr{declare} command. Otherwise, the interpreter use the same function
as in @cr{mutate} command.

@cr{-}@cr{-lazy} defines the function is lazy.

@ci{env} = @ci{name}@cr{,} @ci{name}@cr{=}@ci{expr} @cr{...}

Add extra environment variables when the function gets exectued.

@italic{Examples}:

@samplemod|{
    define fact(n, fact/r) {
        if(= n 1) {
            return 1;
        }
        return (* n (fact/r (- n 1) fact/r));
    }

    (fact 5 fact) => 120
}|

General recursion

@samplemod|{
    define make-fact/r(n) {
        define fact-rr(fact/r, cur=1, n=n) {
            if(< cur n) {
                return (* cur (fact/r fact/r (+ cur 1)));
            }
            return n;
        }
        define fact-n(fact/r=fact-rr, cur=1, n=n) {
            if(< cur n) {
                return (* cur (fact/r fact/r (+ cur 1)));
            }
            return n;
        }
        return fact-n;
    }

    declare fact/r5=(make-fact/r 5);

    (fact/r5) => 120

}|

Returns a function (closure)

@samplemod|{

    define (cons2 first second rest) (
        cons first (cons second rest)
    );

    (cons2 1 2 `(3)) => (1 2 3)
}|

Defines a function without side effects

@samplemod|{

    define make-adder(op1) {
        define (adder op2) <n=op1> (
            + n op2
        );
        return adder;
    }

    declare add10 = (make-adder 10);

    (add10 2) => 12
}|

Use @ci{env}

@section[#:tag "return"]{return@ci{    command}}

@cr{return} @ci{expr}@cr{;}

return a value. @ci{expr} is required. If not in a function call, the interpreter will exit.

@section[#:tag "lazy"]{lazy@ci{    func}}

@cr{(lazy @ci{lazy-val})}

When a function is defined as lazy, all its arguments don't get evaluated immediately. Arguments value are changed to keys
map to @litchar{const_itVecStr} object pointers. Use @ci{lazy} will evaluate this key string. 

This is very different from lazy evaluation in other languages, which automatically determines when evaluation is requried for an @ci{expr}.
Let users specify when an @ci{expr} needs to be evaluated makes it much easier to implement.

@italic{Example}:

@samplemod|{

    define --lazy (make-lazy expr) expr;

    declare true = (lambda (t f) (
        lazy t
    ));

    (true (make-lazy 1) (make-lazy (/ 1 0))) => 1 
}|

@section[#:tag "lambda"]{lambda@ci{    func}}

@cr{(lambda @ci{flag} (@ci{name} ...) <@ci{env}> @ci{expr})}

@(ci "flag") =  @cr{-}@cr{-lazy}

@ci{env} = @ci{name}@cr{,} @ci{name}@cr{=}@ci{expr} @cr{...}

@cr{lambda} syntax is similar to @cr{define} no side effects function, and internally they are the same thing. The only difference is a function
defined with @cr{lambda} don't need to bind to a @ci{name}, so there is no @cr{-}@cr{-overload} flag option.

@italic{Example}:

@samplemod|{

    ((((lambda (x) 
            (lambda (y) <x=x> 
                (lambda (z) <x=x, y=y> (strapp x y z)))) 
            "this is x, ")
        "this is y, ") 
    "this is z.")

    => this is x, this is y, this is z.
}|