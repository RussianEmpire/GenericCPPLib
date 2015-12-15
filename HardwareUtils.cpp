#include "HardwareUtils.h"

#ifdef _MSC_VER
  // GCC might NOT build with this, while MS VS 2013 will NOT build withOUT this
  const CPUInfo CPUInfo::INSTANCE;
#endif