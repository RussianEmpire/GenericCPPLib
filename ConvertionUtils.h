#ifndef ConvertionUtilsH
#define ConvertionUtilsH

#include "FuncUtils.h" // for 'ExecIfPresent'
#include "MathUtils.h" // for 'getDigitOfOrder'

EXEC_MEMBER_FUNC_IF_PRESENT(full, false)

// Abstract
class ConvertionUtils {
  
public:
  // In AMERICAN English, many students are taught NOT to use the word "and"
  //  anywhere in the whole part of a number
  static const char* const ENG_GB_VERBAL_DELIMITER;
  static const char* const DEFAULT_DELIMITER;

  // Do NOT add other dialects of english e.g. Irish, Australian, New Zealand, Indian etc
  enum class ELocale {
    L_RU_RU, // Russian Federation Russian
    L_EN_US, // United States English
    L_EN_GB  // United Kingdom English
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
    ELocale locale = ELocale::L_EN_GB;
  };

  // "Integral mumber to the numeric format string" (321 -> "three hundred twenty-one")
  // Accpets negative numbers
  // TO DO: add fractional numbers support ("минус ??? целых ??? <тысячных>")
  //  (see Fractions and decimals:
  //   http://en.wikipedia.org/wiki/English_numerals#Fractions_and_decimals)
  /* 123,456 = [http://eng5.ru/en/numbers_translation]
     [RU] сто двадцать три целые четыреста пятьдесят шесть тысячных
     [BE] one hundred and twenty-three point four five six 
     [AE] one hundred twenty-three point four five six
  */
  // TO DO: add ordinal numbers support
  //  (http://en.wikipedia.org/wiki/English_numerals#Ordinal_numbers)
  // TO DO: add multiplicative adverbs support
  //  (http://en.wikipedia.org/wiki/English_numerals#Multiplicative_adverbs)
  // TO DO: add long / short scale support
  //  (https://en.wikipedia.org/wiki/Long_and_short_scales)
  // TO DO: add money support (writing a cheque (or check),
  //  the number 100 is always written "one hundred", it is never "a hundred")
  //   [enum: money, count, index, dates, temperatures -
  //    http://en.wikipedia.org/wiki/English_numerals#Negative_numbers]
  // TO DO: add plus sign support
  // TO DO: add specialized numbers support
  //  (http://en.wikipedia.org/wiki/English_numerals#Specialized_numbers)
  // 'TStrType' SHOULD support operator '+='
  template<class TStrType>
  static bool intNumToNumFormatStr(long long int num, TStrType& str,
                                   const LocaleSettings& localeSettings =
                                     LocaleSettings::DEFAULT_LOCALE_SETTINGS,
                                   const char** const errMsg = nullptr) throw()
  {
    auto getNegativeSignStr = [](const ELocale locale) throw() -> const char* {
      switch (locale) {
        case ELocale::L_EN_US: return "negative"; break;
        case ELocale::L_EN_GB: return "minus"; break;
        case ELocale::L_RU_RU: return "минус"; break;
      }
      return "";
    };
    
    if (num < 0LL) { // if negative
      const char* const delimiter = DEFAULT_DELIMITER;
      str += getNegativeSignStr(localeSettings.locale);
      str += delimiter;
      if (full::ExecIfPresent(str)) {
        if (errMsg) *errMsg = "internal error: too short buf.";
        return false;
      }
      num = -num; // revert
    }
    IntNumToNumFormatStrData<TStrType> data(num, str, localeSettings, errMsg);
    
    return intNumToNumFormatStrR(data);
  }
  
private:
  
  ConvertionUtils() throw() = delete;
  ~ConvertionUtils() throw() = delete;
  ConvertionUtils(const ConvertionUtils&) throw() = delete;
  ConvertionUtils(ConvertionUtils&&) throw() = delete;
  ConvertionUtils& operator=(const ConvertionUtils&) throw() = delete;
  ConvertionUtils& operator=(ConvertionUtils&&) throw() = delete;

  template<class TStrType>
  struct IntNumToNumFormatStrData {
    
    IntNumToNumFormatStrData(const unsigned long long num, TStrType& str,
                             const LocaleSettings& localeSettings =
                               LocaleSettings::DEFAULT_LOCALE_SETTINGS,
                             const char** const errMsg = nullptr) throw()
      : num(num), str(str), errMsg(errMsg), localeSettings(&localeSettings)
    {
      currDigit = num % 10U;
    }

    ~IntNumToNumFormatStrData() = default;
    
    unsigned long long num = 0ULL; // NOT the original number!
    decltype(num) originalNum = num;

    size_t order = size_t();
    size_t prevDigit = size_t();
    size_t currDigit = size_t();
    
    bool prevDigitsPartNotEmpty = false;

    TStrType& str;
    const char** errMsg = nullptr;
    const LocaleSettings* localeSettings = &LocaleSettings::DEFAULT_LOCALE_SETTINGS;

    #ifndef NDEBUG // '#ifdef _DEBUG'; collect debug statistic
      size_t recursionDepth = size_t();
    #endif

  private:

    IntNumToNumFormatStrData() throw() = delete; // 'TStrType& str' - ref. can NOT be default inited

    //// Deleted as there are ref. in the struct: 'TStrType& str'
    ////  [can NOT be changed to refer to another object]
    IntNumToNumFormatStrData(const IntNumToNumFormatStrData&) throw() = delete;
    IntNumToNumFormatStrData(IntNumToNumFormatStrData&&) throw() = delete;
    IntNumToNumFormatStrData& operator=(const IntNumToNumFormatStrData&) throw() = delete;
    IntNumToNumFormatStrData& operator=(IntNumToNumFormatStrData&&) throw() = delete;
  };
  
  /* NOTES:
  1) Ten thousand is RARELY called a myriad [do NOT used, as processing by triades]
  2) Indian English - NOT supported
  3) "and" delimiter is used ALSO in Irish English,
      Australian English AND New Zealand English
  4) American English:
      four-digit numbers with non-zero hundreds are often named using multiples of "hundred"
       AND combined with tens and ones: "Eleven hundred three" [OK]
     (In British usage, this style is common for multiples of 100 between 1,000 and 2,000
      e.g. 1,500 as "fifteen hundred" BUT NOT for higher numbers) - [OK]
  5) Americans may pronounce four-digit numbers with non-zero tens AND ones
      as pairs of two-digit numbers without saying "hundred"
       AND inserting "oh" for zero tens: "twenty-six fifty-nine" or "forty-one oh five"
        [does NOT supported]
  6) American English: 1,200,000 = 1.2 million = one point two million;
     23,380,000,000 = 23.38 billion =
      twenty-three point three eight billion [such examples do NOT supported]
  */
  // OPTIMIZATION HINT: redesign to use cycle, rather then the recursion??
  template<class TStrType> // R - recursive
  static bool intNumToNumFormatStrR(IntNumToNumFormatStrData<TStrType>& data) throw() {
    //// Main lambdas definition

    // 0 - 9
    auto getZeroOrderNumberStr = [](const size_t currDigit,
                                    const ELocale locale) throw() -> const char*
    {
      static const char* const EN_TABLE[] =
        {"zero" /*"nought"*/, "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
      static const char* const RU_TABLE[] =
      //                                          пят + ь, шест + ь, сем + ь, восем + ь, девят + ь
        {"ноль", "один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"};
      static_assert(sizeof(EN_TABLE) == sizeof(RU_TABLE) &&
                    10U == std::extent<decltype(EN_TABLE)>::value,
                    "Tables SHOULD have the same size (10)");

      assert(currDigit < std::extent<decltype(EN_TABLE)>::value); // is valid digit?
      switch (locale) {
        case ELocale::L_EN_US:
        case ELocale::L_EN_GB:
          return EN_TABLE[currDigit];
        case ELocale::L_RU_RU:
          return RU_TABLE[currDigit];
      }
      assert(false); // locale error
      return "<invalid locale>";
    };
    // 10 - 90
    auto getFirstOrderNumberStr = [](const size_t currDigit, const size_t prevDigit,
                                     const ELocale locale) throw() -> const char*
    {
      //// Sub. tables: 10 - 19 [1]
      //// Main tables: 20 - 90 [10]
      static const char* const EN_SUB_TABLE[] =
        {"ten", "eleven", "twelve" /*"a dozen"*/,
      //  thir +,     four +,     fif +,     six +,     seven +,     eigh +,     nine + <teen>
         "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
      static const char* const EN_MAIN_TABLE[] =
      //           twen +,                 thir +,   for + <ty>
        {"", "" , "twenty" /*"a score"*/, "thirty", "forty",
      //  fif +,   six +,   seven +,   eigh +,   nine    + <ty>
         "fifty", "sixty", "seventy", "eighty", "ninety"};

      static const char* const RU_SUB_TABLE[] =
      //            оди +,        две +,        три +,        четыр        + <надцать>
        {"десять", "одинадцать", "двенадцать", "тринадцать", "четырнадцать", 
      //  пят +,        шест +,        сем +,        восем +,        девят + <надцать>
         "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"};
      static const char* const RU_MAIN_TABLE[] =
      //          два +,      три + <дцать>;       пят +,       шест + <ьдесят>
        {"", "", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят",
      //  сем +,       восем + <ьдесят>;  девя + <носто>
         "семьдесят", "восемьдесят",     "девяносто"};

      static_assert(sizeof(EN_SUB_TABLE) == sizeof(RU_SUB_TABLE) &&
                    sizeof(EN_MAIN_TABLE) == sizeof(RU_MAIN_TABLE),
                    "Tables SHOULD have the same size");
      
      assert(prevDigit < std::extent<decltype(EN_SUB_TABLE)>::value); // is valid digit?
      assert(currDigit < std::extent<decltype(EN_MAIN_TABLE)>::value);
      switch (locale) {
        case ELocale::L_EN_US:
        case ELocale::L_EN_GB:
          switch (currDigit) {
            case 1U:
              return EN_SUB_TABLE[prevDigit];
            default: // > 1U
              return EN_MAIN_TABLE[currDigit];
          } // END switch (currDigit)
        break; // END case ELocale::L_EN_US

        case ELocale::L_RU_RU:
          switch (currDigit) {
            case 1U:
              return RU_SUB_TABLE[prevDigit];
            default: // > 1U
              return RU_MAIN_TABLE[currDigit];
          } // END switch (currDigit)
        break; // END case ELocale::L_RU_RU
      } // END switch (locale)
      assert(false); // locale error
      return "<invalid locale>";
    };

    // 2^64-1 = 18 446 744 073 709 551 615 (max 20 digits)
    auto getOrderStr = [](size_t order,
                          const size_t digit, // digit: first if NOT zero, last if IS zero
                          const char*& postfix, const ELocale locale) throw() -> const char*
    {
      static const char* const EN_TABLE[] = 
        {"", "thousand", "million", "billion", "trillion",
         "quadrillion", "quintillion", "sextillion", "septillion"};
      static const char* const RU_TABLE[] = // SS: short scale, LS: long scale
        {"", "тысяч", "миллион", "миллиард" /*SS: биллион*/, "триллион" /*LS: биллион*/,
         "квадриллион" /*LS: биллиард*/, "квинтиллион" /*LS: триллион*/,
         "секстиллион" /*LS: триллиард*/, "септиллион" /*LS: квадриллион*/};
      static_assert(sizeof(EN_TABLE) == sizeof(RU_TABLE), "Tables SHOULD have the same size");

      postfix = "";
      if (!order) return "";

      switch (locale) {
        case ELocale::L_EN_US:
        case ELocale::L_EN_GB:
          switch (order) {
            case 2U: return "hundred"; // 0U: ones, 1U: tens
            default:
              order /= 3U; // 6 - 8: millions, 9 - 11: billions etc
              assert(order < std::extent<decltype(EN_TABLE)>::value);
              return EN_TABLE[order]; // [3, 24]
          }
        break;

        case ELocale::L_RU_RU:
          assert(digit < 10U);
          // Determine actual postfix first
          if (3U == order) {
            // Одна тысячА | две три четыре тысячИ | пять шесть семь восемь девять тысяч
            // Десять сто тысяч
            switch (digit) {
              case 1U: postfix = "а"; break;
              case 2U: case 3U: case 4U: postfix = "и"; break;
            }
          } else { // != 3U
            assert(order > 3U); // SHOULD be > 3U
            // Один миллион | два три четыре миллионА |
            //  пять шесть семь восемь девять миллионОВ [миллиард триллион etc]
            //  десять сто миллионОВ миллиардОВ etc
            switch (digit) {
              case 2U: case 3U: case 4U: postfix = "а"; break;
              case 0U: case 5U: case 6U: case 7U: case 8U: case 9U: postfix = "ов"; break;
            }
          }
          order /= 3U; // 6 - 8: миллионы, 9 - 11: миллиарды etc
          assert(order < std::extent<decltype(RU_TABLE)>::value);
          return RU_TABLE[order]; // [3, 24]
        break; // L_RU_RU END
      }
      assert(false); // locale error
      return "<invalid locale>";
    };
    // 100 - 900 [100]
    auto getSecondOrderNumberStr = [](const size_t currDigit, const ELocale locale) throw()
      -> const char*
    {
      #define DELIM " "
      #define HUNDRED "hundred"
      static const char* const EN_TABLE[] =
        {"", "one" DELIM "" HUNDRED, "two" DELIM "" HUNDRED, "three" DELIM "" HUNDRED, "four" DELIM "" HUNDRED,
         "five" DELIM "" HUNDRED, "six" DELIM "" HUNDRED, "seven" DELIM "" HUNDRED, "eight" DELIM "" HUNDRED,
         "nine" DELIM "" HUNDRED};
      static const char* const RU_TABLE[] =
      //             дв + <ести>
        {"", "сто", "двести", 
      //  три +,    четыре + <ста>;  пять +,    шесть +,    семь +,    восемь +,    девять + <сот>
         "триста", "четыреста",     "пятьсот", "шестьсот", "семьсот", "восемьсот", "девятьсот"};

      static_assert(sizeof(EN_TABLE) == sizeof(RU_TABLE), "Tables SHOULD have the same size");
      assert(currDigit < std::extent<decltype(EN_TABLE)>::value);
      switch (locale) {
        case ELocale::L_EN_US:
        case ELocale::L_EN_GB:
          return EN_TABLE[currDigit];
        case ELocale::L_RU_RU:
          return RU_TABLE[currDigit];
      } // END switch (locale)
      assert(false); // locale error
      return "<invalid locale>";
    };
    
    const char* const delimiter = DEFAULT_DELIMITER;

    /* Max. digit count for 64 bit number is 20 => up to a 7 triades
        each triad has up to a 3 additives ('two hundred' 'eighty' 'seven'), up to a 1 postfix
         AND up to 3 delims (between)
          + 6 delims before triads
           => 7*7 + 6 = at least 55 (54, as the first triad does NOT have a postfix)
      Total (in a whole): 56 SHOULD be enough, BUT as a func. is recursive, we need only 8
       WRONG! (coze there endings also exists AND '-'), test shows that really up to 12
        [' eight hundred and forty-seven billion']
    */
    static const auto MAX_ADDITIVES_COUNT = 16U + 8U; // 8 more, to be on a safe side
    const char* additives[MAX_ADDITIVES_COUNT];
    auto additivesCount = size_t();
    
    auto addSecondOrderNumberStr = [&]() throw() {
      switch (data.localeSettings->locale) {
        case ELocale::L_EN_US:
        case ELocale::L_EN_GB:
          //// [!] Adding in the reverse order [!]
          {
            const char* prefix = ""; // NO prefix expected, just a placeholder var.
            additives[additivesCount++] =
              getOrderStr(2U, data.currDigit, prefix, data.localeSettings->locale);
            assert(prefix && !*prefix);
          }
          additives[additivesCount++] = delimiter;
          additives[additivesCount++] =
            getZeroOrderNumberStr(data.currDigit, data.localeSettings->locale);
        break;

        case ELocale::L_RU_RU:
          additives[additivesCount++] =
            getSecondOrderNumberStr(data.currDigit, data.localeSettings->locale);
        break;
      }
      // 'sizeof(additives) / sizeof(std::remove_reference<decltype(*additives)>::type)' can be used
      //  instead of 'MAX_ADDITIVES_COUNT'
      assert(additivesCount <= MAX_ADDITIVES_COUNT);
    };

    //// Special cases & rewind

    // Set digits part size
    auto maxSubOrder = size_t(3U); // procide by triads by default
    auto handleSpecificCases = [&]() throw() {
      switch (data.localeSettings->locale) {
        case ELocale::L_EN_US:
          /*
          In American usage, four-digit numbers with non-zero hundreds
           are often named using multiples of "hundred"
            AND combined with tens and ones:
             "One thousand one", "Eleven hundred three", "Twelve hundred twenty-five",
              "Four thousand forty-two", or "Ninety-nine hundred ninety-nine"
          */
          if (data.originalNum <= 9999ULL) {
            auto dummyFlag = false;
            if (MathUtils::getDigitOfOrder(2U, data.originalNum, dummyFlag)) // If none-zero hundreds
              maxSubOrder = 2U;
          }
        break;

        case ELocale::L_EN_GB:
          /*
          In British usage, this style is common
           for multiples of 100 between 1,000 and 2,000
            (e.g. 1,500 as "fifteen hundred") BUT NOT for higher numbers
          */
          if (data.originalNum >= 1000ULL && data.originalNum <= 2000ULL) {
            // if ALL order digits BEFORE 2U is zero
            if (!(data.originalNum % 100ULL)) // if is multiples of 100
              maxSubOrder = 2U;
          }
        break;
      }
    };
    if (data.localeSettings->verySpecific) handleSpecificCases(); // CAN alter digits part size

    decltype(data.num) nextPartNum;
    auto rewindToFirstNoneZeroDigitsPart = [&]() throw() {
      const auto divider =
        static_cast<size_t>(MathUtils::getTenPositiveDegree(maxSubOrder));
      decltype(data.num) shiftedNum;
      while (true) {
        shiftedNum = data.num / divider;
        nextPartNum = shiftedNum;
        shiftedNum *= divider; // shift back (to the origin)

        if (data.num && shiftedNum == data.num) { // test curr. part - is empty
          // Curr. part is empty ('000', if a triad), while number itself is NOT
          data.prevDigit = size_t();
          data.currDigit = nextPartNum % size_t(10UL);

          data.num = nextPartNum;
          data.order += maxSubOrder; // shift MAIN order

          data.prevDigitsPartNotEmpty = false;
        } else break;
        // skip empty triad
      }
    };
    rewindToFirstNoneZeroDigitsPart(); // rewind AFTER handling spec. cases AND BEFORE adding postfix
    
    //// Postfix [used in recoursive callings]

    auto addOrderPostfix = [&]() throw() {
      // Две тысячИ / двадцать тысяч / двести тысяч
      // Два миллионА / двадцать миллионоВ / двести миллионоВ
      auto postfix = "";
      auto const orderStr =
        getOrderStr(data.order, data.currDigit, postfix, data.localeSettings->locale);

      auto orderAdded = false;
      //// [!] Adding in the reverse order [!]
      if (postfix && *postfix) {
        additives[additivesCount++] = postfix;
        orderAdded = true;
      }
      if (orderStr && *orderStr) {
        additives[additivesCount++] = orderStr;
        orderAdded = true;
      }
      if (orderAdded) additives[additivesCount++] = delimiter;
      assert(additivesCount <= MAX_ADDITIVES_COUNT);
    };
    if (data.order) addOrderPostfix(); // add order postfix (if NOT first call)

    //// Procide by digits parts [main processing cycle]

    auto nextNum = 0ULL;
    auto nextDigit = size_t();
    for (auto subOrder = size_t(); subOrder < maxSubOrder; ++subOrder) {
      nextNum = data.num / size_t(10UL);
      nextDigit = nextNum % size_t(10UL);
      
      switch (subOrder) {
        case size_t(): // ones
          if ((data.currDigit && !nextDigit) || // if curr. digit is NOT zero AND next digit is zero
              !data.num)                        // if curr. number itself is zero
          {
            additives[additivesCount++] =
              getZeroOrderNumberStr(data.currDigit, data.localeSettings->locale);
          } // if curr. digit IS zero OR next digit is NOT zero OR
            //  curr. number itself is NOT zero: empty CURRENT additive
        break;
        
        case size_t(1U): // tens
          if (data.currDigit) {
            // Numbers like ten / twelve etc ARE ALREADY processed by 'getFirstOrderNumberStr'
            if (data.prevDigit && data.currDigit > size_t(1U)) {
              //// [!] Adding in the reverse order [!]
              additives[additivesCount++] = 
                getZeroOrderNumberStr(data.prevDigit, data.localeSettings->locale);
              switch (data.localeSettings->locale) { // add delim.
                case ELocale::L_EN_US: case ELocale::L_EN_GB:
                  additives[additivesCount++] = "-"; break;
                case ELocale::L_RU_RU: additives[additivesCount++] = " "; break;
              }
            }
            additives[additivesCount++] =
              getFirstOrderNumberStr(data.currDigit, data.prevDigit, data.localeSettings->locale);
          }
        break;
        
        case 2U: // hundred(s?)
          // In AMERICAN English, many students are taught not to use the word "and"
          //  anywhere in the whole part of a number
          if (additivesCount && (nextPartNum || data.currDigit) &&
              ELocale::L_EN_GB == data.localeSettings->locale)
          {
            // If some additives already added & UK ENG. & next traids OR curr. hundred is NOT empty
            additives[additivesCount++] = delimiter;
            additives[additivesCount++] = ENG_GB_VERBAL_DELIMITER;
          }
          if (data.currDigit) {
            if (additivesCount) additives[additivesCount++] = delimiter;
            addSecondOrderNumberStr();
          }
        break;
      } // 'switch (subOrder)' END
      assert(additivesCount <= std::extent<std::remove_reference<decltype(additives)>::type>::value);
      if (!data.num) break;

      data.num = nextNum;
      ++data.order; // shift MAIN order
      data.prevDigit = data.currDigit;
      data.currDigit = nextDigit;
    } // procide by digits parts END
    
    #ifndef NDEBUG
      static size_t MAX_RECURSION_DEPTH_; // 4 in current tests
    #endif
    if (data.num) { // still some orders left
      #ifndef NDEBUG
        ++data.recursionDepth;
      #endif
      if (!intNumToNumFormatStrR(data)) return false;
      #ifndef NDEBUG
        if (MAX_RECURSION_DEPTH_ < data.recursionDepth) MAX_RECURSION_DEPTH_ = data.recursionDepth;
        --data.recursionDepth;
      #endif
    }
    if (additivesCount) { // add ALL additives (if exists)
      if (data.prevDigitsPartNotEmpty) additives[additivesCount++] = delimiter;
      assert(additivesCount <= MAX_ADDITIVES_COUNT);
      const char* currAdditive = "";
      
      for (size_t currAdditiveIdx = additivesCount - size_t(1U); ; --currAdditiveIdx) {
        currAdditive = additives[currAdditiveIdx];
        if (currAdditive && *currAdditive) {
          data.str += currAdditive;
          if (full::ExecIfPresent(data.str)) {
            if (data.errMsg) *data.errMsg = "internal error: too short buf.";
            return false;
          }
        }
        if (!currAdditiveIdx) break;
      }
      data.prevDigitsPartNotEmpty = true; // data.prevDigitsPartNotEmpty = additivesCount > 0U;
    } else data.prevDigitsPartNotEmpty = false; // NO additives

    #ifndef NDEBUG // '#ifdef _DEBUG'; collect debug statistic
      //// ALL static fields are zero inited
      static size_t MAX_ADDITIVES_REAL_COUNT_; // 12 in current tests
      if (MAX_ADDITIVES_REAL_COUNT_ < additivesCount) MAX_ADDITIVES_REAL_COUNT_ = additivesCount;
      static size_t MAX_RESULT_STR_LEN_; // 163 in current tests
      if (MAX_RESULT_STR_LEN_ < data.str.length()) MAX_RESULT_STR_LEN_ = data.str.length();
    #endif

    return true;
  }
};

#endif // ConvertionUtilsH