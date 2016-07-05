#ifndef WinUtilsH
#define WinUtilsH

//// [!] Version 1.000 [!]

#include "Portability.h"

#ifdef WINDOWS_
  #include <windows.h>
  //// Undefine MS specific 'max' macro, which interferents with the 'std::mimeric_limits<>::max'
  #ifdef max
    #undef max
  #endif
  #ifdef min
    #undef min
  #endif
#endif

#endif // WinUtilsH