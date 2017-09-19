# Huaji Script: A Simple Modern Language Interpreter

by Shuangyi Tong <[s9tong@uwaterloo.ca](mailto:s9tong@uwaterloo.ca)>

Version 1.062 - Documentation for this version is not fully completed

This file is generated by scribble. For better viewing experience, switch to html view
[HTML-Documentation](http://htmlpreview.github.com/?https://github.com/ShuangyiTong/HuajiScript/blob/master/doc/huaji/index.html)

To build HTML documentation, run the Huaji Interpreter on a Huaji script
```
huaji doc/build-doc.hjs
```

Huaji script language is designed to be an extremely simple,
customizable, and flexible modern language interpreter based on C++.

**Motivation**: I wrote its primitive version to support my LAMSTAR
network’s \(An other project you can found under my
[repository](https://github.com/ShuangyiTong/LAMSTAR)\) training logic.
If you have some C/C++ program and want to control those function calls,
you can use tools provided by famous language interpreters like ctypes
in Python, which obviously most people will choose this way. Or you can
use a simple interpreter and compile the interpreter with your program,
and this is what this language interpreter is for!

**Features**: I think this language interpreter is not aesthetically
beautiful by any means. It’s a toy created by a second-year
undergraduate student who is not in CS major in two months. But for
myself, I was enjoyed in writing an interpreter solely designed by
myself. Some features listed here are not developed intentionally, but
bascially they all contributed to simpler implementations \(at least for
now\).

* Everything is a string, even internally

  I was working with Tcl everyday because of my job when I started
  writing this interpreter. Tcl says eveything is represented by string
  in Tcl, and I take this idea. The different thing is, internally,
  values in Tcl might not be a string, but Huaji script is. All values
  are represented by `std::string`. There are indeed problems with this
  kind of implementation.

* ASTs are generated dynamically

  There is no independent parsing function, which generates an entire
  AST for execution. ASTs are generated dynamically. This makes Huaji
  script slow, but users get full control over command syntax.
  `hjbase::HUAJISCRIPTBASE` are designed to be inherited, that’s how
  users can combine their program with Huaji script interpreter.

* OOP

  OOP features in Huaji script are very primitive. Compared to other
  object-oriented languages, OOP in Huaji script is just a little
  extension on namespace system.

* Functional Programming

  Huaji script provides some functional programming features, including
  lazy functions, closures etc.

**Name Origin**: Huaji \(Chinese: 滑稽\) literally means funny in Chinese.
It’s meaning has changed becuase of a popular sticker on some Chinese
forums, you can go to [百度贴吧](https://tieba.baidu.com) to find out what
exactly the word means.

    1 Basics                                      
      1.1 declare`    command`                    
      1.2 mutate`    command`                     
      1.3 print`    command`                      
      1.4 if`    command`                         
      1.5 elif`    command`                       
      1.6 else`    command`                       
      1.7 while`    command`                      
      1.8 exec`    command`                       
      1.9 Arithemetic Operators`    func`         
      1.10 Order Relations`    func`              
      1.11 resolve`  func`                        
      1.12 Comments                               
                                                  
    2 Functions                                   
      2.1 define`    command`                     
      2.2 return`    command`                     
      2.3 lazy`    func`                          
      2.4 lambda`    func`                        
                                                  
    3 Built-in Data Structures                    
      3.1 slice`     syntax sugar`                
      3.2 String                                  
        3.2.1 strapp`     func`                   
        3.2.2 strlen`     func`                   
        3.2.3 slice:string`     func`             
      3.3 List                                    
        3.3.1 List Construction`     syntax sugar`
        3.3.2 list`     func`                     
        3.3.3 first`     func`                    
        3.3.4 rest`     func`                     
        3.3.5 cons`     func`                     
        3.3.6 append`     func`                   
      3.4 Array                                   
        3.4.1 array-new`     func`                
        3.4.2 array-size`     func`               
        3.4.3 array-ref`     func`                
        3.4.4 array`     command`                 
                                                  
    4 OOP and Scope                               
      4.1 init`     command`                      
      4.2 init`     func`                         
      4.3 @                                       
      4.4 mem-ref`     func`                      
                                                  
    Index                                         

## 1. Basics

Huaji scripts consisted of commands and are parsed dynamically. Besides
`command`, another two ast nodes are `expr` and `block`.

`command` = `(` `...; || ...` `block )`

All `command`s should end with `;` or `block`.

`block` =  `{ ... }`

All `block`s should start with `{` and end with `}`.

`expr` = `(` `(func ...) || (name or val) )`

Expression can start with `(`, and has to end with `)`, or just a `name`
or `val`.

    1.1 declare`    command`           
    1.2 mutate`    command`            
    1.3 print`    command`             
    1.4 if`    command`                
    1.5 elif`    command`              
    1.6 else`    command`              
    1.7 while`    command`             
    1.8 exec`    command`              
    1.9 Arithemetic Operators`    func`
    1.10 Order Relations`    func`     
    1.11 resolve`  func`               
    1.12 Comments                      

### 1.1. declare`    command`

`declare` `name` `,`  `name` `=` `expr` `, ...;`

Declare names, use a comma to separate two names. If no expr is
supplied, the name will be assigned a value of `#:undefined` \(e.g.
`hjbase::cnt::type_tag::UNDEFINED_TYPE`\). Error will be signaled for
name already declared.

_Example_:

```racket
declare a, b=1, c="value-of-c";
```

### 1.2. mutate`    command`

`mutate` `name` `to` `expr;`

Mutate value bind to the name, one at a time. If the name has not
declared yet, the interpreter will signal error.

_Example_:

```racket
mutate i to (+ i 1);
```

### 1.3. print`    command`

`print` `flag (name` `or` `expr);`

`flag` = `--nonl` `|| --expr`

Print to console output. `--nonl` disables printing new line character.
If users want to print `expr`, it has to be led by `--expr` flag.

_Example_:

```racket
declare file_name="doc/huaji/index.html", str1="6.9", str2="1.062";         
print "replacing string \"" str1 "\" with \"" str2 "\" in " file_name "...";
                                                                            
OvO >>> replacing string "6.9" with "1.062" in doc/huaji/index.html...      
```

use `expr`

```racket
declare lst6="(        \"I have space\" 2 3)";
print --expr (first lst6);                    
                                              
OvO >>> I have space                          
```

### 1.4. if`    command`

`if` `expr block`

Evaluate `expr`, return value is considered true except `0`. If return
value is true, execute the `block`.

_Example_:

```racket
if "1" {         
    print "true";
}                
                 
OvO >>> true     
```

### 1.5. elif`    command`

`elif` `expr block`

Same as `if` command, but if the closest \(last evaluated\) `if` or
`elif` condition is true, this command is skipped.

_Example_:

```racket
if "0" {          
    print "true"; 
}                 
elif 1 {          
    print "false";
}                 
                  
OvO >>> false;    
```

### 1.6. else`    command`

`else` `block`

Same as `elif`, but no condition `expr` required.

_Example_:

```racket
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
```

### 1.7. while`    command`

`while` `expr block`

Same as `if`, but after executing the block, the interpreter will
evaluate `expr` again, until `expr` evaluates to `0`.

_Example_:

```racket
declare x=0;            
while(< x 1000000) {    
    mutate x to (+ x 1);
}                       
print "x=" x;           
                        
OvO >>> x=1000000       
```

### 1.8. exec`    command`

`exec` `(name` `or` `val);`

Call system shell command, implemented with `system` defined in
`"stdlib.h"`

### 1.9. Arithemetic Operators`    func`

When the interpreter applies numerical operations, it first converts
strings to numerical values. If a string has a decimal mark `.`, then
the string will be converted to `double`, using `std::stod`. Otherwise,
the string will be converted to `long`, using `std::stol`. The result
will be converted back to string using `std::to_string`.

`(+ numerical-val ...)`

Addition

_Example_:

```racket
(+ 123 456 789) => 1386
```

`(- numerical-val ...)`

Substraction

_Example_:

```racket
(- 123 456 789) => -1122
```

`(* numerical-val ...)`

Multiplication

_Example_:

```racket
(* 123 456 789) => 44253432
```

`(/ numerical-val ...)`

Division

_Example_:

```racket
(/ 123 456 789.0) => 0.000342
```

`(mod numerical-val numerical-val)`

Modulo operation

_Example_:

```racket
(mod 44253435 456) => 3
```

### 1.10. Order Relations`    func`

`(<= numerical-val ...)`

`(>= numerical-val ...)`

`(< numerical-val ...)`

`(> numerical-val ...)`

All these relation operators return string `1` if true, `0` if false.

_Examples_:

```racket
(>= 44253435 456 456) => 1
                          
(<= 123 122) => 0         
                          
(> 123 122 124) => 0      
                          
(< 1 2 3 4 5 6 7 8) => 1  
```

### 1.11. resolve`  func`

`(resolve name)`

Resolve name.

_Examples_:

```racket
declare a=10;      
                   
(resolve "a") => 10
```

### 1.12. Comments

`//`

Comment text on this line after this mark.

`/* ... */`

Comment all text in this block.

_Examples_:

```racket
// line comments      
                      
/*                    
    block comments    
                    */
```

## 2. Functions

Functions are first class values in Huaji script. A function is a string
with its value as the key, maps to a `hjbase::FUNC` object pointer. In
Huaji script, functions are divided into two types based on with or
without side effects.

    2.1 define`    command`
    2.2 return`    command`
    2.3 lazy`    func`     
    2.4 lambda`    func`   

### 2.1. define`    command`

`define flag func-name(name, name=expr ...) block`

Defines a function with side effects

`define flag (func-name name ...) <env> expr;`

Defines a function without side effects

`flag` =  `--overload` `|| --lazy`

`--overload` means the function is previously defined, and now it will
be overloaded. Internally, if `--overload` flag is not provided, the
interpreter use the same function as in `declare` command. Otherwise,
the interpreter use the same function as in `mutate` command.

`--lazy` defines the function is lazy.

`env` = `name,` `name=expr` `...`

Add extra environment variables when the function gets exectued.

_Examples_:

```racket
define fact(n, fact/r) {                 
    if(= n 1) {                          
        return 1;                        
    }                                    
    return (* n (fact/r (- n 1) fact/r));
}                                        
                                         
(fact 5 fact) => 120                     
```

General recursion

```racket
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
                                                     
```

Returns a function \(closure\)

```racket
                                  
define (cons2 first second rest) (
    cons first (cons second rest) 
);                                
                                  
(cons2 1 2 `(3)) => (1 2 3)       
```

Defines a function without side effects

```racket
                                
define make-adder(op1) {        
    define (adder op2) <n=op1> (
        + n op2                 
    );                          
    return adder;               
}                               
                                
declare add10 = (make-adder 10);
                                
(add10 2) => 12                 
```

Use `env`

### 2.2. return`    command`

`return` `expr;`

return a value. `expr` is required. If not in a function call, the
interpreter will exit.

### 2.3. lazy`    func`

`(lazy lazy-val)`

When a function is defined as lazy, all its arguments don’t get
evaluated immediately. Arguments value are changed to keys map to
`const_itVecStr` object pointers. Use `lazy` will evaluate this key
string.

This is very different from lazy evaluation in other languages, which
automatically determines when evaluation is requried for an `expr`. Let
users specify when an `expr` needs to be evaluated makes it much easier
to implement.

_Example_:

```racket
                                             
define --lazy (make-lazy expr) expr;         
                                             
declare true = (lambda (t f) (               
    lazy t                                   
));                                          
                                             
(true (make-lazy 1) (make-lazy (/ 1 0))) => 1
```

### 2.4. lambda`    func`

`(lambda flag (name ...) <env> expr)`

`flag` =  `--lazy`

`env` = `name,` `name=expr` `...`

`lambda` syntax is similar to `define` no side effects function, and
internally they are the same thing. The only difference is a function
defined with `lambda` don’t need to bind to a `name`, so there is no
`--overload` flag option.

_Example_:

```racket
                                                    
((((lambda (x)                                      
        (lambda (y) <x=x>                           
            (lambda (z) <x=x, y=y> (strapp x y z))))
        "this is x, ")                              
    "this is y, ")                                  
"this is z.")                                       
                                                    
=> this is x, this is y, this is z.                 
```

## 3. Built-in Data Structures

    3.1 slice`     syntax sugar`                
    3.2 String                                  
      3.2.1 strapp`     func`                   
      3.2.2 strlen`     func`                   
      3.2.3 slice:string`     func`             
    3.3 List                                    
      3.3.1 List Construction`     syntax sugar`
      3.3.2 list`     func`                     
      3.3.3 first`     func`                    
      3.3.4 rest`     func`                     
      3.3.5 cons`     func`                     
      3.3.6 append`     func`                   
    3.4 Array                                   
      3.4.1 array-new`     func`                
      3.4.2 array-size`     func`               
      3.4.3 array-ref`     func`                
      3.4.4 array`     command`                 

### 3.1. slice`     syntax sugar`

`slice` is designed to be a common method to multiple data structures.
Huaji script provides a syntax sugar for `slice` function.

`name[1st-expr:2nd-expr ...]`

`=> (slice name 1st-expr 2nd-expr ...)`

_Example_:

```racket
declare str="123456";
                     
str[0:-1] => 123456  
```

### 3.2. String

Strings are the fundemental data type in Huaji script. `val`s are
strings, and they might link to internal data structure. Huaji script
provides built-in basic string functions to help user manipulate
strings.

#### 3.2.1. strapp`     func`

`(strapp val ...)`

_Examples_:

Append strings.

```racket
(strapp "append " "strings") => append strings
```

#### 3.2.2. strlen`     func`

`(strlen val)`

Returns string length.

_Examples_:

```racket
(strlen "length ") => 7
```

#### 3.2.3. slice:string`     func`

`(slice val numerical-val)`

Returns `numerical-val`th `char` in the string. Here `char` means a
string with only a single character. If `val` is a negative number, then
the interpreter counts from back. `-1` is the pos of the last `char` in
the string.

_Examples_:

```racket
(slice "abcde" 2) => c 
                       
(slice "abcde" -1) => e
```

`(slice val startpos-val endpos-val)`

Returns a substring starts from `startpos-val`, ends at `endpos-val`.
End point is excluded \(e.g. \[`startpos-val`, `endpos-val`\)\)

_Examples_:

```racket
(slice "abcdefghijk" 2 5) => cde
                                
(slice "abcde" 0 -1) => abcde   
```

### 3.3. List

Built-in list is just a string, and all functions related to it are
string operations. List members are separated by space. Use quotation
mark can group list elements. `(...)` represents sublist. After applying
any list functions, the string gets formatted using
`hjbase::LISTFORMMATER`. Applying list functions to all strings that
satisfy list format is valid.

In `lib/std`, we provide another list implementation using lambda.

In all examples in this list section, previous executed code has effects
on later code.

#### 3.3.1. List Construction`     syntax sugar`

`‘(...) => (...)`

Construct a list without substitution or evaluation.

_Example_:

```racket
declare lst1=`(1   2 3 4 5 6), lst2=`( 1 2 4   5 6   "a b   c"ab   );
                                                                     
lst1 => (1 2 3 4 5 6)                                                
                                                                     
lst2 => (1 2 4 5 6 "a b   c" ab)                                     
```

`’(...) => (list ...)`

Apply function `list`

_Example_:

```racket
declare lst3='(1 (+ 1 1) (+ 1 2) (+ 2 3)),                      
        lst4='("abc123"   4562 (* 3 4.5) (strapp "abc " "efg"));
                                                                
lst3 => (1 2 3 5)                                               
                                                                
lst4 => (abc123 4562 13.500000 "abc efg")                       
```

#### 3.3.2. list`     func`

`(list ...)`

Evaluate `...`, then construct list.

_Example_:

```racket
declare lst5=(list (list (+ 1 1)));
                                   
lst5 => ((2))                      
```

#### 3.3.3. first`     func`

`(first list-val ...)`

Returns the first element in the list if only one `list-val` supplied.
Returns a list of all first elements in `list-val`s if more than one
`list-val`s provided.

_Examples_:

```racket
(first (first lst5)) => 2                                                 
                                                                          
declare lst6="(        \"I have space\" 2 3)";                            
                                                                          
(first lst1 lst2 lst3 lst4 lst5 lst6) => (1 1 1 abc123 (2) "I have space")
```

`lst1, lst2 ...`’s definition are reused here

#### 3.3.4. rest`     func`

`(rest list-val ...)`

Returns the rest of the list except the first element. Same as `first`,
can return a list of rest lists if more than one `list-val`s supplied.

_Examples_:

```racket
(rest lst5) => ()                                                                  
                                                                                   
(rest lst6) => (2 3)                                                               
                                                                                   
(rest lst1 lst2 lst3 lst4 lst5 lst6)                                               
=> ((2 3 4 5 6) (2 4 5 6 "a b   c" ab) (2 3 5) (4562 13.500000 "abc efg") () (2 3))
```

#### 3.3.5. cons`     func`

`(cons val list-val)`

Add one element to the front of the list.

_Examples_:

```racket
(cons 10 lst1) => (10 1 2 3 4 5 6)                                         
                                                                           
(cons " I   have spaces " lst6) => (" I   have spaces " "I have space" 2 3)
                                                                           
(cons lst1 lst2) => ((1 2 3 4 5 6) 1 2 4 5 6 "a b   c" ab)                 
```

#### 3.3.6. append`     func`

`(append list-val ...)`

Append lists.

_Example_:

```racket
(append lst1 lst2 lst3 lst4 lst5 lst6)                                                                
=> (1 2 3 4 5 6 1 2 4 5 6 "a b   c" ab 1 2 3 5 abc123 4562 13.500000 "abc efg" (2) "I have space" 2 3)
```

### 3.4. Array

Array is implemented with `std::vector<std::string>`.

In all examples in this array section, previous executed code has
effects on later code.

#### 3.4.1. array-new`     func`

`(array-new)`

Returns a new, empty `array-val`

_Example_:

```racket
declare test-array=(array-new);
```

#### 3.4.2. array-size`     func`

`(array-size array-val)`

Returns size of `array-val`.

_Example_:

```racket
(array-size test-array) => 0
```

#### 3.4.3. array-ref`     func`

`(array-ref array-val numerical-val)`

Returns `numerical-val`th element in `array-val`

#### 3.4.4. array`     command`

`array push_back array-expr elem-expr;`

Push one element at the back of `array-val` evaluated from the first
`array-expr`, implemented with `std::vector::push_back`.

_Example_:

```racket
array push_back test-array (+ 5 5);
                                   
(array-ref test-array 0) => 10     
```

`array insert array-expr pos-expr elem-expr;`

Insert one element in front of `pos`, evaluated from `pos-expr`, into
`array-val`, evaluated from the first `array-expr`. Implemented with
`std::vector::insert`.

_Example_:

```racket
array insert test-array (- 1 1) (+ 5 2);
                                        
(array-ref test-array 0) => 7           
```

`array mutate  array-expr pos-expr elem-expr;`

Mutate value stored at `pos` in `array-val` to `val`, evaluated from
`elem-expr`.

_Example_:

```racket
array mutate test-array 0 5; 
                             
(array-ref test-array 0) => 5
```

`array erase array-expr pos-expr;`

Erase `pos`th element in `array-val`, evaluated from the first
`array-expr`. Implemented with `std::vector::erase`.

_Example_:

```racket
array erase test-array (- 1 1);
                               
(array-ref test-array 0) => 10 
```

`array pop_back array-expr;`

Pop last element out. Implemented with `std::vector::pop_back`.

_Example_:

```racket
(array-size test-array) => 1
                            
array pop_back test-array;  
                            
(array-size test-array) => 0
```

## 4. OOP and Scope

Huaji script provides extended support on namespace system to
approximate OOP features. A namespace does not bind to any name in any
variables scope, `namespace-name` can be accessed anywhere in the script
after it initialized. An object uses the same implementation as
namespace `std::map<std::string, std::string>`, but is a first-class
value, and can be deleted. In Huaji script, there is no class, object
are created using copy constructor on namespace.

    4.1 init`     command`
    4.2 init`     func`   
    4.3 @                 
    4.4 mem-ref`     func`

### 4.1. init`     command`

`init namespace-name;`

Create an empty namespace with the name `namespace-name`.

_Example_:

```racket
init Vec2;
```

Initialize a namespace called `Vec2`.

`init child-namespace-name parent-namespace;`

Use copy contructor `std::map::map` on `parent-namespace` to create a
new namespace, and bind it to `child-namespace-name`

_Example_:

```racket
init 2D-pos Vec2;
```

Initialize a namespace called `2D-pos` copied from `Vec2`. `Vec2` is
empty, so `2D-pos` is empty too.

### 4.2. init`     func`

`(init namespace-name)`

Return an object constructed by applying copy constructor on
`namespace-name`. It is identical internally, but an object is a
first-class value.

_Example_:

```racket
declare pointA = (init "Vec2");
```

Notice here `Vec2` is in quotation mark, because it is neither a `name`
nor a `val`. Using quotation marks make it a `val`.

### 4.3. @

`name-in-namespace@namespace-name`

Separator for accessing variables in a namespace.

### 4.4. mem-ref`     func`

`(men-ref name-in-object-scope object-name)`

Get the `val` bind to the `name-in-object-scope`.

_Example_:

```racket
init constant;                               
declare pi@constant = 3.14, e@constant = 2.7;
declare cnt_obj = (init "constant");         
                                             
e@constant => 2.7                            
(mem-ref "pi" cnt_obj) => 3.14               
```

Notice here we have to use quotation mark agains, because "mem-ref"
arguments get evaluated first, and it’s not in the scope.

## Index

[A](#alpha:A) [B](#alpha:B) [C](#alpha:C) [D](#alpha:D) [E](#alpha:E)
[F](#alpha:F) G [H](#alpha:H) [I](#alpha:I) J K [L](#alpha:L)        
[M](#alpha:M) N [O](#alpha:O) [P](#alpha:P) Q [R](#alpha:R)          
[S](#alpha:S) T U V [W](#alpha:W) X Y Z                              
                                                                     
@                                                                    
append`     func`                                                    
Arithemetic Operators`    func`                                      
Array                                                                
array`     command`                                                  
array-new`     func`                                                 
array-ref`     func`                                                 
array-size`     func`                                                
Basics                                                               
Built-in Data Structures                                             
Comments                                                             
cons`     func`                                                      
declare`    command`                                                 
define`    command`                                                  
elif`    command`                                                    
else`    command`                                                    
exec`    command`                                                    
first`     func`                                                     
Functions                                                            
Huaji Script: A Simple Modern Language Interpreter                   
if`    command`                                                      
init`     command`                                                   
init`     func`                                                      
lambda`    func`                                                     
lazy`    func`                                                       
List                                                                 
List Construction`     syntax sugar`                                 
list`     func`                                                      
mem-ref`     func`                                                   
mutate`    command`                                                  
OOP and Scope                                                        
Order Relations`    func`                                            
print`    command`                                                   
resolve`  func`                                                      
rest`     func`                                                      
return`    command`                                                  
slice`     syntax sugar`                                             
slice:string`     func`                                              
strapp`     func`                                                    
String                                                               
strlen`     func`                                                    
while`    command`                                                   
