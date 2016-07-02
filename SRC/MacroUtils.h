#ifndef MacroUtilsH
#define MacroUtilsH

namespace MacroUtils {
  #define M_S_(Arg) #Arg
  #define MAKE_STR_(Arg) M_S_(Arg)
}

#endif // MacroUtilsH