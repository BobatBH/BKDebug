# About BKDebug

**BKDebug** is a set of macros designed to help you debug C, C++, and Objective-C code. It also includes a set of supporting variable types.

You can use **BKDebug** with iOS, Mac, Windows, and most other platforms that have a decent C-based development suite. A C99 or newer compiler is required, though if you still need to use a different kind of compiler (C89/C90 or whatever), it _might_ get a little problematic. (I haven't tested this.)


# Basic usage

You can think of the macros as a replacement for `assert()`. That is, where you would normally call assert, you can use a **BKDebug** macro instead.

For example, a common usage is to check the validity of function arguments:

```C++
#include "BKDebug.h"

void foo(int bar)
{
    BKDEBUG_CHECK_NOTEQUAL(bar, -1, "something really bad happened");

    // the rest of your code
}
```

In the Debug build, `bar` will be checked every time `foo()` is called. In the Release build, the macro is compiled out so it doesn't generate any code.

You can `#include/#import` both `.h` files if you wish, but only `BKDebug.h` is required; it will include `BKTypes.h` for its own use.


# Behavior

When a macro **fires**, you will see something like this:

```
Check failed, should be != : something really bad happened
    bar == -1
    0xffffffff == 0xffffffff
    /example/project/main.mm
    Line 31
```

If the debugger is running and you have breakpoints enabled (see the `_BK_DISABLE_CHECK_BREAKPOINTS_` option below), it will stop on the `BKDEBUG_CHECK_NOTEQUAL` line. Otherwise, the breakpoint is ignored and execution continues.

When a macro fires, the debugger is halted as if you set a breakpoint there manually. You are able to inspect variables, check the call stack, step through the code, continue execution, etc.


# Preprocessor Defines

As with many build systems, there are two major kinds of builds: Debug and Release. The Debug build is the version of your app where you spend most of your development time. The compiler adds information to the executable so you can step through the code in a debugger, view the contents of variables, look at the call stack trace, and so on. The Release build is just that: the version that you release. It (usually) has all debugging information removed and the compiler is asked to optimize the code so that it runs faster, is smaller, etc. With that in mind, there are some defines that enable (or disable) the features
in **BKDebug**.

<h2>_BK_DEBUG_</h2>

Specifies that this code is enabled in the Debug build. You can write code that will only be compiled in the Debug build by using the preprocessor to check if
`_BK_DEBUG_` is defined. For example:

```C++
#if defined(_BK_DEBUG_)    // #ifdef _BK_DEBUG_ would work as well
    int debug_counter = 0;

    printf("something useful: %d\n", debug_counter);
    debug_counter++;
#endif
```

If `_BK_DEBUG_` is not defined, the code/macros completely (and automatically) disappear as if it never existed in your codebase.


<h2>_BK_RELEASE_</h2>

This is the reciprocal of `_BK_DEBUG_` in that it tells the compiler to include this section of code only in the Release build. Use the same kind of check as you would for a Debug build (see above).

--

When `_BK_DEBUG_` is defined, all calls are expanded at the point of every call, so expect your application to be larger and slower in the Debug build than the Release build; such is the nature of macros.

**Note**: You can only declare `_BK_DEBUG_` or `_BK_RELEASE_` at any one time, never both at the same time.  (A build error will occur if you try to.) If `_BK_DEBUG_` is not defined, `_BK_RELEASE_` is _not_ implied or assumed; it simply means that no debugging features or code will be enabled.


# Platform Types

These tell the compiler to include a section of code only in a platform-specific build. Use the same kind of check as you would for a Debug build (see above).

```
_BK_IOS_ for iPhone/iPad
_BK_MAC_ for Mac/OS X
_BK_WIN_ for Windows
```

**Note**: You can only declare `_BK_IOS_`, `_BK_MAC_` or `_BK_WIN_` at any one time, never more than one at the same time. (A build error will occur if you try to.)

### A Note for iOS/Mac Developers

When you specify a message for a macro (see below), or if you use the `BKDEBUG_MESSAGE()` macro (see below), do _not_ prefix them with `@` as you would do for NSLog(). All messages are literal C strings (for now).

I do have a version locally that uses `NSLog()` internally and allows `@` use as the default behavior for iOS/Mac platforms, but due to how the macros were written, `@` can only be used for `BKDEBUG_MESSAGE()` (since it's a direct replacement to `NSLog()` but _not_ for any message argument to a macro (see below). This could lead to confusion so I didn't want to include this in the main branch. I will put it in a separate branch so you can take a look at it if you have a notion (or if you prefer `NSLog()` output to `printf()`.)


# Options

<h2>_BK_DISABLE_CHECK_BREAKPOINTS_</h2>

By default, any check that fires will stop the debugger (if it's running). However, you can disable this by defining `_BK_DISABLE_CHECK_BREAKPOINTS_` before the first `#include/#import` of `BKDebug.h`'. You can add it to your build settings if you wish. (See below.)

```Objective-C
#define _BK_DISABLE_CHECK_BREAKPOINTS_
#import "BKDebug.h"
```

**Note: This will _not_ affect `BKDEBUG_BREAKPOINT()`.**


# How To Set Preprocessor Defines

There are two ways to declare preprocessor defines: as a build setting, and via code.

The easiest — and arguably preferred — way to declare them is by using build settings usually (easily done in your IDE). Each build type gets a unique set of build options. For example, here is how you would do it in Xcode for an iOS application:

![Setting build settings in Xcode](http://bobkoon.com/githubsupport/bkdebug_xcodepreprocessorsetting.png)

You can also define them programmatically. Like so:

```Objective-C
#define _BK_IOS_
#define _BK_DEBUG_
#import "BKDebug.h"
```


# Static Analysis

I have made every effort to make sure **BKDebug** builds cleanly. It doesn't generate any warnings when building and no hits are generated from static analyzers (see the Windows note below).

In order to achieve this, I had to turn off a warning (_very_ temporarily; just for one line of code and it's immediately turned on again) for Clang to get the value of the expressions displaying in a type-agnostic way.

**Windows Note**: When I run the analyzer in Visual Studio (2005, that's the only version I own, alas) each macro generates a hit. This is caused by the same line that I need to disable the warning for in Clang. The reason it's a problem (for VS2005) because it doesn't know what to do with `_Pragma()` and `#pragma` isn't usable in this case because it's all pre-processor. Indeed, that's why `_Pragma()` was created. If you are able to test this in a newer version of VS, please feel free to submit a pull request. (And thank you!) Even if a newer version does handle `_Pragma()` correctly, it still requires a change because the options given to it is Clang only. I am willing to update this myself, but I would need to know what arguments to use for the `_Pragma()` calls.


# Macro Use != Error Handling

I use these macros _everywhere_ in my application, especially for validating function arguments. Typically something like this:

```C++
void displayString(char * string)
{
    BKDEBUG_CHECK_NOTEQUAL(string, NULL, "bad pointer");
    BKDEBUG_CHECK_GREATERTHAN(strlen(string), 0);

    // rest of the function
}
```

However, doing this only tells me about immediate (development-time) problems. If I were to finish the routine like this:

```C++
void displayString(char * string)
{
    BKDEBUG_CHECK_NOTEQUAL(string, NULL, "bad pointer");
    BKDEBUG_CHECK_GREATERTHAN(strlen(string), 0);

    printf("The hills are alive with the sound of %s!\n", string);
}
```

I couldn't (indeed, _shouldn't_) call this production code because it would crash in the Release build when string is NULL. I still have to do something like this:

```C++
void displayString(char * string)
{
    BKDEBUG_CHECK_NOTEQUAL(string, NULL, "bad pointer");
    BKDEBUG_CHECK_GREATERTHAN(strlen(string), 0);

    if  (string != NULL)
    {
        printf("The hills are alive with the sound of %s!\n", string);
    }
}
```

Or something like this (which is what I prefer to do):

```C++
void displayString(char * string)
{
    BKDEBUG_CHECK_NOTEQUAL(string, NULL, "bad pointer");
    BKDEBUG_CHECK_GREATERTHAN(strlen(string), 0);

    if  (string == NULL)
    {
        return;
    }

    printf("The hills are alive with the sound of %s!\n", string);
}
```


# Call Tracing

Another handy use of **BKDebug** is tracing the route of the calls that your application makes. The `BKDEBUG_MESSAGE()` macro (see below) is perfect for this. Simply add it to the start of the routine and it will display it in the console log. Like so:

```C++
void displayString(char * string)
{
    BKDEBUG_CHECK_NOTEQUAL(string, NULL, "bad pointer");
    BKDEBUG_CHECK_GREATERTHAN(strlen(string), 0);

    BKDEBUG_MESSAGE("displayString()\n");

    // rest of the function
}
```

You could even take it further and give yourself more information about the arguments given to the routine:

```C++
void displayString(char * string)
{
    BKDEBUG_CHECK_NOTEQUAL(string, NULL, "bad pointer");
    BKDEBUG_CHECK_GREATERTHAN(strlen(string), 0);

    BKDEBUG_MESSAGE("displayString('%s')\n", string);

    // rest of the function
}
```


--


# BKDebug.h Macro List

**Note**: As you use these macros, be very careful if an `expression` involves code, like a function call for example. That code will _not_ exist in the Release build and its absence might cause side-effects in your application. If you call a function in a macro and your production code relies on that call being made, you will get problems in the Release build because that routine is no longer being called.

<h2>BKDEBUG_CHECK()</h2>

Check an expression for "truth".

**Usage:** `BKDEBUG_CHECK(expression, [message]);`

<table>
  <tr>
    <td><code>expression</code></td><td>the expression you want to validate</td>
  </tr>
  <tr>
    <td><code>message</code></td><td>an optional message that is displayed if the check fails (if you don't want a message, just call the macro with one argument)</td>
  </tr>
</table>

"Truth" in this context means that the expression, when evaluated, will be true, i.e. "is `expression` true?". If it is false, it will fire.

**Example:**

```C++
BKDEBUG_CHECK((value == 0) || (value == 1));
```


<h2>BKDEBUG_CHECK_EQUAL()</h2>

Check two expressions for equality.

This macro evaluates two expressions and checks their results for equality, i.e. "does `left_expression` = `right_expression`?". If they are not equal, it will fire.

**Usage:** `BKDEBUG_CHECK_EQUAL(left_expression, right_expression, [message]);`

<table>
  <tr>
    <td><code>left_expression</code></td><td>the expression on the left side of the =</td>
  </tr>
  <tr>
    <td><code>right_expression</code></td><td>the expression on the right side of the =</td>
  </tr>
  <tr>
    <td><code>message</code></td><td>an optional message that is displayed if the check fails (if you don't want a message, just call the macro with two arguments)</td>
  </tr>
</table>

**Example:**

```C++
BKDEBUG_CHECK_EQUAL(first_memblock, NULL, "some memory hasn't been freed");
```


<h2>BKDEBUG_CHECK_NOTEQUAL()</h2>

Check two expressions for inequality.

This macro evaluates two expressions and checks their results for inequality, i.e. "does `left_expression` != `right_expression`?". If they are equal, it will fire.

**Usage:** `BKDEBUG_CHECK_NOTEQUAL(left_expression, right_expression, [message]);`

<table>
  <tr>
    <td><code>left_expression</code></td><td>the expression on the left side of the !=</td>
  </tr>
  <tr>
    <td><code>right_expression</code></td><td>the expression on the right side of the !=</td>
  </tr>
  <tr>
    <td><code>message</code></td><td>an optional message that is displayed if the check fails (if you don't want a message, just call the macro with two arguments)</td>
  </tr>
</table>

**Example:**

```C++
BKDEBUG_CHECK_NOTEQUAL(num_bytes, 0, "trying to allocate a size of 0");
```


<h2>BKDEBUG_CHECK_LESSTHAN()</h2>

Check if the left expression is less than the right expression.

This macro evaluates two expressions and checks if `left_expression` is less than `right_expression`. If it is not, the macro will fire.

**Usage:** `BKDEBUG_CHECK_LESSTHAN(left_expression, right_expression, [message]);`

<table>
  <tr>
    <td><code>left_expression</code></td><td>the expression on the left side of the <</td>
  </tr>
  <tr>
    <td><code>right_expression</code></td><td>the expression on the right side of the <</td>
  </tr>
  <tr>
    <td><code>message</code></td><td>an optional message that is displayed if the check fails (if you don't want a message, just call the macro with two arguments)</td>
  </tr>
</table>

**Example:**

```C++
BKDEBUG_CHECK_LESSTHAN(timer_id, num_timers, "timer_id is too large");
```


<h2>BKDEBUG_CHECK_GREATERTHAN()</h2>

Check if the left expression is greater than the right expression.

This macro evaluates two expressions and checks if `left_expression` is greater than `right_expression`. If it is not, the macro will fire.

**Usage:** `BKDEBUG_CHECK_GREATERTHAN(left_expression, right_expression, [message]);`

<table>
  <tr>
    <td><code>left_expression</code></td><td>the expression on the left side of the ></td>
  </tr>
  <tr>
    <td><code>right_expression</code></td><td>the expression on the right side of the ></td>
  </tr>
  <tr>
    <td><code>message</code></td><td>an optional message that is displayed if the check fails (if you don't want a message, just call the macro with two arguments)</td>
  </tr>
</table>

**Example:**

```C++
BKDEBUG_CHECK_GREATERTHAN(timer_id, -1, "timer_id is too small");
```


<h2>BKDEBUG_CHECK_LESSTHANEQUAL()</h2>

Check if the left expression is less than or equal to the right expression.

This macro evaluates two expressions and checks if `left_expression` is less than or equal to `right_expression`. If it is not, the macro will fire.

**Usage:** `BKDEBUG_CHECK_LESSTHANEQUAL(left_expression, right_expression, [message]);`

<table>
  <tr>
    <td><code>left_expression</code></td><td>the expression on the left side of the <=</td>
  </tr>
  <tr>
    <td><code>right_expression</code></td><td>the expression on the right side of the <=</td>
  </tr>
  <tr>
    <td><code>message</code></td><td>an optional message that is displayed if the check fails (if you don't want a message, just call the macro with two arguments)</td>
  </tr>
</table>

**Example:**

```C++
BKDEBUG_CHECK_LESSTHANEQUAL(frame_data->m_x2, 1.0f);
```


<h2>BKDEBUG_CHECK_GREATERTHANEQUAL()</h2>

Check if the left expression is greater than or equal to the right expression.

This macro evaluates two expressions and checks if `left_expression` is greater than or equal to `right_expression`. If it is not, the macro will fire.

**Usage:** `BKDEBUG_CHECK_GREATERTHANEQUAL(left_expression, right_expression, [message]);`

<table>
  <tr>
    <td><code>left_expression</code></td><td>the expression on the left side of the >=</td>
  </tr>
  <tr>
    <td><code>right_expression</code></td><td>the expression on the right side of the >=</td>
  </tr>
  <tr>
    <td><code>message</code></td><td>an optional message that is displayed if the check fails (if you don't want a message, just call the macro with two arguments)</td>
  </tr>
</table>

**Example:**

```C++
BKDEBUG_CHECK_GREATERTHANEQUAL(string_id, getFirstStringID(), "string_id is too low");
```


<h2>BKDEBUG_CHECK_WITHINRANGE()</h2>

Check if the left expression is within a certain value range of the right expression.

This macro evaluates two expressions and checks that `left_expression` is equal to `right_expression` within the specified `tolerance`. If `left_expression` is outside `tolerance`, the macro will fire.

**Usage:** `BKDEBUG_CHECK_WITHINRANGE(left_expression, right_expression, tolerance, [message]);`

<table>
  <tr>
    <td><code>left_expression</code></td><td>the expression on the left side of the =</td>
  </tr>
  <tr>
    <td><code>right_expression</code></td><td>the expression on the right side of the =</td>
  </tr>
  <tr>
    <td><code>tolerance</code></td><td>the 'range' of acceptable differences to <code>right_expression</code></td>
  </tr>
  <tr>
    <td><code>message</code></td><td>an optional message that is displayed if the check fails (if you don't want a message, just call the macro with three arguments)</td>
  </tr>
</table>

**Example:**

```C++
BKDEBUG_CHECK_WITHINRANGE(num_active_objects, 10, 2);   // 8 <-> 12
```


<h2>BKDEBUG_CHECK_WITHINRANGE_FLOAT()</h2>

Check if the left expression is within a certain value range of the right expression; evaluated as floats.

This macro evaluates two expressions and checks if `left_expression` is equal to `right_expression` within the specified `tolerance`. If `left_expression` is outside `tolerance`, the macro will fire.

**Note**: This macro is the float version of `BKDEBUG_CHECK_WITHINRANGE()`. That is, all arguments are evaluated as floats.

**Usage:** `BKDEBUG_CHECK_WITHINRANGE_FLOAT(left_expression, right_expression, tolerance, [message]);`

<table>
  <tr>
    <td><code>left_expression</code></td><td>the expression on the left side of the =</td>
  </tr>
  <tr>
    <td><code>right_expression</code></td><td>the expression on the right side of the =</td>
  </tr>
  <tr>
    <td><code>tolerance</code></td><td>the 'range' of acceptable differences to <code>right_expression</code></td>
  </tr>
  <tr>
    <td><code>message</code></td><td>an optional message that is displayed if the check fails (if you don't want a message, just call the macro with three arguments)</td>
  </tr>
</table>

**Example:**

```C++
BKDEBUG_CHECK_WITHINRANGE_FLOAT(sine_value, 0.0f, 1.0f);   // -1.0 <-> 1.0
```


<h2>BKDEBUG_MESSAGE()</h2>

Display a `printf()`-style message to the debugging console.

This is simply a wrapper for `printf()` that allows you to see messages in the console as the program runs. Call it as you would call `printf()`.

**Usage:** `BKDEBUG_MESSAGE(format, ...);`


<h2>BKDEBUG_BREAKPOINT()</h2>

Halt execution and interrupt the debugger as if a breakpoint was triggered.

This macro can be thought of as a software breakpoint. Simply add a call to anywhere in your code and start the debugger. The code will stop once the macro is reached.

**Usage:** `BKDEBUG_BREAKPOINT();`

**Note: The `_BK_DISABLE_CHECK_BREAKPOINTS_` option (see above) does _not_ affect this.**


<h2>BKDEBUG_CODE()</h2>

Execute code in the Debug build only.

This macro is a way to inject code in the Debug build. You can declare any code with this macro and it is completely removed from the Release build. You can even declare variables in one section of code, and access them in another.

**Usage:** `BKDEBUG_CODE(code);`

<table>
  <tr>
    <td><code>code</code></td><td>the code statement to execute</td>
  </tr>
</table>

**Example:**

```C++
BKDEBUG_CODE(int debug_test = 0);      // the variable is declared here

/* production code here */

BKDEBUG_CODE(debug_test = 1);          // the variable is accessed again here

/* production code here */

BKDEBUG_MESSAGE("%d\n", debug_test);   // displays '1' here
```

**Note**: Be very careful using this macro. The code will _not_ exist in the Release build and its absence might cause side-effects.


<h2>BKDEBUG_COMPILETIME_ARRAY_CHECK()</h2>

Check a static array at compile time for validity.

### Example usage:

```C++
enum ColorID
{
    kColorIDNone,

    kColorIDRed,
    kColorIDGreen,
    kColorIDBlue,

    kNumColorIDs
};

const char * const  color_table[] =
{
    "kColorIDRed",
    "kColorIDGreen",
    "kColorIDBlue",
};
BKDEBUG_COMPILETIME_ARRAY_CHECK(staticarray_count(color_table) == kNumColorIDs, array_count_doesnt_match_enum_count);
```

This is a special macro in that it is a build-time check instead of the usual run-time checks the other macros perform. Use it with `staticarray_count()` found in `BKTypes.h` (see below).

When this macro fires, you will get a build error on the `BKDEBUG_COMPILETIME_ARRAY_CHECK` line and the error will look something like this:

```
/example/project/main.m:26:1: 'static_assertion_array_count_doesnt_match_enum_count' declared as an array with a negative size
```

### Discussion

You might be asking why this kind of macro is needed at all. Consider this...

```C++
const char * const  color_table[kNumColorIDs] =
{
    "kColorIDRed",
    "kColorIDGreen",
    "kColorIDBlue"
};
BKDEBUG_COMPILETIME_ARRAY_CHECK(staticarray_count(color_table) == kNumColorIDs, array_count_doesnt_match_enum_count);
```

This is clearly not the intent of the author, but it builds quietly. Why?

The array has an explicit size so the compiler creates an entry at each index. After it does this, the size of the array _does_ match the number of entries. (In this case, the `kNumColorIDs` enum.)

To further point out the kinds of errors this can cause, take a look at this code:

```C++
const int num_array_elements = staticarray_count(color_table);
for (int i = 0; i < num_array_elements; i++)
{
    printf("%s\n", color_table[i]);
}
```

It produces the following output (for iOS):

```
kColorIDRed
kColorIDGreen
kColorIDBlue
(null)
```

To prevent all of these potential problems, you can create your array like so:

```C++
const char * const  color_table[] =
{
    "kColorIDNone",

    "kColorIDRed",
    "kColorIDGreen",
    "kColorIDBlue",
};
BKDEBUG_COMPILETIME_ARRAY_CHECK(staticarray_count(color_table) == kNumColorIDs, array_count_doesnt_match_enum_count);
```

This will give you a build error whenever the array count is different to what you expect.


--


# BKTypes.h Macro List

<h2>staticarray_count()</h2>

This macro returns the number of elements in a static array.

**Usage:** `staticarray_count(array);`

<table>
  <tr>
    <td><code>array</code></td><td>a static array to get the count for</td>
  </tr>
</table>

**Example:**

```C++
const int num_array_elements = staticarray_count(color_table);
for (int i = 0; i < num_array_elements; i++)
{
    printf("%s\n", color_table[i]);
}
```


<h2>merged Types</h2>

These types exist as a convenience to access individual bytes from within a larger type.

This is useful when you need to access individual bytes in a type, but still want to handle it as a base type (packet data, for example). This also eliminates the requirement for a struct and prevents the use of bitfields. Using these types also helps with structure packing.

```
merged32
merged16
merged8
```

It is probably easier to understand how these types work by looking at an example:

```C++
merged32    data;

data.m_u32      = 0;    // clear all the bits
data.m_u8.m_A   = 1;    // set only the A byte as unsigned
data.m_u8.m_B   = 2;    // set only the B byte as unsigned
data.m_s16.m_DC = -3;   // set only the CD word as signed

BKDEBUG_MESSAGE("data as u32:    0x%08X\n", data.m_u32);
BKDEBUG_MESSAGE("data as s32:    %d\n", data.m_s32);
BKDEBUG_MESSAGE("data as void *: %p\n", data.m_ptr);
```

Displays:

```
data as u32:    0x0102FFFD
data as s32:    16973821
data as void *: 0x102fffd

 A =   01
 B =   02
CD = FFFD
```

### Why 'DC' and not 'CD'?

I did this because of endianness. When I look at the contents in my debugger as a hex value, I want to see it in ABCD order. To do this, the bytes are reversed in memory: DCBA.


# License

Copyright (c) 2003 Bob Koon, released under an MIT license.
