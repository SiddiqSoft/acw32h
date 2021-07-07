Auto Closing/Releasing Win32 Handle Objects
-------------------------------------------

# Objective
Make it easy to track the Win32 `HANDLE` and `HINTERNET` objects while keeping their use as drop-in replacement for their respective `HANDLE` or `HINTERNET` objects.
Use only when you're holding objects that your application is required to close/release.

# Requirements
- C++17
- Useful for Windows projects

# Usage
From your solution folder `git submodule add https://github.com/SiddiqSoft/acw32handle.git`

This should add `acw32handle` folder in your solution.


```c
#include <windows.h>
#include "acw32handle/acw32h.hpp"

void foo()
{
   // Use the object
   acw32h<HANDLE> h= ::CreateFileA(...);
   // Use and don't worry about any throw, exit C++ will cleanup the handle if it was properly allocated!
}

```

