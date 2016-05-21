#ifndef StaticStrTestsH
#define StaticStrTestsH

//// [!] Version 1.003 [!]

#include "StaticallyBufferedStringLight.h"
#include "PerformanceTester.h"

#include <vector>
#include <deque>
#include <set>
#include <list>
#include <unordered_set>

ADD_STD_HASHER_FOR(StrLight)

namespace StaticStrTests {

// Functor
struct SortTesterHelper {

  size_t operator()(const std::string& str) const throw() {
    vec_dynamic_str_.emplace_back(str);
    vec_static_str_.emplace_back(str);
    return 1U;
  }

  static std::vector<StrLight> vec_static_str_;
  static std::vector<std::string> vec_dynamic_str_;

  static void clean() throw() {
    vec_static_str_.clear();
    vec_static_str_.shrink_to_fit();
    vec_dynamic_str_.clear();
    vec_dynamic_str_.shrink_to_fit();
  }
};

std::vector<StrLight> SortTesterHelper::vec_static_str_;
std::vector<std::string> SortTesterHelper::vec_dynamic_str_;

template <const size_t MaxLen>
struct CStr {

  CStr() throw() {
    clear();
  }

  void clear() throw() {
    *strBuf = '\0';
  }

  void operator+=(const char* const str) throw() {
    strcat(strBuf, str);
  }

  char strBuf[MaxLen + 1U];
};

template <typename T1, typename T2>
struct TestFunctor {

  TestFunctor(T1& obj1, T2& obj2, const size_t mode) throw()
    : obj1(obj1), obj2(obj2), mode(mode) {}

  void operator()() const {
    switch (mode) {
      case 0U: obj1 < obj2; break;
      case 1U: obj1 == obj2; break;
    }
  }

  T1& obj1;
  T2& obj2;
  size_t mode = 0U;
};

template <class TStrType>
auto TestConcat(TStrType& str, const size_t maxLen) throw()
-> decltype(std::chrono::system_clock::now() - std::chrono::system_clock::now()) {
  static const auto MIN_CHAR_ = 'a';
  static const auto MAX_CHAR_ = 'z';
  static_assert(MIN_CHAR_ < MAX_CHAR_, "Invalid chars");

  std::default_random_engine gen;
  std::uniform_int_distribution<int> distr(MIN_CHAR_, MAX_CHAR_);

  char strToAdd[2U] = {0};

  str.clear();
  const auto startTime = std::chrono::high_resolution_clock::now();

  for (size_t charIdx = 0U; charIdx < maxLen; ++charIdx) {
    *strToAdd = distr(gen); // constant complexity
    str += strToAdd; // linear to const complexity
  }
  const auto endTime = std::chrono::high_resolution_clock::now();

  return endTime - startTime;
}

// Comparing chunk should be as a register size
long long int quickCmp(const void* const mem1, const void* const mem2, const size_t memSize) throw() {
  auto mem1reinterpreted = static_cast<const unsigned long int*>(mem1);
  auto mem2reinterpreted = static_cast<const unsigned long int*>(mem2);

  const auto count = memSize / sizeof(*mem1reinterpreted);
  size_t idx = 0U;
  for (; idx < count; ++idx) {
    if (*mem1reinterpreted != *mem2reinterpreted)
      return static_cast<long long int>(*mem2reinterpreted) - *mem1reinterpreted;
    ++mem1reinterpreted, ++mem2reinterpreted;
  }
  return 0LL;
}

// Both functional & performance tests (+a few CRT/STL integration tests)
void testStaticStr() throw() {
  char askUser[8U] = {0};
  std::cout << "\nEnter '1' to perform ALL static str. tests: ";
  std::cin >> askUser;
  const bool runAll = '1' == *askUser && !askUser[1U];
  
  //// Empty strs equality test

  {
    const StaticallyBufferedStringLight<> str_1_, str_2_;
    assert(str_1_ == str_2_ && str_2_ == str_1_);
    assert(!strcmp(str_1_.c_str(), str_2_.c_str()));
    assert(str_1_.hash() == str_2_.hash() && !str_1_.hash());
    assert(str_1_.length() == str_2_.length() && !str_1_.length());

    const StaticallyBufferedStringLight<char, 7U> str_0_;
    const StaticallyBufferedStringLight<char, 31U> str_3_;
    assert(str_0_ == str_3_ && str_3_ == str_0_);
    assert(!strcmp(str_0_.c_str(), str_3_.c_str()));
    assert(str_0_.hash() == str_3_.hash() && !str_0_.hash());
    assert(str_0_.length() == str_3_.length() && !str_0_.length());

    std::string str_4_;
    assert(str_1_ == str_4_);
    assert(!strcmp(str_1_.c_str(), str_4_.c_str()));
    assert(str_1_.length() == str_4_.length());
  }

  //// Tests for 'getFNV1aHash'

  if (4U == sizeof(size_t)) { // if ONLY 32 bit

    // From here: http://isthe.com/chongo/tech/comp/fnv/#alt-FNV-source
    auto fnv_32a_str = [](const char* const str, size_t hval) throw() {
      #define FNV_32_PRIME ((size_t)0x01000193)
      unsigned char* s = (unsigned char *)str;
      while (*s) {
        hval ^= (size_t)*s++;
        hval *= FNV_32_PRIME;
      }
      return hval;
    };

    auto fnv_str = "w7 8a23nx8 m43ct5v w54ywc5yv y4";

    // Uses FNV1_32A_INIT as the 'hval' arg on the first call [offset basis]
    auto fnv_a = fnv_32a_str(fnv_str, (size_t)0x811c9dc5);
    auto fnv_b = MathUtils::getFNV1aHash(fnv_str);
    assert(fnv_a == fnv_b);

    StaticallyBufferedStringLight<> s_str_1_ = "7xtm3g";
    fnv_str = s_str_1_.c_str();
    fnv_a = fnv_32a_str(fnv_str, (size_t)0x811c9dc5);
    fnv_b = s_str_1_.hash();
    assert(fnv_a == fnv_b);

    //// Hash prolongation check

    auto isHashKnown_1_ = false;

    s_str_1_ += " 73x3gt3 g 387bg 378 $@%(U ";
    assert(!s_str_1_.modified() && !s_str_1_.truncated());
    fnv_str = s_str_1_.c_str(); // NOT really needed
    fnv_a = fnv_32a_str(fnv_str, (size_t)0x811c9dc5);
    isHashKnown_1_ = false;
    fnv_b = s_str_1_.getHashIfKnown(isHashKnown_1_);
    assert(fnv_a == fnv_b);

    s_str_1_ += '&';
    assert(!s_str_1_.modified() && !s_str_1_.truncated());
    fnv_str = s_str_1_.c_str(); // NOT really needed
    fnv_a = fnv_32a_str(fnv_str, (size_t)0x811c9dc5);
    isHashKnown_1_ = false;
    fnv_b = s_str_1_.getHashIfKnown(isHashKnown_1_);
    assert(fnv_a == fnv_b);

    auto actuallyAppended_ = size_t();
    s_str_1_.append(1234567890UL, &actuallyAppended_);
    assert(!s_str_1_.modified() && !s_str_1_.truncated() && actuallyAppended_);
    fnv_str = s_str_1_.c_str(); // NOT really needed
    fnv_a = fnv_32a_str(fnv_str, (size_t)0x811c9dc5);
    isHashKnown_1_ = false;
    fnv_b = s_str_1_.getHashIfKnown(isHashKnown_1_);
    assert(fnv_a == fnv_b);

    ////

    s_str_1_.pop_back(); // drop hash
    assert(s_str_1_.modified() && !s_str_1_.truncated());
    auto s_str_2_ = s_str_1_; // 's_str_2_' will have a hash AND SHOULD share it
    assert(!s_str_2_.modified() && !s_str_2_.truncated());
    assert(!s_str_1_.modified() && !s_str_1_.truncated());

    isHashKnown_1_ = false;
    auto hash_1_ = s_str_1_.getHashIfKnown(isHashKnown_1_);
    assert(hash_1_); // SHOULD be konwn - shared from 's_str_2_'
    isHashKnown_1_ = false;
    assert(s_str_2_.getHashIfKnown(isHashKnown_1_) &&
           hash_1_ == s_str_2_.getHashIfKnown(isHashKnown_1_));
    fnv_str = s_str_1_.c_str(); // NOT really needed
    fnv_a = fnv_32a_str(fnv_str, (size_t)0x811c9dc5);
    fnv_b = s_str_1_.hash();
    assert(fnv_a == fnv_b);
    assert(!strcmp(s_str_1_.c_str(), s_str_2_.c_str()));

    ////

    s_str_2_.clear();
    assert(!s_str_2_.modified() && !s_str_2_.truncated());
    s_str_1_.pop_back(); // drop hash
    assert(s_str_1_.modified() && !s_str_1_.truncated());
    s_str_2_ = s_str_1_; // SHOULD get & share hash
    assert(!s_str_2_.modified() && !s_str_2_.truncated());
    assert(!s_str_1_.modified() && !s_str_1_.truncated());

    isHashKnown_1_ = false;
    hash_1_ = s_str_1_.getHashIfKnown(isHashKnown_1_);
    assert(hash_1_); // SHOULD be konwn - shared from 's_str_2_'
    isHashKnown_1_ = false;
    assert(s_str_2_.getHashIfKnown(isHashKnown_1_) &&
           hash_1_ == s_str_2_.getHashIfKnown(isHashKnown_1_));
    fnv_str = s_str_1_.c_str(); // NOT really needed
    fnv_a = fnv_32a_str(fnv_str, (size_t)0x811c9dc5);
    fnv_b = s_str_1_.hash();
    assert(fnv_a == fnv_b);
    assert(!strcmp(s_str_1_.c_str(), s_str_2_.c_str()));
   
    s_str_1_.pop_back(); // drop hash
    assert(s_str_1_.modified() && !s_str_1_.truncated());
    s_str_2_.pop_back();
    assert(s_str_2_.modified() && !s_str_2_.truncated());
    hash_1_ = s_str_1_.hash();
    assert(!s_str_1_.modified() && !s_str_1_.truncated());
    assert(hash_1_);
    fnv_str = s_str_1_.c_str(); // NOT really needed
    fnv_a = fnv_32a_str(fnv_str, (size_t)0x811c9dc5);
    assert(fnv_a == hash_1_);
    assert(s_str_1_ == s_str_2_); // SHOULD hsare hash
    assert(!s_str_1_.modified() && !s_str_1_.truncated());
    assert(!s_str_2_.modified() && !s_str_2_.truncated());
    isHashKnown_1_ = false;
    assert(s_str_2_.getHashIfKnown(isHashKnown_1_) &&
           hash_1_ == s_str_2_.getHashIfKnown(isHashKnown_1_));
    assert(!strcmp(s_str_1_.c_str(), s_str_2_.c_str()));
    
    actuallyAppended_ = decltype(actuallyAppended_)();
    s_str_1_.append(" c98a4tmxth hbr a her %$$& ", 12U, &actuallyAppended_);
    assert(!s_str_1_.modified() && !s_str_1_.truncated() && 12U == actuallyAppended_);
    fnv_str = s_str_1_.c_str(); // NOT really needed
    fnv_a = fnv_32a_str(fnv_str, (size_t)0x811c9dc5);
    isHashKnown_1_ = false;
    assert(s_str_1_.getHashIfKnown(isHashKnown_1_) &&
           fnv_a == s_str_1_.getHashIfKnown(isHashKnown_1_));
    
    s_str_1_ = " 7348cg3ctg 34tg 34gt 4g $%* ";
    isHashKnown_1_ = false;
    assert(!s_str_1_.modified() && !s_str_1_.truncated() &&
           s_str_1_.getHashIfKnown(isHashKnown_1_));
    s_str_2_.clear();
    isHashKnown_1_ = false;
    assert(!s_str_2_.modified() && !s_str_2_.truncated() &&
           !s_str_2_.getHashIfKnown(isHashKnown_1_));
    s_str_2_ = std::move(s_str_1_);
    assert(!s_str_1_.modified() && !s_str_1_.truncated());
    assert(!strcmp(s_str_1_.c_str(), s_str_2_.c_str()));
    isHashKnown_1_ = false;
    assert(s_str_2_.getHashIfKnown(isHashKnown_1_) &&
           s_str_1_.getHashIfKnown(isHashKnown_1_) == s_str_2_.getHashIfKnown(isHashKnown_1_));
    assert(s_str_2_ == s_str_1_);
    assert(s_str_2_.getHashIfKnown(isHashKnown_1_) &&
           s_str_1_.getHashIfKnown(isHashKnown_1_) == s_str_2_.getHashIfKnown(isHashKnown_1_));
    
    s_str_1_ = "  7h34ch8 73thv uibdfj djf5^&  ";
    isHashKnown_1_ = false;
    assert(!s_str_1_.modified() && !s_str_1_.truncated() && s_str_1_.getHashIfKnown(isHashKnown_1_));
    auto s_str_3_(s_str_1_);
    assert(!s_str_3_.modified() && !s_str_3_.truncated());
    assert(!strcmp(s_str_3_.c_str(), s_str_1_.c_str()));
    isHashKnown_1_ = false;
    assert(s_str_3_.getHashIfKnown(isHashKnown_1_) &&
           s_str_1_.getHashIfKnown(isHashKnown_1_) == s_str_3_.getHashIfKnown(isHashKnown_1_));
    assert(s_str_3_ == s_str_1_);
    isHashKnown_1_ = false;
    assert(s_str_1_.getHashIfKnown(isHashKnown_1_) &&
           s_str_1_.getHashIfKnown(isHashKnown_1_) == s_str_3_.getHashIfKnown(isHashKnown_1_));
  }

  //// Strs speed comparison
  
  if (!runAll) {
    std::cout << "\nEnter '1' to perform the concat. speed test: ";
    memset(askUser, 0, sizeof(askUser));
    std::cin >> askUser;
  }
  if (runAll || '1' == *askUser && !askUser[1U]) {
    static const auto STR_BUF_SIZE_ = 440U * 1024U;

    auto const cstr___1_ptr_ = new(std::nothrow) CStr<STR_BUF_SIZE_ - 1U>;
    if (!cstr___1_ptr_) {
      std::cout << "\n[!] NOT enough memory [!]\n";
      assert(false);
      return;
    }
    std::unique_ptr<std::decay<decltype(*cstr___1_ptr_)>::type> autoDeleteCStr(cstr___1_ptr_);
    CStr<STR_BUF_SIZE_ - 1U>& cstr___1_ = *cstr___1_ptr_;

    auto const static_str___1_ptr_ =
      new(std::nothrow) StaticallyBufferedStringLight<char, STR_BUF_SIZE_ - 1U>;
    if (!static_str___1_ptr_) {
      std::cout << "\n[!] NOT enough memory [!]\n";
      assert(false);
      return;
    }
    std::unique_ptr<std::decay<decltype(*static_str___1_ptr_)>::type> autoDeleteStaticStr(static_str___1_ptr_);
    StaticallyBufferedStringLight<char, STR_BUF_SIZE_ - 1U>& static_str___1_ = *static_str___1_ptr_;
    std::string dynamic_str___1_;

    cstr___1_.clear();
    std::cout << cstr___1_.strBuf; // to force the compiler to generate the actual code
    const volatile auto cstr_test_time_ =
      TestConcat(cstr___1_, STR_BUF_SIZE_ - 1U).count();
    cstr___1_.clear();
    std::cout << cstr___1_.strBuf; // to force the compiler to generate the actual code

    static_str___1_.clear();
    std::cout << static_str___1_; // to force the compiler to generate the actual code
    const volatile auto static_str_test_time_ =
      TestConcat(static_str___1_, STR_BUF_SIZE_ - 1U).count();
    cstr___1_.clear();
    std::cout << cstr___1_.strBuf; // to force the compiler to generate the actual code
    
    dynamic_str___1_.clear();
    std::cout << dynamic_str___1_; // to force the compiler to generate the actual code
    const volatile auto dynamic_str_test_time_ =
      TestConcat(dynamic_str___1_, STR_BUF_SIZE_ - 1U).count();
    cstr___1_.clear();
    std::cout << cstr___1_.strBuf; // to force the compiler to generate the actual code

    std::cout << "static str: " << static_str_test_time_ << std::endl;
    std::cout << "dynamic str: " << dynamic_str_test_time_ << std::endl;
    std::cout << "cstr: " << cstr_test_time_ << std::endl;
    std::cout << "\nStatic str.: 1.0, dynamic str.: "
              << static_cast<double>(dynamic_str_test_time_) / static_str_test_time_
              << ", POD C str.: "
              << static_cast<double>(cstr_test_time_) / static_str_test_time_ << std::endl;
  }
  
  //// Tests for str.

  //// Test 'resize'

  {
    StaticallyBufferedStringLight<char, 7U> str_1_;
    auto knownHash = false;
    
    assert(str_1_.resize(1U, 'a'));
    knownHash = false;
    assert(!str_1_.empty() && 1U == str_1_.length() &&
           str_1_.getHashIfKnown(knownHash) && knownHash && !str_1_.modified() &&
           !str_1_.full() && !str_1_.truncated());
    assert(!strcmp(str_1_.c_str(), "a"));

    assert(str_1_.resize(4U, ' '));
    knownHash = false;
    assert(!str_1_.empty() && 4U == str_1_.length() &&
           str_1_.getHashIfKnown(knownHash) && knownHash && !str_1_.modified() &&
           !str_1_.full() && !str_1_.truncated());
    assert(!strcmp(str_1_.c_str(), "a   "));

    assert(str_1_.resize(0U, '\0'));
    knownHash = false;
    assert(str_1_.empty() && 0U == str_1_.length() &&
           !str_1_.getHashIfKnown(knownHash) && knownHash && !str_1_.modified() &&
           !str_1_.full() && !str_1_.truncated());
    assert(!strcmp(str_1_.c_str(), ""));

    assert(str_1_.resize(128U, '7'));
    knownHash = false;
    assert(!str_1_.empty() && str_1_.max_size() == str_1_.length() &&
           str_1_.getHashIfKnown(knownHash) && knownHash && !str_1_.modified() &&
           str_1_.full() && str_1_.truncated());
    assert(!strcmp(str_1_.c_str(), "7777777"));
    
    str_1_.clear();
    assert(!str_1_.resize(4U, '\0'));
    knownHash = false;
    assert(str_1_.empty() && 0U == str_1_.length() &&
           !str_1_.getHashIfKnown(knownHash) && knownHash && !str_1_.modified() &&
           !str_1_.full() && !str_1_.truncated());
    assert(!strcmp(str_1_.c_str(), ""));
  }

  // bool resize(size_t n, const TElemType c)

  ////

  {
    StaticallyBufferedStringLight<> str_1_;
    assert(!str_1_.size() && !str_1_.length() && !*str_1_.c_str());

    auto r_1_ = str_1_.pop_back();

    const char c_1_[] = {"7y3c47x3778xg43xmxh3g"};
    str_1_ += c_1_;
    assert(strlen(c_1_) == str_1_.length() && !strcmp(c_1_, str_1_.c_str()));
  }

  {
    const StaticallyBufferedStringLight<char, 7U> emptyStr1;
    const StaticallyBufferedStringLight<char, 31U> emptyStr2("");
    const StaticallyBufferedStringLight<char, 63U> emptyStr3 = nullptr;
    assert(!strcmp("", emptyStr1.c_str()) &&
           !strcmp("", emptyStr2.c_str()) && !strcmp("", emptyStr3.c_str()));

    assert((emptyStr1 == emptyStr2) && (emptyStr1 == emptyStr3) && (emptyStr2 == emptyStr3));
    assert(!(emptyStr1 != emptyStr2) && !(emptyStr1 != emptyStr3) && !(emptyStr2 != emptyStr3));
    assert((emptyStr1 == "") && (emptyStr2 == "") && (emptyStr3 == ""));

    const StaticallyBufferedStringLight<char, 7U> str1 = "1234567890";
    const StaticallyBufferedStringLight<char> str2 = "1234567890";
    assert(str1 != str2);
    assert(!(str1 == str2));

    assert(strcmp("1234567890", str1.c_str()));
    assert(!strcmp("1234567", str1.c_str()));
    assert(str1 == "1234567");
    assert(str1 != str2);
    assert(!strcmp("1234567890", str2.c_str()));
    assert(str2 == "1234567890");

    const StaticallyBufferedStringLight<char, 15U> str3 = str2;
    assert(str3 == str2);
    assert(!strcmp(str2.c_str(), str2.c_str()));
    {
      const std::string dynamicStr1_ = "x9a3mtx9x3498xn8ghxrerishm7x39";
      StaticallyBufferedStringLight<char, 63U> static_str4_ = dynamicStr1_;
      assert(static_str4_ == dynamicStr1_);
      assert(!strcmp(static_str4_.c_str(), dynamicStr1_.c_str()));
      assert(!(static_str4_ != dynamicStr1_));
    }
    std::string dynamicStr2_;
    StaticallyBufferedStringLight<char, 63U> static_str5_;
    assert(static_str5_ == dynamicStr2_);
    assert(!strcmp(static_str5_.c_str(), dynamicStr2_.c_str()));
    assert(!(static_str5_ != dynamicStr2_));

    auto const str_literal_1_ = "ca498cy3m4cmch,89yc,ds5su36c5c!@#$%_";
    dynamicStr2_ = str_literal_1_;
    static_str5_ = str_literal_1_;
    assert(static_str5_ == dynamicStr2_);
    assert(!strcmp(static_str5_.c_str(), dynamicStr2_.c_str()));
    assert(static_str5_ == dynamicStr2_.c_str());
    assert(!(static_str5_ != dynamicStr2_));

    StaticallyBufferedStringLight<char, 63U> static_str6_;
    static_str6_ = dynamicStr2_;
    assert(static_str6_ == dynamicStr2_);
    assert(!strcmp(static_str6_.c_str(), dynamicStr2_.c_str()));
    assert(static_str6_ == dynamicStr2_.c_str());
    assert(!(static_str6_ != dynamicStr2_));

    static_str6_.clear();
    assert(static_str6_ == "");
    assert(static_str6_ == nullptr);
    assert(static_str6_.empty());
    assert(!(static_str6_ != ""));
    assert(!(static_str6_ != nullptr));

    StaticallyBufferedStringLight<char, 63U> static_str7_;
    assert(static_str6_ == static_str7_);
    assert(!strcmp(static_str6_.c_str(), static_str7_.c_str()));
    assert(static_str6_ == static_str7_.c_str());
    std::string dynamicStr3_;
    assert(static_str6_ == dynamicStr3_);
    assert(!strcmp(static_str6_.c_str(), dynamicStr3_.c_str()));
    assert(static_str6_ == dynamicStr3_.c_str());

    auto const str_literal_2_ = "!@#$%^&*()_+_)(*&^%$#@xhn87ae9c";
    static_str6_ += str_literal_2_;
    dynamicStr3_ += str_literal_2_;
    assert(static_str6_ != nullptr);
    assert(static_str6_ == dynamicStr3_);
    assert(!strcmp(static_str6_.c_str(), dynamicStr3_.c_str()));
    assert(static_str6_ == dynamicStr3_.c_str());
    assert(static_str6_ != (dynamicStr3_.c_str() + 3U));

    static_str6_ += str_literal_2_;
    dynamicStr3_ += str_literal_2_;
    assert(static_str6_ == dynamicStr3_);
    assert(!strcmp(static_str6_.c_str(), dynamicStr3_.c_str()));
    assert(static_str6_ == dynamicStr3_.c_str());
    assert(static_str6_ != (dynamicStr3_.c_str() + 2U));

    static_str6_ += "";
    static_str6_ += nullptr;
    dynamicStr3_ += "";
    assert(static_str6_ == dynamicStr3_);
    assert(!strcmp(static_str6_.c_str(), dynamicStr3_.c_str()));
    assert(static_str6_ == dynamicStr3_.c_str());
    assert(static_str6_ != (dynamicStr3_.c_str() + 1U));

    char char_array_1_[128U] = {0};
    memcpy(char_array_1_, static_str6_.c_str(), static_str6_.length());
    assert(static_str6_ == char_array_1_);
    assert(!(static_str6_ != char_array_1_));

    assert(!strcmp(static_str6_.c_str(), static_str6_.data()));
    assert(static_str6_.length() == static_str6_.size());
    assert(static_str6_.length() == strlen(static_str6_.c_str()));
    assert(static_str6_.max_size() >= static_str6_.length());
    assert(static_str6_.max_size() <= static_str6_.capacity());
    assert(!static_str6_.empty());
    assert(static_str6_.reserve(static_str6_.length()) &&
           static_str6_.reserve(static_str6_.max_size()));

    auto static_str6_copy_ = static_str6_;
    static_str6_.shrink_to_fit();
    assert(static_str6_copy_ == static_str6_);

    const char char_array___0_[] = "xmq8934yx49,qxto,4htz8otxmh,olxlj,x_";
    StaticallyBufferedStringLight<char, 63U> static_str8_ = char_array___0_;
    assert(static_str8_ == static_str8_);
    assert(!(static_str8_ != static_str8_));
    for (size_t charIdx = 0U;
         charIdx < (std::extent<decltype(char_array___0_)>::value - 1U);
         ++charIdx)
    {
      assert(static_str8_.at(charIdx) == char_array___0_[charIdx]);
      assert(static_str8_[charIdx] == char_array___0_[charIdx]);
    }
    assert(static_str8_.back() == char_array___0_[std::extent<decltype(char_array___0_)>::value - 2U]);
    assert(static_str8_.front() == *char_array___0_);

    const auto currLen__1_ = static_str8_.length();
    assert(static_str8_.back() != '6');
    static_str8_.push_back('6');
    assert(static_str8_.length() > currLen__1_);
    assert(static_str8_.back() == '6');
    assert(static_str8_.pop_back() == '6');
    assert(static_str8_.length() == currLen__1_);
    assert(static_str8_.back() != '6');

    auto isHashKnown_2_ = false;
    assert(!static_str8_.getHashIfKnown(isHashKnown_2_));
    assert(static_str8_.hash());
    isHashKnown_2_ = false;
    assert(static_str8_.getHashIfKnown(isHashKnown_2_));
    isHashKnown_2_ = false;
    assert(static_str8_.hash() == static_str8_.getHashIfKnown(isHashKnown_2_));

    const StaticallyBufferedStringLight<char, 63U> static_str9_ = "abc";
    const StaticallyBufferedStringLight<char, 63U> static_str10_ = "abcd";
    assert(static_str9_ < "abcd" && static_str9_ < static_str10_);
    assert(!(static_str9_ < "abc") && !(static_str10_ < "abcd"));
    assert(!(static_str10_ < static_str9_));
  }

  { // C++11 range loop + const. rev. iters + hash container + sort. tests
    const char staticChars_1_[] = "2qz892ryzr923,8zm2h,398zrh3298zh298zh29t8zh,92zh,2398tzhhzt9q32";
    const StaticallyBufferedStringLight<> static_str10_ = staticChars_1_;
    std::string dstr_1_;

    std::unordered_set<StrLight, std::hash<StrLight>, std::equal_to<StrLight>> uset_10_;
    std::vector<StrLight> vec_10_;
    std::vector<std::string> vec_11_;
    std::deque<StrLight> deq_10_;
    std::set<StrLight, std::less<StrLight>> set_10_;
    std::list<StrLight> list_10_;

    auto idx_1_ = 0U;
    for (const auto c_1_ : static_str10_) {
      assert(c_1_ == staticChars_1_[idx_1_++]);
      dstr_1_ += c_1_;

      //// Constructs 'StrLight' from 'std::string'
      const auto emplaceResult = uset_10_.emplace(dstr_1_);
      assert(*emplaceResult.first == dstr_1_ && dstr_1_ == (*emplaceResult.first).c_str());
      assert(emplaceResult.second);
      const auto findResult = uset_10_.find(dstr_1_);
      assert(findResult != uset_10_.end());
      assert(*findResult == dstr_1_ && dstr_1_ == (*findResult).c_str());

      vec_10_.emplace_back(dstr_1_);
      vec_11_.emplace_back(const_cast<const decltype(dstr_1_)&>(dstr_1_));
      deq_10_.emplace_back(dstr_1_);
      list_10_.emplace_back(dstr_1_);
      
      const auto setEmplaceResult = set_10_.emplace(dstr_1_);
      assert(*setEmplaceResult.first == dstr_1_ && dstr_1_ == (*setEmplaceResult.first).c_str());
      assert(setEmplaceResult.second);
      const auto setFindResult = set_10_.find(dstr_1_);
      assert(setFindResult != set_10_.end());
      assert(*setFindResult == dstr_1_ && dstr_1_ == (*setFindResult).c_str());
    }
    assert(idx_1_ == static_str10_.length());
    assert(idx_1_ == std::extent<decltype(staticChars_1_)>::value - 1U);
    assert(dstr_1_ == static_str10_.c_str());
    assert(static_str10_ == dstr_1_);

    std::remove_const<decltype(staticChars_1_)>::type staticChars_2_ = {0};
    idx_1_ = 0U;
    auto iter_2_ = static_str10_.crbegin();
    while (iter_2_ != static_str10_.crend()) {
      const auto elem_1_ = *iter_2_++;
      const auto elem_2_ = staticChars_1_[std::extent<decltype(staticChars_1_)>::value - idx_1_ - 2U];
      assert(elem_1_ == elem_2_);
      staticChars_2_[idx_1_] = elem_1_; // reversed fill
      ++idx_1_;
    }
    std::reverse(staticChars_2_, staticChars_2_ + strlen(staticChars_2_));
    assert(!strcmp(staticChars_2_, staticChars_1_));
    assert(static_str10_ == staticChars_2_);
    assert(dstr_1_ == staticChars_2_);

    decltype(vec_11_) vec_12_;
    for (const auto& elem : vec_10_) {
      vec_12_.emplace_back(elem.c_str());
    }
    assert(vec_11_ == vec_12_); // i. e. vec_11_ == vec_10_, as 'vec_12_' constructed from 'vec_10_'

    std::reverse(vec_10_.begin(), vec_10_.end());
    std::reverse(vec_11_.begin(), vec_11_.end());
    std::stable_sort(vec_10_.begin(), vec_10_.end());
    std::stable_sort(vec_11_.begin(), vec_11_.end());

    vec_12_.clear();
    for (const auto& elem : vec_10_) {
      vec_12_.emplace_back(elem.c_str());
    }
    assert(vec_11_ == vec_12_); // i. e. vec_11_ == vec_10_, as 'vec_12_' constructed from 'vec_10_'
  }
  
  { // Sort test B
    StrLight strLight_1_ = "a", strLight_2_;
    assert(strLight_1_ < "b");
    strLight_1_ = "aba";
    assert(strLight_1_ < "abb");
    strLight_1_ = "bba";
    assert(strLight_1_ < "cbb");
    strLight_1_ = "bbc";
    assert(!(strLight_1_ < "bbb"));

    strLight_1_ = "a";
    strLight_2_ = "b";
    assert(strLight_1_ < strLight_2_);
    assert(!(strLight_2_ < strLight_1_));
    strLight_1_ = "aa";
    strLight_2_ = "ba";
    assert(strLight_1_ < strLight_2_);
    assert(!(strLight_2_ < strLight_1_));
    strLight_1_ = "aa";
    strLight_2_ = "ab";
    assert(strLight_1_ < strLight_2_);
    assert(!(strLight_2_ < strLight_1_));
    strLight_1_ = "aab";
    strLight_2_ = "aac";
    assert(strLight_1_ < strLight_2_);
    assert(!(strLight_2_ < strLight_1_));
    strLight_1_ = "aac";
    strLight_2_ = "abb";
    assert(strLight_1_ < strLight_2_);
    assert(!(strLight_2_ < strLight_1_));

    strLight_1_ = "aaaaaaaaaaaab";
    assert(strLight_1_ < "aaaaaaaaaaaac");
    strLight_2_ = "aaaaaaaaaaaac";
    assert(strLight_1_ < strLight_2_);
    assert(strLight_1_ < "baaaaaaaaaaab");
    strLight_2_ = "baaaaaaaaaaab";
    assert(strLight_1_ < strLight_2_);
    assert(strLight_1_ < "aaaaaaaaaaabb");
    strLight_2_ = "aaaaaaaaaaabb";
    assert(strLight_1_ < strLight_2_);
    assert(strLight_1_ < "aaaaaabaaaaab");
    strLight_2_ = "aaaaaabaaaaab";
    assert(strLight_1_ < strLight_2_);
    assert(strLight_1_ < "aaabaaaaaaaab");
    strLight_2_ = "aaabaaaaaaaab";
    assert(strLight_1_ < strLight_2_);

    strLight_1_ = "aa";
    assert(strLight_1_ < "aaa");
    strLight_2_ = "aaa";
    assert(strLight_1_ < strLight_2_);
    strLight_1_ = "2acaa_abaaamaa&aaeaa1a";
    assert(strLight_1_ < "2acaa_abazamaa&aaeaa1a");
    strLight_2_ = "2acaa_abazamaa&aaeaa1a";
    assert(strLight_1_ < strLight_2_);

    union u1 {
      unsigned int ui;
      char a[sizeof(unsigned int)];
    } u1_a_, u1_b_;
    //                                    '\0'                     97 ('a')
    strcpy(u1_a_.a, "aac"); // 6 512 993 = [0] 0110 0011 0110 0001 0110 0001
    strcpy(u1_b_.a, "abb"); // 6 447 713 = [0] 0110 0010 0110 0010 0110 0001

    // Reverse the bit order - NO effect
    // 2 256 979 456 = 1000 0110 1000 0110 1100 0110 0000 0000
    const unsigned int u1_a_i_ = MathUtils::ReverseBitsEx(u1_a_.ui);
    // 2 252 752 384 = 1000 0110 0100 0110 0100 0110 0000 0000
    const unsigned int u1_b_i_ = MathUtils::ReverseBitsEx(u1_b_.ui);

    //// Reverse bytes - Ok
    //// 1 633 772 288
    std::swap(u1_a_.a[0U], u1_a_.a[3U]);
    std::swap(u1_a_.a[1U], u1_a_.a[2U]);
    //// 1 633 837 568
    std::swap(u1_b_.a[0U], u1_b_.a[3U]);
    std::swap(u1_b_.a[1U], u1_b_.a[2U]);

    const auto r1_ = u1_a_.ui < u1_b_.ui;
    const auto r2_ = u1_a_.ui - u1_b_.ui;

    char strBuf_23_[4U] = {0};
    HashTester::Params<std::extent<decltype(strBuf_23_)>::value> params(strBuf_23_);
    HashTester::createAndTestCharSeq<SortTesterHelper>(params, false);
    std::reverse(SortTesterHelper::vec_dynamic_str_.begin(), SortTesterHelper::vec_dynamic_str_.end());
    std::reverse(SortTesterHelper::vec_static_str_.begin(), SortTesterHelper::vec_static_str_.end());
    std::stable_sort(SortTesterHelper::vec_dynamic_str_.begin(),
                     SortTesterHelper::vec_dynamic_str_.end());
    std::stable_sort(SortTesterHelper::vec_static_str_.begin(),
                     SortTesterHelper::vec_static_str_.end());
    
    decltype(SortTesterHelper::vec_dynamic_str_) vec_13_;
    for (const auto& elem : SortTesterHelper::vec_static_str_) {
      vec_13_.emplace_back(elem.c_str(), elem.size());
    }
    // Check SortTesterHelper::vec_dynamic_str_ == SortTesterHelper::vec_static_str_,
    //  as 'vec_13_' constructed from 'SortTesterHelper::vec_static_str_'
    assert(SortTesterHelper::vec_dynamic_str_ == vec_13_);

    SortTesterHelper::clean();
  }

  { // Sort test C
    const char auto_arr_1_[] = "a";
    const char auto_arr_2_[] = "b";
    StrLight strLight_1_ = auto_arr_1_;
    assert(strLight_1_ < auto_arr_2_);
    assert(!(strLight_1_ < auto_arr_1_));

    const char auto_arr_3_[] = "aba";
    const char auto_arr_4_[] = "abb";
    strLight_1_ = auto_arr_3_;
    assert(strLight_1_ < auto_arr_4_);
    assert(!(strLight_1_ < auto_arr_3_));

    const char auto_arr_5_[] = "bba";
    const char auto_arr_6_[] = "cbb";
    strLight_1_ = auto_arr_5_;
    assert(strLight_1_ < auto_arr_6_);
    assert(!(strLight_1_ < auto_arr_5_));

    const char auto_arr_7_[] = "aba";
    const char auto_arr_8_[] = "abb";
    strLight_1_ = auto_arr_8_;
    assert(!(strLight_1_ < auto_arr_7_));
    
    const char auto_arr_9a_[][24U] =
      { {"aa"}, {"aa"}, {"aab"}, {"aaaaaaaaaaaab"}, {"aaaaaaaaaaaac"}, {"aaaaaaaaaaaac"},
        {"aaaaaaaaaaaac"}, {"aaaaaaaaaaaac"}, {"aa"}, {"2acaa_abaaamaa&aaeaa1a"} };
    const char auto_arr_9b_[][24U] =
      { {"ba"}, {"ab"}, {"aac"}, {"aaaaaaaaaaaac"}, {"baaaaaaaaaaab"}, {"aaaaaaaaaaabb"},
        {"aaaaaabaaaaab"}, {"aaabaaaaaaaab"}, {"aaa"}, {"2acaa_abazamaa&aaeaa1a"} };
    static_assert(std::extent<decltype(auto_arr_9a_)>::value ==
                  std::extent<decltype(auto_arr_9b_)>::value,
                  "Inccorect array size!");
    strLight_1_.clear();
    for (size_t idx = 0U; idx < std::extent<decltype(auto_arr_9a_)>::value; ++idx) {
      strLight_1_ = auto_arr_9a_[idx];
      assert(strLight_1_ < auto_arr_9b_[idx]);
      assert(!(strLight_1_ < auto_arr_9a_[idx]));

      strLight_1_ = auto_arr_9b_[idx];
      assert(!(strLight_1_ < auto_arr_9a_[idx]));
      assert(!(strLight_1_ < auto_arr_9b_[idx]));
    }
  }

  {
    StrLight st_str_33_;
    assert(!st_str_33_.modified() && !st_str_33_.truncated() && !st_str_33_.full());

    assert(!st_str_33_.setCharAt(0U, 'a') && !st_str_33_.setCharAt(1U, 'b'));
    assert(!st_str_33_.modified() && !st_str_33_.truncated() && !st_str_33_.full());

    st_str_33_ = "abc";
    assert(!st_str_33_.modified() && !st_str_33_.truncated() && !st_str_33_.full());

    assert(st_str_33_.setCharAt(1U, 'a'));
    assert(st_str_33_.modified() && 'a' == st_str_33_[1U] &&
           !st_str_33_.truncated() && !st_str_33_.full());

    auto isHashKnown_3_ = false;
    st_str_33_.getHashIfKnown(isHashKnown_3_);
    assert(st_str_33_.modified() && !st_str_33_.truncated() && !st_str_33_.full());
    assert(st_str_33_.hash());
    assert(!st_str_33_.modified() && !st_str_33_.truncated() && !st_str_33_.full());

    for (size_t idx = 0U; idx < st_str_33_.max_size() + 8U; ++idx) {
      const char c = 'A' + idx / 3U;
      st_str_33_ += c;
    }
    assert(!st_str_33_.modified() && st_str_33_.truncated() && st_str_33_.full());
  }
  
  // String performance tests
  char c_02_ = '\0';
  if (!runAll) {
    std::cout << "\nEnter 1 to perform a speed comparison test: ";
    std::cin >> c_02_;
  }
  if (runAll || '1' == c_02_) {
    const auto SIZE_ = 1024U * 1024U * 8U;
    auto mem1 = new char[SIZE_];
    memset(mem1, 200, SIZE_ - 1U);
    auto mem2 = new char[SIZE_];
    memset(mem2, 200, SIZE_ - 1U);
    mem1[SIZE_ - 1U] = '\0';
    mem2[SIZE_ - 1U] = '\0';
    
    // (64 bit OS, 64 bit CPU, 32 bit application)
    decltype(std::chrono::system_clock::now()) time1, time2;
    static const auto COUNT_001_ = 7U;
    static const auto TEST_COUNT_001_ = 40U;
    decltype((time2 - time1).count()) counts[COUNT_001_] = {0};
    auto memCmp_faster_then_memcmp_count = size_t();
    auto memCmp_faster_then_quickCmp = size_t();
    unsigned long long avg[COUNT_001_] = {0};
    const auto iterCount_1_ = 40U;

    for (size_t idx = 0U; idx < iterCount_1_; ++idx) {
      time1 = std::chrono::system_clock::now();
      volatile long long int r0 = 0LL;
      for (size_t testIdx = size_t(); testIdx < TEST_COUNT_001_; ++testIdx) {
        // 'strCmp<>' with 4U works MUCH faster, then with the 8U
        r0 = strCmp<>(mem1, mem2);
      }
      time2 = std::chrono::system_clock::now();
      *counts = (time2 - time1).count();
      *avg += *counts;

      time1 = std::chrono::system_clock::now();
      volatile long long int r1 = 0LL;
      for (size_t testIdx = size_t(); testIdx < TEST_COUNT_001_; ++testIdx) {
        // 'quickCmp' looks like with 8U works faster, then with the 4U
        //  (BUT we can't use 8U AND this func. is NOT used)
        r1 = quickCmp(mem1, mem2, SIZE_);
      }
      time2 = std::chrono::system_clock::now();
      counts[1] = (time2 - time1).count();
      avg[1U] += counts[1];

      time1 = std::chrono::system_clock::now();
      volatile long long int r2 = 0LL;
      for (size_t testIdx = size_t(); testIdx < TEST_COUNT_001_; ++testIdx) {
        r2 = memcmp(mem1, mem2, SIZE_);
      }
      time2 = std::chrono::system_clock::now();
      counts[2] = (time2 - time1).count();
      avg[2U] += counts[2];

      time1 = std::chrono::system_clock::now();
      volatile long long int r3 = 0LL;
      for (size_t testIdx = size_t(); testIdx < TEST_COUNT_001_; ++testIdx) {
        r3 = strcmp(mem1, mem2);
      }
      time2 = std::chrono::system_clock::now();
      counts[3] = (time2 - time1).count();
      avg[3U] += counts[3];

      time1 = std::chrono::system_clock::now();
      volatile long long int r4 = 0LL;
      const auto count_1_ = SIZE_ / sizeof(std::uintptr_t);
      for (size_t testIdx = size_t(); testIdx < TEST_COUNT_001_; ++testIdx) {
        // 'memCmp' with 8U works faster, then with 4U (BUT we can't do that)
        r4 = memCmp<>(mem1, mem2, count_1_);
      }
      time2 = std::chrono::system_clock::now();
      counts[4] = (time2 - time1).count();
      avg[4U] += counts[4];

      time1 = std::chrono::system_clock::now();
      volatile bool r5 = false;
      for (size_t testIdx = size_t(); testIdx < TEST_COUNT_001_; ++testIdx) {
        r5 = isEqualMemD(mem1, mem2, SIZE_);
      }
      time2 = std::chrono::system_clock::now();
      counts[5] = (time2 - time1).count();
      avg[5U] += counts[5];

      time1 = std::chrono::system_clock::now();
      volatile bool r6 = false;
      const auto count_02_ = SIZE_ / 8U;
      for (size_t testIdx = size_t(); testIdx < TEST_COUNT_001_; ++testIdx) {
        r6 = isEqualMem<8U>(mem1, mem2, count_02_);
      }
      time2 = std::chrono::system_clock::now();
      counts[6] = (time2 - time1).count();
      avg[6U] += counts[6];

      std::cout << "\n";
      std::cout << "count6 - isEqualMem<8U>    : " << counts[6] << "\n";
      std::cout << "count5 - isEqualMemD       : " << counts[5] << "\n";
      std::cout << "count4 - memCmp<>          : " << counts[4] << "\n";
      std::cout << "count3 - strcmp            : " << counts[3] << "\n";
      std::cout << "count2 - memcmp            : " << counts[2] << "\n";
      std::cout << "count1 - quickCmp          : " << counts[1] << "\n";
      std::cout << "count0 - strCmp<>          : " << *counts   << "\n";
      std::cout << "\n" << static_cast<double>(counts[3]) / counts[1] << "\n";
      assert(r1 == r2);

      if (counts[4] < counts[2]) ++memCmp_faster_then_memcmp_count;
      if (counts[4] < counts[1]) ++memCmp_faster_then_quickCmp;
    }
    std::cout << "\nmemCmp_faster_then_memcmp_count: "
              << memCmp_faster_then_memcmp_count  << " / "
              << iterCount_1_ << std::endl;
    std::cout << "memCmp_faster_then_quickCmp: "
              << memCmp_faster_then_quickCmp << " / "
              << iterCount_1_ << std::endl << std::endl;

    const char* const names_001_[]
      = {"strCmp<>          ",
         "quickCmp          ",
         "memcmp            ",
         "strcmp            ",
         "memCmp<>          ",
         "isEqualMemD       ",
         "isEqualMem<8U>    "};
    auto idx_0001_ = 0U;
    for (volatile auto& currAvg : avg) {
      currAvg /= iterCount_1_;
      std::cout << "avg " << names_001_[idx_0001_] << " - #" << idx_0001_ << ": " << currAvg
                << std::endl;
      ++idx_0001_;
    }
    delete[] mem1, delete[] mem2;
    mem1 = mem2 = nullptr;

    // Speed test 2
    const char static_chars_01_[]
      = {"It had an average daily circulation of 2.2 million copies in March 2014."
      "Between July and December 2013 the paper had an average daily readership"
      " of approximately 5.5 million, with approximately 31% of those falling into"
      " the ABC1 demographic and 68% in the C2DE demographic. Approximately 41% of "
      "readers are women. The Sun has been involved in many controversies in its history, "
      "including its coverage of the 1989 Hillsborough football stadium disaster. "
      "Regional editions of the newspaper for Scotland, Northern Ireland and the Republic of Ireland"
      " are published in Glasgow (The Scottish Sun),"
      " Belfast (The Sun) and Dublin (The Irish Sun) respectively."};
    StaticallyBufferedStringLight<char, 799U> s_str_01_ = static_chars_01_;
    const auto pos = static_cast<size_t>(s_str_01_.length() * 0.7);
    assert(s_str_01_.setCharAt(pos, '^'));

    TestFunctor<decltype(s_str_01_), decltype(static_chars_01_)>
      testLessThen1(s_str_01_, static_chars_01_, 0U);

    std::string dstr_01_ = s_str_01_.c_str();
    assert(s_str_01_ == dstr_01_ && dstr_01_ == s_str_01_.c_str());

    TestFunctor<decltype(dstr_01_), decltype(static_chars_01_)>
      testLessThen2(dstr_01_, static_chars_01_, 0U);

    //// On release static. str COULD be a bit slower (~7%) [OR COULD be NOT]

    PerformanceTester::TestResults results1;
    PerformanceTester::test(testLessThen1, testLessThen2, 6000000U, results1); // less then

    testLessThen1.mode = testLessThen2.mode = 1U; // equality comparing speed test
    PerformanceTester::TestResults results2;
    PerformanceTester::test(testLessThen1, testLessThen2, 6000000U, results2);

    // Another speed test
    const decltype(s_str_01_) s_str_02_ = static_chars_01_;
    const decltype(dstr_01_) dstr_02_ = static_chars_01_;

    volatile auto result__01_ = false;
    time1 = std::chrono::system_clock::now();
    for (size_t testIdx = size_t(); testIdx < 100000U; ++testIdx) {
      result__01_ = s_str_01_ < s_str_02_;
    }
    time2 = std::chrono::system_clock::now();
    counts[1] = (time2 - time1).count();

    time1 = std::chrono::system_clock::now();
    for (size_t testIdx = size_t(); testIdx < 100000U; ++testIdx) {
      result__01_ = dstr_01_ < dstr_02_;
    }
    time2 = std::chrono::system_clock::now();
    counts[2] = (time2 - time1).count();

    time1 = std::chrono::system_clock::now();
    for (size_t testIdx = size_t(); testIdx < 100000U; ++testIdx) {
      result__01_ = strcmp(s_str_01_.c_str(), s_str_02_.c_str()) == 0;
    }
    time2 = std::chrono::system_clock::now();
    counts[3] = (time2 - time1).count();

    volatile auto diff = static_cast<double>(counts[2]) / counts[1];
    std::cout << "\nStatic str. 'operator<' " << diff << " times faster then the dynamic one\n";
    diff = static_cast<double>(counts[3]) / counts[1];
    std::cout << "  and " << diff << " times faster then the 'strcmp'\n";
  }
   
  {
    StaticallyBufferedStringLight<> str_45_ = "xrXR2C";
    str_45_ += str_45_;
    assert(str_45_ == "xrXR2C""xrXR2C");
    assert(!strcmp(str_45_.c_str(), "xrXR2C""xrXR2C"));
  }

  {
    const std::string d_str_03_ =
      "c85s4y54p489y89sc5,89h4p,hcy54hyxo9,hxy9yhx8rchr98ch,yr,c98hcsxht87x4o,zywm45xy54wx0yy,f";
    const auto pos_02_ = static_cast<size_t>(d_str_03_.length() * 0.57);
    const std::string d_str_04_(d_str_03_, pos_02_, std::string::npos);
    const StaticallyBufferedStringLight<> st_str_01_(d_str_03_, pos_02_, std::string::npos);
    assert(d_str_04_ == st_str_01_ && st_str_01_ == d_str_04_);

    const std::string d_str_05_;
    const auto pos_03_ = 999U;
    try { // mmm incorrect test, throws at the very first lint in a 'try' block
      const std::string d_str_06_(d_str_05_, pos_03_, 99U);
      const StaticallyBufferedStringLight<> st_str_02_(d_str_05_, pos_03_, 99U);
      assert(d_str_06_ == st_str_02_ && st_str_02_ == d_str_06_);

      const StaticallyBufferedStringLight<> st_str_03_(st_str_01_, size_t());
      const StaticallyBufferedStringLight<> st_str_04_(st_str_02_, size_t());
      assert(st_str_03_ == st_str_01_ && st_str_04_ == st_str_02_);
    } catch (...) {}
  }
  
  // Sort with the russ. symbols test
  {
    std::vector<std::string> vec_d_str_02_ =
      {"срыcw4сп", "свнсы", "сфxtцсыре", "скqxвсфоxеc6н", "есc4wеуфчф", "идеcwгвме", "сфцcyфяау",
       "тwcфцау", "юрпc4wыуа", "ваыпcwцо", "оcклавд", "ыцеwнгн4wшпр", "трдрятлоwы", "опc4x3вый3пвл",
       "пврxqмнкxq3сысы", "мxq3асxqцч", "соскыxqфцxqлн", "сыxчqxносx"};
    std::vector<StaticallyBufferedStringLight<>>
      vec_s_str_02_(vec_d_str_02_.begin(), vec_d_str_02_.end());
    auto iter_02_ = vec_s_str_02_.begin();
    for (const auto& str : vec_d_str_02_) {
      assert(str == *iter_02_);
      assert(*iter_02_ == str);
      ++iter_02_;
    }
    std::sort(vec_d_str_02_.begin(), vec_d_str_02_.end());
    std::sort(vec_s_str_02_.begin(), vec_s_str_02_.end());
    
    decltype(vec_d_str_02_) vec_d_str_03_;
    for (const auto& str : vec_s_str_02_) {
      vec_d_str_03_.emplace_back(str.c_str(), str.length()); // make 'vec_d_str_03_' == 'vec_s_str_02_'
    }
    const auto r_01_ = strcmp("оcклавд", "свнсы");
    const auto r_02_ = strCmp<>("оcклавд", "свнсы");
    const auto r_03_ = memcmp("оcклавд", "свнсы", std::min<>(strlen("оcклавд"), strlen("свнсы")) + 1U);
    assert(r_01_ > 0 == r_02_ > 0 && r_01_ > 0 == r_03_ > 0); // check equal sign
    
    StaticallyBufferedStringLight<> s01_ = "оcклавд", s02_ = "свнсы";
    const auto r_00_ = s01_ < s02_;
    assert(r_00_ == (r_01_ < 0));
    
    auto r_04_ = strcmp("свнсы", "ваыпcwцо");
    auto r_05_ = strCmp<>("свнсы", "ваыпcwцо");
    auto r_06_ = memcmp("свнсы", "ваыпcwцо", std::min<>(strlen("оcклавд"), strlen("свнсы")) + 1U);
    assert(r_04_ > 0 == r_05_ > 0 && r_04_ > 0 == r_06_ > 0); // check equal sign

    s01_ = "свнсы", s02_ = "ваыпcwцо";
    auto r_07_ = s01_ < s02_;
    assert(r_07_ == (r_04_ < 0));
    
    r_04_ = strcmp("оcклавд", "опc4x3вый3пвл");
    r_05_ = strCmp<>("оcклавд", "опc4x3вый3пвл");
    r_06_ = memcmp("оcклавд", "опc4x3вый3пвл",
                   std::min<>(strlen("оcклавд"), strlen("опc4x3вый3пвл")) + 1U);
    assert(r_04_ > 0 == r_05_ > 0 && r_04_ > 0 == r_06_ > 0); // check equal sign

    s01_ = "оcклавд", s02_ = "опc4x3вый3пвл";
    r_07_ = s01_ < s02_;
    assert(r_07_ == (r_04_ < 0));

    assert(vec_d_str_03_ == vec_d_str_02_);
  }
  
  {
    StaticallyBufferedStringLight<> s_001_;
    std::string str_001_;
    assert(str_001_ == s_001_ && s_001_ == str_001_);

    const char* const c_001_ = "xa34,7a3hxt873tx4378xta43o4zg,t37iz,3gtnc67ixsgtx37x";
    s_001_ = c_001_;
    str_001_ = c_001_;
    assert(str_001_ == s_001_ && s_001_ == str_001_);

    s_001_.clear();
    str_001_.clear();
    assert(str_001_ == s_001_ && s_001_ == str_001_);
  }
  
  {
    StaticallyBufferedStringLight<> s_001_ = "012345678";
    for (size_t i = 0U; i < s_001_.length(); ++i) {
      s_001_[i] = s_001_.back() - i; // NOTE that such an actions compromises an internall hash
    }
    assert(s_001_ == "876543210");
    
    struct Functor {
      char operator()(char& c) const throw() {
        ++c;
        return c;
      }
    };

    std::for_each(s_001_.begin(), s_001_.end(), Functor());
    assert(s_001_ == "987654321");
    
    std::reverse(s_001_.rbegin(), s_001_.rend());
    assert(s_001_ == "123456789");
    
    std::rotate(s_001_.begin(), s_001_.begin() + s_001_.length() / 2U, s_001_.end());
    assert(s_001_ == "567891234");
    
    const auto s_001_2 = s_001_; // restore the correct hash here
    assert(s_001_2 == s_001_ && s_001_ == s_001_2);
    
    std::random_shuffle(s_001_.rbegin(), s_001_.rend());

    s_001_.front() = 'T';
    s_001_.back() = 'X';
    assert('T' == s_001_.front() && 'X' == s_001_.back());

    auto s_001_2_ = s_001_;
    s_001_2_ += 'Q';
    assert(s_001_ < s_001_2_);
  }
  
  {
    StaticallyBufferedStringLight<> strLight_56_;
    char ch56_[128U] = {0};
    auto val_56_ = size_t();
    auto val_66_ = 0.0;
    
    for (; val_56_ < 999U; val_56_ += 10U, val_66_ += 1234.654321) {
      memset(ch56_, 0, sizeof(ch56_));
      sprintf(ch56_, "%u", val_56_);
      strLight_56_.clear();
      strLight_56_.append(val_56_);

      assert(strLight_56_ == ch56_);
      assert(!strcmp(strLight_56_.c_str(), ch56_));
      assert(strlen(ch56_) == strLight_56_.length());
      assert(!strLight_56_.truncated() && !strLight_56_.modified());

      memset(ch56_, 0, sizeof(ch56_));
      sprintf(ch56_, "%f", val_66_);
      strLight_56_.clear();
      strLight_56_.append(val_66_);

      assert(strLight_56_ == ch56_);
      assert(!strcmp(strLight_56_.c_str(), ch56_));
      assert(strlen(ch56_) == strLight_56_.length());
      assert(!strLight_56_.truncated() && !strLight_56_.modified());
    }
    
    StaticallyBufferedStringLight<char, 7U> strLight_57_ = "123456";
    assert(!strLight_57_.truncated() && !strLight_57_.modified());
    strLight_57_.append(99UL);
    assert(strLight_57_.truncated() && !strLight_57_.modified());
    assert(strlen(strLight_57_.c_str()) == strLight_57_.length());

    std::cout << "\nTest insert: " << strLight_57_ << std::endl;

    strLight_57_.clear();
    std::cout << "Enter smth.: ";
    std::cin >> strLight_57_;
    assert(strLight_57_.length() && !strLight_57_.modified());
    std::cout << "         -> '" << strLight_57_ << "'\n";

    const auto val_34_ = 16.42L;
    strLight_56_.clear();
    strLight_56_ << val_34_;
    memset(ch56_, 0, sizeof(ch56_));
    sprintf(ch56_, "%Lf", val_34_);
    assert(strLight_56_ == ch56_);
    assert(!strcmp(strLight_56_.c_str(), ch56_));
    assert(strlen(ch56_) == strLight_56_.length());
    assert(!strLight_56_.truncated() && !strLight_56_.modified());

    strLight_56_.clear();
    strLight_56_ << 134.4374565L;
    strLight_56_ << "  test test    " << 123456 << " good   " << 129134.437;
    memset(ch56_, 0, sizeof(ch56_));
    sprintf(ch56_, "%Lf  test test    %i good   %f", 134.4374565L, 123456, 129134.437);
    assert(strLight_56_ == ch56_);
    assert(!strcmp(strLight_56_.c_str(), ch56_));
    assert(strlen(ch56_) == strLight_56_.length());
    assert(!strLight_56_.truncated() && !strLight_56_.modified());
  }

  //// Speed test 4

  if (!runAll) {
    memset(askUser, 0, sizeof(askUser));
    std::cout << "\nEnter '1' to perform the alloc./dealloc. speed test: ";
    std::cin >> askUser;
  }
  if (runAll || '1' == *askUser && !askUser[1U]) {
    std::cout << "\nTesting...\n";

    struct Funct1__ {
      volatile int operator()() throw() {
        volatile StaticallyBufferedStringLight<> str;
        return 0;
      }
    } funct1__;

    struct Funct2__ {
       volatile int operator()() throw() {
        std::string str;
        str.reserve(127U);
        return 0;
      }
    } funct2__;

    PerformanceTester::TestResults results1;
    PerformanceTester::test(funct1__, funct2__, 9999999U, results1);
  }

  {
    StaticallyBufferedStringLight<> str__001_ = "uxy4a73", str__002_ = "uxy!a73";
    assert(str__001_ != str__002_);
    assert(!(str__001_ == str__002_));
  }

  //// Speed test 5

  if (!runAll) {
    memset(askUser, 0, sizeof(askUser));
    std::cout << "\nEnter '1' to perform the mass equality comparison speed test: ";
    std::cin >> askUser;
  }
  if (runAll || '1' == *askUser && !askUser[1U]) {
    std::cout << "\nTesting...\n";

    static auto const STR1
      = "cam834mht8xth,a4xh387th,txh87c347837 g387go4 4w78t g34 3g7rgo bvgq7 tgq3874g478g8g oebgbg8 b"
        "cwmc8htcw,o7845mt754cm8w4gcm8w4gc78w4gcw4cw4yc4c4xn34x63gc7sch74s4h5mc7h7cn34xm7xg78gxm7384x";

    static auto const STR2 //  !                  
      = "cam834mht8xth,a4xh387t_,txh87c347837 g387go4 4w78t g34 3g7rgo bvgq7 tgq3874g478g8g oebgbg8 b"
        "cwmc8htcw,o7845mt754cm8w4gcm8w4gc78w4gcw4cw4yc4c4xn34x63gc7sch74s4h5mc7h7cn34xm7xg78gxm7384x";

    struct Funct1__ {

      Funct1__() throw() : str1(STR1), str2(STR2) {}

      volatile int operator()() const throw() {
        volatile auto result = (str1 != str2);
        return 0;
      }

      StaticallyBufferedStringLight<char, 255U> str1;
      StaticallyBufferedStringLight<char, 255U> str2;
    } funct1__;

    struct Funct2__ {

      Funct2__() throw() : str1(STR1), str2(STR2) {}

      volatile int operator()() const throw() {
        volatile auto result = (str1 != str2);
        return 0;
      }

      std::string str1;
      std::string str2;
    } funct2__;

    PerformanceTester::TestResults results1;
    PerformanceTester::test(funct1__, funct2__, 9999999U, results1);
  }
}

} // namespace END

#endif // StaticStrTestsH