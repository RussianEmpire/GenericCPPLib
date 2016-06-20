#ifndef CPPUtilsH
#define CPPUtilsH

#ifndef CONSTEXPR_ // used to conditionally enable 'constexpr' funcs & variables
  #if __cplusplus >= 201103L // http://stackoverflow.com/questions/5047971/how-do-i-check-for-c11-support
    #define CONSTEXPR_ constexpr
  #else
    #define CONSTEXPR_
  #endif
#endif // CONSTEXPR_

#endif // CPPUtilsH