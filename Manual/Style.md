# Our Modern C/C++/C# Standards and Style Guide for Enthusiasts
Welcome to the our Modern C/C++/C# Standards and Style Guide tailored for enthusiasts like you. This guideline is inspired by the "NASA C++ Coding Standards and Style Guide" but with a fresh and modern approach to meet the demands of today's development landscape.

## Introduction
>Note: Feel free to adapt other Standards or Style Guides for your project's extensions. However, these rules apply exclusively to the project itself. Any exposed "API" should maintain a consistent look and feel.

This guide aims to provide a framework for creating projects that are efficient, maintainable, easy to read, and well-organized. Note that these guidelines don't affect standard libraries, template libraries, or third-party libraries, which each have their own set of rules. Nonetheless, when offering an API to end-users, ensure a consistent style.

###### Terminology
- [PascalCase](https://en.wikipedia.org/wiki/Camel_case)
- [camelCase](https://en.wikipedia.org/wiki/Camel_case)
- [lowercase](https://en.wikipedia.org/wiki/Letter_case)
- [UPPERCASE](https://en.wikipedia.org/wiki/Letter_case)
- [snake_case](https://en.wikipedia.org/wiki/Snake_case)

## Names
Prioritize meaningful and readable names, minimizing the need for extensive comment explanations.

### Overview
| Description                          | Style      |
| :-                                   | :-:        |
| Classes <br> Structs <br> Types      | PascalCase |
| Directories and Files                | PascalCase |
| Extensions                           | lowercase  |
| Enumeration Types <br> Enumerations  | PascalCase |
| Functions, Methods                   | PascalCase |
| Namespaces                           | PascalCase |
| Arguments                            | camelCase  |
| Constants                            | PascalCase |
| Preprocessor Macros                  | UPPERCASE  |
| Variables (*special prefix usage)    | camelCase  |

## Rules

### Comments
Utilize ```//``` for comments and ```///``` for Doxygen comments.<br>
Reserve ```/* */``` multi-line comments for code disabling, favoring ```#ifdef ... #endif``` preprocessor directives instead.

##### Doxygen Tags

###### General Tags

- `@brief <text>`: Element's purpose.
- `@details <text>`: Additional details.
- `@remark <text>`: General remark.
- `@todo <text>`: Pending work.
- `@deprecated <text>`: Deprecated element.
- `@attention <name> <text>`: Important message.
- `@note <text>`: Note or special instruction.
- `@warning <text>`: Warning message.
- `@see <reference_name> <text>`: Cross-reference.

###### Documenting Elements

- `@namespace <namespace_name> <text>`: Description of namespace.
- `@class <class_name> <text>`: Description of class.
- `@struct <struct_name> <text>`: Description of struct.
- `@enum <enum_name> <text>`: Description of enum.
- `@example <text>`: Code example.
- `@ingroup <group_name> <text>`: Assigns to group.
- `@link <link_name> <text>`: Create link.

###### Function and Parameter Tags

- `@overload <function_name> <text>`: Overloaded function.
- `@param[in] <param_name> <text>`: Input parameter.
- `@param[out] <param_name> <text>`: Output parameter.
- `@param <param_name> <text>`: Parameter description.
- `@tparam <T> <text>`: Template parameter description.
- `@return <text>`: Return value description.

### Namespaces
>Note: Due to C++ module use, namespaces should be divided within module files.

##### Example
```cpp
// Private Interface
namespace Project::Helpers {

bool ShouldHelp() {
    // ...
}

}

// Public Interface
export namespace Project::Helpers {

class HelpMe {
    //...
};

}
```

### Classes, Structs and Types
##### Example
```cpp
// Simple Type
using StringVector = std::vector<std::string>;

// Complex Type
struct PlanetData {
    // Properties
    size_t Size {};
}

// Everything else...
class Earth {
private
    // 1st Friends and Usings
    friend Satellite;
    using Landscapes = std::unordered_map<std::string, std::string>;

public:
    // 2nd Constructors and Deconstructor
    Earth(string_view name):
        mName(name) {
    }
    virtual ~Earth() = default;
    
    // 3rd Accessors, Mutators and Operators
    std::string GetName() const { return mName; }
    void SetName(string_view name) { mName = name };

    // 4th Methods
    void Rotate();

protected:
    // 5th Methods accessible by friends
    void GetCurrentPosition();

private:
    // 5th Properties
    std::string mName;
    PlanetData mData {};
}
```

### Formatting

#### Blocks
##### Structure
For structured code, predefined symbols can be used for separation and specification.

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
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
// ☷ Level C
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
```

## Examples

### Module
```cpp
module;

// Optional Includes
#include <cool-lib>

export module Module;

// 1st: Additional Exports in this order: Default Libraries, 3rd-Party Libraries, Libraries
import export Module.SubModule;
// 2nd: Imports in this order: Default Libraries, 3rd-Party Libraries, Libraries
import std;

export namespace Namespace {

void Log(std::string_view message) {
    std::cout << message << std::endl;
}

}
```

### Header
```cpp
#pragma once

// Libraries and Modules
#include <iostream>     // 1st: Default Libraries
#include <string>

#include <windows>      // 2nd: 3rd-Party Libraries

#include "Settings.h"   // 3rd: Libraries

import Module;          // 4th: Modules

// Macros
#define STYLE_GUIDE 1

// Namespaces
namespace Namespace {

// Enumerations and Types
using String = std::string;

enum class Enum {
    Default = 0,
    First   = 1,
    Second  = 2,
}

// Forward Declarations
class OtherClass;

// Declarations
class Class {
    // Friends and Usings
    friend OtherClass;
    using std::cout;

public:
    // Constructors and Deconstructor
    Class() = default;
    ~Class() = default;

    // Accessors, Mutators and Operators
    const Enum GetValue();
    void SetValue(Enum enumValue);

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
