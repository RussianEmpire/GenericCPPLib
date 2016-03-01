#ifndef MacroUtilsH
#define MacroUtilsH

namespace MacroUtils {
  #define M_S_(arg) #arg
  #define MAKE_STR_(arg) M_S_(arg)
}

#endif // MacroUtilsH