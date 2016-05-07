#ifndef ConvertionUtilsTestsH
#define ConvertionUtilsTestsH

#include "ConvertionUtils.h"

#include <string>

#ifdef _MSC_VER // MS VS
  #define STRCMPI _stricmp // ISO C++ conformant
#elif __GNUC__ // GCC
  #include <strings.h>
  #define STRCMPI strcasecmp // int strcasecmp(const char *s1, const char *s2)
#endif

namespace ConvertionUtilsTests {

// 'TStrType' SHOULD have a default constructor, SHOULD support 'clear' method AND operator '+='
template <class TStrType = std::string>
// Functional tests
void test() throw() {
  auto result = false;
  auto num = 0LL;
  auto errMsg = "", resulStr = "";
  ConvertionUtils::LocaleSettings localeSettings;
  TStrType str__01_;

  /* To replace ALL occurrences like 'num = 0;' -> 'num = 0LL' used the following regexp.:
     '(?<1>num = [^L;]*);' -> '$1LL;'
  */
  //// 0
  str__01_.clear();
  num = 0LL;
  resulStr = "zero";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 0LL;
  resulStr = "zero";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 0LL;
  resulStr = "ноль";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// 9
  str__01_.clear();
  num = 9LL;
  resulStr = "nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 9LL;
  resulStr = "nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 9LL;
  resulStr = "девять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// 4
  str__01_.clear();
  num = 4LL;
  resulStr = "four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 4LL;
  resulStr = "four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 4LL;
  resulStr = "четыре";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// 10
  str__01_.clear();
  num = 10LL;
  resulStr = "ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 10LL;
  resulStr = "ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 10LL;
  resulStr = "десять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// -13
  str__01_.clear();
  num = -13LL;
  resulStr = "negative thirteen";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -13LL;
  resulStr = "minus thirteen";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -13LL;
  resulStr = "минус тринадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// -19
  str__01_.clear();
  num = -19LL;
  resulStr = "negative nineteen";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -19LL;
  resulStr = "minus nineteen";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -19LL;
  resulStr = "минус девятнадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// -20
  str__01_.clear();
  num = -20LL;
  resulStr = "negative twenty";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -20LL;
  resulStr = "minus twenty";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -20LL;
  resulStr = "минус двадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// 20
  str__01_.clear();
  num = 20LL;
  resulStr = "twenty";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 20LL;
  resulStr = "twenty";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 20LL;
  resulStr = "двадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// 90
  str__01_.clear();
  num = 90LL;
  resulStr = "ninety";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 90LL;
  resulStr = "ninety";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 90LL;
  resulStr = "девяносто";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// 21
  str__01_.clear();
  num = 21LL;
  resulStr = "twenty-one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 21LL;
  resulStr = "twenty-one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 21LL;
  resulStr = "двадцать один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 32LL;
  resulStr = "thirty-two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -79LL;
  resulStr = "minus seventy-nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -99LL;
  resulStr = "минус девяносто девять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 100LL;
  resulStr = "one hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -900LL;
  resulStr = "minus nine hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 2000LL;
  resulStr = "two thousand";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -85000LL;
  resulStr = "minus eighty-five thousand";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 999000LL;
  resulStr = "nine hundred and ninety-nine thousand";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -999000LL;
  resulStr = "negative nine hundred ninety-nine thousand";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 1000000LL;
  resulStr = "one million";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -10000000LL;
  resulStr = "minus ten million";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -334LL;
  resulStr = "minus three hundred and thirty-four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 208LL;
  resulStr = "two hundred and eight";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 1000000000LL;
  resulStr = "one billion";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 1000000000000LL;
  resulStr = "one trillion";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 1000000000000000000LL;
  resulStr = "one quintillion";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 250000000LL;
  resulStr = "two hundred fifty million";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 1LL;
  resulStr = "один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 2LL;
  resulStr = "два";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 3LL;
  resulStr = "три";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 18LL;
  resulStr = "восемнадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 80LL;
  resulStr = "восемьдесят";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 800LL;
  resulStr = "восемьсот";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -88888LL;
  resulStr = "минус восемьдесят восемь тысяч восемьсот восемьдесят восемь";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 39LL;
  resulStr = "тридцать девять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -952LL;
  resulStr = "минус девятьсот пятьдесят два";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 732847047284204LL;
  resulStr =
    "семьсот тридцать два триллиона восемьсот сорок семь миллиардов сорок семь миллионов"
    " двести восемьдесят четыре тысячи двести четыре";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -732847047284204LL;
  resulStr =
    "minus seven hundred and thirty-two trillion eight hundred and forty-seven billion"
    " and forty-seven million two hundred and eighty-four thousand two hundred and four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 732847047284204LL;
  resulStr =
    "seven hundred thirty-two trillion eight hundred forty-seven billion forty-seven million"
    " two hundred eighty-four thousand two hundred four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -26748LL;
  resulStr = "минус двадцать шесть тысяч семьсот сорок восемь";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 26748LL;
  resulStr = "twenty-six thousand seven hundred forty-eight";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -26748LL;
  resulStr = "minus twenty-six thousand seven hundred and forty-eight";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 10928374655466LL;
  resulStr =
    "десять триллионов девятьсот двадцать восемь миллиардов триста семьдесят четыре миллиона"
    " шестьсот пятьдесят пять тысяч четыреста шестьдесят шесть";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -10928374655466LL;
  resulStr =
    "negative ten trillion nine hundred twenty-eight billion three hundred seventy-four"
    " million six hundred fifty-five thousand four hundred sixty-six";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -10928374655466LL;
  resulStr =
    "minus ten trillion nine hundred and twenty-eight billion three hundred and"
    " seventy-four million six hundred and fifty-five thousand four hundred and sixty-six";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 102030405LL;
  resulStr = "one hundred and two million and thirty thousand four hundred and five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -102030405LL;
  resulStr =
    "negative one hundred two million"
    " thirty thousand four hundred five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 192939495LL;
  resulStr =
    "one hundred ninety-two million nine hundred"
    " thirty-nine thousand four hundred ninety-five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -192939495LL;
  resulStr =
    "minus one hundred and ninety-two million nine hundred and"
    " thirty-nine thousand four hundred and ninety-five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 903204705LL;
  resulStr = "nine hundred and three million two hundred and four thousand seven hundred and five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -903204705LL;
  resulStr = "negative nine hundred three million two hundred four thousand seven hundred five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 102LL;
  resulStr = "one hundred two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -102LL;
  resulStr = "minus one hundred and two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 2001LL;
  resulStr = "two thousand and one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -2001LL;
  resulStr = "negative two thousand one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = 2010LL;
  resulStr = "two thousand and ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  str__01_.clear();
  num = -2010LL;
  resulStr = "negative two thousand ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));

  //// Test specific

  str__01_.clear();
  num = 1001LL;
  resulStr = "one thousand one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1610LL;
  resulStr = "sixteen hundred ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1103LL;
  resulStr = "eleven hundred three";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 4042LL;
  resulStr = "four thousand forty-two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1225LL;
  resulStr = "twelve hundred twenty-five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 9999LL;
  resulStr = "ninety-nine hundred ninety-nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1000LL;
  resulStr = "ten hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1500LL;
  resulStr = "fifteen hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 2000LL;
  resulStr = "twenty hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::intNumToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resulStr));
  localeSettings.verySpecific = false;
}

} // namespace END

#endif // ConvertionUtilsTestsH