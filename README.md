# Contents
[Compiler](https://github.com/Fdhvdu/lib/blob/master/README.md#compiler)<br>
[About compilation errors](https://github.com/Fdhvdu/lib/blob/master/README.md#about-compilation-errors)<br>
[Future work](https://github.com/Fdhvdu/lib/blob/master/README.md#future-work)
# Compiler
VC++ 14.2<br>
or any compiler which supports C++14<br>
(if you will use nThread::CLock_forward_list, you have to get a C++17 compiler, such as VC++ 14.2)
# About compilation errors
If you get error C4996 when you compile [insert_iterator.cpp](tutorial/insert_iterator.cpp) by VC++, [see this](http://stackoverflow.com/questions/25046829/what-does-use-d-scl-secure-no-warnings-mean).
# Future work
constexpr<br>
noexcept<br>
