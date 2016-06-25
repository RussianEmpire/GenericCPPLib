#ifndef TestUtilsH
#define TestUtilsH

//// [!] Version 1.002 [!]

#include "CPPUtils.h"   // for 'CONSTEXPR_11_'
#include "MacroUtils.h" // for 'MAKE_STR_'

#include <cassert>   // for 'assert'

#include <fstream>   // for 'std::ofstream'
#include <iostream>  // for 'std::cout'
#include <stdexcept> // for 'std::runtime_error'

namespace TestUtils {
  #ifndef ASSERT_
    // 'CustomErrMsg' CAN be empty ("") - if so, auto. generated one will be used instead
    #define ASSERT_(Assumption, CustomErrMsg, DoThrow)\
    /*'volatile' used to prevent the compiler from optimizing out the code
       (to ensure it will be executed AND tested)*/\
    if (const volatile auto assert__result__local__temp__ = !(Assumption)) {\
      assert(! #Assumption); /*'assert' used ONLY in the debug mode*/\
      /*Do NOT mark consts as 'astatic' to prevent C2603 Compiler Error in future!*/\
      CONSTEXPR_11_ const char* const CUSTOM_ERR_MSG_ = "" CustomErrMsg;\
      CONSTEXPR_11_ auto const AUTO_ERR_MSG_ =\
        "[!] '"#Assumption"' failed at line #" MAKE_STR_(__LINE__) " in file \"" __FILE__ "\"";\
      CONSTEXPR_11_ auto const ERR_MSG_ = /* if custom is NOT empty - use it*/\
        CUSTOM_ERR_MSG_ && *CUSTOM_ERR_MSG_ ? CUSTOM_ERR_MSG_ : AUTO_ERR_MSG_;\
      \
      std::cout << '\n' << ERR_MSG_ << '\n';\
      /*'assert__result__local__temp__' used here to get rid of '-Wunused-variable' warning*/\
      if (assert__result__local__temp__ && DoThrow) throw std::runtime_error(ERR_MSG_);\
    }
  #else
    static_assert(false, "Error: 'ASSERT_' macro is already defined somewhere");
  #endif
  
  #ifndef ASSERT__
    // Uses auto. generated err. msg. & NO throw (default mode)
    #define ASSERT__(Assumption) ASSERT_(Assumption, "", false)
  #else
    static_assert(false, "Error: 'ASSERT__' macro is already defined somewhere");
  #endif
}

#endif // TestUtilsH