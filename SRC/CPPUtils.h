#ifndef CPPUtilsH
#define CPPUtilsH

//// [!] Version 1.002 [!]

// Used to conditionally enable latest supported 'constexpr'
#ifdef CONSTEXPR__
  static_assert(false, "Error: 'CONSTEXPR__' macro is already defined somewhere");
#endif

// C++14 'constexpr' functions may ALSO use local (including static AND thread) variables, loops, switches, 
//  unions, 'if's, other 'constexpr's; can have multiple 'return's
// [!] GCC does NOT support static 'constexpr' vars inside 'constexpr' funcs, even in the 6.1 version [!]
//      'static constexpr' vars NOT inside 'constexpr' funcs is OK
// http://stackoverflow.com/questions/26089319/is-there-a-standard-definition-for-cplusplus-in-c14
#ifndef CONSTEXPR_14_ // used to conditionally enable extended 'constexpr' funcs
  #if __cplusplus >= 201402L
    #define CONSTEXPR_14_ constexpr
    #ifndef CONSTEXPR__
      #define CONSTEXPR__ CONSTEXPR_14_
    #endif
  #else
    #define CONSTEXPR_14_
  #endif
#else
  static_assert(false, "Error: 'CONSTEXPR_14_' macro is already defined somewhere");
#endif // CONSTEXPR_

// C++11 'constexpr' functions:
//  use recursion rather than iteration; SHOULD have exactly ONE 'return' statement
//  CAN have: '?' ternary conditional operator
//  Can NOT have: 'assert', 'throw' AND MANY other statements; CAN't return void
#ifndef CONSTEXPR_11_ // used to conditionally enable simple 'constexpr' funcs & variables
  #if __cplusplus >= 201103L // http://stackoverflow.com/questions/5047971/how-do-i-check-for-c11-support
    #define CONSTEXPR_11_ constexpr
    #ifndef CONSTEXPR__
      #define CONSTEXPR__ CONSTEXPR_11_
    #endif
  #else
    #define CONSTEXPR_11_
  #endif
#else
  static_assert(false, "Error: 'CONSTEXPR_11_' macro is already defined somewhere");
#endif // CONSTEXPR_

// NO 'constexpr' supported
#ifndef CONSTEXPR__
  #define CONSTEXPR__
#endif

#endif // CPPUtilsH