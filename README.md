Auto Closing/Releasing Win32 Handle Objects
-------------------------------------------

# Objective
Make it easy to track the Win32 `HANDLE` and `HINTERNET` objects while keeping their use as drop-in replacement for their respective `HANDLE` or `HINTERNET` objects.
Use only when you're holding objects that your application is required to close/release.

# Requirements
- C++17
- Useful for Windows projects

# Usage

- Use the nuget [SiddiqSoft.acw32h]()
- You can also git submodule: `git submodule add https://github.com/SiddiqSoft/acw32handle.git`

Example (when using nuget to add the header in the solution)

```c
#include <windows.h>
#include "siddiqsoft/acw32h.hpp"

void foo()
{
   // Use the object
   siddiqsoft::acw32h<HANDLE> h( ::CreateFileA(...));
   // Use and don't worry about any throw, exit; C++ will cleanup the handle if it was properly allocated!
}

```

<small align="right">

&copy; 2020 Siddiq Software LLC. All rights reserved. Refer to [LICENSE](LICENSE).

</small>
