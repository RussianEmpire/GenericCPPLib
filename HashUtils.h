#ifndef HashUtilsH
#define HashUtilsH

#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstring>

#include <time.h>

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

class HashTester {

public:
  
  typedef std::unordered_map<size_t, std::string> THashCodeToStrMap;
  
  static const auto MIN_CHAR_ = 'a';
  static const auto MAX_CHAR_ = 'z'; // 'a'-'z': 26
  static const auto ALPHABET_LEN_ = MAX_CHAR_ - MIN_CHAR_ + 1U;
  static const char* const OUT_FILE_NAME_;
  static const char* const OUT_FILE_NAME_EXT_;

  struct Stats {
    Stats(const unsigned long long int comboCount = 0ULL) throw() : comboCount(comboCount) {}

    unsigned long long int totalCount = 0ULL, duplicateCount = 0ULL, comboCount;
  };

  // Internal
  template <const size_t BufCapacity>
  struct Params {
    Params(char (&strBuf)[BufCapacity], const size_t currLen = size_t(),
           THashCodeToStrMap* const map = nullptr,
           Stats* const stats = nullptr,
           std::ofstream* const outFile = nullptr) throw()
           : currLen(currLen), strBuf(strBuf), map(map), stats(stats), outFile(outFile)
    {}

    size_t currLen = size_t();
    char (&strBuf)[BufCapacity];
    size_t lastCompletenessPercentage = size_t();

    THashCodeToStrMap* map = nullptr;
    Stats* stats = nullptr;
    std::ofstream* outFile = nullptr;
  };

  // Char Sequence Generator Type
  enum class ECCharSeqGenType {
    CSGT_ALL_VARIANTS // a^N combinations (a - alphabet power; N - length of the seq., fixed)
  };

  template <class THashFunctor, const size_t MaxStrLen = 4U>
  static double getCollisionProbability(bool& error, const bool logCollisions = true) throw() {
    char strBuf[MaxStrLen + 1U] = {0}; // even to 8: MaxStrLen + 8U - (MaxStrLen % 8U)
    strBuf[std::extent<decltype(strBuf)>::value - 1U] = '\0';
    static const auto COMBINATION_COUNT_ = static_cast<unsigned long long int>(std::pow(ALPHABET_LEN_, MaxStrLen));
    
    THashCodeToStrMap map;
    auto toReserveBucketCount
      = static_cast<size_t>(COMBINATION_COUNT_ / static_cast<size_t>(std::pow(10.0, MaxStrLen / 2.0)));
    while (true) {
      try {
        map.reserve(toReserveBucketCount);
        break;
      } catch (...) {
        toReserveBucketCount /= 2U;
      }
    }
    std::ofstream outFile;
    
    static const auto OUT_FILE_NAME_BUF_SIZE_ = 64U;
    char outFileName[OUT_FILE_NAME_BUF_SIZE_] = {0};
    if (logCollisions && !openFile(outFile, outFileName, sizeof(outFileName))) {
      error = true;
      return nan("");
    }
    Stats stats(COMBINATION_COUNT_);
    Params<std::extent<decltype(strBuf)>::value> params(strBuf, 0U, &map, &stats, &outFile);

    if (params.stats->comboCount) std::cout << std::endl;
    error = !createAndTestCharSeq<THashFunctor>(params, logCollisions); // returns false on ANY error
    if (params.stats->comboCount) std::cout << std::endl;

    if (!params.stats->duplicateCount) { // NO duplicates - empty file
      outFile.close(); // file will be auto flushed
      const auto fileDeleteResult = remove(outFileName); // on failure, a nonzero value is returned
    }
    { // Shrink-to-fit
      decltype(map) mapClearer(std::move(map)); // release mem. before flushing file
    }
    return static_cast<double>(stats.duplicateCount) / stats.totalCount * 100.0; // file will be auto flushed AND closed here
  }
  
  // Generic strategy to test auto. generated char. sequences
  //  (used to test hashes, through can be useful elsewhere)
  // [!] Will stop if mets zero hash. (AND return false) [!]
  template <class TStrFunctor, const size_t BufCapacity>
  static bool createAndTestCharSeq(Params<BufCapacity>& params, bool logCollisions = true) throw() {
    static_assert(MIN_CHAR_ < MAX_CHAR_, "Invalid char. sequence");
    static const auto LAST_CHAR_IDX_ = BufCapacity - 1U;

    logCollisions = logCollisions && params.outFile;

    const auto currIdx = params.currLen;
    const auto nextIdx = currIdx + 1U;
    const auto toReserveCount = nextIdx + 1U;
    size_t currHash = 0U;
    std::string currStr;
    decltype(std::make_pair(currHash, std::move(currStr))) currPair;
    decltype(params.map->insert(std::move(currPair))) insertResult;
    TStrFunctor strFunctor;
    auto completenessPercentage = 0U;

    auto tryInsert = [&]() throw() {
      //// Fill pair
      currStr.reserve(toReserveCount);
      currStr = params.strBuf;
      currHash = strFunctor(currStr);

      if (!params.map) return true; // skip other part
      currPair.first = currHash;
      if (logCollisions) currPair.second = currStr; // do NOT move, COPY

      try {
        insertResult = params.map->insert(std::move(currPair));
      } catch (...) {
        return false;
      };

      if (!insertResult.second) { // if NOT inserted (duplicate)
        if (params.stats) ++params.stats->duplicateCount; // if collect stats

        if (logCollisions) {
          auto& dupStr = (*insertResult.first).second;
          (*params.outFile) << currHash << ": '" << currStr << "' == '" << dupStr << "'\n";
          if (!(*params.outFile)) return false;
        }
      }
      return true;
    };
    
    static_assert(MIN_CHAR_ < MAX_CHAR_, "Invalid char codes");
    for (char currChar = MIN_CHAR_; currChar <= MAX_CHAR_; ++currChar) {
      params.strBuf[currIdx] = currChar;

      if (nextIdx < LAST_CHAR_IDX_) { // prolong seq.
        params.currLen = nextIdx;
        if (!createAndTestCharSeq<TStrFunctor, BufCapacity>(params, logCollisions)) return false;
      } else { // seq. end
        if (!tryInsert()) return false; // saving OR logging failed
        if (!currHash) return false; // force stop (skip other) [OPTIMIZATION]

        if (params.stats) { // if collect stats
          ++params.stats->totalCount;

          if (params.stats->comboCount) { // show progress
            completenessPercentage
              = static_cast<decltype(completenessPercentage)>(static_cast<double>(params.stats->totalCount)
              / params.stats->comboCount * 100.0);

            if (completenessPercentage > params.lastCompletenessPercentage &&
                !(completenessPercentage % 10U))
            { // step = 10U
              std::cout << ' ' << completenessPercentage;
              params.lastCompletenessPercentage = completenessPercentage;
            }
          }
        }
      }
    } // 'for currChar' END
    return true;
  } // 'createAndTestCharSeqA' END

private:
  
  static bool openFile(std::ofstream& outFile, char* const fileNameBuf, const size_t fileNameBufSize) throw() {
    if (!fileNameBuf || fileNameBufSize < 2U) return false;
    //// Create unique file name (ALSO clock ticks can be replaced with the time AND/OR random)
    ////  i. e. C++11 'std::chrono' AND/OR 'std::uniform_int_distribution'
    ////   ALSO C 'tmpnam' / 'tmpfile'
    strncat(fileNameBuf, OUT_FILE_NAME_, fileNameBufSize - 1U);
    static const auto STR_TIME_BUF_SIZE_ = 32U;
    char strTimeBuf[STR_TIME_BUF_SIZE_] = {0};
    // Better use C++11 http://en.cppreference.com/w/cpp/chrono/high_resolution_clock
    const auto nowTime = time(nullptr);
    sprintf(strTimeBuf, "%lli", nowTime);
    strncat(fileNameBuf, strTimeBuf, fileNameBufSize - 1U);
    strncat(fileNameBuf, OUT_FILE_NAME_EXT_, fileNameBufSize - 1U);

    outFile.open(fileNameBuf, std::ios::out | std::ios::trunc);
    return !(!outFile);
  }
};

const char* const HashTester::OUT_FILE_NAME_ = "collison_test_results#";
const char* const HashTester::OUT_FILE_NAME_EXT_ = ".txt ";

// Functor
#define ADD_HASHER_FOR(ClassName, HasherName) \
template <typename T = ClassName>\
struct HasherName {\
  auto operator()(const T& obj) const throw() -> decltype(obj.hash()) {\
    return obj.hash();\
  }\
};

// Functor
#define ADD_STD_HASHER_FOR(ClassName) \
namespace std {\
  template <>\
  class hash<ClassName> {\
  public:\
    size_t operator()(const ClassName& obj) const throw() {\
      return obj.hash();\
    }\
  };\
};

#endif // HashUtilsH