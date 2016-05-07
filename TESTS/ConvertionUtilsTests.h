#ifndef ConvertionUtilsTestsH
#define ConvertionUtilsTestsH

//// [!] Version 1.111 [!]

#include "ConvertionUtils.h"

#include <string>

#ifdef _MSC_VER // MS VS
  #define STRCMPI _stricmp // ISO C++ conformant
#elif __GNUC__ // GCC
  #include <strings.h>
  #define STRCMPI strcasecmp // int strcasecmp(const char *s1, const char *s2)
#endif

// Functional tests
namespace ConvertionUtilsTests {

//// 'TStrType' SHOULD have a default constructor,
//// SHOULD support 'clear' method AND operator '+='
//// [!] MS VS 'long double' type precision: 15~16 digits (+1 for rounding):
////  http://www.codeproject.com/Questions/1086351/MS-VS-LDBL-DIG-const-From-float-h-is-incorrect [!]

template <class TStrType = std::string>
void test() throw() {
  auto result = false;
  auto num = 0.0L;
  auto errMsg = "", resultStr = "";
  ConvertionUtils::LocaleSettings localeSettings;
  localeSettings.shortFormat = true;
  localeSettings.foldFraction = true;
  TStrType str__01_;

  /* To replace ALL occurrences like 'num = 0;' -> 'num = 0LL' used the following regexp.:
     '(?<1>num = [^L;]*);' -> '$1.0;'
  */
  //// 0
  str__01_.clear();
  num = 0.0L;
  resultStr = "zero";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 0.0L;
  resultStr = "nought";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 0.0L;
  resultStr = "ноль";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// 9
  str__01_.clear();
  num = 9.0L;
  resultStr = "nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 9.0L;
  resultStr = "nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 9.0L;
  resultStr = "девять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// 4
  str__01_.clear();
  num = 4.0L;
  resultStr = "four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 4.0L;
  resultStr = "four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 4.0L;
  resultStr = "четыре";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// 10
  str__01_.clear();
  num = 10.0L;
  resultStr = "ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 10.0L;
  resultStr = "ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 10.0L;
  resultStr = "десять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// -13
  str__01_.clear();
  num = -13.0L;
  resultStr = "negative thirteen";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -13.0L;
  resultStr = "minus thirteen";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -13.0L;
  resultStr = "минус тринадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// -19
  str__01_.clear();
  num = -19.0L;
  resultStr = "negative nineteen";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -19.0L;
  resultStr = "minus nineteen";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -19.0L;
  resultStr = "минус девятнадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// -20
  str__01_.clear();
  num = -20.0L;
  resultStr = "negative twenty";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -20.0L;
  resultStr = "minus twenty";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -20.0L;
  resultStr = "минус двадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// 20
  str__01_.clear();
  num = 20.0L;
  resultStr = "twenty";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 20.0L;
  resultStr = "twenty";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 20.0L;
  resultStr = "двадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// 90
  str__01_.clear();
  num = 90.0L;
  resultStr = "ninety";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 90.0L;
  resultStr = "ninety";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 90.0L;
  resultStr = "девяносто";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// 21
  str__01_.clear();
  num = 21.0L;
  resultStr = "twenty-one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 21.0L;
  resultStr = "twenty-one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 21.0L;
  resultStr = "двадцать один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 32.0L;
  resultStr = "thirty-two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -79.0L;
  resultStr = "minus seventy-nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -99.0L;
  resultStr = "минус девяносто девять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 100.0L;
  resultStr = "one hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -900.0L;
  resultStr = "minus nine hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 2000.0L;
  resultStr = "two thousand";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -85000.0L;
  resultStr = "minus eighty-five thousand";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 999000.0L;
  resultStr = "nine hundred and ninety-nine thousand";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -999000.0L;
  resultStr = "negative nine hundred ninety-nine thousand";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1000000.0L;
  resultStr = "one million";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -10000000.0L;
  resultStr = "minus ten million";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -334.0L;
  resultStr = "minus three hundred and thirty-four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 208.0L;
  resultStr = "two hundred and eight";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1000000000.0L;
  resultStr = "one billion";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1000000000000.0L;
  resultStr = "one trillion";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1000000000000000000.0L;
  resultStr = "one quintillion";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 250000000.0L;
  resultStr = "two hundred fifty million";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1.0L;
  resultStr = "один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 2.0L;
  resultStr = "два";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 3.0L;
  resultStr = "три";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 18.0L;
  resultStr = "восемнадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 80.0L;
  resultStr = "восемьдесят";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 800.0L;
  resultStr = "восемьсот";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -88888.0L;
  resultStr = "минус восемьдесят восемь тысяч восемьсот восемьдесят восемь";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 39.0L;
  resultStr = "тридцать девять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -952.0L;
  resultStr = "минус девятьсот пятьдесят два";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 732847047284204.0L;
  resultStr =
    "семьсот тридцать два триллиона восемьсот сорок семь миллиардов сорок семь миллионов"
    " двести восемьдесят четыре тысячи двести четыре";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -732847047284204.0L;
  resultStr =
    "minus seven hundred and thirty-two trillion eight hundred and forty-seven billion and forty-seven"
    " million two hundred and eighty-four thousand two hundred and four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 732847047284204.0L;
  resultStr =
    "seven hundred thirty-two trillion eight hundred forty-seven billion forty-seven million"
    " two hundred eighty-four thousand two hundred four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -26748.0L;
  resultStr = "минус двадцать шесть тысяч семьсот сорок восемь";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 26748.0L;
  resultStr = "twenty-six thousand seven hundred forty-eight";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -26748.0L;
  resultStr = "minus twenty-six thousand seven hundred and forty-eight";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 10928374655466.0L;
  resultStr =
    "десять триллионов девятьсот двадцать восемь миллиардов триста семьдесят четыре миллиона"
    " шестьсот пятьдесят пять тысяч четыреста шестьдесят шесть";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -10928374655466.0L;
  resultStr =
    "negative ten trillion nine hundred twenty-eight billion three hundred seventy-four"
    " million six hundred fifty-five thousand four hundred sixty-six";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -10928374655466.0L;
  resultStr =
    "minus ten trillion nine hundred and twenty-eight billion three hundred and"
    " seventy-four million six hundred and fifty-five thousand four hundred and sixty-six";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 102030405.0L;
  resultStr = "one hundred and two million and thirty thousand four hundred and five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -102030405.0L;
  resultStr =
    "negative one hundred two million"
    " thirty thousand four hundred five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 192939495.0L;
  resultStr =
    "one hundred ninety-two million nine hundred"
    " thirty-nine thousand four hundred ninety-five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -192939495.0L;
  resultStr =
    "minus one hundred and ninety-two million nine hundred and"
    " thirty-nine thousand four hundred and ninety-five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 903204705.0L;
  resultStr = "nine hundred and three million two hundred and four thousand seven hundred and five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -903204705.0L;
  resultStr = "negative nine hundred three million two hundred four thousand seven hundred five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -100305006.0L;
  resultStr = "минус сто миллионов триста пять тысяч шесть";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 102.0L;
  resultStr = "one hundred two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -102.0L;
  resultStr = "minus one hundred and two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 2001.0L;
  resultStr = "two thousand and one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -2001.0L;
  resultStr = "negative two thousand one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 2010.0L;
  resultStr = "two thousand and ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -2010.0L;
  resultStr = "negative two thousand ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// Test specific

  str__01_.clear();
  num = 1001.0L;
  resultStr = "one thousand one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1610.0L;
  resultStr = "sixteen hundred ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1103.0L;
  resultStr = "eleven hundred three";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 4042.0L;
  resultStr = "four thousand forty-two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1225.0L;
  resultStr = "twelve hundred twenty-five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 9999.0L;
  resultStr = "ninety-nine hundred ninety-nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1000.0L;
  resultStr = "ten hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 1500.0L;
  resultStr = "fifteen hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 2000.0L;
  resultStr = "twenty hundred";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  localeSettings.verySpecific = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;
}

template <class TStrType = std::string>
void testFractions() throw() {
  auto result = false;
  auto num = 0.0L;
  auto errMsg = "", resultStr = "";
  ConvertionUtils::LocaleSettings localeSettings; // default settings
  localeSettings.foldFraction = true;
  TStrType str__01_;
  // -122000.5476
  str__01_.clear();
  num = -122000.5476L;
  resultStr = "negative one hundred twenty-two thousand point five four seven six";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -122000.5476L;
  resultStr = "minus one hundred and twenty-two thousand point five four seven six";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -122000.5476L;
  resultStr = "минус сто двадцать две тысячи целых пять тысяч четыреста семьдесят шесть десятитысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  //
  str__01_.clear();
  num = 13000.010203L;
  resultStr = "тринадцать тысяч целых десять тысяч двести три миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  // 73.09080706050403
  str__01_.clear();
  num = 73.09080706050403L;
  resultStr = "семьдесят три целых девять триллионов восемьдесят миллиардов семьсот шесть миллионов "
              "пятьдесят тысяч четыреста три стотриллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 73.09080706050403L;
  resultStr = "seventy-three point nought nine nought eight nought seven nought six nought five nought four nought three";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  // -539.0123456789
  str__01_.clear();
  num = -539.0123456789L;
  resultStr = "минус пятьсот тридцать девять целых сто двадцать три миллиона четыреста "
              "пятьдесят шесть тысяч семьсот восемьдесят девять десятимиллиардных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -539.0123456789L;
  resultStr = "negative five hundred thirty-nine point zero one two three four five six seven eight nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  //
  str__01_.clear();
  num = 1930.34645L;
  resultStr = "одна тысяча девятьсот тридцать целых тридцать четыре тысячи шестьсот сорок пять стотысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  // 9522.0
  str__01_.clear();
  num = 9522.0L;
  resultStr = "nine thousand five hundred twenty-two point zero";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 9522.0L;
  resultStr = "nine thousand five hundred and twenty-two point nought";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 9522.0L;
  resultStr = "ninety-five hundred twenty-two point o";
  errMsg = "";
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;

  str__01_.clear();
  num = 9522.0L;
  resultStr = "девять тысяч пятьсот двадцать две целых ноль десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 12406.006L;
  resultStr = "двенадцать тысяч четыреста шесть целых шесть тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -78125.000000009L; // -78125.000000008993
  resultStr = "minus seventy-eight thousand one hundred and twenty-five point "
              "nought nought nought nought nought nought nought nought eight nine nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 129862.0L;
  resultStr = "сто двадцать девять тысяч восемьсот шестьдесят два";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.shortFormat = false;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 129862.0L;
  resultStr = "сто двадцать девять тысяч восемьсот шестьдесят две целых ноль десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 0.000000000001L;
  resultStr = "одна триллионная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.shortFormat = false;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -620928.0010003005L;
  resultStr = "minus six hundred and twenty thousand nine hundred and twenty-eight point "
              "nought nought one nought nought nought three nought nought five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 583950487.1030401L;
  resultStr = "five hundred eighty-three million nine hundred fifty thousand four hundred "
              "eighty-seven point one zero three zero four zero one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -355411129.98761L;
  resultStr = "минус триста пятьдесят пять миллионов четыреста одиннадцать тысяч сто двадцать "
              "девять целых девяносто восемь тысяч семьсот шестьдесят одна стотысячная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 8004090871.0007L;
  resultStr = "eight billion and four million and ninety thousand eight hundred and "
              "seventy-one point nought nought nought seven";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 14360122008.09L;
  resultStr = "fourteen billion three hundred sixty million one hundred "
              "twenty-two thousand eight point zero nine";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -198980062333.5357L;
  resultStr = "минус сто девяносто восемь миллиардов девятьсот восемьдесят миллионов шестьдесят "
              "две тысячи триста тридцать три целых пять тысяч триста пятьдесят семь десятитысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1123980191425.0L;
  resultStr = "one trillion one hundred and twenty-three billion nine hundred and eighty "
              "million one hundred and ninety-one thousand four hundred and twenty-five "
              "point nought";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.000005L;
  resultStr = "nought point nought nought nought nought nought five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  localeSettings.shortFormat = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -15113888730099.91L;
  resultStr = "negative fifteen trillion one hundred thirteen billion eight hundred eighty-eight "
              "million seven hundred thirty thousand ninety-nine point nine one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.8L;
  resultStr = "одна целая восемь десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -19.021L;
  resultStr = "minus nineteen point nought two one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  // 0.000000000000000001
  str__01_.clear();
  num = 0.000000000000000001L;
  resultStr = "ноль целых одна квинтиллионная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 0.000000000000000001L;
  resultStr = "nought point nought nought nought nought nought nought nought "
              "nought nought nought nought nought "
              "nought nought nought nought nought one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  //
  str__01_.clear();
  num = -999000.0127908L;
  resultStr = "negative nine hundred ninety-nine thousand point zero one two seven nine zero eight";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 334.057603005L;
  resultStr = "триста тридцать четыре целых пятьдесят семь миллионов шестьсот три тысячи пять миллиардных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -0.2500000543002L;
  resultStr = "minus nought point two five nought nought nought nought nought five four three nought nought two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 212.4560053053041L;
  resultStr = "two hundred twelve point four five six zero zero five three zero five "
              "three zero four one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -27.0891230032109L;
  resultStr = "минус двадцать семь целых восемьсот девяносто один миллиард двести тридцать "
              "миллионов тридцать две тысячи сто девять десятитриллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = -0.54354354L;
  resultStr = "минус ноль целых пятьдесят четыре миллиона триста пятьдесят"
              " четыре тысячи триста пятьдесят четыре стомиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  auto wasPrecison = localeSettings.precison;
  localeSettings.precison = size_t(8U);
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.precison = wasPrecison; // restore
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -0.700080000907L;
  resultStr = "negative zero point seven zero zero zero eight zero zero zero zero nine zero seven";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.000005L;
  resultStr = "пять миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.shortFormat = false;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.1L;
  resultStr = "ноль целых одна десятая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.02L;
  resultStr = "ноль целых две сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.023L;
  resultStr = "ноль целых двадцать три тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0044L;
  resultStr = "ноль целых сорок четыре десятитысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.foldFraction = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.foldFraction = true;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.5678L;
  resultStr = "ноль целых пять тысяч шестьсот семьдесят восемь десятитысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 10.7L;
  resultStr = "десять целых семь десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.67L;
  resultStr = "ноль целых шестьдесят семь сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.75L;
  resultStr = "семьдесят пять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.shortFormat = false;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.12L;
  resultStr = "одна целая двенадцать сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.632L;
  resultStr = "ноль целых шестьсот тридцать две тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 3.18L;
  resultStr = "три целых восемнадцать сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.6L;
  resultStr = "одна целая шесть десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 22.49L;
  resultStr = "двадцать две целых сорок девять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.597L;
  resultStr = "ноль целых пятьсот девяносто семь тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 3.85L;
  resultStr = "три целых восемьдесят пять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 2.7L;
  resultStr = "две целых семь десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 11.4L;
  resultStr = "одиннадцать целых четыре десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 401.1L;
  resultStr = "четыреста одна целая одна десятая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 667.8L;
  resultStr = "шестьсот шестьдесят семь целых восемь десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.8L;
  resultStr = "ноль целых восемь десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 9.9L;
  resultStr = "девять целых девять десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 99.9L;
  resultStr = "девяносто девять целых девять десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  wasPrecison = localeSettings.precison;
  localeSettings.precison = size_t(6U);
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.precison = wasPrecison;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 909.9L;
  resultStr = "девятьсот девять целых девять десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 5.64L;
  resultStr = "пять целых шестьдесят четыре сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 21.87L;
  resultStr = "двадцать одна целая восемьдесят семь сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 381.77L;
  resultStr = "триста восемьдесят одна целая семьдесят семь сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.foldFraction = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.foldFraction = true;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 54.6L;
  resultStr = "пятьдесят четыре целых шесть десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 2.81L;
  resultStr = "две целых восемьдесят одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.55L;
  resultStr = "ноль целых пятьдесят пять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.foldFraction = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.foldFraction = true;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.09L;
  resultStr = "ноль целых девять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.77L;
  resultStr = "ноль целых семьдесят семь сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.foldFraction = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.foldFraction = true;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.579L;
  resultStr = "одна целая пятьсот семьдесят девять тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 12.882L;
  resultStr = "двенадцать целых восемьсот восемьдесят две тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 326.703L;
  resultStr = "триста двадцать шесть целых семьсот три тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 145.008L;
  resultStr = "сто сорок пять целых восемь тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 21.094L;
  resultStr = "двадцать одна целая девяносто четыре тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.049L;
  resultStr = "ноль целых сорок девять тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.001L;
  resultStr = "ноль целых одна тысячная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 203.6L;
  resultStr = "двести три целых шесть десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 20.36L;
  resultStr = "двадцать целых тридцать шесть сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.02036L;
  resultStr = "ноль целых две тысячи тридцать шесть стотысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.20506L;
  resultStr = "ноль целых двадцать тысяч пятьсот шесть стотысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.010101L;
  resultStr = "ноль целых десять тысяч сто одна миллионная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.foldFraction = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.foldFraction = true;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 43.7569L;
  resultStr = "сорок три целых семь тысяч пятьсот шестьдесят девять десятитысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.2L;
  resultStr = "ноль целых две десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.0L;
  resultStr = "один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.shortFormat = false;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.0L;
  resultStr = "одна целая ноль десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 2.01L;
  resultStr = "две целых одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 4.991L;
  resultStr = "четыре целых девятьсот девяносто одна тысячная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 5.1123L;
  resultStr = "пять целых одна тысяча сто двадцать три десятитысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 10.12345L;
  resultStr = "десять целых двенадцать тысяч триста сорок пять стотысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 11.213451L;
  resultStr = "одиннадцать целых двести тринадцать тысяч четыреста пятьдесят одна миллионная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 12.43456789L;
  resultStr = "двенадцать целых сорок три миллиона четыреста пятьдесят шесть"
              " тысяч семьсот восемьдесят девять стомиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.2999999999L;
  resultStr = "одна целая два миллиарда девятьсот девяносто девять миллионов "
              "девятьсот девяносто девять тысяч девятьсот девяносто девять десятимиллиардных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.1234567891011L;
  resultStr = "ноль целых один триллион двести тридцать четыре миллиарда пятьсот"
              " шестьдесят семь миллионов восемьсот девяносто одна тысяча одиннадцать десятитриллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 121.0L;
  resultStr = "сто двадцать один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.shortFormat = false;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 121.22L;
  resultStr = "сто двадцать одна целая двадцать две сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.foldFraction = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.foldFraction = true;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1000.16L;
  resultStr = "одна тысяча целых шестнадцать сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1002.51L;
  resultStr = "одна тысяча две целых пятьдесят одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 120101.1L;
  resultStr = "сто двадцать тысяч сто одна целая одна десятая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 2000000.0L;
  resultStr = "два миллиона";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.shortFormat = false;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 11102345.23L;
  resultStr = "одиннадцать миллионов сто две тысячи триста сорок пять целых двадцать три сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 123456789.321234L;
  resultStr = "сто двадцать три миллиона четыреста пятьдесят шесть тысяч семьсот восемьдесят "
              "девять целых триста двадцать одна тысяча двести тридцать четыре миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 882077.5144L;
  resultStr = "восемьсот восемьдесят две тысячи семьдесят семь целых пять тысяч сто сорок четыре десятитысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 424065.5382252L;
  resultStr = "четыреста двадцать четыре тысячи шестьдесят пять целых пять "
              "миллионов триста восемьдесят две тысячи двести пятьдесят две десятимиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 930658.14L;
  resultStr = "девятьсот тридцать тысяч шестьсот пятьдесят восемь целых четырнадцать сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 876693.81506L;
  resultStr = "восемьсот семьдесят шесть тысяч шестьсот девяносто три целых "
              "восемьдесят одна тысяча пятьсот шесть стотысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  wasPrecison = localeSettings.precison;
  localeSettings.precison = size_t(12U);
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.precison = wasPrecison;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 54.693L;
  resultStr = "fifty-four point six nine three";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 14.65L;
  resultStr = "fourteen point six five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 63.57L;
  resultStr = "sixty-three point five seven";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 6.785L;
  resultStr = "six point seven eight five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 9.2416L;
  resultStr = "nine point two four one six";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 2.95L;
  resultStr = "two point nine five";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 3.04L;
  resultStr = "three point nought four";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 2.02L;
  resultStr = "two point nought two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 5.06L;
  resultStr = "five point nought six";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 6.92L;
  resultStr = "six point nine two";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  // 14000000
  str__01_.clear();
  num = 14000000L;
  resultStr = "fourteen million";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 14000000L;
  resultStr = "четырнадцать миллионов";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  //
  str__01_.clear();
  num = 433964.9L;
  resultStr = "четыреста тридцать три тысячи девятьсот шестьдесят четыре целых девять десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0L;
  resultStr = "nought";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 0.0L;
  resultStr = "nought point nought";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  localeSettings.shortFormat = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0L;
  resultStr = "zero";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0L;
  resultStr = "o";
  errMsg = "";
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 0.0L;
  resultStr = "o";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.verySpecific = false;
  // 0.01 + '!shortFormat'
  str__01_.clear();
  num = 0.01L;
  resultStr = "zero point zero one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  localeSettings.shortFormat = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 0.01L;
  resultStr = "nought point nought one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.01L;
  resultStr = "ноль целых одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  // 0.01 + 'shortFormat'
  str__01_.clear();
  num = 0.01L;
  resultStr = "point zero one";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 0.01L;
  resultStr = "point nought one";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 0.01L;
  resultStr = "одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.shortFormat = false;
  // 10 + 'shortFormat'
  str__01_.clear();
  num = 10.0L;
  resultStr = "ten";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 10.0L;
  resultStr = "ten";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 10.0L;
  resultStr = "десять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.shortFormat = false;
  
  str__01_.clear();
  num = 0.2L;
  resultStr = "nought point two";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.2L;
  resultStr = "point two";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.shortFormat = false;
  
  str__01_.clear();
  num = 0.7L;
  resultStr = "point seven";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  localeSettings.shortFormat = false;
  
  str__01_.clear();
  num = 0.7L;
  resultStr = "nought point seven";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.7L;
  resultStr = "zero point seven";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.7L;
  resultStr = "o point seven";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.7L;
  resultStr = "point seven";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.003L;
  resultStr = "point nought nought three";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.2L;
  resultStr = "nought point two";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.2L;
  resultStr = "point two";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.00002L;
  resultStr = "ноль целых две стотысячных";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0000002L;
  resultStr = "две десятимиллионных";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 32107.0L;
  resultStr = "тридцать две тысячи сто семь";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 32107.0L;
  resultStr = "тридцать две тысячи сто семь целых ноль десятых";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.5L;
  resultStr = "point five";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.25L;
  resultStr = "point two five";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.73L;
  resultStr = "point seven three";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.05L;
  resultStr = "point zero five";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.6529L;
  resultStr = "point six five two nine";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.03L;
  resultStr = "nought point nought three";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.03L;
  resultStr = "point nought three";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.001L;
  resultStr = "point nought nought one";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.02L;
  resultStr = "one point nought two";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.03L;
  resultStr = "nought point nought three";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.03L;
  resultStr = "zero point zero three";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.03L;
  resultStr = "point nought three";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.43L;
  resultStr = "nought point four three";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.02L;
  resultStr = "one point nought two";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.02L;
  resultStr = "one point o two";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.067L;
  resultStr = "nought point nought six seven";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.067L;
  resultStr = "zero point zero six seven";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1200.004321L;
  resultStr = "twelve hundred point o o four three two one";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.000000001234L;
  resultStr = "ноль целых одна тысяча двести тридцать четыре триллионных";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.000000001234L;
  resultStr = "one point zero zero zero zero zero zero zero zero one two three four";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0001234L;
  resultStr = "point zero zero zero one two three four";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.0001234L;
  resultStr = "одна целая одна тысяча двести тридцать четыре десятимиллионных";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0001L;
  resultStr = "point o o o one";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 22204.4604925L;
  resultStr = "twenty-two thousand two hundred four point four six zero four nine two five";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 2.22044604925L;
  resultStr = "two point two two nought four four six nought four nine two five";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 123456L;
  resultStr = "сто двадцать три тысячи четыреста пятьдесят шесть";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 2220446.04925L;
  resultStr = "two million two hundred twenty thousand four hundred forty-six point zero four nine two five";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 9L;
  resultStr = "nine";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 22e-3L;
  resultStr = "двадцать две тысячных";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.foldFraction = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.foldFraction = true;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 21e-3L;
  resultStr = "двадцать одна тысячная";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 123e30L;
  resultStr = "сто двадцать три нониллиона";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 321e-30L;
  resultStr = "триста двадцать одна нониллионная";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1.1L;
  resultStr = "one point one";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 12.12L;
  resultStr = "двенадцать целых двенадцать сотых";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 123.123L;
  resultStr = "one hundred twenty-three point one two three";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0L;
  resultStr = "zero";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0L;
  resultStr = "zero point zero";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1000000287600.0L;
  resultStr = "один триллион двести восемьдесят семь тысяч шестьсот целых ноль десятых";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 12.1e-3L;
  resultStr = "point zero one two one";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.0021L;
  resultStr = "одна целая двадцать одна десятитысячная";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1239.0e10L;
  resultStr = "twelve trillion three hundred and ninety billion";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 123.9e0L;
  resultStr = "one hundred and twenty-three point nine";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1.21e2L;
  resultStr = "one hundred twenty-one";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 123.456L;
  resultStr = "сто двадцать три целых четыреста пятьдесят шесть тысячных";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.0000123e-8L;
  resultStr = "o point o o o o o o o o o o o o one two three";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1e30L;
  resultStr = "one nonillion";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1e-30L;
  resultStr = "одна нониллионная";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 12.1e-1L;
  resultStr = "одна целая двадцать одна сотая";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1.21e1L;
  resultStr = "twelve point one";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 186942862939.192e0L;
  resultStr = "сто восемьдесят шесть миллиардов девятьсот сорок два миллиона восемьсот шестьдесят две тысячи "
              "девятьсот тридцать девять целых сто девяносто две тысячных";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.192416287965e0L;
  resultStr = "сто девяносто два миллиарда четыреста шестнадцать миллионов двести восемьдесят семь тысяч "
              "девятьсот шестьдесят пять триллионных";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.1188916446365e0L;
  resultStr = "один триллион сто восемьдесят восемь миллиардов девятьсот шестнадцать миллионов четыреста сорок шесть тысяч"
              " триста шестьдесят пять десятитриллионных";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 1869428629.391L;
  resultStr = "один миллиард восемьсот шестьдесят девять миллионов четыреста двадцать восемь тысяч"
              " шестьсот двадцать девять целых триста девяносто одна тысячная";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.391618e0L;
  resultStr = "триста девяносто одна тысяча шестьсот восемнадцать миллионных";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1.12345671e0L;
  resultStr = "одна целая двенадцать миллионов триста сорок пять тысяч шестьсот семьдесят одна стомиллионная";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// Test too high / low values

  str__01_.clear();
  num = 1.0e200L;
  resultStr = "";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(!result);
  assert(errMsg && *errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1.0e-200L;
  resultStr = "";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(!result);
  assert(errMsg && *errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  //// Test actual folding + positive sign

  localeSettings.positiveSign = true;
  localeSettings.foldFraction = true;

  str__01_.clear();
  num = 1.3333333L;
  resultStr = "plus one point three repeating";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = -2.678678L;
  resultStr = "минус две целых и шестьсот семьдесят восемь в периоде";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.999999999L;
  resultStr = "плюс ноль целых и девять в периоде";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
  
  str__01_.clear();
  num = 0.45454545L;
  resultStr = "плюс ноль целых и сорок пять в периоде";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 1.181818181818L;
  resultStr = "плюс одна целая и восемнадцать в периоде";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  localeSettings.foldFraction = false;
  localeSettings.positiveSign = false;

  str__01_.clear();
  num = 422.28L;
  resultStr = "four hundred and twenty-two point two eight";
  errMsg = "";
  localeSettings.shortFormat = true;
  localeSettings.verySpecific = false;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_GB;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_.clear();
  num = 422.29L;
  resultStr = "four hundred twenty-two point two nine";
  errMsg = "";
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = true;
  localeSettings.locale = ConvertionUtils::ELocale::L_EN_US;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
}

template <class TStrType = std::string>
void testRusNumbers() throw() {
  auto result = false;
  auto num = 0.0L;
  auto errMsg = "", resultStr = "";
  ConvertionUtils::LocaleSettings localeSettings; // default settings
  localeSettings.foldFraction = true;
  localeSettings.shortFormat = false;
  localeSettings.verySpecific = true;
  TStrType str__01_;

  str__01_ = "number:";
  num = 1215.3405354L;
  resultStr = "number: одна тысяча двести пятнадцать целых три миллиона четыреста пять тысяч"
              " триста пятьдесят четыре десятимиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1984.6594646L;
  resultStr = "number: одна тысяча девятьсот восемьдесят четыре целых шесть миллионов пятьсот"
              " девяносто четыре тысячи шестьсот сорок шесть десятимиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1976.0404423L;
  resultStr = "number: одна тысяча девятьсот семьдесят шесть целых четыреста четыре тысячи"
              " четыреста двадцать три десятимиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1223.9595577L;
  resultStr = "number: одна тысяча двести двадцать три целых девять миллионов пятьсот"
              " девяносто пять тысяч пятьсот семьдесят семь десятимиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 31187.4417319L;
  resultStr = "number: тридцать одна тысяча сто восемьдесят семь целых четыре миллиона"
              " четыреста семнадцать тысяч триста девятнадцать десятимиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 8812.5582681L;
  resultStr = "number: восемь тысяч восемьсот двенадцать целых пять миллионов пятьсот"
              " восемьдесят две тысячи шестьсот восемьдесят одна десятимиллионная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = -14467.9375518L;
  resultStr = "number: минус четырнадцать тысяч четыреста шестьдесят семь целых девять миллионов"
              " триста семьдесят пять тысяч пятьсот восемнадцать десятимиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 25532.0624482L;
  resultStr = "number: двадцать пять тысяч пятьсот тридцать две целых шестьсот двадцать"
              " четыре тысячи четыреста восемьдесят две десятимиллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.00003L;
  resultStr = "number: ноль целых три стотысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = -0.000975L;
  resultStr = "number: минус ноль целых девятьсот семьдесят пять миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.001125L;
  resultStr = "number: ноль целых одна тысяча сто двадцать пять миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.001275L;
  resultStr = "number: ноль целых одна тысяча двести семьдесят пять миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 89.0L;
  resultStr = "number: восемьдесят девять целых ноль десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.0015L;
  resultStr = "number: ноль целых пятнадцать десятитысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.001395L;
  resultStr = "number: ноль целых одна тысяча триста девяносто пять миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 38798.027L;
  resultStr = "number: тридцать восемь тысяч семьсот девяносто восемь целых двадцать семь тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  localeSettings.foldFraction = false;

  str__01_ = "number:";
  num = 115377.99L;
  resultStr = "number: сто пятнадцать тысяч триста семьдесят семь целых девяносто девять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  localeSettings.shortFormat = true;

  str__01_ = "number:";
  num = 1700.0L;
  resultStr = "number: одна тысяча семьсот";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 11600.0L;
  resultStr = "number: одиннадцать тысяч шестьсот";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1400.0L;
  resultStr = "number: одна тысяча четыреста";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 2000000.0L;
  resultStr = "number: два миллиона";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1000.0L;
  resultStr = "number: одна тысяча";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 2000000000.0L;
  resultStr = "number: два миллиарда";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 23500000000.0L;
  resultStr = "number: двадцать три миллиарда пятьсот миллионов";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 20456.09L;
  resultStr = "number: двадцать тысяч четыреста пятьдесят шесть целых девять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 3652.93L;
  resultStr = "number: три тысячи шестьсот пятьдесят две целых девяносто три сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 81.81L;
  resultStr = "number: восемьдесят одна целая восемьдесят одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 14408.81L;
  resultStr = "number: четырнадцать тысяч четыреста восемь целых восемьдесят одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 17145.81L;
  resultStr = "number: семнадцать тысяч сто сорок пять целых восемьдесят одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 111.29L;
  resultStr = "number: сто одиннадцать целых двадцать девять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1423.68L;
  resultStr = "number: одна тысяча четыреста двадцать три целых шестьдесят восемь сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 23500000000.0L;
  resultStr = "number: двадцать три миллиарда пятьсот миллионов";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 100.0L;
  resultStr = "number: сто";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 164182200.0L;
  resultStr = "number: сто шестьдесят четыре миллиона сто восемьдесят две тысячи двести";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1641822.0L;
  resultStr = "number: один миллион шестьсот сорок одна тысяча восемьсот двадцать два";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 200000001.0L;
  resultStr = "number: двести миллионов один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1451401033.0L;
  resultStr = "number: один миллиард четыреста пятьдесят один миллион четыреста одна тысяча тридцать три";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.01L;
  resultStr = "number: ноль целых одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.shortFormat = true;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 27352942.0L;
  resultStr = "number: двадцать семь миллионов триста пятьдесят две тысячи девятьсот сорок два";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 27352942.0L;
  resultStr = "number: двадцать семь миллионов триста пятьдесят две тысячи девятьсот сорок два";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 500000000.0L;
  resultStr = "number: пятьсот миллионов";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 14299060.61L;
  resultStr = "number: четырнадцать миллионов двести девяносто девять тысяч шестьдесят целых шестьдесят одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1429906060.0L;
  resultStr = "number: один миллиард четыреста двадцать девять миллионов девятьсот шесть тысяч шестьдесят";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 396562485.81L;
  resultStr = "number: триста девяносто шесть миллионов пятьсот шестьдесят две тысячи"
              " четыреста восемьдесят пять целых восемьдесят одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 39656248581.0L;
  resultStr = "number: тридцать девять миллиардов шестьсот пятьдесят шесть"
              " миллионов двести сорок восемь тысяч пятьсот восемьдесят один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 2005380419.0L;
  resultStr = "number: два миллиарда пять миллионов триста восемьдесят тысяч четыреста девятнадцать";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 20053804.19L;
  resultStr = "number: двадцать миллионов пятьдесят три тысячи восемьсот четыре целых девятнадцать сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 7872.81L;
  resultStr = "number: семь тысяч восемьсот семьдесят две целых восемьдесят одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 12516366381.0L;
  resultStr = "number: двенадцать миллиардов пятьсот шестнадцать миллионов"
              " триста шестьдесят шесть тысяч триста восемьдесят один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 120163663.81L;
  resultStr = "number: сто двадцать миллионов сто шестьдесят три"
              " тысячи шестьсот шестьдесят три целых восемьдесят одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.309393L;
  resultStr = "number: ноль целых триста девять тысяч триста девяносто три миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.3L;
  resultStr = "number: три десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  localeSettings.shortFormat = false;
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 5.01L;
  resultStr = "number: пять целых одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 4.015L;
  resultStr = "number: четыре целых пятнадцать тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 150.000003L;
  resultStr = "number: сто пятьдесят целых три миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1.00421L;
  resultStr = "number: одна целая четыреста двадцать одна стотысячная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 7.8L;
  resultStr = "number: семь целых восемь десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 5.45L;
  resultStr = "number: пять целых сорок пять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.007L;
  resultStr = "number: ноль целых семь тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 33.0008L;
  resultStr = "number: тридцать три целых восемь десятитысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.1L;
  resultStr = "number: ноль целых одна десятая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 0.2L;
  resultStr = "number: ноль целых две десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 121.0L;
  resultStr = "number: сто двадцать один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 121000.0L;
  resultStr = "number: сто двадцать одна тысяча";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 121000000.0L;
  resultStr = "number: сто двадцать один миллион";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 122.0L;
  resultStr = "number: сто двадцать два";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 122000.0L;
  resultStr = "number: сто двадцать две тысячи";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 122000000.0L;
  resultStr = "number: сто двадцать два миллиона";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 123.0L;
  resultStr = "number: сто двадцать три";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 123000.0L;
  resultStr = "number: сто двадцать три тысячи";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 123000000.0L;
  resultStr = "number: сто двадцать три миллиона";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 129.0L;
  resultStr = "number: сто двадцать девять";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 129000.0L;
  resultStr = "number: сто двадцать девять тысяч";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 129000000.0L;
  resultStr = "number: сто двадцать девять миллионов";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 111000.0L;
  resultStr = "number: сто одиннадцать тысяч";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 7.034L;
  resultStr = "number: семь целых тридцать четыре тысячных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 8.01L;
  resultStr = "number: восемь целых одна сотая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 3.000003L;
  resultStr = "number: три целых три миллионных";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 42.52L;
  resultStr = "number: сорок две целых пятьдесят две сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 1.05L;
  resultStr = "number: одна целая пять сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 4.601L;
  resultStr = "number: четыре целых шестьсот одна тысячная";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 9521.0L;
  resultStr = "number: девять тысяч пятьсот двадцать один";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = true;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 9521.0L;
  resultStr = "number: девять тысяч пятьсот двадцать одна целая ноль десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  localeSettings.shortFormat = false;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 22.2L;
  resultStr = "number: двадцать две целых две десятых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 9521.1L;
  resultStr = "number: девять тысяч пятьсот двадцать одна целая одна десятая";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));

  str__01_ = "number:";
  num = 22.22L;
  resultStr = "number: двадцать две целых двадцать две сотых";
  errMsg = "";
  localeSettings.locale = ConvertionUtils::ELocale::L_RU_RU;
  result = ConvertionUtils::numToNumFormatStr(num, str__01_, localeSettings, &errMsg);
  assert(result);
  assert(errMsg && !*errMsg);
  assert(!STRCMPI(str__01_.c_str(), resultStr));
}

void testStrToL() throw() {
  auto val = 0L;
  auto result = false;
  const char* errMsg = nullptr;

  result = ConvertionUtils::strToL(val, "", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, nullptr, errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "-9999999999999999999999999999999999", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "9999999999999999999999999999999999", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "-", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "fg659", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "-683.0", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "dcrthctrchd", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "+683.0", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, ".0", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "134.", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "23433353h", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "4w65465654w &", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "4323235 ", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "+8", errMsg);
  assert(result && 8L == val && errMsg && !*errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "-8", errMsg);
  assert(result && -8L == val && errMsg && !*errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "+845353", errMsg);
  assert(result && 845353L == val && errMsg && !*errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "-2112845353", errMsg);
  assert(result && -2112845353L == val && errMsg && !*errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "-0", errMsg);
  assert(result && 0L == val && errMsg && !*errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "+0", errMsg);
  assert(result && 0L == val && errMsg && !*errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "0", errMsg);
  assert(result && 0L == val && errMsg && !*errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "0.0", errMsg);
  assert(!result && !val && errMsg && *errMsg);

  char strBuf[32U] = {};
  sprintf(strBuf, "%Ld", LONG_MIN);
  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, strBuf, errMsg);
  assert(result && LONG_MIN == val && errMsg && !*errMsg);

  sprintf(strBuf, "%Ld", LONG_MAX);
  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, strBuf, errMsg);
  assert(result && LONG_MAX == val && errMsg && !*errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "-006", errMsg);
  assert(result && -6L == val && errMsg && !*errMsg);

  val = decltype(val)(); // reset
  errMsg = nullptr;
  result = ConvertionUtils::strToL(val, "+09", errMsg);
  assert(result && 9L == val && errMsg && !*errMsg);
}

} // namespace END

#endif // ConvertionUtilsTestsH