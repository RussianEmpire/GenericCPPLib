#ifndef ConvertionUtilsH
#define ConvertionUtilsH

//// [!] Version 1.116 [!]

#include "MacroUtils.h"
#include "FuncUtils.h" // for 'ExecIfPresent'
#include "MemUtils.h"  // for 'AUTO_ADJUST_MEM'
#include "MathUtils.h" // for 'getDigitOfOrder'

#include <cfloat>  // for 'LDBL_DIG'
#include <cerrno>  // for 'ERANGE'
#include <climits> // for 'LONG_MIN'

EXEC_MEMBER_FUNC_IF_PRESENT(truncated, false) // special for limited size storage

namespace ConvertionUtils {
  
  // A wrapper to the standart 'strtol' with the extended error recognition
  // In case of error sets 'errMsg' to the static non-empty str.
  //  ('strerror_s' can be used then to get a system specific error message (if exists))
  static bool strToL(long int& num, const char* const str,
                     const char*& errMsg, const int base = 10) throw()
  {
    if (!str) {
      num = std::decay<decltype(num)>::type(); // reset
      errMsg = "invalid pointer";
      return false;
    }
    if (!*str) {
      num = std::decay<decltype(num)>::type(); // reset
      errMsg = "empty string";
      return false;
    }
    char* endPtr = nullptr;
    /* If this variable is intended to be used for error checking after a std. C library function call,
        it SHOULD be reset by the program to zero before the call
         (since ANY previous call to a library function may have altered its value)
    */
    errno = 0; // clear error (NO library function sets its value back to zero once changed)
    num = strtol(str, &endPtr, base);
    // 'errno' CAN be set on error ('ERANGE'); 'endptr' SHOULD point to the str. terminator
    if (errno || *endPtr) { // C++11 requires 'errno' to be implemented in a per-thread basis
      if (ERANGE == errno) { // range error
        switch (num) {
          case LONG_MIN: errMsg = "too low value"; break;
          case LONG_MAX: errMsg = "too big value"; break;
          default: errMsg = "out of range";
        } // functions of the standard library may set 'errno' to ANY value
      } else errMsg = "invalid string"; // NOT a range error; some incorrect symb. is met probably
      num = std::decay<decltype(num)>::type();
      return false;
    }
    errMsg = "";
    return true;
  };

  // In AMERICAN English, many students are taught NOT to use the word "and"
  //  anywhere in the whole part of a number
  // "and" delimiter is used ALSO in Irish, Australian AND New Zealand English
  static const char* const ENG_GB_VERBAL_DELIMITER = "and";
  static const char DEFAULT_DELIMITER = ' ';

  // Do NOT add other dialects of english e.g. Irish, Australian, New Zealand, Indian etc
  enum ELocale {
    L_RU_RU, // Russian Federation Russian
    L_EN_US, // United States English
    L_EN_GB, // United Kingdom English
    COUNT    // USED ONLY to get the enum elems count
  };
  
  struct LocaleSettings {
    LocaleSettings() = default;
    ~LocaleSettings() = default;
    LocaleSettings(const LocaleSettings&) = default;
    LocaleSettings& operator=(const LocaleSettings&) = default;
    
    static const LocaleSettings DEFAULT_LOCALE_SETTINGS;

    // Enables some language very specific rules for numbers spelling
    //  (like pronouncing four-digit numbers in US & UK Eng.)
    bool verySpecific = false;
    bool positiveSign = false; // add positive sign [for positive nums]
    // Если целая часть равна нулю, то она может не читаться: 0.75 (.75) – point seventy five
    bool shortFormat  = false; // skip mention zero int. / fract. part
    bool foldFraction = false; // try find repeated pattern & treat it
    ELocale locale = ELocale::L_EN_GB;
    size_t precison = size_t(LDBL_DIG); // max. digits count (<= 'LDBL_DIG')
  };

  // TO DO:
  // - add ordinal numbers support
  //   http://en.wikipedia.org/wiki/English_numerals#Ordinal_numbers
  // - add multiplicative adverbs support
  //   http://en.wikipedia.org/wiki/English_numerals#Multiplicative_adverbs
  // - add long / short scale support
  //   https://en.wikipedia.org/wiki/Long_and_short_scales
  // - add money support (writing a cheque (or check),
  //    the number 100 is always written "one hundred", it is never "a hundred")
  //     [enum: money, count, index, dates, temperatures -
  //      http://en.wikipedia.org/wiki/English_numerals#Negative_numbers]
  // - add specialized numbers support
  //   http://en.wikipedia.org/wiki/English_numerals#Specialized_numbers
  // - add special fractions support: 1/2 — a half [«one half»]; 1/3 — a / one third; 1/4: «one quarter»
  //    ['HandleSpecificValues' - if 'verySpecific' is true]
  // - add 'double o' 'triple o' etc folding support [0.001 — 'nought point double о one']

  /* NOTES on known, BUT does NOT supported features:
    1) Ten thousand is RARELY called a myriad [do NOT used, as processing by triades]
    2) Americans may pronounce four-digit numbers with non-zero tens AND/OR ones
        as pairs of two-digit numbers without saying "hundred"
         AND inserting "oh" for zero tens: "twenty-six fifty-nine" or "forty-one oh five"
          (currently uses ANOTHER format: 9522 = 'ninety-five hundred twenty-two')
    3) American English: 1,200,000 = 1.2 million = one point two million;
        23,380,000,000 = 23.38 billion = twenty-three point three eight billion
         [such folding currently does NOT supported]
    4) English fractions can ALSO use russian-style representation:
        'one ten-thousandth' (одна десятитысячная) [0.005 five thousandths] [0.54 fifty four hundredths]
  */

  // 'TStrType' SHOULD support operator '+=', 'empty' AND 'size' methods
  // 'ReserveBeforeAdding' can be used to DISABLE possible 'trade-space-for-time' optimization
  template<class TStrType, const bool ReserveBeforeAdding = true>
  // "Number to the numeric format string" (321 -> "three hundred twenty-one")
  // Accpets negative numbers AND fractions
  // Complexity: linear in the number's digit count
  bool numToNumFormatStr(long double num, TStrType& str,
                         LocaleSettings& localeSettings =
                           LocaleSettings::DEFAULT_LOCALE_SETTINGS,
                         const char** const errMsg = nullptr) {
    auto negativeNum = false;
    if (num < 0.0L) {
      negativeNum = true;
      num = -num; // revert
    }
    //// Check borders
    static const auto VAL_UP_LIMIT_ = 1e100L; // see 'getOrderStr'
    if (num >= VAL_UP_LIMIT_) {
      if (errMsg) *errMsg = "too big value";
      return false;
    }
    if (ELocale::L_RU_RU == localeSettings.locale) { // for rus. lang. ONLY
      static const auto VAL_LOW_LIMIT_RU_ = 10.0L / VAL_UP_LIMIT_;
      if (num && num < VAL_LOW_LIMIT_RU_) {
        if (errMsg) *errMsg = "too low value";
        return false;
      }
    }
    //// Treat sign
    const auto delimiter = DEFAULT_DELIMITER;
    auto getSignStr = [](const ELocale locale, const bool positive) throw() -> const char* {
      switch (locale) {
        case ELocale::L_EN_US: return positive ? "positive" : "negative";
        case ELocale::L_EN_GB: return positive ? "plus" : "minus";
        case ELocale::L_RU_RU: return positive ? "плюс" : "минус";
      }
      assert(false); // locale error
      // Design / implementation error, NOT runtime error!
      return "<locale error [" MAKE_STR_(__LINE__) "]>"; // works OK in GCC
    };
    if (negativeNum || (localeSettings.positiveSign && num)) { // add sign
      if (!str.empty()) str += delimiter; // if needed
      str += getSignStr(localeSettings.locale, !negativeNum);
    }
    if (truncated::ExecIfPresent(str)) { // check if truncated
      if (errMsg) *errMsg = "too short buffer"; return false;
    }
    //// Get str. representation in the scientific notation

    // +2(+3) for rounding without loosing the precison: http://stackoverflow.com/questions/16839658
    //  (see also https://en.wikipedia.org/wiki/Extended_precision#Need_for_the_80-bit_format)
    /* SOME compilers support a 'long double' format that has more precision than 'double'
        (Microsoft MSVC is NOT, BUT Borland C++ BuilderX has 'LDBL_DIG' = 18)
        'LDBL_DIG' is the number of decimal digits that can be represented without losing precision
    */
    // 'printf("%.*Lf", 999, 1.1L)': '1.100000000000000000021684043449710088680149056017398834228515625'
    /* 'std::numeric_limits<T>::digits10': number of base-10 digits that can be represented by the type 'T'
        without change - ANY number with this many decimal digits can be converted to a value of type 'T'
         AND back to decimal form, without change due to rounding OR overflow
    */
    static const size_t MAX_DIGIT_COUNT_ = size_t(LDBL_DIG);
    // Normalized form (mantissa is a 1 digit ONLY):
    //  first digit (one of 'MAX_DIGIT_COUNT_') + '.' + [max. digits AFTER '.' - 1] + 'e+000'
    //   [https://en.wikipedia.org/wiki/Scientific_notation#Normalized_notation]
    static const size_t MAX_STR_LEN_ = 6U + MAX_DIGIT_COUNT_;

    // +24 to be on a safe side in case if NOT normalized form (unlikely happen) + for str. terminator
    static const size_t BUF_SIZE_ = AUTO_ADJUST_MEM(MAX_STR_LEN_ + 24U, 8U);
    char strBuf[BUF_SIZE_];
    // 21 digits is max. for 'long double' [https://msdn.microsoft.com/ru-ru/library/4hwaceh6.aspx]
    //  (20 of them can be AFTER decimal point in the normalized scientific notation)
    if (localeSettings.precison > MAX_DIGIT_COUNT_) localeSettings.precison = MAX_DIGIT_COUNT_;
    const ptrdiff_t len = sprintf(strBuf, "%.*Le", localeSettings.precison, num); // scientific format
    // On failure, a negative number is returned
    if (len < static_cast<decltype(len)>(localeSettings.precison)) {
      if (errMsg) *errMsg = "number to string convertion failed";
      return false;
    }
    //// Fraction repeated pattern recognition [123 from 1.123123, 7 from 123.777 etc]
    //// [!] Warning: does NOT work with the zero ending patterns (like '0.15015') [!]
    //    (add zeroes to the end of the str. to match correctly??)

    // Return nullptr if a pattern of such a len. is EXISTS (returns last NOT matched occurrence else)
    auto testPattern = [](const char* const str, const char* const strEnd,
                          const size_t patternSize) throw() {
      assert(str); // SHOULD NOT be nullptr
      auto equal = true;
      auto nextOccurance = str + patternSize;
      while (true) {
        if (memcmp(str, nextOccurance, patternSize)) return nextOccurance; // NOT macthed
        nextOccurance += patternSize;
        if (nextOccurance >= strEnd) return decltype(nextOccurance)(); // ALL matched, return nullptr
      }
    };

    // Retruns pattern size if pattern exist, 0 otherwise
    // TO DO: add support for advanced folding: 1.25871871 [find repeated pattern NOT ONLY from start]
    //  [in cycle: str+1, str+2, ...; get pattern start, pattern len. etc in 'tryFindPatternEx']
    //   ['сто двадцать целых двадцать пять до периода и шестьдесят семь в периоде']
    //    [controled by 'enableAdvancedFolding' new option]]
    auto tryFindPattern = [&](const char* const str, const size_t totalLen) throw() {
      const size_t maxPatternLen = totalLen / size_t(2U);
      auto const strEnd = str + totalLen; // past the end
      for (auto patternSize = size_t(1U); patternSize <= maxPatternLen; ++patternSize) {
        if (totalLen % patternSize) continue; // skip invalid dividers [OPTIMIZATION]
        if (!testPattern(str, strEnd, patternSize)) return patternSize;
      }
      return size_t();
    };

    //// Analyze str. representation in the scientific notation

    char* currSymbPtr;    // ptr. used to iterate over the numeric str.
    char* fractPartStart; // in the original scientific representation
    char* fractPartEnd;   // past the end [will point to the str. terminator, replacing the exp. sign]
    long int expVal;      // 3 for '1.0e3'
    auto fractPartLen = ptrdiff_t();
    size_t intPartLen; // real len.
    size_t intPartBonusOrder; // of the current digit
    size_t fractPartLeadingZeroesCount; // extra zeroes count BEFORE first meaning digit
    static const auto DECIMAL_DELIM_ = '.'; // [decimal separator / decimal mark] to use
    auto analyzeScientificNotationRepresentation = [&]() throw() {
      currSymbPtr = strBuf + len - size_t(1U); // from the end to start (<-)
      //// Get exp.
      static const auto EXP_SYMB_ = 'e';
      while (EXP_SYMB_ != *currSymbPtr) {
        --currSymbPtr; // rewind to the exp. start
        assert(currSymbPtr > strBuf);
      }
      fractPartEnd = currSymbPtr;
      *currSymbPtr = '\0'; // break str.: 2.22044604925031310000e+016 -> 2.22044604925031310000 +016
      const char* errMsg;
      const auto result = strToL(expVal, currSymbPtr + size_t(1U), errMsg);
      assert(result);
      //// Get int. part len.
      fractPartStart = currSymbPtr - localeSettings.precison;
      intPartLen = fractPartStart - strBuf;
      assert(intPartLen);
      if (localeSettings.precison) --intPartLen; // treat zero fract. precison ('1e0')
      assert((currSymbPtr - strBuf - int(localeSettings.precison) - 1) >= 0);
      assert(localeSettings.precison ? DECIMAL_DELIM_ == *(strBuf + intPartLen) : true);
      //// Finishing analyse (partition the number): get int. part real len.
      if (expVal < 0L) { // negative exp.
        if (static_cast<size_t>(-expVal) >= intPartLen) { // NO int. part
          fractPartLeadingZeroesCount = -(expVal + static_cast<long int>(intPartLen));
          intPartLen = size_t(); // skip processing int. part
        } else { // reduce int. part
          intPartLen += expVal; // decr. len.
          fractPartLeadingZeroesCount = size_t();
        }
        intPartBonusOrder = size_t();
        if (localeSettings.precison) // if fract. part exists [in the scientific represent.]
          --fractPartLen; // move delim. into the fract part., so reduce it length
      } else { // non-negative exp.: incr. len.
        const auto additive =
          std::min<decltype(localeSettings.precison)>(expVal, localeSettings.precison);
        intPartLen += additive;
        fractPartLeadingZeroesCount = size_t();
        intPartBonusOrder = expVal - additive;
      }
    };
    analyzeScientificNotationRepresentation();
    // Rewind to the fract. start [BEFORE getting fract. part real len.]
    currSymbPtr = strBuf + intPartLen +
      (expVal > decltype(expVal)() ? size_t(1U) : size_t()); // 1.23e1 = 12.3e0 [move right +1]

    auto fractPartTrailingZeroesCount = size_t(), fractPartAddedCount = size_t();
    char* fractPartRealStart;
    auto folded = false; // true if repeated pattern founded
    auto calcFractPartRealLen = [&]() throw() {
      if (DECIMAL_DELIM_ == *currSymbPtr) ++currSymbPtr; // skip delimiter when it separtes ('1.1e0')
      assert(fractPartEnd >= currSymbPtr); // 'currSymbPtr' SHOULD now be a real fract. part start
      fractPartRealStart = currSymbPtr;
      fractPartLen += fractPartEnd - currSymbPtr; // 'fractPartLen' CAN be negative BEFORE addition
      assert(fractPartLen >= ptrdiff_t()); // SHOULD NOT be negative now
      if (!fractPartLen) return; // NO fract. part
      //// Skip trailing zeroes
      auto fractPartCurrEnd = fractPartEnd - size_t(1U); // will point to the last non-zero digit symb.
      while ('0' == *fractPartCurrEnd && fractPartCurrEnd >= currSymbPtr) --fractPartCurrEnd;
      assert(fractPartCurrEnd >= strBuf); // SHOULD NOT go out of the buf.
      fractPartTrailingZeroesCount = fractPartEnd - fractPartCurrEnd - size_t(1U);
      assert(fractPartLeadingZeroesCount >= size_t() &&
             fractPartLen >= static_cast<ptrdiff_t>(fractPartTrailingZeroesCount));
      fractPartLen -= fractPartTrailingZeroesCount;
      //// Fraction folding (if needed)
      if (fractPartLen > size_t(1U) && localeSettings.foldFraction) {
        //// Remove delim. (if needed)
        assert(fractPartStart && fractPartStart > strBuf); // SHOULD be setted (delim. founded)
        if (fractPartRealStart < fractPartStart) { // move: "12.1e-1" -> "1 21e-1"
          currSymbPtr = fractPartStart - size_t(1U);
          assert(*currSymbPtr == DECIMAL_DELIM_);
          while (currSymbPtr > fractPartRealStart)
            *currSymbPtr-- = *(currSymbPtr - size_t(1U)); // reversed move
          *currSymbPtr = '\0';
          fractPartRealStart = currSymbPtr + size_t(1U); // update, now SHOULD point to the new real start
          assert(fractPartLen);
        }
        //// Actual folding (if needed)
        if (fractPartLen > size_t(1U)) {
          const auto patternLen = tryFindPattern(fractPartRealStart, fractPartLen);
          if (patternLen) {
            fractPartLen = patternLen; // actual folding (reduce fract. part len. to the pattern. len)
            folded = true;
          }
        }
      }
    };
    // We are NOT using 'modfl' to get part values trying to optimize by skipping zero parts
    calcFractPartRealLen(); // update len.
    assert(fractPartLen ? localeSettings.precison : true);
    const auto fractPartWillBeMentioned = fractPartLen || !localeSettings.shortFormat;
    currSymbPtr = strBuf; // start from the beginning, left-to-right (->)

    //// Language-specific morphological lambdas (a result of the morphological analysis)
    ////  [by default they returns numerals in the nominative case]

    /* Word can have up to a 3 morphems (affixes) in addition to the root:
         1) prefix:   placed BEFORE the stem of a word
         2) infix:    inserted INSIDE a word stem
             OR
            interfix: [linkage] placed in BETWEEN two morphemes AND does NOT have a semantic meaning
         3) postfix:  (suffix OR ending) placed AFTER the stem of a word
       Word = [prefix]<root>[infix / interfix][postfix (suffix, ending)]

        Online tools:
          склонение:
            http://morpher.ru/Demo.aspx
            http://www.morfologija.ru
            http://numeralonline.ru
          spelling:
            http://www.webmath.com/saynum.html
            http://eng5.ru/en/numbers_translation
            http://prutzkow.com/numbers/index_en.htm
            http://www.mathcats.com/explore/reallybignumbers.html
            http://www.calculator.org/calculate-online/mathematics/text-number.aspx
        */
    /// RU: returns root, which can be used to add ending of the appropriate case
    ///  (by default, returns ending for the nominative case) [нолЬ / нолЯ / нолЮ | нулём! нолём?]

    // 0 - 9 [1]
    auto getZeroOrderNumberStr = [&](const size_t currDigit, const size_t order, const char*& postfix,
                                     const LocaleSettings& localeSettings) throw() -> const char* {
      static const char* const EN_TABLE[] = // roots
        {"", "one", "tw", "th", "fo", "fi", "six", "seven", "eigh", "nine"};
      static const char* const EN_POSTFIXES[] = // endings
        {"", "", "o", "ree", "ur", "ve", "", "", "t", ""};
      static const char* const RU_TABLE[] =
        {"нол", "од", "дв", "тр", "четыр", "пят", "шест", "сем", "вос", "девят"};
      static const char* const RU_POSTFIXES[] = // восЕМЬ восЬМИ восЕМЬЮ
        // одИН одНОГО одНОМУ одНИМ; двА двУХ двУМ двУМЯ; трИ трЕМЯ; четырЕ четырЬМЯ четырЁХ
        {"ь", "ин", "а", "и", "е", "ь", "ь", "ь", "емь", "ь"};
      // НолЬ нолЯ нолЮ; пятЬ пятЬЮ пятЕРЫХ; шестЬ шестЬЮ шестИ; семЬ семИ семЬЮ; девятЬ девятЬЮ девятИ
      static_assert(sizeof(EN_TABLE) == sizeof(RU_TABLE) && sizeof(EN_TABLE) == sizeof(EN_POSTFIXES) &&
                    sizeof(RU_TABLE) == sizeof(RU_POSTFIXES) &&
                    size_t(10U) == std::extent<decltype(EN_TABLE)>::value,
                    "Tables SHOULD have the same size (10)");
      assert(currDigit < std::extent<decltype(EN_TABLE)>::value); // is valid digit?
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB:
          postfix = EN_POSTFIXES[currDigit];
          if (!currDigit) { // en.wikipedia.org/wiki/Names_for_the_number_0_in_English
            // American English:
            //  zero:       number by itself, decimals, percentages, phone numbers, some fixed expressions
            //  o (letter): years, addresses, times and temperatures
            //  nil:        sports scores
            if (localeSettings.verySpecific) return "o"; // 'oh'
            return localeSettings.locale == ELocale::L_EN_US ? "zero" : "nought";
          }
          return EN_TABLE[currDigit];
        case ELocale::L_RU_RU:
          postfix = "";
          switch (order) {
            case size_t(0U): // last digit ['двадцать две целых ноль десятых']
              // Один | одНА целая ноль десятых | одна целая одНА десятая
              if (!fractPartWillBeMentioned) break;
            case size_t(3U): // тысяч[?]
              switch (currDigit) {
                case size_t(1U): postfix = "на"; break; // 'ста двадцать одНА тысяча'
                case size_t(2U): postfix = "е"; break; // 'ста двадцать двЕ тысячи' []
              }
            break;
          }
          if (!*postfix) postfix = RU_POSTFIXES[currDigit]; // if NOT setted yet
          return RU_TABLE[currDigit];
      }
      assert(false); // locale error
      return "<locale error [" MAKE_STR_(__LINE__) "]>";
    };
    // 10 - 19 [1]; 20 - 90 [10]
    auto getFirstOrderNumberStr = [&](const size_t currDigit, const size_t prevDigit,
                                      const char*& infix, const char*& postfix,
                                      const LocaleSettings& localeSettings) throw() -> const char* {
      //// Sub. tables: 10 - 19 [1]; Main tables: 20 - 90 [10]

      static const char* const EN_SUB_TABLE[] = {"ten", "eleven"}; // exceptions [NO infixes / postfixes]
      static const char* const EN_SUB_INFIXES[] = // th+ir+teen; fo+ur+teen; fi+f+teen
        {"", "", "", "ir", "ur", "f", "", "", "", ""};
      #define ESP_ "teen" // EN_SUB_POSTFIX
      static const char* const EN_SUB_POSTFIXES[] = // tw+elve ["a dozen"]; +teen ALL others
        {"", "", "elve", ESP_, ESP_, ESP_, ESP_, ESP_, ESP_, ESP_}; // +teen of ALL above 2U (twelve)
      static const char* const EN_MAIN_INFIXES[] = // tw+en+ty ["a score"]; th+ir+ty; fo+r+ty; fi+f+ty
        {"", "", "en", "ir", "r", "f", "", "", "", ""}; // +ty ALL

      #define R23I_ "дцат" // RU_20_30_INFIX [+ь]
      #define RT1I_ "на" R23I_ // RU_TO_19_INFIX [на+дцат+ь]
      static const char* const RU_SUB_INFIXES[] = // +ь; одиннадцатЬ одиннадцатИ одиннадцатЬЮ
        // ДесятЬ десятИ десятЬЮ; од и надцат ь / тр и надцат ь; дв е надцат ь; вос ем надцат ь
        {"", "ин" RT1I_, "е" RT1I_, "и" RT1I_, RT1I_, RT1I_, RT1I_, RT1I_, "ем" RT1I_, RT1I_};

      // ДвадцатЬ двадцатЬЮ двадцатЫЙ двадцатОМУ двадцатИ; семьдесят BUT семидесяти!
      #define R5T8I_ "ьдесят" // RU_50_TO_80_INFIX [NO postfix]
      static const char* const RU_MAIN_INFIXES[] = // дв а дцат ь; тр и дцат ь; пят шест сем +ьдесят
        {"", "", "а" R23I_, "и" R23I_, "", R5T8I_, R5T8I_, R5T8I_, "ем" R5T8I_, ""}; // вос ем +ьдесят
      static const char* const RU_MAIN_POSTFIXES[] = // дв а дцат ь; тр и дцат ь; пят шест сем +ьдесят
        {"", "", "ь", "ь", "", "", "", "", "", "о"}; // сорок; вос ем +ьдесят; девяност о девяност а

      static_assert(sizeof(EN_SUB_INFIXES) == sizeof(EN_MAIN_INFIXES) &&
                    sizeof(EN_SUB_POSTFIXES) == sizeof(RU_MAIN_POSTFIXES) &&
                    sizeof(RU_SUB_INFIXES) == sizeof(RU_MAIN_INFIXES), "Tables SHOULD have the same size");
      assert(prevDigit < std::extent<decltype(EN_SUB_POSTFIXES)>::value); // is valid digits?
      assert(currDigit < std::extent<decltype(EN_SUB_POSTFIXES)>::value);
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB:
          switch (prevDigit) {
            case size_t(1U): // ten - nineteen
              infix = EN_SUB_INFIXES[currDigit], postfix = EN_SUB_POSTFIXES[currDigit];
              if (currDigit < size_t(2U)) return EN_SUB_TABLE[currDigit]; // exceptions
            break;
            default: // twenty - ninety
              assert(!prevDigit && currDigit > size_t(1U));
              infix = EN_MAIN_INFIXES[currDigit], postfix = "ty"; // +ty for ALL
            break;
          }
        break;
        case ELocale::L_RU_RU:
          switch (prevDigit) {
            case size_t(1U): // десять - девятнадцать
              infix = RU_SUB_INFIXES[currDigit], postfix = "ь"; // +ь for ALL
              if (!currDigit) return "десят";
            break;
            default: // двадцать - девяносто
              assert(currDigit > size_t(1U));
              infix = RU_MAIN_INFIXES[currDigit], postfix = RU_MAIN_POSTFIXES[currDigit];
              switch (currDigit) {
                case size_t(4U): return "сорок"; // сорокА
                case size_t(9U): return "девяност"; // девяностО девяностЫХ девяностЫМ
              }
            break;
          }
        break;
        default: assert(false); // locale error
          return "<locale error [" MAKE_STR_(__LINE__) "]>";
      } // END switch (locale)
      const char* tempPtr;
      return getZeroOrderNumberStr(currDigit, size_t(), tempPtr, localeSettings);
    };
    // 100 - 900 [100]
    auto getSecondOrderNumberStr = [&](const size_t currDigit, const char*& infix, const char*& postfix,
                                       const LocaleSettings& localeSettings) throw() -> const char* {
      static const char* const RU_POSTFIXES[] =
        {"", "", "сти", "ста", "ста", "сот", "сот", "сот", "сот", "сот"};
      static_assert(size_t(10U) == std::extent<decltype(RU_POSTFIXES)>::value,
                    "Table SHOULD have the size of 10");
      assert(currDigit && currDigit < std::extent<decltype(RU_POSTFIXES)>::value);
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB:
          postfix = " hundred";
          return getZeroOrderNumberStr(currDigit, size_t(), infix, localeSettings);
        case ELocale::L_RU_RU:
          postfix = RU_POSTFIXES[currDigit];
          switch (currDigit) {
            case size_t(1U): infix = ""; return "сто"; break;
            case size_t(2U): {
              const char* temp;
              infix = "е"; //ALWAYS 'е'
              return getZeroOrderNumberStr(currDigit, size_t(), temp, localeSettings); // дв е сти
            }
          }
          return getZeroOrderNumberStr(currDigit, size_t(), infix, localeSettings);
      } // END switch (locale)
      assert(false); // locale error
      return "<locale error [" MAKE_STR_(__LINE__) "]>";
    };
    // Up to 10^99 [duotrigintillions]
    auto getOrderStr = [](size_t order, const size_t preLastDigit, const size_t lastDigit,
                          const char*& postfix, const LocaleSettings& localeSettings)
                          throw() -> const char* {
      // https://en.wikipedia.org/wiki/Names_of_large_numbers
      static const char* const EN_TABLE[] = // uses short scale (U.S., part of Canada, modern British)
        {"", "thousand", "million", "billion", "trillion", "quadrillion", "quintillion", "sextillion",
         "septillion", "octillion", "nonillion", "decillion", "undecillion", "duodecillion" /*10^39*/,
         "tredecillion", "quattuordecillion", "quindecillion", "sedecillion", "septendecillion",
         "octodecillion", "novemdecillion ", "vigintillion", "unvigintillion", "duovigintillion",
         "tresvigintillion", "quattuorvigintillion", "quinquavigintillion", "sesvigintillion",
         "septemvigintillion", "octovigintillion", "novemvigintillion", "trigintillion" /*10^93*/,
         "untrigintillion", "duotrigintillion"};
      // https://ru.wikipedia.org/wiki/Именные_названия_степеней_тысячи
      static const char* const RU_TABLE[] = // SS: short scale, LS: long scale
        {"", "тысяч", "миллион", "миллиард" /*SS: биллион*/, "триллион" /*LS: биллион*/,
         "квадриллион" /*LS: биллиард*/, "квинтиллион" /*LS: триллион*/,
         "секстиллион" /*LS: триллиард*/, "септиллион" /*LS: квадриллион*/, "октиллион", "нониллион",
         "дециллион", "ундециллион", "додециллион", "тредециллион", "кваттуордециллион" /*10^45*/,
         "квиндециллион", "седециллион", "септдециллион", "октодециллион", "новемдециллион",
         "вигинтиллион", "анвигинтиллион", "дуовигинтиллион", "тревигинтиллион", "кватторвигинтиллион",
         "квинвигинтиллион", "сексвигинтиллион", "септемвигинтиллион", "октовигинтиллион" /*10^87*/,
         "новемвигинтиллион", "тригинтиллион", "антригинтиллион", "дуотригинтиллион"}; // 10^99
      static_assert(sizeof(EN_TABLE) == sizeof(RU_TABLE), "Tables SHOULD have the same size");
      static const size_t MAX_ORDER_ =
        (std::extent<decltype(EN_TABLE)>::value - size_t(1U)) * size_t(3U); // first empty

      static const char* const RU_THOUSAND_POSTFIXES[] = // десять двадцать сто двести тысяч
        // Одна тысячА | две три четыре тысячИ | пять шесть семь восемь девять тысяч
        {"", "а", "и", "и", "и", "", "", "", "", ""};
      static const char* const RU_MILLIONS_AND_BIGGER_POSTFIXES[] = // один миллион; два - четыре миллионА
        // Пять шесть семь восемь девять миллионОВ [миллиардОВ триллионОВ etc]
        // Десять двадцать сто двести миллионОВ миллиардОВ etc
        {"ов", "", "а", "а", "а", "ов", "ов", "ов", "ов", "ов"};
      static_assert(size_t(10U) == std::extent<decltype(RU_THOUSAND_POSTFIXES)>::value &&
                    size_t(10U) == std::extent<decltype(RU_MILLIONS_AND_BIGGER_POSTFIXES)>::value,
                    "Tables SHOULD have the size of 10");
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB:
          postfix = "";
          if (size_t(2U) == order) return "hundred"; // 0U: ones, 1U: tens
          order /= 3U; // 0 - 1: empty, 3 - 5: thousands, 6 - 8: millions, 9 - 11: billions etc
          assert(order < std::extent<decltype(EN_TABLE)>::value);
          return EN_TABLE[order]; // [0, 33]
        case ELocale::L_RU_RU:
          assert(preLastDigit < size_t(10U) && lastDigit < size_t(10U));
          if (size_t(3U) == order) { // determine actual postfix first
            if (size_t(1U) != preLastDigit) {
              postfix = RU_THOUSAND_POSTFIXES[lastDigit];
            } else postfix = ""; // 'тринадцать тысяч'
          } else if (order > size_t(3U)) { // != 3U
            if (size_t(1U) == preLastDigit) { // десять одиннадцать+ миллионОВ миллиардОВ etc
              postfix = "ов";
            } else postfix = RU_MILLIONS_AND_BIGGER_POSTFIXES[lastDigit];
          }
          order /= 3U; // 6 - 8: миллионы, 9 - 11: миллиарды etc
          assert(order < std::extent<decltype(RU_TABLE)>::value);
          return RU_TABLE[order]; // [0, 33]
      }
      assert(false); // locale error
      return "<locale error [" MAKE_STR_(__LINE__) "]>";
    };

    // 'intPartPreLastDigit' AND 'intPartLastDigit' CAN be negative (in case of NO int. part)
    auto getFractionDelimiter = [](const ptrdiff_t intPartPreLastDigit, const ptrdiff_t intPartLastDigit,
                                   const char*& postfix, const bool folded,
                                   const LocaleSettings& localeSettings) throw() -> const char* {
      assert(intPartPreLastDigit < ptrdiff_t(10) && intPartLastDigit < ptrdiff_t(10));
      postfix = "";
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB: return "point"; // also 'decimal'
        case ELocale::L_RU_RU: // "целые" НЕ употребляются в учебниках!
          if (intPartLastDigit < ptrdiff_t() && localeSettings.shortFormat) return ""; // NO int. part
          if (folded) postfix = "и";
          return ptrdiff_t(1) == intPartLastDigit ?
            (ptrdiff_t(1) == intPartPreLastDigit ? "целых" : "целая") : // одинадцать целЫХ | одна целАЯ
            "целых"; // ноль, пять - девять целЫХ; две - четыре целЫХ; десять цел ых
      }
      assert(false); // locale error
      return "<locale error [" MAKE_STR_(__LINE__) "]>";
    };
    
    auto getFoldedFractionEnding = [](const LocaleSettings& localeSettings) throw() {
      // Also possibly 'continuous', 'recurring'; 'reoccurring' (Australian)
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: return "to infinity"; // also 'into infinity', 'to the infinitive'
        case ELocale::L_EN_GB: return "repeating"; // also 'repeated'
        case ELocale::L_RU_RU: return "в периоде";
      }
      assert(false); // locale error
      return "<locale error [" MAKE_STR_(__LINE__) "]>";
    };

    //// Language-specific processing lambdas

    auto getMinDigitsSubPartSizeToAddOrder = [](const LocaleSettings& localeSettings) throw() {
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB: return size_t(2U); // hundreds
        case ELocale::L_RU_RU: return size_t(3U); // тысячи
      }
      assert(false); // locale error
      return size_t();
    };

    // Returns zero (NOT set, undefined) if NOT spec. case
    auto getSpecificCaseSubPartSize = [](const long double& num,
                                         const LocaleSettings& localeSettings) throw() {
      switch (localeSettings.locale) {
        /*
        In American usage, four-digit numbers with non-zero hundreds
         are often named using multiples of "hundred"
          AND combined with tens AND/OR ones:
           "One thousand one", "Eleven hundred three", "Twelve hundred twenty-five",
            "Four thousand forty-two", or "Ninety-nine hundred ninety-nine"
        */
        case ELocale::L_EN_US:
          if (num < 10000.0L) {
            bool zeroTensAndOnes;
            const auto hundreds =
              MathUtils::getDigitOfOrder(size_t(2U), static_cast<long long int>(num), zeroTensAndOnes);
            if (hundreds && !zeroTensAndOnes) return size_t(2U); // if none-zero hundreds
          }
        break;
        // In British usage, this style is common for multiples of 100 between 1,000 and 2,000
        //  (e.g. 1,500 as "fifteen hundred") BUT NOT for higher numbers
        case ELocale::L_EN_GB:
          if (num >= 1000.0L && num < 2001.0L) {
            // If ALL digits of order below 2U [0, 1] is zero
            if (!(static_cast<size_t>(num) % size_t(100U))) return size_t(2U); // if is multiples of 100
          }
        break;
      }
      return size_t();
    };

    auto getIntSubPartSize = [&]() throw() {
      auto subPartSize = size_t();
      if (localeSettings.verySpecific)
        subPartSize = getSpecificCaseSubPartSize(num, localeSettings); // CAN alter digits subpart size
      if (!subPartSize) { // NOT set previously
        switch (localeSettings.locale) { // triads by default
          // For eng. numbers step = 1 can be ALSO used: 64.705 — 'six four point seven nought five'
          case ELocale::L_EN_US: case ELocale::L_EN_GB: case ELocale::L_RU_RU: subPartSize = size_t(3U);
        }
      }
      return subPartSize;
    };

    auto getFractSubPartSize = [](const LocaleSettings& localeSettings) throw() {
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB:
          // Step = 2 OR 3 can be ALSO used: 14.65 - 'one four point sixty-five'
          return size_t(1U); // point one two seven
        case ELocale::L_RU_RU: return size_t(3U); // сто двадцать семь сотых
      }
      assert(false); // locale error
      return size_t();
    };

    // Currently there is NO specific handling for 'short format' AND 'very specific' options
    auto estimatePossibleLength = [](const size_t digitsPartSize, const bool fractPart,
                                     const LocaleSettings& localeSettings) throw() {
      // If processing by the one digit per time; EN GB uses 'nought' instead of 'zero'
      static const auto EN_US_AVG_CHAR_PER_DIGIT_NAME_ = size_t(4U); // 40 / 10 ['zero' - 'nine']
      static size_t AVG_SYMB_PER_DIGIT_[ELocale::COUNT]; // for ALL langs; if processing by triads
      
      struct ArrayIniter { // 'AVG_SYMB_PER_DIGIT_' initer
        ArrayIniter() throw() {
          //// All this value is a result of the statistical analysis
          AVG_SYMB_PER_DIGIT_[ELocale::L_EN_GB] = size_t(10U); // 'one hundred and twenty two thousand'
          AVG_SYMB_PER_DIGIT_[ELocale::L_EN_US] = size_t(9U);  // 'one hundred twenty two thousand'
          AVG_SYMB_PER_DIGIT_[ELocale::L_RU_RU] = size_t(8U);  // 'сто двадцать две тысячи'
        }
      }; static const ArrayIniter INITER_; // static init. is a thread safe in C++11

      static const auto RU_DELIM_LEN_ = size_t(5U); // "целых" / "целая"
      // Frequent postfixes (up to trillions: 'десятитриллионных')
      static const auto RU_MAX_FREQ_FRACT_POSTFIX_LEN_ = size_t(17U);

      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB:
          if (!fractPart) return AVG_SYMB_PER_DIGIT_[localeSettings.locale] * digitsPartSize;
          // For the fract part [+1 for the spacer]
          return (EN_US_AVG_CHAR_PER_DIGIT_NAME_ + size_t(1U)) * digitsPartSize;
        case ELocale::L_RU_RU: // RU RU processes fract. part by the triads (like an int. part)
          {
            size_t len_ = AVG_SYMB_PER_DIGIT_[ELocale::L_RU_RU] * digitsPartSize;
            if (fractPart && digitsPartSize) len_ += RU_DELIM_LEN_ + RU_MAX_FREQ_FRACT_POSTFIX_LEN_;
            return len_;
          }
      }
      assert(false); // locale error
      return size_t();
    };
    
    auto addFractionPrefix = [&]() {
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB: // 'nought nought nought' for 1.0003
          {
            const char* postfix;
            for (auto leadingZeroIdx = size_t(); leadingZeroIdx < fractPartLeadingZeroesCount;) {
              assert(str.size()); // NOT empty
              str += delimiter;
              str += getZeroOrderNumberStr(size_t(), leadingZeroIdx, postfix, localeSettings);
              str += postfix;
              ++leadingZeroIdx;
            }
            return;
          }
        case ELocale::L_RU_RU: return; // NO specific prefix
      }
      assert(false); // locale error
    };

    size_t currDigit, prevDigit;
    // 'order' is an order of the last digit of a fractional part + 1 (1 based idx.)
    //  [1 for the first, 2 for the second etc]
    auto addFractionEnding = [&](const size_t orderExt) {
      if (folded) { // add postifx for the folded fraction
        auto const ending = getFoldedFractionEnding(localeSettings);
        if (*ending) { // if NOT empty
          str += delimiter;
          str += ending;
        }
        return;
      }
      //// Add 'normal' postifx
      switch (localeSettings.locale) {
        case ELocale::L_EN_US: case ELocale::L_EN_GB: break; // NO specific ending currently
        case ELocale::L_RU_RU: {
            auto toAdd = "";
            //// Add prefix / root
            assert(orderExt); // SHOULD NOT be zero
            const size_t subOrder = orderExt % size_t(3U);
            switch (subOrder) { // zero suborder - empty prefix
              case size_t(1U): // ДЕСЯТ ая(ых) | ДЕСЯТ И тысячная(ых) ДЕСЯТ И миллиардная(ых)
                toAdd = orderExt < size_t(3U) ? "десят" : "десяти"; break;
              case size_t(2U): // СОТ ая(ых) | СТО тысячная(ых) СТО миллиардная(ых)
                toAdd = orderExt < size_t(3U) ? "сот" : "сто"; break;
            }
            if (*toAdd) {
              str += delimiter;
              str += toAdd;
            }
            //// Add root (if NOT yet) + part of the postfix (if needed)
            if (orderExt > size_t(2U)) { // from 'тысяч н ая ых'
              if (!*toAdd) str += delimiter; // deim. is NOT added yet
              const char* temp;
              str += getOrderStr(orderExt, size_t(), size_t(), temp, localeSettings);
              str += "н"; // 'десят И тысяч Н ая ых', 'сто тысяч Н ая ых'
            }
            //// Add postfix
            assert(prevDigit < size_t(10U) && currDigit < size_t(10U));
            if (size_t(1U) == prevDigit) { // одинадцать двенадцать девятнадцать сотЫХ десятитысячнЫХ
              toAdd = "ых";
            } else { // NOT 1U prev. digit
              if (size_t(1U) == currDigit) {
                toAdd = "ая"; // одна двадцать одна десятАЯ, тридцать одна стотысячнАЯ
              } else toAdd = "ых"; // ноль десятых; двадцать две тридцать пять девяносто девять тясячнЫХ
            }
            str += toAdd;
          }
        break;
        default: // locale NOT present
          assert(false); // locale error
          str += "<locale error [" MAKE_STR_(__LINE__) "]>";
      }
    };

    // Also for 'and' in EN GB
    const auto minDigitsSubPartSizeToAddOrder = getMinDigitsSubPartSizeToAddOrder(localeSettings);
    auto totalAddedCount = size_t();
    // ONLY up to 3 digits
    auto processDigitOfATriad = [&](const size_t subOrder, const size_t order, size_t& currAddedCount,
                                    const size_t normalDigitsSubPartSize, const bool fractPart) {
      auto addFirstToZeroOrderDelim = [&]() {
        char delim_;
        switch (localeSettings.locale) { // choose delim.
          case ELocale::L_EN_US: case ELocale::L_EN_GB: delim_ = '-'; break; // 'thirty-four'
          case ELocale::L_RU_RU: default : delim_ = delimiter; break; // 'тридцать четыре'
        }
        str += delim_;
      };
      auto addDelim = [&](const char delim) {
        if (ELocale::L_EN_GB == localeSettings.locale) {
          // In AMERICAN English, many students are taught NOT to use the word "and"
          //  anywhere in the whole part of a number
          if (totalAddedCount && normalDigitsSubPartSize >= minDigitsSubPartSizeToAddOrder) {
            str += delim;
            str += ENG_GB_VERBAL_DELIMITER;
          }
        }
        str += delim;
      };
      assert(subOrder < size_t(3U) && prevDigit < size_t(10U) && currDigit < size_t(10U));
      const char* infix, *postfix;
      switch (subOrder) {
        case size_t(): // ones ('three' / 'три') AND numbers like 'ten' / 'twelve'
          if (size_t(1U) == prevDigit) { // 'ten', 'twelve' etc
            if (!str.empty()) addDelim(delimiter); // if needed
            str += getFirstOrderNumberStr(currDigit, prevDigit, infix, postfix, localeSettings);
            str += infix, str += postfix;
            ++currAddedCount, ++totalAddedCount;
          } else if (currDigit || size_t(1U) == normalDigitsSubPartSize) { // prev. digit is NOT 1
            //// Simple digits like 'one'
            if (prevDigit) { // NOT zero
              assert(prevDigit > size_t(1U));
              addFirstToZeroOrderDelim();
            } else if (!str.empty()) addDelim(delimiter); // prev. digit IS zero
            str += getZeroOrderNumberStr(currDigit, order, postfix, localeSettings);
            str += postfix;
            ++currAddedCount, ++totalAddedCount;
          }
        break;

        case size_t(1U): // tens ['twenty' / 'двадцать']
          if (currDigit > size_t(1U)) { // numbers like ten / twelve would be proceeded later
            if (!str.empty()) addDelim(delimiter); // if needed
            str += getFirstOrderNumberStr(currDigit, size_t(), infix, postfix, localeSettings);
            str += infix, str += postfix;
            ++currAddedCount, ++totalAddedCount;
          } // if 'currDigit' is '1U' - skip (would be proceeded later)
        break;

        case size_t(2U): // hundred(s?)
          if (!currDigit) break; // zero = empty
          if (!str.empty()) str += delimiter; // if needed
          switch (localeSettings.locale) {
            case ELocale::L_EN_US: case ELocale::L_EN_GB: // 'three hundred'
              str += getZeroOrderNumberStr(currDigit, order, postfix, localeSettings);
              str += postfix;
              str += delimiter;
              {
                const char* postfix_; // NO postfix expected, just a placeholder var.
                str += getOrderStr(size_t(2U), size_t(0U), currDigit, postfix_, localeSettings);
                assert(postfix_ && !*postfix_);
              }
            break;
            case ELocale::L_RU_RU: // 'триста'
              str += getSecondOrderNumberStr(currDigit, infix, postfix, localeSettings);
              str += infix, str += postfix;
            break;
          }
          ++currAddedCount, ++totalAddedCount;
        break;
      } // 'switch (subOrder)' END
    };

    //// Generic processing lambdas

    auto intPartPreLastDigit = ptrdiff_t(-1), intPartLastDigit = ptrdiff_t(-1); // NO part by default
    auto addFractionDelimiter = [&]() {
      const char* postfix;
      auto const fractionDelim =
        getFractionDelimiter(intPartPreLastDigit, intPartLastDigit, postfix, folded, localeSettings);
      if (*fractionDelim) { // if NOT empty
        if (!str.empty()) str += delimiter;
        str += fractionDelim;
      }
      if (*postfix) {
        if (*fractionDelim) str += delimiter;
        str += postfix;
      }
    };

    auto addedCount = size_t(); // during processing curr. part
    auto emptySubPartsCount = size_t();
    // Part order is an order of the last digit of the part (zero for 654, 3 for 456 of the 456654 etc)
    // Part (integral OR fractional) of the number is consists of the subparts of specified size
    //  (usually 3 OR 1; for ENG.: 3 for int. part., 1 for fract. part)
    // 'subPartOrderExt' SHOULD exists ONLY for a LAST subpart
    auto processDigitsSubPart = [&](const size_t currDigitsSubPartSize,
                                    const size_t normalDigitsSubPartSize,
                                    const size_t order, size_t subPartOrderExt, const bool fractPart) {
      assert(currDigitsSubPartSize && currDigitsSubPartSize <= size_t(3U));
      auto currAddedCount = size_t(); // reset
      auto emptySubPart = true; // true if ALL prev. digits of the subpart is zero
      prevDigit = std::decay<decltype(prevDigit)>::type(); // reset
      for (size_t subOrder = currDigitsSubPartSize - size_t(1U);;) {
        if (DECIMAL_DELIM_ != *currSymbPtr) { // skip decimal delim.
          currDigit = *currSymbPtr - '0'; // assuming ANSI ASCII
        PPOCESS_DIGIT_:
          assert(*currSymbPtr >= '0' && currDigit < size_t(10U));
          emptySubPart &= !currDigit;
          processDigitOfATriad(subOrder + subPartOrderExt, order, currAddedCount,
                               normalDigitsSubPartSize, fractPart);
          if (subPartOrderExt) { // treat unpresented digits [special service]
            --subPartOrderExt;
            prevDigit = currDigit;
            currDigit = std::decay<decltype(currDigit)>::type(); // remove ref. from type
            goto PPOCESS_DIGIT_; // don't like 'goto'? take a nyan cat here: =^^=
          }
          if (!subOrder) { // zero order digit
            ++currSymbPtr; // shift to the symb. after the last in an int. part
            break;
          }
          --subOrder, prevDigit = currDigit;
        }
        ++currSymbPtr;
      }
      if (emptySubPart) ++emptySubPartsCount; // update stats
      // Add order str. AFTER part (if exist)
      if (currAddedCount && normalDigitsSubPartSize >= minDigitsSubPartSizeToAddOrder) {
        const char* postfix;
        auto const orderStr = getOrderStr(order, prevDigit, currDigit, postfix, localeSettings);
        assert(orderStr && postfix);
        if (*orderStr) { // if NOT empty (CAN be empty for zero order [EN, RU])
          assert(str.size()); // NOT zero
          str += delimiter, str += orderStr, str += postfix;
          ++currAddedCount;
        }
      }
      addedCount += currAddedCount;
    };

    size_t intPartAddedCount, strLenWithoutFractPart;
    // Strategy used to process both integral AND fractional parts of the number
    // 'digitsPartSize' is a total part. len. in digits (i. e. 1 for 4, 3 for 123, 6 for 984532 etc)
    //  [CAN be zero in some cases]
    // 'partBonusOrder' will be 3 for 124e3, 9 for 1.2e10, 0 for 87654e0 etc
    // 'fractPart' flag SHOULD be true if processing fraction part
    auto processDigitsPart = [&](size_t digitsPartSize, const size_t digitsSubPartSize,
                                 size_t partBonusOrder, const bool fractPart) {
      currDigit = size_t(), prevDigit = size_t(); // reset
      if (digitsPartSize) {
        assert(digitsSubPartSize); // SHOULD be NOT zero
        size_t currDigitsSubPartSize =
          (digitsPartSize + partBonusOrder) % digitsSubPartSize; // 2 for 12561, 1 for 9 etc
        if (!currDigitsSubPartSize) currDigitsSubPartSize = digitsSubPartSize; // if zero remanider
        // Will be 2 for '12.34e4' ('1234e2' = '123 400' - two last unpresented zeroes); 1 for 1e1
        auto subPartOrderExt = size_t(); // used ONLY for a last subpart
        
        // OPTIMIZATION HINT: redesign to preallocate for the whole str., NOT for a diffirent parts? 
        if (ReserveBeforeAdding) // optimization [CAN acquire more / less space then really required]
          str.reserve(str.length() + estimatePossibleLength(digitsPartSize, fractPart, localeSettings));
        do {
          if (currDigitsSubPartSize > digitsPartSize) { // if last AND unnormal [due to the '%']
            subPartOrderExt = currDigitsSubPartSize - digitsPartSize;
            partBonusOrder -= subPartOrderExt;
            currDigitsSubPartSize = digitsPartSize; // correct
          }
          digitsPartSize -= currDigitsSubPartSize;
          processDigitsSubPart(currDigitsSubPartSize, digitsSubPartSize,
                               digitsPartSize + partBonusOrder, subPartOrderExt, fractPart);
          currDigitsSubPartSize = digitsSubPartSize; // set default [restore]
        } while (digitsPartSize);
      }
      auto mentionZeroPart = [&]() {
        if (!str.empty()) str += delimiter;
        const char* postfix;
        str += getZeroOrderNumberStr(size_t(), size_t(), postfix, localeSettings);
        str += postfix;
        ++totalAddedCount;
      };
      if (!addedCount) { // NO part
        if (!localeSettings.shortFormat || folded) { // NOT skip mention zero parts
          if (fractPart) {
            addFractionDelimiter(); // 'ноль целых'
          } else intPartLastDigit = ptrdiff_t(); // now. IS int. part
          mentionZeroPart();
          ++addedCount;
        } else if (fractPart) { // short format AND now processing fraction part
          assert(!folded); // NO fract. part - SHOULD NOT be folded
          assert(strLenWithoutFractPart <= str.size()); // SHOULD NOT incr. len.
          if (!intPartAddedCount) { // NO int. part [zero point zero -> zero] <EXCEPTION>
            mentionZeroPart(); // do NOT incr. 'addedCount'!!
          }
        }
      }
    };

    //// Process int. part

    processDigitsPart(intPartLen, getIntSubPartSize(), intPartBonusOrder, false);
    if (truncated::ExecIfPresent(str)) { // check if truncated
      if (errMsg) *errMsg = "too short buffer"; return false;
    }
    if (intPartLen) { // if int. part exist
      assert(currSymbPtr > strBuf);
      intPartLastDigit = *(currSymbPtr - ptrdiff_t(1)) - '0';
      assert(intPartLastDigit > ptrdiff_t(-1) && intPartLastDigit < ptrdiff_t(10));
      if (intPartLen > size_t(1U)) { // there is also prelast digit
        auto intPartPreLastDigitPtr = currSymbPtr - ptrdiff_t(2);
        if (DECIMAL_DELIM_ == *intPartPreLastDigitPtr) --intPartPreLastDigitPtr; // skip delim.: 2.3e1
        assert(intPartPreLastDigitPtr >= strBuf); // check borders
        intPartPreLastDigit = *intPartPreLastDigitPtr - '0';
        assert(intPartPreLastDigit > ptrdiff_t(-1) && intPartPreLastDigit < ptrdiff_t(10));
      }
    }
    strLenWithoutFractPart = str.size(); // remember (for future use)
    intPartAddedCount = addedCount;
    addedCount = decltype(addedCount)(); // reset

    //// Process fract. part
    
    if (fractPartLen) {
      addFractionDelimiter();
      addFractionPrefix(); // if needed
      currSymbPtr = fractPartRealStart; // might be required if folded [in SOME cases]
    }
    processDigitsPart(fractPartLen, getFractSubPartSize(localeSettings), size_t(), true);
    if (addedCount) { // smth. added (even if zero part)
      fractPartAddedCount = addedCount;
      //// Add specific ending (if needed, like 'десятимиллионная')
      assert(fractPartLen >= decltype(fractPartLen)());
      size_t fractPartLastDigitOrderExt = fractPartLeadingZeroesCount + fractPartLen;
      if (!fractPartLastDigitOrderExt) fractPartLastDigitOrderExt = size_t(1U); // at least one
      addFractionEnding(fractPartLastDigitOrderExt);
    }
    assert(totalAddedCount); // SHOULD NOT be zero
    if (truncated::ExecIfPresent(str)) { // check if truncated
      if (errMsg) *errMsg = "too short buffer"; return false;
    } return true;
  }
};

#endif // ConvertionUtilsH