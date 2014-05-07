Compile-Time-Eval
=================

Compile time "eval"

This code compile.

```cpp
#include<typeinfo>
#include"CTE/CTE.hpp"
auto main ( ) -> int
{
	auto eval1 = CTE_EVAL ( "1+20*10/(5+5)" ) ;
	auto eval2 = CTE_EVAL ( "1+20*10/(5+5)+.5" ) ;
	auto eval3 = CTE_EVAL ( u8R"*****("Hello"+" "+"World")*****" ) ;
	std::cout << eval1 << std::endl ;
	std::cout << typeid ( eval1 ).name ( ) << std::endl ;
	std::cout << eval2 << std::endl ;
	std::cout << typeid ( eval2 ).name ( ) << std::endl ;
	std::cout << eval3 << std::endl ;
	std::cout << typeid ( eval3 ).name ( ) << std::endl ;
}
```

output.

```
$ ./prog
21
i
21.5
d
Hello World
Ss
```

GCC4.8.1
Clang3.4