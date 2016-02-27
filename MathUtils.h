#ifndef MathUtilsH
#define MathUtilsH

//// [!] Version 1.019 [!]

#include "..\..\TypeHelpers.h"

#include <cstring>
#include <cassert>
#include <cstdio>

#include <mutex>
#include <atomic>
#include <limits>
#include <algorithm>

// Abstract
// [!] In C++14 many of this funcs can be 'constexpr' [!]
class MathUtils {

public:

  static const long double DEFAULT_CMP_EPSILON_NEIGHBORHOOD_;

  // Epsilon-neighborhood: https://en.wikipedia.org/wiki/Neighbourhood_(mathematics)#In_a_metric_space
  // Complexity: constant O(1)
  template<typename TType1, typename TType2>
  static bool isApproxEqual(const TType1& val1, const TType2& val2,
                            const long double& eps = DEFAULT_CMP_EPSILON_NEIGHBORHOOD_) throw()
  {
    // https://ru.wikipedia.org/wiki/Эпсилон-окрестность
    return std::abs(val1 - val2) < std::abs(eps);
  }

  // Up to 10^18; returns zero if degree > 18
  // Complexity: constant - O(1)
  static long long int getTenPositiveDegree(const size_t degree = 0U) throw() {
    static const long long int TABLE[] =
       // 32 bit
      {1LL, 10LL, 100LL, 1000LL, 10000LL, 100000LL, 1000000LL, 10000000LL, 100000000LL, 1000000000LL,
       // 64 bit
       10000000000LL, 100000000000LL, 1000000000000LL, 10000000000000LL, 100000000000000LL,
       1000000000000000LL, 10000000000000000LL, 100000000000000000LL, 1000000000000000000LL};
    return degree < (sizeof(TABLE) / sizeof(*TABLE)) ? TABLE[degree] : 0LL; // 0 if too high
  }

  // Returns 1 for (0, 21), 6 for (1, 360), 2 for (1, 120), 7 for (0, 7), 4 for (1, 40);
  //         0 for (0, 920) OR (3, 10345) OR (0, 0) OR (4, 10) etc
  // 'allBelowOrderDigitsIsZero' will be true for
  //  (0, 7) OR (0, 20) OR (1, 130) OR (2, 12300) OR (0, 0)
  // Negative numbers are allowed; order starts from zero
  //  (for number 3219 zero order digit is 9)
  // Complexity: constant - O(1)
  static size_t getDigitOfOrder(const size_t order, const long long int num,
                                bool& allBelowOrderDigitsIsZero) throw()
  {
    const auto degK = getTenPositiveDegree(order);
    const auto shiftedVal = num / degK; // shifting to the speicified order, ignoring remnant
    allBelowOrderDigitsIsZero = num == (shiftedVal * degK); // shift back without adding remnant
    return static_cast<size_t>(std::abs(shiftedVal % 10LL)); // getting the exact digit
  }

  // 'arraySize' will holds {9, 0, 2, 6, 4, 1} for 902641,
  //                        {5, 4, 8, 2, 3} for 54823 etc if 'symbols' is false
  //  {'9', '0', '2', '6', '4', '1', '\0'} AND {'5', '4', '8', '2', '3', '\0'} otherwise
  // 'count' will hold actual digits count (64 bit numbers can have up to 20 digits)
  //  i. e. 'count' determines digit's degree (highest nonze-zero order)
  // Fills buffer in the reversed order (from the end), returns ptr. to the actual str. start
  // Returns nullptr on ANY error
  // Negative numbers ok, adds sign. for the negative num. if 'symbols' is true
  // Can be used as a num-to-str. convertion function
  //  (produces correct POD C str. if 'symbols' is true)
  // 'num' will hold the remain digits, if the buffer is too small
  // [!] OPTIMIZATION HINT: wrap args. into the struct passed by reference [!]
  // Complexity: linear - O(n) in the number degree (log10(num))
  static char* getArrayOfDigits(long long int& num, size_t& count,
                                char* arrayPtr, size_t arraySize,
                                size_t (&countByDigit)[10U], // statistics
                                const bool symbols = false) throw()
  {
    count = size_t();
    memset(countByDigit, 0, sizeof(countByDigit));

    if (!arrayPtr || arraySize < 2U) return nullptr;
    arrayPtr += arraySize; // shift to the past-the-end
    if (symbols) {
      *--arrayPtr = '\0'; // placing str. terminator
      --arraySize; // altering remain size
    }
    const bool negative = num < 0LL;
    if (negative) num = -num; // revert ('%' for the negative numbers produces negative results)
    char currDigit;
    
    auto getCurrCharUpdateIfReqAndAdd = [&]() throw() {
      currDigit = num % 10LL;
      num /= 10LL;

      ++countByDigit[currDigit];
      if (symbols) currDigit += '0';
      
      *--arrayPtr = currDigit;
      ++count;
    };
    
    while (true) {
      getCurrCharUpdateIfReqAndAdd(); // do the actual job
      if (!num) break; // that was a last digit
      if (count >= arraySize) return nullptr; // check borders
    }
    if (negative && symbols) {
      if (count >= arraySize) return nullptr; // check borders
      *--arrayPtr = '-'; // place negative sign.
    }
    return arrayPtr;
  }

  // 90980000 -> 9098, -1200 -> -12, 46570 -> 4657, -44342 -> -44342, 0 -> 0 etc
  // Ok with negative numbers
  // Complexity: linear - O(n) in the 1 + trailing zero digits count in a number,
  //  up to the number degree (log10(num))
  static void rewindToFirstNoneZeroDigit(long long int& num,
                                         size_t& skippedDigitsCount) throw()
  {
    skippedDigitsCount = size_t();
    if (!num) return;

    auto currDigit = num % 10LL;
    while (!currDigit) { // while zero digit
      num /= 10LL;
      ++skippedDigitsCount;

      currDigit = num % 10LL;
    }
  }

  // (2, 1234) -> 34; (0, <whatever>) -> 0; (1, -6732) -> 2; (325, 12) -> 12
  // Complexity: constant - O(1)
  template<typename TNumType>
  static TNumType getLastNDigitsOfNum(const size_t n, const TNumType num) throw() {
    if (!n || !num) return static_cast<TNumType>(0);

    const auto divider = getTenPositiveDegree(n);
    if (!divider) return num; // if too high 'n'
    
    auto result = static_cast<TNumType>(num % divider);
    if (result < TNumType()) result = -result; // revert if negative

    return result;
  }
  
  // [!] Unoptimal, linear complexity, do NOT use [!]
  // Better for the low numbers
  // 0 -> 1; 3 -> 1; 45 -> 2; -45675 = 5; 236693856836413986 = 18
  template<typename TNumType>
  static size_t getDigitCount(TNumType num) throw() {
    static const auto divider = static_cast<TNumType>(10);
    size_t count = size_t();
    do {
      ++count;
      num /= divider;
    } while (num);
    return count;
  }

  // [!] Use standart log10 instead,
  //  see: http://stackoverflow.com/questions/7317414/what-is-the-complexity-of-the-log-function[!]
  // Returns 10 for 2362659283, 4 for -9543, 1 for 9, 1 for 0 etc
  // Complexity: logarithmic - O(log2(TABLE item count) + 1)
  static size_t getDigitCountEx(long long int num) throw() {
    static const long long int TABLE[] =
       // 32 bit
      {0LL, 10LL, 100LL, 1000LL, 10000LL, 100000LL, 1000000LL, 10000000LL, 100000000LL, 1000000000LL,
       // 64 bit
       10000000000LL, 100000000000LL, 1000000000000LL, 10000000000000LL, 100000000000000LL,
       1000000000000000LL, 10000000000000000LL, 100000000000000000LL, 1000000000000000000LL};
    static const auto TABLE_END = TABLE + std::extent<decltype(TABLE)>::value; // past-the-end elem.
    
    if (num < 0LL) num = -num; // reverse
    // Returns a ptr. to the first elem. in the range, which compares greater
    return std::upper_bound<>(TABLE, TABLE_END, num) - TABLE;
  }

  // (0, <whatever>) -> 0; (97, 21) -> 21; (56453, 0) -> 0; (3, 546429) -> 546
  // Complexity: amortized constant ~O(1), based on the 'log10' complexity
  //  [http://stackoverflow.com/questions/7317414/what-is-the-complexity-of-the-log-function]
  template<typename TNumType>
  static TNumType getFirstNDigitsOfNum(const size_t n, const TNumType num) throw() {
    if (!n) return TNumType();
    
    const auto count = static_cast<size_t>(log10(num)) + 1U;
    if (count <= n) return num;
    
    return num / static_cast<TNumType>(getTenPositiveDegree(count - n)); // reduce
  }
  
  // [!] Involves precison penalty on 'fractPart'
  //      (999999999.99999905 -> 999999999.99999904632568359), use standart C 'modf' instead [!]
  // [http://stackoverflow.com/questions/2594913/getting-the-fractional-part-of-a-float-without-using-modf]
  // Separates fraction on integral AND fractional parts
  // [!] Both parts will be non-negative regardless of the original num. sign. [!]
  // Complexity: constant O(1)
  static void separateNum(double num, unsigned long long int& intPart, double& fractPart) throw() {
    if (num < 0.0) num = -num; // revert; [std::abs]
    intPart = static_cast<std::decay<decltype(intPart)>::type>(num);
    fractPart = num - static_cast<double>(intPart);
  }

  struct SeparateFractionData {
    
    SeparateFractionData(const long double fract_,
                         const char** const errMsg_ = nullptr,
                         char* const strBuf_ = nullptr, const size_t strBufSize_ = size_t()) throw()
      : fract(fract_), errMsg(errMsg_), strBuf(strBuf_), strBufSize(strBufSize_)
    {}
    
    ~SeparateFractionData() = default;

    long double fract;

    unsigned long long int intPart;
    size_t intPartLen; // in digits [NOT including sign], at least one each (0.0)

    // CAN have a precison penalty (12345.54321 -> 5432099999998174 instead of 54321)
    unsigned long long int fractPart;
    size_t fractPartlen;
    size_t fractPartZeroesAheadSkippedCount;

    size_t totalLen; // of the result str.
    char fractionDelim = '.'; // OR ','

    //// Optional

    size_t errCount;
    const char** errMsg = nullptr;

    char* strBuf = nullptr;
    size_t strBufSize = size_t();

  private:

    //// Drop as ref. can NOT be copied
    SeparateFractionData(const SeparateFractionData&) throw() = delete;
    SeparateFractionData(SeparateFractionData&&) throw() = delete;
    SeparateFractionData& operator=(const SeparateFractionData&) throw() = delete;
    SeparateFractionData& operator=(SeparateFractionData&&) throw() = delete;
  };
  
  // [!] Involves precision penalty (like 'std::modf'), use standart 'sprintf' to get rid of it [!]
  // Separate fraction, get it's integral AND fractional parts as a two unsigned ints (+ other data)
  //  AND convert it to a str. (if correct buffer provided)
  // [!] Remember about the int. type capacity limit which is ~1e20 (far less then double's 1e308) [!]
  // Returns buf.; recommanded buf. size is at least 48 (20 + 20 + 2)
  // Check 'errMsg' if ANY problem occured
  //  (it will hold the LAST error msg., while func. will try to persevere)
  // Returns false on ANY error
  // Complexity: linear in the number's digit count
  //  (INCLUDING additional digits from the precision penalty)
  static bool separateFraction(SeparateFractionData& data) throw() {
    auto strBuf_ = data.strBuf;
    data.errCount = decltype(data.errCount)(); // reset

    //// Separate
    size_t signLen;
    if (data.fract < 0.0L) { // if negative
      data.fract = -data.fract; // revert [std::abs]
      signLen = size_t(1U);
    } else signLen = size_t();
    long double intPartFraction;
    long double fractPartFraction = std::modfl(data.fract, &intPartFraction);
    
    //// Check borders & set int. part (zeroise if too big)
    static const auto VAL_UP_LIMIT_ = 18446744073709500000.0L; // ~ 2^64 [ULL]
    if (data.fract > VAL_UP_LIMIT_) { // possible overflow
      data.intPart = std::decay<decltype(data.intPart)>::type(); // reset
      ++data.errCount;
      if (data.errMsg) *data.errMsg = "too big value";
    } else data.intPart = static_cast<decltype(data.intPart)>(intPartFraction);

    //// Get int. part str. (if possible)
    size_t currDigit;
    const size_t intPartRealLen = static_cast<size_t>(std::log10(intPartFraction)) + size_t(1U);
    auto tooShortOrNoBuf = false;
    if (strBuf_) {
      if (data.strBufSize > (intPartRealLen + signLen)) { // +1 for str. terminator
        if (signLen) *strBuf_++ = '-';
        auto const intPartBufEnd = strBuf_ + intPartRealLen;
        strBuf_ = intPartBufEnd; // reversed filling
        auto intPartRestLen = intPartRealLen;
        while (intPartRestLen--) { // while int. part
          currDigit = static_cast<decltype(currDigit)>(std::fmod(intPartFraction, 10.0L));
          *--strBuf_ = currDigit + '0';
          intPartFraction /= 10.0L;
        }
        strBuf_ = intPartBufEnd;
      } else { // NOT enough free space
        tooShortOrNoBuf = true;
        ++data.errCount;
        if (data.errMsg) *data.errMsg = "too short buffer";
      }
    } else tooShortOrNoBuf = true;
    data.intPartLen = data.intPart ? intPartRealLen : decltype(data.intPartLen)(1U);

    //// Get fract. part (AND fract. part str. if possible)

    data.fractPart = decltype(data.fractPart)(); // reset
    data.fractPartlen = decltype(data.fractPartlen)() + size_t(1U);
    data.fractPartZeroesAheadSkippedCount = decltype(data.fractPartZeroesAheadSkippedCount)();
    if (fractPartFraction) { // if exist
      auto const strBufLast =
        data.strBuf ? data.strBuf + data.strBufSize - size_t(1U) : nullptr;

      //// Try add separator
      if (strBuf_ < strBufLast) // if there still free space
        *strBuf_++ = data.fractionDelim; // '1.' OR '.1' is OK

      //// Main processing cycle
      long double temp;
      while (true) {
        fractPartFraction *= 10.0L;
        currDigit = static_cast<decltype(currDigit)>(std::fmod(fractPartFraction, 10.0L));
        // An IEEE double has 53 significant bits (that's the value of DBL_MANT_DIG in <cfloat>)
        // That's approximately 15.95 decimal digits (log10(2^53))
        //  the implementation sets 'DBL_DIG' to 15, not 16, because it has to round down
        data.fractPart += currDigit; // ULL can hold up to a 19 digits, so it SHOULD be ok
        
        //// Get fract. part str. (if possible)
        if (!tooShortOrNoBuf) { // buf presented AND contains free
          if (strBuf_ < strBufLast) { // last one for a str. terminator
            *strBuf_++ = currDigit + '0';
          } else { // NOT enough free space
            tooShortOrNoBuf = true;
            ++data.errCount;
            if (data.errMsg) *data.errMsg = "too short buffer";
          }
        }
        if (!std::modfl(fractPartFraction, &temp)) break; // NO fract. part
        if (data.fractPart) { // skip zeroes ahead
          data.fractPart *= size_t(10U);
          ++data.fractPartlen;
        } else ++data.fractPartZeroesAheadSkippedCount;
      };
    }
    if (strBuf_ && data.strBufSize) *strBuf_ = '\0'; // finalize
    data.totalLen = strBuf_ - data.strBuf;

    if (data.errCount) { // if errors
      return false;
    } else { // NO error
      if (data.errMsg) *data.errMsg = "";
      return true;
    }
  }

  // [!] Do NOT confuse this with the std. C++ keyword 'xor'
  //  (which is an alias for the bitwise operator '^') [!]
  // Better use in logic utils
  static bool XOR(const bool b1, const bool b2) throw() {
    return (b1 || b2) && !(b1 && b2);
  }
  
  // Returns zero if too high pow. (if > 63)
  // [!] Use 63 idx. very carefully - ONLY to get the appropriate bit. mask
  //  (zeroes + sign bit set), BUT NOT the appropriate value (returns negative num.) [!]
  static long long int getPowerOf2(const size_t pow) throw() {
    static const long long int POWERS[] = { // 0 - 62
      1LL, 2LL, 4LL, 8LL, 16LL, 32LL, 64LL, // 8 bit
      128LL, 256LL, 512LL, 1024LL, 2048LL, 4096LL, 8192LL, 16384LL, 32768LL, // 16 bit
      //// 32 bit
      65536LL, 131072LL, 262144LL, 524288LL, 1048576LL, 2097152LL,
      4194304LL, 8388608LL, 16777216LL, 33554432LL,
      67108864LL, 134217728LL, 268435456LL, 536870912LL, 1073741824LL,
      //// 64 bit
      2147483648LL, 4294967296LL, 8589934592LL, 17179869184LL, 34359738368LL, 68719476736LL,
      137438953472LL, 274877906944LL, 549755813888LL, 1099511627776LL, 2199023255552LL,
      4398046511104LL, 8796093022208LL, 17592186044416LL, 35184372088832LL, 70368744177664LL,
      140737488355328LL, 281474976710656LL, 562949953421312LL, 1125899906842624LL, 2251799813685248LL,
      4503599627370496LL, 9007199254740992LL, 18014398509481984LL, 36028797018963968LL,
      72057594037927936LL, 144115188075855872LL, 288230376151711744LL, 576460752303423488LL,
      1152921504606846976LL, 2305843009213693952LL, 4611686018427387904LL,
      -9223372036854775808LL // 63-th bit (sign bit) set bit mask
    };
    return pow >= std::extent<decltype(POWERS)>::value ? 0LL : POWERS[pow];
  }

  // [!] Use standart 'exp2' instead [!]
  // In C++11 better use templated constexpr version
  // Supports negative power, uses lazy evaluation
  // Returns zero if absolute pow. is too high (> 63)
  // [!] Does NOT thread safe lazy init.; slower [!]
  static long double getPowerOf2Ex(const int pow) throw() {
    static const auto MAX_POWER = 63U; // hello, Homer!
    if (std::abs(pow) > MAX_POWER) return 0.0L; // too high

    static bool INITED = false; // false does NOT really needed here, coze static
    static long double POWERS[MAX_POWER * 2U + 1U]; // zero initialized, coze static
    static const size_t MIDDLE = MAX_POWER;

    if (!INITED) {
      auto idx = 1U; // power[63] == 0 (rest as is), power[64] == 2, power[62] == 1/2
      for (long double k = 2.0L; idx <= MAX_POWER; k *= 2.0L, ++idx) {
        POWERS[MIDDLE + idx] = k;
        POWERS[MIDDLE - idx] = 1.0L / k;
      }
      POWERS[MIDDLE] = 1.0L; // set power[63] to the 1

      INITED = true;
    }
    return POWERS[MIDDLE + pow];
  }
  
  // [!] Use standart 'log2' instead [!]
  // Returns zero if the incoming number have more then 1 bit setted OR NO bit setted
  // '100' (4) -> 3; '1' (1) -> 0
  // Complexity: logarithmic O(log2(64))
  // Almost equivalent to the log2(X)
  // OPTIMIZATION HINT 1: better use the compiler intrinsic func. like '__lzcnt'
  //  (https://en.wikipedia.org/wiki/Find_first_set)
  //  (see 'LZCNT' VS 'BSR' correct usage in 'getBitStrEx')
  // OPTIMIZATION HINT 2: O(1) algo. using De Bruijn sequence -
  //  http://stackoverflow.com/questions/3465098/bit-twiddling-which-bit-is-set
  static size_t getBitIdx(const unsigned long long int oneBitNum) throw() {
    if (!oneBitNum) return size_t();

    static const unsigned long long int POWERS[] = { // 0 - 63
      1ULL, 2ULL, 4ULL, 8ULL, 16ULL, 32ULL, 64ULL, // 8 bit
      128ULL, 256ULL, 512ULL, 1024ULL, 2048ULL, 4096ULL, 8192ULL, 16384ULL, 32768ULL, // 16 bit
      //// 32 bit
      65536ULL, 131072ULL, 262144ULL, 524288ULL, 1048576ULL, 2097152ULL, 4194304ULL,
      8388608ULL, 16777216ULL, 33554432ULL, 67108864ULL, 134217728ULL, 268435456ULL,
      536870912ULL, 1073741824ULL,
      //// 64 bit
      2147483648ULL, 4294967296ULL, 8589934592ULL, 17179869184ULL, 34359738368ULL, 68719476736ULL,
      137438953472ULL, 274877906944ULL, 549755813888ULL, 1099511627776ULL, 2199023255552ULL,
      4398046511104ULL, 8796093022208ULL, 17592186044416ULL, 35184372088832ULL, 70368744177664ULL,
      140737488355328ULL, 281474976710656ULL, 562949953421312ULL, 1125899906842624ULL,
      2251799813685248ULL, 4503599627370496ULL, 9007199254740992ULL, 18014398509481984ULL,
      36028797018963968ULL, 72057594037927936ULL, 144115188075855872ULL, 288230376151711744ULL,
      576460752303423488ULL, 1152921504606846976ULL, 2305843009213693952ULL, 4611686018427387904ULL,
      9223372036854775808ULL
    }; // ofc. this table could be filled automatically using '<<' operator
    static const auto COUNT = std::extent<decltype(POWERS)>::value;

    auto comparator = [](const void* const value1Ptr, const void* const value2Ptr) throw() -> int {
      const auto num1 = *static_cast<const unsigned long long int* const>(value1Ptr);
      const auto num2 = *static_cast<const unsigned long long int* const>(value2Ptr);

      return num1 < num2 ? -1 : (num1 > num2 ? 1 : 0);
    };
    // OPTIMIZATION HINT: replace with the C++ std::lower_bound (OR binary_search)
    auto const elemPtr = bsearch(&oneBitNum, POWERS, COUNT, sizeof(*POWERS), comparator);
    if (!elemPtr) return size_t(); // NOT found

    return static_cast<decltype(&*POWERS)>(elemPtr) - POWERS; // get idx. computing ptr. shift
  }

  struct BitCount {

    BitCount& operator+=(const BitCount& bitCount) throw() {
      total += bitCount.total;
      positive += bitCount.positive;

      return *this;
    }

    void clear() throw() {
      total = size_t();
      positive = size_t();
    }

    size_t total; // significant ONLY (intermediate zero bits including ofc.)
    size_t positive;
  };
  
  // Linear complexity, optimized by skipping zero bits
  // Works with the negative numbers
  //  (setted sign. bit counts as positive AND meaning: included in 'total')
  template<typename TNumType>
  static void getBitCount(TNumType num, BitCount& count) throw() {
    count.clear();

    if (!num) return;
    if (num < TNumType()) { // if negative
      // Reverse, reset sign bit (to allow Brian Kernighan's trix to work correct)
      num &= std::numeric_limits<decltype(num)>::max();
      ++count.positive; // count sign bit as positive
      ++count.total; // count sign bit as meaning (significant)
    }
    // Determine bit count by the most significant bit set
    count.total += static_cast<size_t>(log2(num)) + 1U;
    do {
      ++count.positive;
      // Brian Kernighan's trix (clear the least significant bit set)
      num &= (num - TNumType(1U));
    } while (num);
  }
  
  // [!] 'TNumType' should have 2N bytes size [!]
  // Should be faster, uses lazy-inited lookup table
  // [!] Does NOT thread safe when NOT yet inited [!]
  // Works with the negative numbers
  //  (setted sign. bit counts as positive AND meaning: included in 'total')
  template<typename TNumType>
  static void getBitCountEx(TNumType num, BitCount& count) throw() {
    count.clear();

    if (!num) return;
    if (num < TNumType()) { // if negative
      // Reverse, reset sign bit (to allow bitwise right shifting to work correct)
      num &= std::numeric_limits<decltype(num)>::max();
      ++count.positive; // count sign bit as positive
      ++count.total; // count sign bit as meaning (significant)
    }
    static_assert(!(sizeof(num) % 2U), "'TNumType' should have 2N bytes size");
    
    const auto counts = getBitCountsLookupTable();

    static const TNumType FIRST_2_BYTES = 65535; // 1111 1111 1111 1111
    size_t idxInTable;
    
    size_t iterCount = 0U, lastTotal = 0U;
    for (;; ++iterCount, num >>= 16U) { // 16 bits in 2 bytes (2 * 8)
      if (!num) break;

      idxInTable = num & FIRST_2_BYTES; // get ONLY first 2 (lest significant) bytes
      if (!idxInTable) continue;

      const auto& currCounts = counts[idxInTable];
      count.positive += currCounts.positive;
      lastTotal = currCounts.total; // replace by the last (most significant pair) count
    }
    // 16 bits in 2 bytes (in EACH iteration); AT least one iteration
    count.total += (lastTotal + (iterCount - 1U) * 16U);
  }
  
  // 'bitCount' determines how many bits will be set to 1
  //  (ALL others will be set to 0) AND can NOT be larger then 64
  // Bits will be filled starting from the least significant
  //  (by default) OR from most significant (if 'reversedOrder' is true)
  static unsigned long long int getBitMask(const size_t bitCount,
                                           const bool reversedOrder = false) throw()
  {
    static const unsigned long long int MASKS[] 
      = {0ULL, 1ULL, 3ULL, 7ULL, 15ULL, 31ULL, 63ULL, 127ULL, 255ULL, 511ULL, 1023ULL,
         2047ULL, 4095ULL, 8191ULL, 16383ULL, 32767ULL, 65535ULL, 131071ULL, 262143ULL,
         524287ULL, 1048575ULL, 2097151ULL, 4194303ULL, 8388607ULL, 16777215ULL, 33554431ULL,
         67108863ULL, 134217727ULL, 268435455ULL, 536870911ULL, 1073741823ULL, 2147483647ULL,
         4294967295ULL, 8589934591ULL, 17179869183ULL, 34359738367ULL, 68719476735ULL,
         137438953471ULL, 274877906943ULL, 549755813887ULL, 1099511627775ULL, 2199023255551ULL,
         4398046511103ULL, 8796093022207ULL, 17592186044415ULL, 35184372088831ULL,
         70368744177663ULL, 140737488355327ULL, 281474976710655ULL, 562949953421311ULL,
         1125899906842623ULL, 2251799813685247ULL, 4503599627370495ULL, 9007199254740991ULL,
         18014398509481983ULL, 36028797018963967ULL, 72057594037927935ULL, 144115188075855871ULL,
         288230376151711743ULL, 576460752303423487ULL, 1152921504606846975ULL, 2305843009213693951ULL,
         4611686018427387903ULL, 9223372036854775807ULL,
         // Last, equal to the 'std::numeric_limits<unsigned long long int>::max()'
         18446744073709551615ULL};
    static const auto COUNT = std::extent<decltype(MASKS)>::value;

    if (bitCount >= COUNT) return MASKS[COUNT - 1U]; // return last
    return reversedOrder ? MASKS[COUNT - 1U] ^ MASKS[COUNT - 1U - bitCount] : MASKS[bitCount];
  }

  // Will set bits with the specified indexes to 1 (OR to 0 if 'SetToZero' is true)
  // ANY value from the 'idxs' array should NOT be larger then 62
  // [!] Should work correct with the signed num. types (NOT tested) [!]
  template<const bool SetToZero = false, typename TNumType, const size_t IndexesCount>
  static void setBits(TNumType& valueToUpdate, const size_t(&idxs)[IndexesCount]) throw() {
    static const size_t MAX_BIT_IDX =
      std::min(static_cast<unsigned int>(sizeof(valueToUpdate) * 8U - 1U), 62U);
    
    decltype(0LL) currBitMask;
    for (const auto bitIdx : idxs) {
      if (bitIdx > MAX_BIT_IDX) continue; // skip invalid idxs

      currBitMask = getPowerOf2(bitIdx);
      if (SetToZero) {
        valueToUpdate &= ~currBitMask; // use reversed mask to UNset the specified bit
      } else valueToUpdate |= currBitMask; // set specified bit
    }
  }
  
  //// Use next two funcs to parse individual bits, individual bytes,
  ////  variable count of bits (multiply of 2 OR 8 OR NOT)
  //// For other bit-working algos see http://graphics.stanford.edu/~seander/bithacks.html
  
  /*
  [!] OPTIMIZATION: use standart C++ bit fields (http://en.cppreference.com/w/cpp/language/bit_field) [!]
  [!] Remember about bit/byte endianness, type sizes AND alignment (padding) [!]
  
  template <typename TDataType,
          const size_t Part1Size, const size_t Part2Size, const size_t Part3Size, const size_t Part4Size, 
          const size_t Part5Size, const size_t Part6Size, const size_t Part7Size, const size_t Part8Size>
  union UBitParser {
    static_assert(std::is_standard_layout<TDataType>::value,
                  "'UBitParser' union: 'UBitParser' SHOULD be a POD type OR a standard layout type");
    struct BitMasks {
      unsigned char part1 : Part1Size, part2 : Part2Size, part3 : Part3Size, part4 : Part4Size,
                    part5 : Part5Size, part6 : Part6Size, part7 : Part7Size, part8 : Part8Size;
    } bitMasks;
    TDataType data;
    static_assert(sizeof(BitMasks) == sizeof(TDataType),
                  "'UBitParser' union: types SHOULD have an equal size");
  };
  
  //           bytes:  1st     2nd bte     3rd     4th
  UBitParser<uint32_t, 5U, 3U, 2U, 4U, 2U, 7U, 1U, 8U> bitParser;
  */
  
  // Returns meaning (holding actual data) parts count (other parts will be filled with zeroes)
  // First meaning part will hold the least significant bit(s) AND so on
  // At the func. end 'num' will hold unprocessed data (if left)
  //  shifted to the beginning (to the least significant bit)
  // [!] 'bitCountInPart' can NOT be larger then the actual bit count
  //  in the types 'TPartType' OR 'TNumType' (MAX = 64) [!]
  // Prefer 'TPartType' to be unsigned integral number type (as so 'TNumType')
  // [!] If 'num' is a NEGATIVE number, then the sign.
  //  bit will be FORCEFULLY RESETED, so do NOT use NEGATIVE numbers here [!]
  template<typename TNumType, class TPartType, const size_t PartCount>
  static unsigned int parseByBits(TNumType& num, size_t bitCountInPart,
                                  TPartType (&parts)[PartCount]) throw()
  {
    auto filledParts = 0U, meaningParts = 0U; // counters
    
    if (bitCountInPart && num) { // fill meaning parts
      if (num < TNumType()) { // if negative
        //// Reverse, reset sign bit (to allow bitwise right shifting to work correct)
        //// std::remove_reference<decltype(num)>::type == TNumType
        num &= std::numeric_limits<typename std::remove_reference<decltype(num)>::type>::max();
      }
      static const auto PART_SIZE =
        std::min(sizeof(TNumType) * 8U, sizeof(TPartType) * 8U); // in bits

      if (bitCountInPart > PART_SIZE) bitCountInPart = PART_SIZE; // check & correct
      const TNumType bitMask =
        static_cast<TNumType>(getBitMask(bitCountInPart, false)); // to extract the part
      
      do {
        parts[meaningParts++] = static_cast<TPartType>(num & bitMask);
        num >>= bitCountInPart;
      } while (meaningParts < PartCount && num);
    }
    filledParts += meaningParts;

    while (filledParts < PartCount)
      parts[filledParts++] = static_cast<TPartType>(0U); // zeroize remaining parts
    return meaningParts;
  }
  
  // Return the last meaning (holding actual data) part INDEX
  //  OR -1 if NOT any meaning part is presented
  // NOT meaning parts (including zero-size AND negative-size parts) will be filled with zeroes
  // First meaning part will hold the least significant bit(s) AND so on
  // At the func. end 'num' will hold unprocessed data (if left)
  //  shifted to the beginning (to the least significant bit)
  // [!] ANY part size from the 'partSizes' can NOT be higher then
  //  the actual size in bits of the types 'TNumType' OR 'TPartType' (MAX = 64) [!]
  // Prefer 'TPartType' AND 'TPartSizeType' to be unsigned integral number types (as so 'TNumType')
  // [!] If 'num' is a NEGATIVE number, then the sign. bit will be FORCEFULLY RESETED,
  //  so do NOT use NEGATIVE numbers here [!]
  template<typename TNumType, class TPartSizeType, class TPartType, const size_t PartCount>
  static int parseByBitsEx(TNumType& num, const TPartSizeType (&partSizes)[PartCount],
                           TPartType (&parts)[PartCount]) throw()
  {
    auto filledParts = 0U;
    auto lastMeaning = -1; // idx.

    if (num) { // fill meaning parts AND invalid-size parts (if presented)
      auto meaningParts = 0U, incorrectParts = 0U; // counters
      
      if (num < TNumType()) { // if negative
        //// Reverse, reset sign bit (to allow bitwise right shifting to work correct)
        //// std::remove_reference<decltype(num)>::type == TNumType
        num &= std::numeric_limits<typename std::remove_reference<decltype(num)>::type>::max();
      }
      static const auto MAX_PART_SIZE =
        std::min(sizeof(TNumType) * 8U, sizeof(TPartType) * 8U); // in bits

      TNumType bitMask;
      TPartSizeType currPartSize;
      do {
        currPartSize = partSizes[meaningParts];
        
        if (currPartSize < TPartSizeType(1U)) { // invalid-size part
          parts[filledParts++] = TPartType(); // zeroize part
          ++incorrectParts;
          continue;
        }
        if (currPartSize > MAX_PART_SIZE) currPartSize = MAX_PART_SIZE; // check & correct
        
        bitMask = static_cast<TNumType>(getBitMask(currPartSize, false)); // to extract the part
        
        parts[lastMeaning = filledParts++] = static_cast<TPartType>(num & bitMask);
        ++meaningParts;

        num >>= currPartSize;
      } while (num && filledParts < PartCount);
    }
    while (filledParts < PartCount)
      parts[filledParts++] = TPartType(); // zeroize remaining parts
    
    return lastMeaning;
  }
  
  // Linear complexity (N = count of the meaning bits in a number,
  //  up to a sizeof(TNumType) * 8 for negative numbers)
  // Returns ptr. to the actual beginning of the str.
  //  (buffer will be filled in the reversed order - from the end)
  // For the negative numbers sign bit simb. in the str. will be set at its place
  //  (according to the size of the 'TNumType')
  //   AND intermediate unmeaning zero bits will be filled -
  //    BUT ALL of this will happen ONLY if the buf. is large enough
  // If the provided buf. is too small to hold all the data containing in 'num',
  //  then 'num' will hold the remaining data
  //   (INCLUDING sign bit at it's ACTUAL place for the NEGATIVE number) 
  //    shifted to the beginning (to the LEAST significant bit) after return from the func.
  // [!] Works with the negative numbers [!]
  // Returns bit str. representation in the direct bit order
  template<typename TNumType>
  static char* getBitStr(TNumType& num, char* const strBuf, const size_t strBufSize) throw() {
    if (!strBuf || !strBufSize) return nullptr;
    
    const auto negative = num < TNumType();
    if (negative)
      num &= std::numeric_limits<TNumType>::max(); // reverse, reset sign bit

    char* const bufLastCharPtr = strBuf + (strBufSize - 1U);
    char* start = bufLastCharPtr; // point to the last smb.
    *start-- = '\0'; // set the str. terminator

    while (num && start >= strBuf) { // reverse fill buf.
      *start-- = '0' + (num & TNumType(1U));
      num >>= 1U;
    }
    if (negative) {
      char* const signBitSymbPtr = bufLastCharPtr - sizeof(TNumType) * 8U;
      // If buffer size is quite enough to hold the sign bit symb.
      if (signBitSymbPtr >= strBuf) {
        while (start > signBitSymbPtr)
          *start-- = '0'; // set unmeaning zero bits between sign AND meaning bits
        *start-- = '1'; // set sign bit symb.
      }
      if (num) // return sign bit ONLY if some data remains
        num |= std::numeric_limits<TNumType>::min();
    }
    return start + 1U;
  }
  
  //// [!] Both 'getBitStrEx' variants: you can provide negative nums to them freely,
  ////  coze 'unsigned int n = -1' gives the correct bits representation [!]
  
  #ifdef _MSC_VER // GCC does NOT have '__lzcnt' intrinsic
  
  // Direct filling of the buf., returns 'strBuf'
  // If 'allBits' is true - str. will be filled with ALL the bits:
  //  meaning AND NOT (str. will be with the fixed size up to a 32 chars)
  // If 'allBits' is true, BUT the provided buf.
  //  is too small to hold all the data containing in 'num',
  //   then 'num' will hold the remaining data shifted to the end
  //    (to the MOST significant bit) after return from the func.
  static char* getBitStrEx(unsigned int& num, char* strBuf,
                           const size_t strBufSize, bool allBits = false) throw()
  {
    static const auto NUM_SIZE = sizeof(num) * 8U; // in bits

    if (!strBuf || !strBufSize) return nullptr;
    if (!num) {
      *strBuf = '\0';
      return strBuf;
    }
    // Forcefully adding ALL the bits for a negative num. [REDUNDANT CHECK]
    if (num < 0U) allBits = true;
    
    //// Check if the 'LZCNT' (leading zeroes count) instruction is NOT supported by the CPU,
    ////  then the 'BSR' (bit scan reverse) PROBABLY is used 
    ////   See additional info here: https://msdn.microsoft.com/en-us/library/bb384809.aspx
    ////    http://x86.renejeschke.de/html/file_module_x86_id_20.html,
    ////     https://msdn.microsoft.com/ru-ru/library/hskdteyh.aspx
    static auto LZCNT_OK =
      (32U == __lzcnt(0x0U) && 24U == __lzcnt(0xFFU) && 16U == __lzcnt(0xFFFFU));
    static auto BSR_OK = // 'FF' is a one byte (255), 'FFFF' is two (65535) etc
      (7U == __lzcnt(0xFFU) && 15U == __lzcnt(0xFFFFU) && 31U == __lzcnt(0xFFFFFFFFU));
    static auto ANYONE_OK = XOR(LZCNT_OK, BSR_OK);

    if (!ANYONE_OK) { // some WTF error
      *strBuf = '\0';
      return strBuf;
    }
    // MOST significant bit set idx. (1 - based), ZERO if 'num' is zero
    const auto MSTIdx = // also '__lzcnt64()' AND 'log2()' can be used
      allBits ? NUM_SIZE : (LZCNT_OK ? (NUM_SIZE - __lzcnt(num)) : (__lzcnt(num) + 1U));
    //                                          LZCNT^             BSR^

    // Bit mask to get the MST (MOST significant bit set), ZERO if 'MSTIdx' is zero
    const auto getMST =
      MSTIdx ? static_cast<std::remove_reference<decltype(num)>::type>(getPowerOf2(MSTIdx - 1U)) : 0U;
    
    char* const strBufStart = strBuf; // remember buf. start
    char* const strBufEnd = strBuf + (strBufSize - 1U);
    
    for (auto bitIdx = 0U; bitIdx < MSTIdx && strBuf < strBufEnd; ++bitIdx) { // from MST to LSB
      // Extract the most significant bit
      *strBuf++ = '0' + ((num & getMST) ? char(1) : '\0');
      num <<= 1U;
    }
    *strBuf = '\0';

    return strBufStart;
  }
  
  // 'uint16_t' has a fixed size of 16 bits (http://en.cppreference.com/w/cpp/types/integer)
  // 'allBits' is meaning ONLY at the very first call,
  //  when the lookup table is NOT yet inited, thus specifying content
  //   (after the first call ALL other calls will provide the same result
  //    regardless of the given value of that flag)
  // Faster, uses lazy-inited lookup table
  // [!] Does NOT thread safe when NOT yet inited [!]
  static const char* getBitStrEx(const uint16_t num, bool allBits = false) throw() {
    //// We can have AND use a two variants of the lookup table:
    ////  +1 another for the fixed size strs ('allBits' == true)
    static bool INITED; // false by default as static
    static char BIT_STRS[65536U][sizeof(num) * 8U + 8U]; // 1.5 mb, zero initialized as a static
    static const auto COUNT = std::extent<decltype(BIT_STRS)>::value;

    static const size_t NUM_TYPE_CAPACITY =
      static_cast<size_t>(getPowerOf2(sizeof(num) * 8U));
    assert(NUM_TYPE_CAPACITY == COUNT); // C++14: use 'constexpr' & 'static_assert' here
    
    if (!INITED) {
      auto currNum = 0U;
      for (decltype(currNum) strIdx = 0U; strIdx < COUNT; ++strIdx) {
        currNum = strIdx;
        getBitStrEx(currNum, BIT_STRS[strIdx], COUNT, allBits);
      }
      INITED = true;
    }
    return BIT_STRS[num];
  }
  
  #endif // _MSC_VER
  
  // 'hashSizeInBits' AND 'FNVPrime' should NOT be zero
  //  (for FNV primes see here: http://isthe.com/chongo/tech/comp/fnv/)
  // 'hashSizeInBits' should NOT be larger (OR even equal)
  //  then size of 'unsigned long long int' in bits
  // In the general case, almost any offset_basis will serve so long as it is non-zero
  static unsigned long long int generateFNV1OffsetBasis(const size_t hashSizeInBits,
                                                        const unsigned long long int FNVPrime) throw()
  {
    static const auto ULL_SIZE_IN_BITS = sizeof(unsigned long long int) * 8U;
    static const char* const OFFSET_STR =
      "chongo <Landon Curt Noll> /\\../\\!"; // http://www.isthe.com/chongo/odd/bat.html

    if (!hashSizeInBits || hashSizeInBits >= ULL_SIZE_IN_BITS || !FNVPrime) return 0ULL;
    
    const unsigned long long int hashMod =
      static_cast<decltype(hashMod)>(std::pow(2.0, ULL_SIZE_IN_BITS));
    auto offsetBasis = 0ULL;
    
    for (auto currChar = OFFSET_STR; *currChar; ++currChar) {
      offsetBasis = (offsetBasis * FNVPrime) % hashMod;
      offsetBasis ^= *currChar;
    }
    return offsetBasis;
  }
  
  // FNV-1a algorithm description: http://isthe.com/chongo/tech/comp/fnv/#FNV-1a
  // [?!] 'currByte' SHOULD have NO more then 8 bits meaningfull?? [?!]
  static void FNV1aAccumulate(size_t& hash, const size_t currByte) throw() {
    //// C++11 OPTIMIZATION HINT: better use 'constexpr' instead of 'const'
    static_assert(4U == sizeof(size_t) || 8U == sizeof(size_t),
                  "Known primes & offsets for 32 OR 64 bit hashes ONLY");

    // Some primes do hash better than other primes for a given integer size
    static const unsigned long long int PRIMES[] =
      {16777619ULL, 1099511628211ULL}; // 32 bit, 64 bit
    static const auto PRIME =
      static_cast<size_t>(PRIMES[sizeof(size_t) / 4U - 1U]);

    hash ^= currByte; // xor is performed on the low order octet (8 bits) of hash
    hash *= PRIME;
  }

  static size_t getFNV1aStdOffsetBasis() throw() {
    //// C++11 OPTIMIZATION HINT: better use 'constexpr' instead of 'const'
    static_assert(4U == sizeof(size_t) || 8U == sizeof(size_t),
                  "Known primes & offsets for 32 OR 64 bit hashes ONLY");

    // In the general case, almost any offset basis will serve so long as it is non - zero
    static const unsigned long long int BASISES[] =
      {2166136261ULL, 14695981039346656037ULL}; // 32 bit, 64 bit
    static const size_t OFFSET_BASIS =
      static_cast<decltype(OFFSET_BASIS)>(BASISES[sizeof(size_t) / 4U - 1U]);

    return OFFSET_BASIS;
  }

  // 'str' SHOULD be a POD C null-terminated str.
  // Returns 'preHash' for an empty str.
  // Set 'accumulate' to true AND provide a 'preHash' to take advantage of the accumulation mechanics
  //  ('preHash' will be discarded if NOT accumulating)
  // 'accumulate' flag is used instead of a simple '!preHash' check to avoid
  //  a possible very rare 'zero hash of none-empty str.' error
  static size_t getFNV1aHash(const char* str, size_t preHash = size_t(),
                             const bool accumulate = false) throw() {
    if (str && *str) {
      if (!accumulate) preHash = getFNV1aStdOffsetBasis(); // considering 'str' points to the start
      do {
        FNV1aAccumulate(preHash, *str++);
      } while (*str);
      assert(preHash); // NOT an error; used to detect the none-empty strs. with the zero hash
    }
    return preHash;
  }

  // See 'Fixed width integer types (since C++11)' [http://en.cppreference.com/w/cpp/types/integer]
  static uint32_t x64BitHashTo32BitHash(const uint64_t hash64) throw() {
    static const uint64_t LOW_PART = ~uint32_t(); // 'std::numeric_limits<uint32_t>::max()'
    // Shuffling bits of the high & low parts
    return static_cast<uint32_t>((hash64 >> 32U) ^ (hash64 & LOW_PART));
  }

  //// Use 'parseNum' to format file size, date/time numbers etc. in to the human readable strs
  ////  (bytes count, seconds count in formated str. (short|long):
  ////   1025 b = 1 kb 1 b, 421 sec = 1 h 1 min 1 sec)
  //// For example:
  ////  gbytes mbytes kbytes bytes - divider = 1024
  ////  millions thousands hundreds tens ones etc - divider = 10 ^ x

  // 'dividers' is an array of numbers which will be used as a dividers to get the appropriate result,
  //   which will be placed at the corresponding place in the 'results' array
  // Func. will stop it work when either 'num' will have NO more data OR zero divider is met
  //  OR the 'resultsCount' limit is reached
  // Func. will use either 'divider' OR 'dividers' dependig on the content of this values:
  //  if 'divider' is NOT zero - it will be used, otherwise i-th value from the 'dividers' will be used
  //   (if 'dividers' is NOT a nullptr)
  // Both 'dividers' AND 'results' arrays SHOULD have at least 'resultsCount' size
  // After return 'resultsCount' will hold an actual elems count in the 'results' array
  // After return 'num' will hold the rest data which is NOT fitted into the 'results' array
  static void parseNum(unsigned long long int& num,
                       const size_t divider, const size_t* const dividers,
                       unsigned long long int* const results, size_t& resultsCount) throw()
  {
    if (!results || !resultsCount) {
      resultsCount = 0U;
      return;
    }
    auto resultIdx = 0U;
    size_t currDivider;
    
    for (; num && resultIdx < resultsCount; ++resultIdx) {
      currDivider = divider ? divider : (dividers ? dividers[resultIdx] : 0U);
      if (!currDivider) break;

      results[resultIdx] = num % currDivider;
      num /= currDivider;
    }
    resultsCount = resultIdx;
  }
  
  #pragma warning(disable: 4005)
  #ifdef _MSC_VER
    #define _CRT_SECURE_NO_WARNINGS
  #endif
  #pragma warning(default: 4005)

  // Date/time str. wil be added (concatenated) to the provided 'str'; returns 'str'
  // 'TStrType' should support '+=' operator on the POD C strs AND chars
  //  AND 'empty' func. which returns true if the str. IS empty (false otherwise)
  // [!!] Use this to format ONLY the time passed, NOT the actual date/time [!!]
  // TO DO: improve RU language support
  template<typename TStrType>
  static TStrType& addFormattedDateTimeStr(unsigned long long int timeInSecs, TStrType& str,
                                           const bool longPrefixes = true,
                                           const bool eng = true) throw()
  {
    static const char* const ENG_LONG_POSTFIXES[] =
      {"seconds", "minutes", "hours", "days", "months", "years", "centuries", "millenniums"};
    static const char* const ENG_SHORT_POSTFIXES[] =
      {"sec", "min", "hr", "d", "mon", "yr", "cen", "mil"};

    static const char* const RU_LONG_POSTFIXES[] =
      {"секунд", "минут", "часов", "дней", "месяцев", "лет", "веков", "тысячелетий"};
    static const char* const RU_SHORT_POSTFIXES[] =
      {"сек", "мин", "час", "дн", "мес", "лет", "век", "тыс"};

    static_assert(sizeof(ENG_LONG_POSTFIXES) == sizeof(ENG_SHORT_POSTFIXES) &&
                  sizeof(RU_LONG_POSTFIXES) == sizeof(RU_SHORT_POSTFIXES) &&
                  sizeof(ENG_LONG_POSTFIXES) == sizeof(RU_LONG_POSTFIXES),
                  "Arrays of postfixes SHOULD have the same size"); // check elems count
    static const auto COUNT = std::extent<decltype(ENG_LONG_POSTFIXES)>::value;

    auto getPostfix = [&](const size_t idx) throw() {
      auto const list = longPrefixes ? (eng ? ENG_LONG_POSTFIXES : RU_LONG_POSTFIXES) // LONG
                                     : (eng ? ENG_SHORT_POSTFIXES : RU_SHORT_POSTFIXES); // SHORT
      return idx < COUNT ? list[idx] : "";
    };

    static const size_t DIVIDERS[] =
    // \/ 60 seconds in the minute    \/ 10 centuries in the millennium
      {60U, 60U, 24U, 30U, 12U, 100U, 10U};
    //                 ^ Month last approximately 29.53 days
    
    unsigned long long int timings[COUNT]; // = {0}
    auto timingsCount = std::extent<decltype(DIVIDERS)>::value;
    
    if (!timeInSecs) { // zero secs
      *timings = 0U;
      timingsCount = 1U;
    } else parseNum(timeInSecs, 0UL, DIVIDERS, timings, timingsCount);
    
    bool prev = !(str.empty());
    char strBuf[32U];
    
    if (timeInSecs) { // some data rest (after ALL divisions)
      sprintf(strBuf, "%llu", timeInSecs);

      if (prev) str += ' '; else prev = true;
      str += strBuf;
      str += ' ';
      str += getPostfix(COUNT - 1U);
    }
    for (auto timingIdx = timingsCount - 1UL; ; --timingIdx) {
      if (timings[timingIdx]) { // if NOT zero
        sprintf(strBuf, "%llu", timings[timingIdx]);
        
        if (prev) str += ' '; else prev = true;
        str += strBuf;
        str += ' ';
        str += getPostfix(timingIdx);
      }
      if (!timingIdx) break; // last
    }
    return str;
  }
  #ifdef _MSC_VER
    #undef _CRT_SECURE_NO_WARNINGS
  #endif

  // Ineffective
  static unsigned char ReverseBitsInByte(const unsigned char c) throw() {
    if (!c) return c; // zero byte, nothing to do

    static const auto BIT_COUNT = sizeof(c) * 8U;
    static const auto STOP_BIT_IDX = BIT_COUNT / 2U;

    auto RBitMask = 1U, LBitMask = 0U;
    auto currBitR = 0U, currBitL = 0U; // left <- right
    auto shiftDistance = 0U;
    size_t c_ = c; // to fit the register [OPTIMIZATION]

    for (size_t currBitIdx = 0U; currBitIdx < STOP_BIT_IDX; ++currBitIdx) {
      currBitR = c_ & RBitMask;
      
      shiftDistance = BIT_COUNT - 2U * currBitIdx - 1U;
      LBitMask = RBitMask << shiftDistance;
      currBitL = c & LBitMask;

      // If ONLY one enabled (nothing to do if both in the same state)
      if (XOR(currBitL != 0U, currBitR != 0U)) {
        if (currBitL) { // 'L' enabled, 'R' - NOT
          c_ &= ~LBitMask; // disable 'L'
          c_ |= RBitMask; // enable 'R'
        } else { // 'R' enabled, 'L' - NOT
          c_ &= ~RBitMask; // disable 'R'
          c_ |= LBitMask; // enable 'L'
        }
      }
      RBitMask <<= 1U;
    }
    return static_cast<unsigned char>(c_);
  }

  // Effective, uses lookup table (const complexity)
  // [http://stackoverflow.com/questions/746171/best-algorithm-for-bit-reversal-from-msb-lsb-to-lsb-msb-in-c]
  static unsigned char ReverseBitsInByteEx(const unsigned char c) throw() {
    static const unsigned char LOOKUP_TABLE[] = {
      0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
      0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
      0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
      0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
      0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
      0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
      0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
      0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
      0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
      0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
      0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
      0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
      0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
      0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
      0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
      0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF };
    return LOOKUP_TABLE[c];
  }
  
  struct ReverseBitsInByteExFunctor {
    auto operator()(const unsigned char c) throw() -> decltype(ReverseBitsInByteEx(c)) {
      return ReverseBitsInByteEx(c);
    }
  };

  // ANY integral num. types
  // Linear complexity in sizeof(TIntegralNumType)
  template <typename TIntegralNumType,
            typename TIntgeralNumPartType = unsigned char,
            class ReverseBitsInPartFunctor = ReverseBitsInByteExFunctor>
  static TIntegralNumType ReverseBits(const TIntegralNumType num) throw() {
    static_assert(1U == sizeof(unsigned char), "'unsigned char' type SHOULD be 1 byte long!");
    // SHOULD fits perfectly
    static_assert(!(sizeof(decltype(num)) % sizeof(TIntgeralNumPartType)),
                  "Size of 'num' type SHOULD be even size of 'TIntgeralNumPartType'");
    typedef typename std::remove_const<decltype(num)>::type TMutableNumType;
    union Converter {
      TMutableNumType i;
      // Sizeof(char) is ALWAYS 1U, considering to the CPP standart
      // 'sizeof(decltype(num))' obviously SHOULD be larger then
      //  'sizeof(TIntgeralNumPartType)' OR compile error
      TIntgeralNumPartType c[sizeof(decltype(num)) / sizeof(TIntgeralNumPartType)];
    } resultConverter, originalConverter;
    originalConverter.i = num;
    static const auto COUNT = std::extent<decltype(resultConverter.c)>::value;

    TIntgeralNumPartType currOriginalPart;
    ReverseBitsInPartFunctor bitReversingFunctor;
    //// Reversing algo.: 1) reverse parts 2) reverse bits in parts
    for (size_t partIdx = 0U; partIdx < COUNT; ++partIdx) {
      currOriginalPart =
        originalConverter.c[COUNT - partIdx - 1U]; // reverse the part order
      resultConverter.c[partIdx] =
        bitReversingFunctor(currOriginalPart); // reverse the bit order
    }
    return resultConverter.i;
  }

  // Just like 'ReverseBits', BUT uses dynamically generated AND larger lookup table
  // Thread safe; const complexity, should be faster, then 'ReverseBits'
  // [!] Size of 'TIntegralNumType' SHOULD divisible by 2 [!]
  template <typename TIntegralNumType>
  static TIntegralNumType ReverseBitsEx(const TIntegralNumType num) throw() {
    static_assert(2U == sizeof(uint16_t), "'uint16_t' SHOULD be 2 bytes large!");
    static uint16_t LOOKUP_TABLE[65536U]; // 2^16 items, total of 128 Kb
    typedef std::decay<decltype(*LOOKUP_TABLE)>::type TPartType;
    static const auto COUNT = std::extent<decltype(LOOKUP_TABLE)>::value;

    static std::atomic<bool> LOOKUP_TABLE_FILLED(false);
    static std::mutex MUTEX;

    auto fillLookupTable = [&]() throw() -> void {
      
      union Separator {
        TPartType ui;
        unsigned char ucs[sizeof(TPartType)];
      } separator = {0};
      static_assert(sizeof(separator) <= sizeof(size_t),
                    "'size_t' SHOULD be large enough!");
      
      // Zero item inited with zeroes as a static
      for (size_t idx = 1U; idx < COUNT; ++idx) {
        separator.ui = idx;
        
        *separator.ucs =
          ReverseBitsInByteEx(*separator.ucs); // first, reverse bits order
        separator.ucs[1U] =
          ReverseBitsInByteEx(separator.ucs[1U]); // from the small lookup table
        std::swap(*separator.ucs, separator.ucs[1U]); // second, reverse the bytes order

        LOOKUP_TABLE[idx] = separator.ui;
      }
      LOOKUP_TABLE_FILLED = true;
    };
    
    if (!LOOKUP_TABLE_FILLED) { // if first launch, atomic flag check will 
      const std::lock_guard<decltype(MUTEX)> autoLock(MUTEX);
      if (!LOOKUP_TABLE_FILLED) fillLookupTable(); // double checking lock
    }
    
    struct ReverseBitsInWordFunctor { // word = 2 bytes
      auto operator()(const TPartType num) throw() -> decltype(LOOKUP_TABLE[num]) {
        return LOOKUP_TABLE[num];
      }
    };
    
    return ReverseBits<decltype(num), TPartType, ReverseBitsInWordFunctor>(num);
  }

  class ByteOrderTester {

  public:

    static ByteOrderTester INSTANCE;

    const bool reversedOrder; // little-endian

  private:
    
    ByteOrderTester() throw() : reversedOrder(isReversedOrder()) {}

    ~ByteOrderTester() = default;

    ByteOrderTester(const ByteOrderTester&) throw() = delete;
    ByteOrderTester(ByteOrderTester&&) throw() = delete;
    ByteOrderTester& operator=(const ByteOrderTester&) throw() = delete;
    ByteOrderTester& operator=(ByteOrderTester&&) throw() = delete;

    static bool isReversedOrder() throw() {
      // sizeof(char) SHOULD be ALWAYS 1U, due to the CPP standart
      static_assert(sizeof(char) == 1U, "'char' type is NOT 1 byte large!");
      static_assert(sizeof(size_t) > sizeof('A'), "Too small 'int' size");

      union Converter {
        size_t i;
        unsigned char c[sizeof(decltype(i))];
      } converter = {};

      *converter.c = 'A'; // sets zero byte - then test is zero byte LSB OR MSB
      // true if zero byte considered LSB (least significant byte)
      //  => the bit order is left <- right (last byte is MSB - most significant byte)
      return 'A' == converter.i; // see C example here: https://ru.wikipedia.org/wiki/Порядок_байтов
    }
  };

  enum class ECompareStrategy {
    CS_LEFT_TO_RIGHT,      // direct
    CS_RIGHT_TO_LEFT,      // reversed
    CS_SHUFFLE_CONVERGING, // right-left-right etc, edges -> center
    CS_SHUFFLE_DIVERGENT   // middle, right-left-right etc, center -> edges
  };
  
  template<class T>
  typename std::enable_if<(sizeof(T) < 2U)>::type // 1 byte size
  static ReverseBytes(T& obj) throw() {} // nothing to reverse

  template<class T>
  typename std::enable_if<(sizeof(T) > 1U)>::type // 2, 4, 8
  static ReverseBytes(T& obj) throw() {
    ReverseBytes_(obj);
  }

  //// [!] 'fastCompareMemChunks' AND 'compareMem' - BAD concept, slow, do NOT use [!]

  // [!] WARNING: looks like can be slower then 'strcmp' OR 'memcmp' [!]
  // 'SampleChunkSize' SHOULD be {1, 2, 4, 8}
  // If 'stopAtZeroBlock' is true - comparison will end when the first zeroised mem. chunk is met,
  //  use it when comparing two arrays (with the known size) of a POD C strs.
  //   (end of arrays SHOULD be zeroised!) [OPTIMIZATION]
  // Set 'KeepByteOrder' when the real diff. value of the mem. chunks
  //  is imprtnat (NOT just the fact of diff. itself)
  template<const size_t SampleChunkSize, const bool SignedSample = true,
           const bool KeepByteOrder = false>
  static long long int
    fastCompareMemChunks(const void* const chunk1, const void* const chunk2, size_t iterCount,
                         const ECompareStrategy cmpStrat = ECompareStrategy::CS_SHUFFLE_CONVERGING,
                         const bool stopAtZeroBlock = false) throw()
  {
    static_assert(1U == SampleChunkSize || 2U == SampleChunkSize ||
                  4U == SampleChunkSize || 8U == SampleChunkSize,
                  "Invalid 'SampleChunkSize' template param. for 'fastCompareMemChunks'");
    typedef typename IntegralTypeBySize<SampleChunkSize, SignedSample>::Type TSampleChunkType;
    
    if (!iterCount || chunk1 == chunk2) return 0LL;
    if (!chunk1 || !chunk2)
      return (reinterpret_cast<long long int>(chunk1) - reinterpret_cast<long long int>(chunk2));
    
    //// When converting between two pointer types, it's possible that
    ////  the specific memory address held in the pointer needs to change
    ////   'static_cast will' make the appropriate adjustment,
    ////    'reinterpret_cast' will avoid changing the pointer's stored value
    auto chunk1Re = reinterpret_cast<const TSampleChunkType*>(chunk1);
    auto chunk2Re = reinterpret_cast<const TSampleChunkType*>(chunk2);

    TSampleChunkType elem1, elem2;

    const auto iterCountIsEven = !(iterCount % 2U);
    // If iter. count is even - separately chek the uncheked
    //  (first in the seq.) elem. (ONLY for CS_SHUFFLE_DIVERGENT)
    // Special case to NOT overlook the first elem.
    if (ECompareStrategy::CS_SHUFFLE_DIVERGENT == cmpStrat && iterCountIsEven) {
      elem1 = *chunk1Re, elem2 = *chunk2Re;
      // If 'KeepByteOrder' used for strs lower cmp.
      if (KeepByteOrder && ByteOrderTester::INSTANCE.reversedOrder) {
        ReverseBytes(elem1), ReverseBytes(elem2);
      }
      if (elem1 != elem2) // compare first elems
        return static_cast<long long int>(elem1) - static_cast<long long int>(elem2);
      if (1U == iterCount) return 0LL; // the single elem. which was checked above
    }
    const auto lastIterIdx = iterCount - 1U;
    const auto middleElemIdx = iterCount / 2U;
    
    ptrdiff_t ptrShift = 1; // per iteration; 'CS_LEFT_TO_RIGHT' by default (from start)
    // Shift (per 2 iters) of the 'ptrShift' itself (ONLY for the shuffle strats)
    ptrdiff_t ptrShiftShift = 0;
    bool shuffleStrat = false;

    auto prepare = [&]() throw() {
      switch (cmpStrat) {
        case ECompareStrategy::CS_RIGHT_TO_LEFT:
          chunk1Re += lastIterIdx, chunk2Re += lastIterIdx; // move to the END
          ptrShift = -1; // from the end
        break;
        
        case ECompareStrategy::CS_SHUFFLE_CONVERGING:
          shuffleStrat = true;
          chunk1Re += lastIterIdx, chunk2Re += lastIterIdx; // shift to the END
          ptrShift = 0 - lastIterIdx;
          ptrShiftShift = -1; // converging
        break;
        
        case ECompareStrategy::CS_SHUFFLE_DIVERGENT:
          shuffleStrat = true;
          chunk1Re += middleElemIdx, chunk2Re += middleElemIdx; // shift to the middle
          ptrShift = 1;
          ptrShiftShift = 1; // diverging
          // [!] Special case - the very first elem. was already checked [!]
          if (iterCountIsEven) --iterCount;
        break;
      }
    };
    prepare();

    for (size_t iterIdx = 0U; iterIdx < iterCount; ++iterIdx) {
      elem1 = *chunk1Re, elem2 = *chunk2Re;
      // 'KeepByteOrder' used for strs lower cmp.
      if (KeepByteOrder && ByteOrderTester::INSTANCE.reversedOrder) {
        ReverseBytes(elem1), ReverseBytes(elem2);
      }
      if ((stopAtZeroBlock && !elem1) || elem1 != elem2)
        return static_cast<long long int>(elem1) - static_cast<long long int>(elem2);

      chunk1Re += ptrShift, chunk2Re += ptrShift;
      if (shuffleStrat) {
        ptrShift = -ptrShift; // reverse
        ptrShift += (ptrShift < 0) ? -ptrShiftShift : ptrShiftShift; // shift shifter
      }
    }
    return 0LL;
  }

  // Returns sizeof(T), where T - is an unsigned integral type,
  //  which meets the condition '0U == toFitSize % sizeof(T)'
  // 'fitsRegister' flag used for OPTIMIZATION,
  //  true by default AND should possibly never be torned off
  /* EXPLANATION:
  If a type is bigger than a register, then simple arithmetic operations
   would require more than one instruction
  If it is smaller than a register, then loading and storing the values of a register
   would require the program to mask out the unused bits,
    to avoid overwriting other data
  */
  static size_t GetLargestFittableUIntSize(const size_t toFitSize,
                                           const bool fitsRegister = true) throw()
  {
    // C++ standart guarantees char to be 1 byte long
    static_assert(1U == sizeof(unsigned char),
                  "'unsigned char' SHOULD be 1 byte long");
    static const size_t TYPE_SIZES[] 
      = {sizeof(unsigned long long int), sizeof(unsigned long int),
         sizeof(unsigned int), sizeof(unsigned short int), sizeof(unsigned char)};

    for (const auto typeSize : TYPE_SIZES) {
      if (!(toFitSize % typeSize)) { // fits size

        // SHOULD be placeable in to the CPU register (which is typically sizeof(int) large)
        if (fitsRegister) {
          // TO DO: better use '__cpuid' intrinsic
          //  to determine if underlying processor is actually 64-bit 
          if (sizeof(int) >= typeSize) return typeSize; // fits (effectively OR NOT)
        } else return typeSize;
      }
    }
    assert(false); // critical failure
    return size_t();
  }
  
  // [!] WARNING: looks like can be slower then 'strcmp' OR 'memcmp' [!]
  // 'chunk1Size' AND 'chunk2Size' is in bytes
  // Set ANY of 'chunk1Size' OR 'chunk2Size' to zero if it represents
  //  a null-terminated str. which actuall len. is unknown
  //   (the comparison will goes up to a terminating char in this caze)
  // If the chunks compared NOT like a strs -
  //  chuks with the diff. sizes considered clearly diffirent
  // Returns diff. between the two last unmatched subchunks of mem.
  //  (NOT between the two last unmatched elems!!)
  // See 'stopAtZeroBlock' description at 'fastCompareMemChunks'
  // 'stopAtZeroBlock' AND 'cmpStrat' used ONLY if comparing NOT as a strs.
  //  (if BOTH mem. blocks size is known), otherwise ignored
  // Use this to compare strs with the diff. char types
  // 'SignednessCompare' meaningfull ONLY if NOT compared as a strs (if raw compare)
  // See 'KeepByteOrder' descr. at 'fastCompareMemChunks' func. descr.
  template<const bool SignednessCompare = false, const bool KeepByteOrder = false,
           typename TElemType1, class TElemType2>
  static long long int compareMem(const TElemType1* chunk1, const size_t chunk1Size,
                                  const TElemType2* chunk2, const size_t chunk2Size,
                                  const ECompareStrategy cmpStrat =
                                    ECompareStrategy::CS_SHUFFLE_CONVERGING,
                                  const bool stopAtZeroBlock = false) throw()
  {
    if (static_cast<const void*>(chunk1) == static_cast<const void*>(chunk2)) return 0LL;
    if (!chunk1 || !chunk2)
      return (reinterpret_cast<long long int>(chunk1) - reinterpret_cast<long long int>(chunk2));

    // Size is in bytes; true if actuall len. is unknown
    const auto compareAsStrs = !chunk1Size || !chunk2Size;
    
    auto compareStrs = [&]() throw() {
      while (true) {
        if (!*chunk1 || (*chunk1 != *chunk2)) {
          if (SignednessCompare) {
            const unsigned long long int ui1 = *chunk1, ui2 = *chunk2;
            return static_cast<long long int>(ui1) - static_cast<long long int>(ui2);
          } else {
            return static_cast<long long int>(*chunk1) - static_cast<long long int>(*chunk2);
          }
        }
        ++chunk1, ++chunk2;
      }
    };
    
    // 'chunk1Size' AND 'chunk2Size' SHOULD be equal AND NOT zero!!
    auto rawCompare = [&]() throw() {
      // TO DO: better determine underlying processor's actual size
      //  of the register (CPU bit's count), then simply using 'int'
      typedef int TCPURegType; // signedness??

      // End tail size (NOT fitted into the main comparing sequence)
      const auto restBytesCount = chunk1Size % sizeof(TCPURegType);
      
      // Firstly, compare the rest tail at the end
      auto firstComparingStage = [&]() throw() {
        auto chunk1Shifted = reinterpret_cast<const char*>(chunk1);
        auto chunk2Shifted = reinterpret_cast<const char*>(chunk2);
        const auto skipBytesCount = chunk1Size - restBytesCount;
        chunk1Shifted += skipBytesCount, chunk2Shifted += skipBytesCount; // shift ptrs. to the tail
        
        // If 'short int' is fittable into the tail
        if (!(restBytesCount % sizeof(unsigned short int))) {
          return fastCompareMemChunks<sizeof(short int), !SignednessCompare, KeepByteOrder>
                 (chunk1Shifted, chunk2Shifted, restBytesCount / sizeof(short int), // int >= short int
                  cmpStrat, stopAtZeroBlock);
        }
        // 'sizeof(char)' SHOULD be ALWAYS 1U according to the CPP standart
        return fastCompareMemChunks<sizeof(char), !SignednessCompare, KeepByteOrder>
               (chunk1Shifted, chunk2Shifted, restBytesCount / sizeof(char), // short int >= char
                cmpStrat, stopAtZeroBlock);
      };
      if (restBytesCount) { // tail exist - exec. the first stage
        const auto firstComparingStageResult = firstComparingStage();
        if (firstComparingStageResult)
          return firstComparingStageResult; // if NOT equal (return diff.)
      }
      // If tail is NOT the whole sequence - exec. second comparing stage
      if (restBytesCount < chunk1Size) {
        return fastCompareMemChunks<sizeof(TCPURegType), !SignednessCompare, KeepByteOrder>
               (chunk1, chunk2, chunk1Size / sizeof(TCPURegType), cmpStrat, stopAtZeroBlock);
      }
      return 0LL; // tail is the whole sequence AND equal
    };
    
    if (!compareAsStrs) {
      // If the chunks compared NOT like a strs -
      //  chuks with the diff. sizes considered clearly diffirent
      if (chunk1Size != chunk2Size) return chunk1Size - chunk2Size;
      return rawCompare();
    } else {
      // Comparing elem-to-elem from start to end, up to a str. terminator (zero elem.)
      return compareStrs();
    }
  }

private:

  MathUtils() throw() = delete;
  ~MathUtils() throw() = delete;
  MathUtils(const MathUtils&) throw() = delete;
  MathUtils(MathUtils&&) throw() = delete;
  MathUtils& operator=(const MathUtils&) throw() = delete;
  MathUtils& operator=(MathUtils&&) throw() = delete;

  // For 'getBitCountEx' ONLY
  // [!] Does NOT thread safe, lazy init. [!]
  static const BitCount* getBitCountsLookupTable() throw() {
    static bool INITED = false;
    static BitCount COUNTS[65536U]; // for 2 bytes [65 kb * 4 = 260 Kbytes]
    static const auto COUNT = std::extent<decltype(COUNTS)>::value;

    if (!INITED) {
      for (size_t idx = 0U; idx < COUNT; ++idx) {
        getBitCount(idx, COUNTS[idx]);
      }
      INITED = true;
    }
    return COUNTS;
  }

  template <typename T>
  static void ReverseBytes_(T& obj) throw() {
    static_assert(sizeof(T) && !(sizeof(T) % 2U), "Size of type 'T' SHOULD be multiple of 2!");
    static const auto STOP_BYTE_IDX = sizeof(T) / 2U;

    /* OK conversion [http://en.cppreference.com/w/cpp/language/reinterpret_cast](see 'Type aliasing')
    AliasedType is char or unsigned char:
     this permits examination of the object representation of any object as an array of unsigned char
    */
    auto const data = reinterpret_cast<unsigned char*>(&obj);
    for (size_t currByteIdx = 0U; currByteIdx < STOP_BYTE_IDX; ++currByteIdx) {
      std::swap(data[currByteIdx], data[(sizeof(T) - 1U) - currByteIdx]);
    }
  }
};

#endif // MathUtilsH