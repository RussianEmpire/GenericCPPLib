#include "WinAPIUtils.h"

#include <algorithm>

// Does NOT synched.
const char* WinAPIUtils::getLogFileNameFromAppName() throw() {
  static auto* const LOG_FILE_EXT = ".txt";
  static const auto LOG_FILE_EXT_LEN = strlen(LOG_FILE_EXT); // use constexpr if possible
  static char appName[MAX_PATH + 1U + 8U - (MAX_PATH + 1U) % 8U]; // make 8 even
  static_assert(!(sizeof(appName) % 8U), "SHOULD be 8 even");

  if (!*appName) { // NOT yet determined
    auto len = GetModuleFileName(nullptr, appName, std::extent<decltype(appName)>::value - 1U);
    static const decltype(len) MAX_LEN = std::extent<decltype(appName)>::value - 1U - LOG_FILE_EXT_LEN;

    if (len && *appName) { // something received
      if (!appName[len - 1U]) { // there CAN be str. terminator OR NOT
        --len; // set to the str. terminator
      }
      const auto pos = std::min<>(MAX_LEN, len);
      strcat_s(appName + pos, LOG_FILE_EXT_LEN + 1U, LOG_FILE_EXT);
    } else return ""; // call GetLastError to get extended error information
  }
  return appName;
}

// Returns ptr. to the provided storage [!] which SHOULD NOT be larger than 64K bytes [!]
// Returns empty str. if NO error
// If 'fromLowerCase' flag is true - msg. will start from lower case letter
char* WinAPIUtils::getErrorMsg(char* const msgBuf, const size_t bufSize,
                               const DWORD errorCode, const bool fromLowerCase) throw() {
  if (!msgBuf || !bufSize) return nullptr;
  *msgBuf = '\0';
  if (ERROR_SUCCESS == errorCode) return msgBuf; // operation completed successfully
  
  static const DWORD FLAGS
    = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK;
  static const auto LANG = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT); // en
  // Return value is the number of TCHARs stored in the output buffer,
  //  excluding the terminating null character
  const auto count = FormatMessage(FLAGS, nullptr, errorCode, LANG, msgBuf, bufSize, nullptr);
  if (!count || !*msgBuf) { // if the function fails, the return value is zero
    strncpy_s(msgBuf, bufSize, "unlocalized error", bufSize - 1U);
    return msgBuf; // to get extended error information, call 'GetLastError'
  }
  char* currCharPtr = msgBuf + (count - 1U);
  while (' ' == *currCharPtr || '.' == *currCharPtr)
    *currCharPtr-- = '\0'; // erase end spacers and point
  if (fromLowerCase) *msgBuf = tolower(*msgBuf);
  
  return msgBuf;
}

#include "Dbt.h"

// Returns nullptr if 'lpEventData' is NOT provided OR NOT required OR at ANY error
// [!] Do NOT forget to 'delete[]' the returned ptr. when data is NO longer needed [!]
void* WinAPIUtils::copyEventData(const DWORD dwControl, const DWORD dwEventType,
                                 LPVOID const lpEventData, bool& error) throw() {
  error = false;
  
  if (!lpEventData) return nullptr;
  auto dataSize = DWORD();
  
  switch (dwControl) {
    case SERVICE_CONTROL_SESSIONCHANGE: {
        auto const sessionData = reinterpret_cast<const WTSSESSION_NOTIFICATION*>(lpEventData);
        dataSize = std::max<>(sessionData->cbSize, static_cast<DWORD>(sizeof(*sessionData)));
      }
    break;
    
    case SERVICE_CONTROL_DEVICEEVENT: {
        auto const deviceData = reinterpret_cast<const DEV_BROADCAST_HDR*>(lpEventData);
        dataSize = std::max<>(deviceData->dbch_size, static_cast<DWORD>(sizeof(*deviceData)));
      }
    break;
    
    case SERVICE_CONTROL_POWEREVENT:
      if (PBT_POWERSETTINGCHANGE != dwEventType) return nullptr; // unused event, NOT required
      {
        auto const powerSettings = reinterpret_cast<const POWERBROADCAST_SETTING*>(lpEventData);
        dataSize = sizeof(*powerSettings) + powerSettings->DataLength - sizeof(powerSettings->Data);
      }
    break;
    
    case SERVICE_CONTROL_TIMECHANGE:
      dataSize = sizeof(SERVICE_TIMECHANGE_INFO);
    break;
    
    default: // unused control, NOT required
      return nullptr;
  }
  dataSize += 8UL; // add a few bytes to the end (NOT really required, BUT let it be)
  auto const newData = new char[dataSize](); // zero (default) initialized
  
  if (newData) { // alocated
    memcpy_s(newData, dataSize, lpEventData, dataSize);
  } else error = true; // NOT allocated
  
  return newData;
}