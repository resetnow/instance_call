# instance_call

C++ helper calls for binding a class instance to a callback. Intended to be used
with callbacks when `std::function` can't be used due to space/allocation 
constraints. Primarily targeted for embedded software.

## Usage

```c++
#include <instance_call.hpp>

class Bar {
private:
    int m_a1;

public:
    Bar(int a1):
        m_a1(a1) {}

    int baz(int a2) {
        return m_a1 + a2;
    }
};

int foo() {
    Bar b { 3 };
    
    // C++17
    auto * f = &instance_call<&Bar::baz>::helper::call;

    // C++14
    auto * f = &instance_call<
        decltype(&Bar::baz),
        &Bar::baz
    >::helper::call;

    // f has signature int (*)(void*, int)
    // f can now be passed around
    // Intended way of calling it — returns 5
    return f(&b, 2);
}
```

## Limitations

Function argument types are intentionally limited to those conforming to 
`std::is_fundamental` or `std::is_pointer`.

## Using with C

It's possible to use this code with C but there is no guarantee the
calling convention for C++ static class menthods and C functions will be 
the same. However, on many platforms and compilers (GCC) it will work —
limited set of allowed argument types helps in this situation.
