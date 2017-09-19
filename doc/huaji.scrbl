#lang scribble/manual

@title{Huaji Script: A Simple Modern Language Interpreter}

by @author+email["Shuangyi Tong" "s9tong@uwaterloo.ca"]

Huaji script language is designed to be an extremely simple, customizable, and flexible modern language interpreter based on C++.

@bold{Motivation}: I wrote its primitive version to support my LAMSTAR network's 
(An other project you can found under my @(link "https://github.com/ShuangyiTong/LAMSTAR" "repository")) training logic. 
If you have some C/C++ program and want to control those function calls, you can use tools provided by famous language interpreters like ctypes in Python, 
which obviously most people will choose this way. 
Or you can use a simple interpreter and compile the interpreter with your program, and this is what this language interpreter is for!

@bold{Features}: I think this language interpreter is not aesthetically beautiful by any means. 
It's a toy created by a second-year undergraduate student who is not in CS major in two months. 
But for myself, I was enjoyed in writing an interpreter solely designed by myself.
Some features listed here are not developed intentionally, 
but bascially they all contributed to simpler implementations (at least for now).

@itemlist[
        @item{
            Everything is a string, even internally

            I was working with Tcl everyday because of my job when I started writing this interpreter. 
            Tcl says eveything is represented by string in Tcl, and I take this idea. 
            The different thing is, internally, values in Tcl might not be a string, but Huaji script is.
            All values are represented by @(litchar "std::string"). There are indeed problems with this kind of implementation.
        } 
        @item{
            ASTs are generated dynamically

            There is no independent parsing function, which generates an entire AST for execution. ASTs are generated dynamically.
            This makes Huaji script slow, but users get full control over command syntax. @(litchar "hjbase::HUAJISCRIPTBASE")
            are designed to be inherited, that's how users can combine their program with Huaji script interpreter. 
        }
        @item{
            OOP

            OOP features in Huaji script are very primitive. Compared to other object-oriented languages, OOP in Huaji script is just a little extension
            on namespace system.
        }
        @item{
            Functional Programming

            Huaji script provides some functional programming features, including lazy functions, closures etc.
        }]

@bold{Name Origin}: Huaji (Chinese: 滑稽) literally means funny in Chinese. It's meaning has changed becuase of a popular sticker on some Chinese forums, 
you can go to @(link "https://tieba.baidu.com" "百度贴吧") to find out what exactly the word means.

@table-of-contents[]

@include-section["basics.scrbl"]
@include-section["function.scrbl"]
@include-section["data-struct.scrbl"]
@include-section["oop-scope.scrbl"]

@index-section[]