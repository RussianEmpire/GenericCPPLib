#ifndef TestUtilsH
#define TestUtilsH

#include "CPPUtils.h"   // for 'CONSTEXPR_'
#include "MacroUtils.h" // for 'MAKE_STR_'

#include <cassert>   // for 'assert'
#include <stdexcept> // for 'std::runtime_error'

namespace TestUtils {
  // 'CustomErrMsg' CAN be empty ("") - if so, auto. generated one will be used instead
  #define ASSERT_(Assumption, CustomErrMsg)\
  if (const volatile auto result = !(Assumption)) {/**/\
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

  // Uses auto. generated err. msg.
  #define ASSERT__(Assumption) ASSERT_(Assumption, "")
}

#endif // TestUtilsH