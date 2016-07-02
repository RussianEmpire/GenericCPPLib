#ifndef CompatibilityH
#define CompatibilityH

//// '_countof' - MS VS specific (from 'stdlib.h')
#if !defined(_MSC_VER) && !defined(_countof)
  #include <type_traits> // for 'std::extent'
  #define _countof(Array) std::extent<decltype(Array)>::value // first(0) dimension of the array
#endif

//// '_stricmp'
#ifdef __GNUC__ // GCC
  #include <strings.h>        // for 'int strcasecmp(const char *s1, const char *s2)'
  #define _stricmp strcasecmp // '_stricmp' - ISO C++ conformant
  #define _strcmpi strcasecmp // '_strcmpi' - is non standard alias to the '_stricmp'
  #define  strcmpi strcasecmp // 'strcmpi' - POSIX
  #define  stricmp strcasecmp // 'stricmp' - POSIX
#endif

//// 'localtime_s' - returns null pointer on error
#ifdef _MSC_VER // MS VS specific func. returns zero if successful
  #define __STDC_WANT_LIB_EXT1__ 1 // for bounds-checked functions, like 'gmtime_s'
  #include <time.h>
  #define localtime_s(TimePtr, ResultPtr) (localtime_s((ResultPtr), (TimePtr)) ? nullptr : (ResultPtr))
#elif __GNUC__ // GCC specific func. ['localtime_r'] returns the NULL in case an error was detected
  #define __STDC_WANT_LIB_EXT1__ 1
  #include <time.h> // for 'localtime_r' [thread-safe, libc 5.2.5+]
  tm* localtime_s(const time_t* time, tm* result) {
    // POSIX defines a thread-safe alternative 'localtime_r',
    //  which is similar to the C11 function 'localtime_s',
    //   except that it does NOT check the validity of its input parameters
    if (!time || !result) return nullptr;
    tzset(); // for portable code 'tzset' SHOULD be called BEFORE 'localtime_r';
    return localtime_r(time, result);
  }
  /* The macro realization provided below was rewrited as a func. (see above),
      as it produces annoying warning ["-Waddress"] in the GCC with '-Wall'
  #define localtime_s(TimePtr, ResultPtr)\
    (tzset(), ((TimePtr) && (ResultPtr) ? localtime_r((TimePtr), (ResultPtr)) : nullptr))
  */
#endif

#endif // CompatibilityH