# Contents
[Naming Convention](https://github.com/Fdhvdu/lib/blob/master/README.md#naming-convention)<br>
[Compiler](https://github.com/Fdhvdu/lib/blob/master/README.md#compiler)<br>
[About compilation errors](https://github.com/Fdhvdu/lib/blob/master/README.md#about-compilation-errors)<br>
[Future work](https://github.com/Fdhvdu/lib/blob/master/README.md#future-work)
# Naming Convention
Macros are all written in capital letter.<br>
For those which do not use capital letter in the begining (such as [algorithm.hpp](header/algorithm/algorithm.hpp)), it contains only functions.<br>
For those which have `I` in the begining (such as [IThreadPoolItemBase.hpp](https://github.com/Fdhvdu/ThreadPool/blob/master/header/IThreadPoolItemBase.hpp)), it means they are `struct` and have pure functions.<br>
For those which have `C` in the begining (such as [CAtomic_stack.hpp](header/thread/CAtomic_stack.hpp)), it means they are `Class`.<br>
For those which do not have `I` or `C` and begin with capital letter (such as [Atomic_flag.hpp](header/thread/Atomic_flag.hpp)), it means they are `struct`. (One exception is [Boolean](header/tool/Boolean.hpp), this may be modified in the future.)<br>
The old naming convention is CamelCase. (See [QuantumCircuit](https://github.com/Fdhvdu/QuantumCircuit/tree/master/header) and [ThreadPool](https://github.com/Fdhvdu/ThreadPool/tree/master/header))<br>
The new naming convention uses underscores to separate words.
# Compiler
VC++ 14.2<br>
or any compiler which supports C++14<br>
(if you will use nThread::CLock_forward_list, you have to get a C++17 compiler, such as VC++ 14.2)
# About compilation errors
If you get error C4996 when you compile [insert_iterator.cpp](tutorial/insert_iterator.cpp) by VC++, [see this](http://stackoverflow.com/questions/25046829/what-does-use-d-scl-secure-no-warnings-mean).
# Future work
constexpr<br>
replace ::value with _v<br>
noexcept<br>
replace using with static_assert<br>
memory order<br>
allocator_type of CAlloc_obj should not be static<br>
style change (replace {} with ())