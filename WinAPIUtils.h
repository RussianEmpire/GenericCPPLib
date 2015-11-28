#ifndef WinAPIUtilsH
#define WinAPIUtilsH

#include "Windows.h"

// Abstract
class WinAPIUtils {

public:
  
  static const auto DEFAULT_ERR_MSG_BUF_SIZE_ = 256U;
  
  static const char* getLogFileNameFromAppName() throw();
  
  static char* getErrorMsg(char* const msgBuf, const size_t bufSize,
                           const DWORD errorCode, const bool fromLowerCase = true) throw();
  
  // Returns nullptr if 'lpEventData' is NOT provided OR NOT required OR at ANY error
  // [!] Do NOT forget to 'delete[]' the returned ptr. when data is NO longer needed [!]
  static void* copyEventData(const DWORD dwControl, const DWORD dwEventType,
                             LPVOID const lpEventData, bool& error) throw();

private:
  //// Google C++ Style Guide: if you do not want to support copy/move operations on your type,
  ////  explicitly disable them using = delete
  WinAPIUtils() throw() = delete;
  ~WinAPIUtils() throw() = delete;
  WinAPIUtils(const WinAPIUtils&) throw() = delete;
  WinAPIUtils(WinAPIUtils&&) throw() = delete;
  WinAPIUtils& operator=(const WinAPIUtils&) throw() = delete;
  WinAPIUtils& operator=(WinAPIUtils&&) throw() = delete;
};

#endif // WinAPIUtilsH