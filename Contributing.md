# <a name="main-heading"></a>Ultimanite Coding Style and Guidlines

# <a name="main-section-overview"></a>Main sections
- [C++ coding style and formatting](#cpp-coding-style-and-formatting)
- [C++ code-specific guidelines](#cpp-code-specific-guidelines)

# <a name="cpp-coding-style-and-formatting"></a>C++ coding style and formatting

Summary:

- [General](#cpp-style-general)
- [Naming](#cpp-style-naming)
- [Conditionals](#cpp-style-conditionals)
- [Classes and structs](#cpp-style-classes-and-structs)
- [UE4](#cpp-code-ue4)

## <a name="cpp-style-general"></a>General
- Try to limit lines of code to a maximum of 100 characters.
    - Note that this does not mean you should try and use all 100 characters every time you have the chance. Typically with well formatted code, you normally shouldn't hit a line count of anything over 80 or 90 characters.
- The indentation style we use is 2 spaces per level.
- The opening brace for namespaces, classes, functions, enums, structs, unions, conditionals, and loops go on the next line.
  - With array initializer lists and lambda expressions it is OK to keep the brace on the same line.
- References and pointers have the ampersand or asterisk against the type name, not the variable name. Example: `int* var`, not `int *var`.
- Use multi-line comments (`/* Comment text */`) for multi-lines and parameters, use single-line comments (`// Comment text`) otherwise.
- Always make sure you are doing whatever you are doing under the correct file context, make sure to read everything and check for duplications.

## <a name="cpp-style-naming"></a>Naming
- All variables, class, enum, function, and struct names should be in upper CamelCase. If the name contains an abbreviation uppercase it.
  - `class SomeClassName`
  - `enum IPCCommandType`
- All compile time constants should be fully uppercased. With constants that have more than one word in them, use an underscore to separate them.
  - `constexpr double PI = 3.14159;`
  - `constexpr int MAX_PATH = 260;`
- Please do not use [Hungarian notation](http://en.wikipedia.org/wiki/Hungarian_notation) prefixes with variables. The only exceptions to this are the variable prefixes below.
  - Global variables – `g`
  - Class variables – `m`
  - Static variables – `s`
- Keep every context in a namespace, keep every context in a seprated file if possible.
## <a name="cpp-style-conditionals"></a>Conditionals
- Do not leave `else` or `else if` conditions dangling unless the `if` condition lacks braces.
  - Yes:

    ```c++
    if (condition)
    {
      // code
    }
    else
    {
      // code
    }
    ```
  - Acceptable:

    ```c++
    if (condition)
      // code line
    else
      // code line
    ```
  - No:

    ```c++
    if (condition)
    {
      // code
    }
    else
      // code line
    ```


## <a name="cpp-style-classes-and-structs"></a>Classes and structs
- If making a [POD](http://en.wikipedia.org/wiki/Plain_Old_Data_Structures) type, use a `struct` for this. Use a `class` otherwise.
- Class layout should be in the order, `private`, `protected`, and then `public`.
  - If one or more of these sections are not needed, then simply don't include them.
- For each of the above specified access levels, the contents of each should follow this given order: constructor, destructor, operator overloads, functions, then variables.
- When defining the variables, define `static` variables before the non-static ones.

```c++
class ExampleClass : public SomeParent
{
public:
  ExampleClass(int x, int y);

  int GetX() const;
  int GetY() const;

protected:
  virtual void SomeProtectedFunction() = 0;
  static float SomeVariable;

private:
  int m_x;
  int m_y;
};
```

# <a name="cpp-code-specific-guidelines"></a>C++ code-specific guidelines

Summary:

- [General](#cpp-code-general)
- [Headers](#cpp-code-headers)
- [Loops](#cpp-code-loops)
- [Functions](#cpp-code-functions)
- [Classes and Structs](#cpp-code-classes-and-structs)

## <a name="cpp-code-general"></a>General
- The codebase currently uses C++20 (Latest).
- Use the [nullptr](http://en.cppreference.com/w/cpp/language/nullptr) type over the macro `NULL`.
- If a [range-based for loop](http://en.cppreference.com/w/cpp/language/range-for) can be used instead of container iterators, use it.
- Obviously, try not to use `goto` unless you have a *really* good reason for it.
- If a compiler warning is found, please try and fix it.
- Try to avoid using raw pointers (pointers allocated with `new`) as much as possible. There are cases where using a raw pointer is unavoidable, and in these situations it is OK to use them. An example of this is functions from a C library that require them. In cases where it is avoidable, the STL usually has a means to solve this (`vector`, `unique_ptr`, etc).
- Do not use the `auto` keyword everywhere. While it's nice that the type can be determined by the compiler, it cannot be resolved at 'readtime' by the developer as easily. Use auto only in cases where it is obvious what the type being assigned is (note: 'obvious' means not having to open other files or reading the header file). Some situations where it is appropriate to use `auto` is when iterating over a `std::map` container in a foreach loop, or to shorten the length of container iterator variable declarations.
- Do not use `using namespace [x];` in headers. Try not to use it at all if you can.
- The preferred form of the increment and decrement operator in for-loops is prefix-form (e.g. `++var`).

## <a name="cpp-code-headers"></a>Headers
- All needed headers should be included in framework header, include any new header files in the framework header.
- If you find duplicate includes of a certain header, remove it.
- Each of the above header sections should also be in importance order.
- This project uses `#pragma once` as header guards.

## <a name="cpp-code-loops"></a>Loops
- If an infinite loop is required, do not use `for (;;)`, use `while (true)`.
- Empty-bodied loops should use braces after their header, not a semicolon.
  - Yes: `while (condition) {}`
  - No: `while (condition);`
- For do-while loops, place 'while' on the same line as the closing brackets

  ```c++
  do
  {
    // code
  } while (false);
  ```

## <a name="cpp-code-functions"></a>Functions
- If a function parameter is a pointer or reference and its value or data isn't intended to be changed, please mark that parameter as `const`.
- Functions that specifically modify their parameters should have the respective parameter(s) marked as a pointer so that the variables being modified are syntaxically obvious.
  - What not to do:

    ```c++
    template<class T>
    inline void Clamp(T& val, const T& min, const T& max)
    {
      if (val < min)
        val = min;
      else if (val > max)
        val = max;
    }
    ```

    Example call: `Clamp(var, 1000, 5000);`

  - What to do:

    ```c++
    template<class T>
    inline void Clamp(T* val, const T& min, const T& max)
    {
      if (*val < min)
        *val = min;
      else if (*val > max)
        *val = max;
    }
    ```

    Example call: `Clamp(&var, 1000, 5000);`
    - Otherwise, you can use references.

- Class member functions that you do not want to be overridden in inheriting classes should be marked with the `final` specifier.

  ```c++
  class ClassName : ParentClass
  {
  public:
    void Update() final;
  };
  ```

- Overridden member functions that can also be inherited should be marked with the `override` specifier to make it easier to see which functions belong to the parent class.

  ```c++
  class ClassName : ParentClass
  {
  public:
    void Update() override;
  };
  ```

## <a name="cpp-code-classes-and-structs"></a>Classes and structs
- Classes and structs that are not intended to be extended through inheritance should be marked with the `final` specifier.

  ```c++
  class ClassName final : ParentClass
  {
    // Class definitions
  };
  ```


## <a name="cpp-code-ue4"></a>UE4
- ProcessEvent is the game thread, about 80% of game calles are passed thorugh ProcessEvent, You should be very cautious adding anything to the processevent hook.
  - **Donot** use inline functions inside the hook, use static functions instead.
  - Force inline if the function is a class member getter.
  - If you have to add some blocking code to a ufunction hook inside processevent hook (e.g: Finding alot of objects when `xxxx` is called), then hook `xxxx` **directly** by doing `DetourAttach(&(UFunction->Func), xxxxHook);`.
  - Avoid using `GetAsyncKeyState`, Use proper ufunction hook if possible, if not use UE4 input hook, direct input hook.

- Don't call ProcessEvent directly unless necessary.
  - Don't:
  
  ```c++
	static UObject* SwitchLevel = FindObject(L"Function /Script/Engine.PlayerController.SwitchLevel");

  struct {
    FString URL = L"...";
  } Params;

	ProcessEvent(InController, SwitchLevel, &Prams);
  ```

  - Do :
  ```c++
  static UObject* SwitchLevel = FindObject(L"Function /Script/Engine.PlayerController.SwitchLevel");
  auto ret = Controller->Call</* RETURN TYPE IF EXISTS */>(SwitchLevel, FString(L"...") /* ACCEPTS ANY NUMBER\TYPE OF PARAMS */);
  ```

- if you are going to use hardcoded offset outside of a full class\struct, please cast the pointer to `uintptr_t` and add the offset to it instead of make a full struct.

- Check for invalid pointers by doing `UObject.isValid()` or `IsBadReadPtr`, sometimes pointers can be valid but not valid for reading.

- If you are finding the same object over and over make sure to define the variable as `static`.

- Avoid allocation, reallocating memory manually unless needed.

- If FNameToString was used directly, make sure to free the memory.

- Feel free to copy any needed classes from the sdk, but make sure to implement all classes it needs and not feel it with padding.

- **Everything** is a wide string, only use normal strings if necessary, all classes has functions for both string types.

- **Don't** use C Strings unless necessary, use STD strings in normal cases.

- Avoid copying strings over and over, use references when possible.

------

- Please, try to be as strict as possible applying these guidelines to maintain a stable project shape.
  - Keep in mind, Exceptions can be made, you can ask me `kemo#1337` for any help or style tips.