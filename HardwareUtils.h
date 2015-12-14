#ifndef HardwareUtilsH
#define HardwareUtilsH

#ifdef _MSC_VER
  #include <intrin.h> // Microsoft Specific
#elif __GNUC__
  #include <cpuid.h> // GCC
#endif

class CPUInfo {

public:
  // Static singleton (static init. is a thread safe in C++11)
  static const CPUInfo INSTANCE;

  const bool is64BitCPU = false;

private:

  CPUInfo() throw()
    : is64BitCPU(findIs64BitCPU())
  {}

  ~CPUInfo() = default;

  CPUInfo(const CPUInfo&) throw() = delete;
  CPUInfo(CPUInfo&&) throw() = delete;
  CPUInfo& operator=(const CPUInfo&) throw() = delete;
  CPUInfo& operator=(CPUInfo&&) throw() = delete;

  /* Reference:
  http://stackoverflow.com/questions/12212385/detect-if-the-processor-is-64-bit-under-32-bit-os
  https://en.wikipedia.org/wiki/CPUID
  https://en.wikipedia.org/wiki/Long_mode
  https://msdn.microsoft.com/en-us/library/hskdteyh(v=vs.120).aspx
  https://msdn.microsoft.com/en-us/library/windows/desktop/ms684139(v=vs.85).aspx
  http://stackoverflow.com/questions/14266772/how-do-i-call-cpuid-in-linux
  */
  // HINT: Better rewrite this using an assembly
  //  (see examples: https://en.wikipedia.org/wiki/CPUID#CPUID_usage_from_high-level_languages)
  static bool findIs64BitCPU() throw() {
    static const auto GET_MAX_CMD_SUPPORTED = 0x80000000U; // 2 147 483 648
    
    unsigned int cpuInfo[4U] = {0}; // from EAX, EBX, ECX, and EDX
    #ifdef _MSC_VER
      auto const cpuInfoRe = reinterpret_cast<int*>(cpuInfo); // reinterpreted
      __cpuid(cpuInfoRe, GET_MAX_CMD_SUPPORTED);
      // Max. value of 'function_id' supported for extended functions
      const auto maxFuncIDSupported = *cpuInfo; // at 'EAX'
    #elif __GNUC__
      // 'ext' SHOULD be 0x8000000 to return highest supported value for extended 'cpuid' information
      // Returns 0 if 'cpuid' is not supported or whatever 'cpuid' returns in EAX register
      const auto maxFuncIDSupported = __get_cpuid_max(GET_MAX_CMD_SUPPORTED, nullptr);
    #else
      static_assert(false, "Unsupported complier"); // __BORLANDC__, __MINGW32__
    #endif
    
    // Get Extended Processor Info and Feature Bits
    static const auto GET_EXTENDED_INFO  = 0x80000001U; // 2 147 483 649
    // If does NOT supports extended flags
    if (maxFuncIDSupported < GET_EXTENDED_INFO) return false;
    #ifdef _MSC_VER
      memset(cpuInfo, 0, sizeof(cpuInfo));
      __cpuid(cpuInfoRe, GET_EXTENDED_INFO);
    #elif __GNUC__
      // Checks if 'cpuid' is supported and returns 1 for valid cpuid information or
      //  0 for unsupported cpuid level
      if (!__get_cpuid(GET_EXTENDED_INFO, cpuInfo, cpuInfo + 1U, cpuInfo + 2U, cpuInfo + 3U)) {
        return false;
      }
    #endif
    
    //' LM' (Long Mode) flag for AMD / 'EM64T' for Intel
    static const auto LONG_MODE_BIT = 536870912U; // 2 pow 29: 29-th bit
    // Check if bit is signaled
    return 0U != (LONG_MODE_BIT & cpuInfo[3U]); // from EDX (cpuInfo[3U])
  }

};

#ifdef _MSC_VER
  // GCC might NOT build with this, while MS VS 2013 will NOT build withOUT this
  const CPUInfo CPUInfo::INSTANCE;
#endif

#endif // HardwareUtilsH