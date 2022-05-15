# C/C++/C# Standards and Style Guide
This guideline is based on the ["NASA C++ Coding Standards and Style Guide"](https://ntrs.nasa.gov/citations/20080039927). It contains rules for C, C++ and C# with some minor changes and extensions.

## Introduction
>Note: If you prefer other Standards or Style Guides, feel free to use them in your extensions for the project. These rules only apply to the project itself. Anyways everything which is exposed to the end-user as "API" should have a similar look and feel.

The purpose is to offer a standard for efficient, maintable, simple to read and understand and well organized project for developers and contributors. The guidelines for the standard library, standard template library and 3rd-Party libraries aren't effected! They have their own set of rules which should be respected. Anyways a wrapper API should be offered to the end-users.

###### Terms
- [camelCase](https://en.wikipedia.org/wiki/Camel_case)
- [lowercase](https://en.wikipedia.org/wiki/Letter_case)
- [PascalCase](https://en.wikipedia.org/wiki/Camel_case)
- [UPPERCASE](https://en.wikipedia.org/wiki/Letter_case)

## Names
Choose names that are meaningful and readable, instead of pasting a large comment block beforehead explaining its purpose.

###### Overview
| Description                          | Style       |
| :-                                   | :-:         |
| Classes <br> Structs <br> Types      | Pascal-Case |
| Directories and Files                | Pascal-Case |
| Extensions                           | Lower-Case  |
| Enumeration Types <br> Enumerations  | Pascal-Case |
| Functions, Methods                   | Pascal-Case |
| Namespaces                           | Lower-Case  |
| Arguments                            | Camel-Case  |
| Constants                            | Pascal-Case |
| Preprocessor Macros                  | Upper-Case  |
| Variables (*special use with prefix) | Camel-Case  |

### Rules
#### Classes, Structs and Types
###### Example
```cpp
// Simple Type
using StringVector = std::vector<std::string>;

// Complex Type
struct Planet {
    // 1st Properties
    // 2nd Constructors and Deconstructor
    // 3rd Accessors, Mutators and Operators
    // 4th Methods
}

// Everything else...
class Earth {
    // 1st Friends and Usings
public:
    // 2nd Constructors and Deconstructor
    ...
    // 3rd Accessors, Mutators and Operators
    ...
    // 4th Methods
protected:
private:
    ...
    // 5th Properties
}
```

#### Namespaces
###### Example
```cpp
namespace first.second {

class A {}

namespace third {

class B {}

}

}
```

## Formatting

### Blocks

### Comments
Use ```//``` for all comments and ```///``` for doxygen comments.
Multi-line comments are only allowed ```/* */``` only for disabling code, but here the way with pre-processor should be prefered.

### Logging
The timestamp and the log level are automatically delivered by the internal applog class. Everything beyond the pipe character should be filled like in the example. The timestamp should be logged always as UTC.
###### Format
```cpp
Option A: $Message (no prefix of timestamp, log level and project module, useful for structuring)
Option B: YYYY-MM-ddTHH:mm:ss.ns | $LogLevel | $Project.Module: $Message
```
###### Examples
```cpp
Application started.
2020-01-01T17:00:00.000000 | Fatal | [App.Module]: Critical ... the application will be terminated immediatly.
2020-01-01T17:00:00.000000 | Error | [App.Module]: Error ... the application will abort the current operation.
2020-01-01T17:00:00.000000 | Warn  | [App.Module]: Warning ... something is odd, but we can still proceed.
2020-01-01T17:00:00.000000 | Info  | [App.Module]: Information ... the application will do something/has done something.
2020-01-01T17:00:00.000000 | Debug | [App.Module]: Debugging ... the module will do something/has done something.
2020-01-01T17:00:00.000000 | Trace | [App.Module]: Verbose ... the function will do something/has done something.
```

### Structure
If you need a more structured code use the predefiend symbols for seperation and specification (also used in log files).

###### Level Separation
```cpp
- 1st-Level: ■ [U+25A0]
- 2nd-Level: ━ [U+2500]
- 3rd-Level: ‐ [U+2010] 
```

###### Level Specifier
```cpp
- 1st: ☰ [U+2630]
- 2nd: ☲ [U+2632]
- 3rd: ☷ [U+2637]
```

###### Example
```cpp
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// ☰ Level A
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬
// ☲ Level B
//▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬
// ☷ Level C
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
```

## Examples
### Header
```cpp
#pragma once

// Libraries and Modules
#include <iostream>     // 1st: Default Libraries
#include <string>

#include <windows>      // 2nd: 3rd-Party Libraries

#include "Settings.h"   // 3rd: Libraries

import Module;          // Same as Libraries

// Macros
#define STYLE_GUIDE 1

// Namespaces
namespace app {

// Enumerations and Types
using String = std::string;

enum class Enum {
    Default = 0,
    First   = 1,
    Second  = 2,
}

// Pre-Declarations
class OtherClass;

// Declarations
class Class {
    // Friends and Usings
    friend OtherClass;
    
    using std::cout;

public:
    // Constructors and Deconstructor
    Class() = default
    Class(Enum enum);
    ~Class() = default;

    // Accessors, Mutators and Operators
    const Enum GetValue();

    void SetValue(Enum enum);

    operator Enum();

    // Methods
    void Method();    

protected:
    void ForMyFriend();

private:
    void MySecret();

    // Properties
    Enum mEnum;
};

}
```

### Module
```cpp
module;
#include <iostream>

export module Module;

export namespace app {

void log(const char *message) {
    std::cout << message << std::endl;
}

}
```

### Source
```cpp
#include "Class.h"

// Constructors and Deconstructor
Class::Class(Enum enum):
    mEnum(enum) {
}

// Accessors, Mutators and Operators
const Enum GetValue() {
    return mEnum;
}

void SetValue(Enum enum) {
    mEnum = enum;
}

operator Enum() {
    return mEnum;
}

// Methods
void ForMyFriend() {
    std::cout << "Hello Friend!" << std::endl;
}

void MySecret() {
    std::cout << "*******" << std::endl;
}

```
