#ifndef FuncUtilsH
#define FuncUtilsH

#include <utility>

#define EXEC_MEMBER_PROC_IF_PRESENT(ProcName) namespace ProcName {\
  void ExecIfPresent(...) throw() {}\
  \
  template <class C, typename... TArgs>\
  void ExecIfPresent(C& obj, TArgs&... args) {\
    obj.ProcName(std::forward<TArgs>(args)...);\
  }\
};

// If NOT exists - returns the 'DefaultValue', which SHOULD be the same type as a decltype(*.FuncName())
// Works with static/const/virtual funcs
#define EXEC_MEMBER_FUNC_IF_PRESENT(FuncName, DefaultValue) namespace FuncName {\
  template <typename TReturnType = decltype(DefaultValue)>\
  auto ExecIfPresent(...) -> TReturnType {\
    return std::move(DefaultValue);\
  }\
  \
  template <class C, typename... TArgs>\
  auto ExecIfPresent(C& obj, TArgs&... args)\
   -> decltype(obj.FuncName(std::forward<TArgs>(args)...)) /* do NOT use 'const C& obj' NOR 'C::FuncName()'*/\
  {\
    return std::move(obj.FuncName(std::forward<TArgs>(args)...));\
  }\
};

#endif // FuncUtilsH