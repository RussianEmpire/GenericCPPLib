#ifndef TestUtilsH
#define TestUtilsH

//// [!] Version 1.001 [!]

#include "CPPUtils.h"   // for 'CONSTEXPR_'
#include "MacroUtils.h" // for 'MAKE_STR_'

#include <cassert>   // for 'assert'
#include <stdexcept> // for 'std::runtime_error'

namespace TestUtils {
  #ifndef ASSERT_
    // 'CustomErrMsg' CAN be empty ("") - if so, auto. generated one will be used instead
    #define ASSERT_(Assumption, CustomErrMsg)\
    /*'volatile' used to prevent the compiler from optimizing out the code
       (to ensure it will be executed AND tested then)*/\
    if (const volatile auto assert__result__local__temp__ = !(Assumption)) {\
      assert(! #Assumption);\
      \
      static CONSTEXPR_ const char* const CUSTOM_ERR_MSG_ = "" CustomErrMsg;\
      static CONSTEXPR_ auto const AUTO_ERR_MSG_ =\
        "[!] '"#Assumption"' failed at line #" MAKE_STR_(__LINE__) " in file \"" __FILE__ "\"";\
      static CONSTEXPR_ auto const ERR_MSG_ = /* if custom is NOT empty - use it*/\
        CUSTOM_ERR_MSG_ && *CUSTOM_ERR_MSG_ ? CUSTOM_ERR_MSG_ : AUTO_ERR_MSG_;\
      \
      std::cout << '\n' << ERR_MSG_ << '\n';\
      throw std::runtime_error(ERR_MSG_);\
    }
  #else
    static_assert(false, "Error: 'ASSERT_' macro is already defined somewhere");
  #endif
  
  #ifndef ASSERT__
    // Uses auto. generated err. msg.
    #define ASSERT__(Assumption) ASSERT_(Assumption, "")
  #else
    static_assert(false, "Error: 'ASSERT__' macro is already defined somewhere");
  #endif
}

#endif // TestUtilsH