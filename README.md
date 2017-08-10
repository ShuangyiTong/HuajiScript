# Huaji Script Interpreter

### Unfinished (v1.05)

## Motivation

A extremely simple, customizable, and flexible modern language interpreter based on C++. I wrote its primitive version to support my LAMSTAR network (An other project you can found under my repository) training logic. If you have some C/C++ program and want to control those function calls, you can use tools provided by famous lamguage interpreters like ctypes in Python, which obviously most people will choose this way. Or you can use a simple interpreter and compile the interpreter with your program, and this is what this language interpreter is for!

Huaji (Chinese: 滑稽) literally means funny, if you have enough time and you know Chinese well, you can go to https://tieba.baidu.com to find out what exactly the word means.

## Compile

The class hjbase::HUAJISCRIPTBASE is designed to be inherited if you want to combine your C++ program with the interpreter. Maybe I will write a clear document in the future after most things finished. There are only three files, one header and two cpp, no make file needed, no dependencies, only STL is used. You can replace driver.cpp with your own framework.

I am using clang 4.0.0, g++ 7.1.1 should be fine too. (actually any C++14 compiler should be OK)

    clang++ -std=c++14 hjscript.cpp driver.cpp -o hjscript -O3

Check example folder to see what it can do!

## Things to do

1. Garbage collection 

    其实现在的function如果是定义在栈上的（然而现在只能定义在栈上哈哈哈哈），在stack frame被pop了之后就没法访问了（尴尬），看上去function存在func_pool里，所以检测不出内存泄漏，而且在destructor里会一并清除，但是应该算作一种内存泄漏，因为已经没有用了。所以做一个GB是非常有必要的。

2. Namespace together with basic OOP

    这个可以和OOP一起做，OOP稍微麻烦一点，namespace还是比较简单的

3. lambda/closure support
    
    主要是scope的问题我感觉比较麻烦，计划在一个stack frame里建一个closure专用的小栈，因为要支持curry function所以我也想不出比较好的办法

4. More built-in data structures (array - based on std::vector, dict - based on std::map)

    用vector作array，用map作dict，关键是要GB完成

5. More built-in functions, especially for loop, slice, if, cond etc.

    一些必要的内建函数

6. IO, system function calls, network lib maybe

    IO和系统函数，网络栈主要是平台的问题，不是很想用除STL外的库（然而STL并没有network(#滑稽）），又懒得自己写，还想要支持多平台的话只能等大神或者爆肝了（而且网络栈比较外围的问题交给其他语言解决也可以嘛）

7. Make hjbase::ufunc::Signal_Error output looks better

    错误信息函数优化

8. Maybe Exception handling

    异常处理？没有也是完全没问题的，就是有些时候比较方便，但是implement起来感觉比较麻烦，大概只能等大神了

9. Chinese Support

    改语言是很简单的，这些差不多做完了做一个Chinese Flavour的，不知道要不要改成std::wstring?