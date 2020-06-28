# Symbols

## Comments
### Level Specifier
```cpp
- 1st: ☰ [U+2630]
- 2nd: ☲ [U+2632]
- 3rd: ☷ [U+2637]
```
### Visual Separation
```cpp
- 1st-Level: ■ [U+25A0]
- 2nd-Level: ━ [U+2500]
- 3rd-Level: ‐ [U+2010] 
```

## Logging
```cpp
```
# Naming
>If you prefere other styling guidelines, feel free to use them.

This is the way how I like to code in C-based languages, it is a mix of many different guidelines, where I picket the things that I liked.

## Common
> Camel-Case
- Arguments
- Variables
> Lower-Case
- Namespaces
> Pascal-Case
- Class and its Members (Methods and Properties)
- Functions
> Upper-Case
- Preprocessor Macros
```cpp
#define PREP_MACRO_B

namespace a { namespace b {

namespace c {
    int NumberRedirector(int number) {
       return number; 
    }
}

class Class {
    bool Active;

public:
    Class(): default;
    Class(bool active):
        Value {active} {};
    
    bool GetActive() const {
        return Active;
    }
    void SetActive(const bool active) {
        Active = active;
    }
}

}}
```

- Underscore: Avoid them as much as possible...
- 

## C Code
- Nothing particular, only the encapsulation is important.

## C++ Code
- Pointer prefix: p

## Files
