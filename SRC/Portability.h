#ifndef PortabilityH
#define PortabilityH

//// [!] Version 1.000 [!]

//// OS-specific [https://sourceforge.net/p/predef/wiki/OperatingSystems/]

//// WINDOWS

// '_WIN32', '_WIN64' - Visual C++ specific macro
//  http://www.viva64.com/ru/k/0040/ | https://msdn.microsoft.com/en-us/library/b0084kay(VS.100).aspx
// 'WINVER' is from Windows SDK ('SDKDDKVer.h')
// '__windows__', 'windows' - GNU m4
//  https://www.gnu.org/software/m4/manual/m4-1.4.15/html_node/Platform-macros.html
// '__CYGWIN__' : Cygwin Environment
// '__WIN32__'  : Borland C++ [https://sourceforge.net/p/predef/wiki/OperatingSystems/]
// '__WINDOWS__': Watcom C/C++
// '__TOS_WIN__' : xlC
// '__WIN32', 'WIN32', 'WINNT': MSYS2 GCC [https://github.com/rust-lang-deprecated/time/issues/45]
#if defined _WIN32 || __WIN32 || __WIN32__ || _WIN64 || WINVER || __WINDOWS__ || __windows__ || windows ||\
            __CYGWIN__ || __TOS_WIN__
  #ifndef WINDOWS_
    #define WINDOWS_
  #else
    static_assert(false, "'WINDOWS_' macro already used");
  #endif
#endif

//// *nix

//// SOLARIS
#if defined(sun) || defined(__sun)
  #if defined(__SVR4) || defined(__svr4__) // distinguish between Solaris and SunOS
    #ifndef SOLARIS_
      #define SOLARIS_
    #else
      static_assert(false, "'SOLARIS_' macro already used");
    #endif
  #endif
#endif

//// BSD Unix

//// FreeBSD
#if defined __FreeBSD__ || __FreeBSD_kernel__ || __FreeBSD_version
  #ifndef FREE_BSD_
    #define FREE_BSD_
  #else
    static_assert(false, "'FREE_BSD_' macro already used");
  #endif
#endif

//// NetBSD
#if defined __NetBSD__ || __NetBSD_Version__
  #ifndef NET_BSD_
    #define NET_BSD_
  #else
    static_assert(false, "'NET_BSD_' macro already used");
  #endif
#endif

//// BSD Environment
#if defined FREE_BSD_ || NET_BSD_ || __OpenBSD__ || __bsdi__ || __DragonFly__ || _SYSTYPE_BSD ||\
            BSD || BSD4_2 || BSD4_3 || BSD4_4 // version
  #ifndef BSD_
    #define BSD_
  #else
    static_assert(false, "'BSD_' macro already used");
  #endif
#endif

//// Linux - based

// ' __gnu_linux__ ': GNU/Linux
// '__linux__', '__linux', 'linux': Linux kernel
// [!] Android is based on Linux, and that the Linux macros also are defined for Android [!]
#if defined __gnu_linux__ || __linux__ || __linux || linux
  #define LINUX_
#endif

//// General

#if defined LINUX_ || BSD_ || SOLARIS_
  #define UNIX_
#endif

#endif // PortabilityH