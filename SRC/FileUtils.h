#ifndef FileUtilsH
#define FileUtilsH

//// [!] Version 1.000 [!]

#include "Portability.h"
#include "CPPUtils.h" // for 'CONSTEXPR_11_'
#include "MemUtils.h" // for 'AUTO_ADJUST_MEM'

#include <cstddef> // for 'size_t', 'ptrdiff_t'

#include <mutex>
#include <atomic>

#ifdef UNIX_ // LINUX_ || BSD_ || SOLARIS_
  #include <errno.h>
  #include <unistd.h> // for 'readlink'
  #include <limits.h> // for 'PATH_MAX'
  
  #ifdef FREE_BSD_
    #include <syslimits.h> // for 'PATH_MAX' in FreeBSD
  #endif
  
  #ifdef PATH_MAX // 'PATH_MAX' need NOT be a defined constant, AND may have to be obtained using 'pathconf'
    #define PATH_MAX_DEFINED_ true
  #else
    #define PATH_MAX_DEFINED_ false
  #endif
#elif defined WINDOWS_
  #include "WinUtils.h"
#else
  static_assert(false, "Unsupported OS");
#endif

// [!] UNIX supports only the forward slash (/) as a path delimiter,
//      whereas Win32 support both the backslash (\) AND the forward slash (/) [!]
// [!] Paths AND filenames are case sensitive in UNIX, while Win32 FAT is NOT case sensitive, though
//      NTFS preserves case for directory listings BUT ignores case in file searches
//       AND other system operations [!]
namespace FileUtils {
  /* Unix consts:
     '_POSIX_PATH_MAX' - maximum number of bytes in a pathname; value: 255; <limits.h>
     'MAXNAMLENGTH' - FreeBSD; <dir.h> [https://forums.freebsd.org/threads/34200/]
     'NAME_MAX' - uniform system limit (if ANY) for the length of a file name component,
                   NOT including the terminating null character
                  the GNU C Library defines 'NAME_MAX', BUT does NOT actually enforce this limit
     'FILENAME_MAX' - maximum length of a file name string; <stdio.h>
                      unlike 'PATH_MAX', this macro is defined even if there is NO actual limit imposed,
                       in such a case, its value is typically a very large number (use dynamic allocation)
     'MAXNAMLEN' - Solaris; 255; (PATH_MAX = 1024)
                   [https://docs.oracle.com/cd/E19455-01/806-1075/msgs-1358/index.html]
  */
  // Thread-safe as a static init. is a thread safe in the C++11
  // Returns zero if the limit is NOT set OR in case of ANY error
  // Note that buffer for the full file name ALWAYS SHOULD have +1 length for the str. terminator
  #ifdef WINDOWS_
    CONSTEXPR_11_
  #endif
  static size_t getMaxPathLen() throw() {
    #ifdef UNIX_
      // 'realpath' works by default with the path up to 'PATH_MAX'
      // See 'PATH_MAX' problems:
      //  http://stackoverflow.com/questions/833291/is-there-an-equivalent-to-winapis-max-path-under-linux-unix
      //  http://insanecoding.blogspot.ru/2007/11/pathmax-simply-isnt.html
      // '_PC_PATH_MAX' - this number does not include a terminating 'NULL' for pathnames stored as strings
      // POSIX warns that the result of 'pathconf' may be huge AND unsuitable for mallocing memory,
      //  AND on the other hand 'pathconf' may return -1 to signify that 'PATH_MAX' is NOT bounded
      // 'errno' is thread-local
      static const ptrdiff_t MAX_PATH_LEN_ =
        PATH_MAX_DEFINED_ ? PATH_MAX : (errno = 0, pathconf("/", _PC_PATH_MAX));
      static const size_t RESULT_LEN_ =
        MAX_PATH_LEN_ <= ptrdiff_t() ? size_t() : static_cast<size_t>(MAX_PATH_LEN_);

      return RESULT_LEN_;
    #elif defined WINDOWS_
      // "D:\some 256-character path string<NUL>"
      /* The Windows API has many functions that also have Unicode versions to permit an extended-length path
          for a maximum total path length of 32,767 characters
         This type of path is composed of components separated by backslashes, each up to the value returned
          in the 'lpMaximumComponentLength' parameter of the 'GetVolumeInformation' function
           (this value is commonly 255 characters)
      */
      return static_cast<size_t>(MAX_PATH);
    #endif
  }
  
  // Thread-safe as a static init. is a thread safe in the C++11
  // Returns aligned size
  #ifdef WINDOWS_
    CONSTEXPR_11_
  #endif
  static size_t getBufSizeForFullPath() throw() {
    #ifdef UNIX_
      static CONSTEXPR_11_ const size_t DEFAULT_BUF_SIZE_ = 4096U + 8U; // typical is 256, 1024
      static CONSTEXPR_11_ const size_t BUF_SIZE_LIMIT_ = 64U * 1024U; // NTFS allows up to 32K characters

      static const size_t BUF_SIZE_RAW_ = getMaxPathLen() + 1U;
      static const size_t BUF_SIZE_ADJUSTED_ = BUF_SIZE_RAW_ < size_t(2U) || BUF_SIZE_RAW_ > BUF_SIZE_LIMIT_ ?
                                                 DEFAULT_BUF_SIZE_ : AUTO_ADJUST_MEM(BUF_SIZE_RAW_, 8U);
      return BUF_SIZE_ADJUSTED_;
    #elif defined WINDOWS_
      // That probably would be NEVER fixed:
      //  https://visualstudio.uservoice.com/forums/121579-visual-studio/suggestions/2156195-fix-260-character-file-name-length-limitation
      return static_cast<size_t>(AUTO_ADJUST_MEM(MAX_PATH, 8U));
    #endif
  }
  
  // Thread-safe
  // Returns empty str. at ANY error
  static const char* getExeName() throw() {
    static const auto NAME_BUF_SIZE_ = getBufSizeForFullPath();
    // SHOULD be lock-free; NOT atomic init., BUT synched as a static
    static std::atomic<char*> NAME_BUF_(nullptr);
    
    thread_local char* NAME_BUF_THREAD_; // will be nullptr as a static: 'thread_local' is implicitly 'static'
    if (NAME_BUF_THREAD_) return NAME_BUF_THREAD_; // optimization

    // Writes in other threads (that release) are visible in the current thread
    if ((NAME_BUF_THREAD_ = NAME_BUF_.load(std::memory_order_acquire))) return NAME_BUF_THREAD_;
    static std::mutex MUTEX_;
    const std::lock_guard<decltype(MUTEX_)> autoLock(MUTEX_);
    // Double checking lock; no synchronization or ordering constraints, only atomicity
    if ((NAME_BUF_THREAD_ = NAME_BUF_.load(std::memory_order_relaxed))) return NAME_BUF_THREAD_;
    
    NAME_BUF_THREAD_ = new(std::nothrow) char[NAME_BUF_SIZE_]; // returns nullptr if failed
    if (!NAME_BUF_THREAD_) return "";
    *NAME_BUF_THREAD_ = '\0';

    #ifdef UNIX_
      /* The same 'ELF' executable format is used by the Linux, Solaris, BSD
         BSD family have binary compatibility with the Linux kernel in usermode
          by translating Linux system calls into BSD ones;
           this enables the application and libraries code that run on Linux-based OSes to be run on BSD
         'lxrun' is a compatibility layer to allow Linux binaries to run on UnixWare, SCO OpenServer and Solaris
          without recompilation
      */
      static CONSTEXPR_11_ const char* const EXE_NAME_LINKS_[] =
        {"/proc/self/exe"     /*Linux*/,                  "/proc/curproc/exe"     /*NetBSD*/,
         "/proc/curproc/file" /*FreeBSD, DragonFly BSD*/, "/proc/self/path/a.out" /*Solaris*/};
      
      decltype(readlink("", NAME_BUF_THREAD_, size_t())) charCount;
      for (auto const currLink: EXE_NAME_LINKS_) {
        // Returns something like '/home/tMckGg/prog'
        charCount = readlink(currLink, NAME_BUF_THREAD_, NAME_BUF_SIZE_ - 1U);
        if (charCount > decltype(charCount)()) { // return the number of bytes placed; -1 on error
          NAME_BUF_THREAD_[charCount] = '\0'; // 'readlink' system call does NOT append a NUL character
          break; // found
        }
      } // see 'errno' for error code if ANY
    #elif defined(WINDOWS_) // receives the fully qualified path of the module
      #if !(WINVER >= _WIN32_WINNT_WINXP || WINVER >= _WIN32_WINNT_WS03)
        static_assert(false, "Minimum supported OS: Windows XP / Windows Server 2003");
      #endif
      // Path of the executable file of the current process (as a null-terminated string)
      if (!GetModuleFileNameA(nullptr, NAME_BUF_THREAD_, NAME_BUF_SIZE_)) // returns 0 if fails
        *NAME_BUF_THREAD_ = '\0'; // call 'GetLastError' for the extended error information
      
      /* '_get_pgmptr' will return the full path of the executable file
          IF the program is run from the command interpreter
         When a program is NOT run from the command line, '_get_pgmptr' might return the program name
          (the file's base name without the file name extension) OR to a file name, relative path, or full path
         MinGW CAN define it to 'NULL'
         Also, '_pgmptr' is probably initialised using 'GetModuleFileName' internally
      */
    #endif
    
    // Writes in the current thread are visible in other threads
    NAME_BUF_.store(NAME_BUF_THREAD_, std::memory_order_release);
    return NAME_BUF_THREAD_;
  }
}

#endif // FileUtilsH