#include "StaticallyBufferedStringLight.h"

struct HashCodeChecker::HashChecker {
  size_t operator()(const std::string& str) const throw() {
    if (!COLLECT_STATS && !NEXT_HASH_LARGER_OR_EQ) return 0U; // skip [OPTIMIZATION]
    STR = str;

    const auto currHash = STR.hash();
    const auto nextLargerOrEq = (currHash >= PREV_HASH);
    nextLargerOrEq ? ++STATS[1U] : ++*STATS;
    NEXT_HASH_LARGER_OR_EQ &= nextLargerOrEq;
    PREV_HASH = currHash;

    return currHash;
  }

  static bool COLLECT_STATS; // will be initially false as a static
  // First - next lower count (NOT OK!), second - next larger count (OK); both zero at the start
  static size_t STATS[2U]; // first: NOT larger, second: next larger count
  static size_t PREV_HASH; // will be initially zero as a static
  static bool NEXT_HASH_LARGER_OR_EQ; // initially true!!
  static StaticallyBufferedStringLight<char, 7U> STR;
};

bool HashCodeChecker::HashChecker::COLLECT_STATS;
size_t HashCodeChecker::HashChecker::STATS[2U];
size_t HashCodeChecker::HashChecker::PREV_HASH;
bool HashCodeChecker::HashChecker::NEXT_HASH_LARGER_OR_EQ = true;
decltype(HashCodeChecker::HashChecker::STR) HashCodeChecker::HashChecker::STR;

template <const bool SkipStatistics>
bool HashCodeChecker::ishashOfLargerStrLarger() throw() {
  //// Test A
  static const auto MAX_LEN_ = 55U;
  static_assert('A' + MAX_LEN_ < 128, "Incorrect max. len.");
  StaticallyBufferedStringLight<char, MAX_LEN_> str;

  decltype(str.hash()) prevHash = str.hash(), nowHash = 0U;
  auto currChar = 'A'; // from 'A' to 'A'+MAX_LEN_
  auto nextHashLargerOrEqCount = 0U, nextHashLowerCount = 0U;

  for (auto charIdx = 0U; charIdx < str.max_size(); ++charIdx) {
    str += currChar;
    ++currChar;
    nowHash = str.hash();

    nowHash >= prevHash ? ++nextHashLargerOrEqCount : ++nextHashLowerCount;
    if (SkipStatistics && nextHashLowerCount) return false;
    prevHash = nowHash;
  }
  //// Test B
  char strBuf[4U] = {0};
  HashTester::Params<std::extent<decltype(strBuf)>::value> params(strBuf);
  const auto result = HashTester::createAndTestCharSeq<HashChecker>(params, false);

  return !nextHashLowerCount && result && HashChecker::NEXT_HASH_LARGER_OR_EQ;
}

#ifdef _MSC_VER
  // GCC might NOT build with this, while MS VS 2013 will NOT build withOUT this
  const HashCodeChecker HashCodeChecker::INSTANCE;
#endif