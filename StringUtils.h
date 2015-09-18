#ifndef StringUtilsH
#define StringUtilsH

// Will use ALL available options by default
// Pwd. complexity: 26 + 10 + 32 + 26 = 94^N
struct GeneratePwdOptions {
  enum ECharSetType {
    CST_ALPHA,  // latin letters; low: [97, 122] AND up: [65, 90] (total of 26 unique)
    CST_DIGITS, // arabic numbers [48, 57] (10 unique)
    CST_SYMBOLS // printable symbols: punctuation AND so on, EXCLUDING space
  };

  static const ECharSetType CHAR_SETS[];
  static const size_t CHAR_SET_COUNT_; // big AND small alphas counts as a one charset

  //// ALL codes are shown for the ANSI ASCII table
  bool useAlpha      = true;
  bool useDigits     = true;
  bool useSymbols    = true;
  bool caseSensetive = true; // use both lower AND upper case (if false - ONLY lowercase)
};
const GeneratePwdOptions DEFAULT_GENERATE_PWD_OPTIONS_;
const GeneratePwdOptions::ECharSetType GeneratePwdOptions::CHAR_SETS[] =
  {ECharSetType::CST_ALPHA, ECharSetType::CST_DIGITS, ECharSetType::CST_SYMBOLS};
const size_t GeneratePwdOptions::CHAR_SET_COUNT_ = sizeof(CHAR_SETS) / sizeof(*CHAR_SETS);

#include <cstring>
#include <functional>
#include <random>
#include <chrono>

// 'pwdBuf' should be at least (len + 1U) large
// 'len' should be >= 8U (returns empty str. if NOT)
// Returns empty str. on ANY error
// 'charPerSet' is an OUTPUT statistics (OPTIONAL)
// Complexity - linear: O(2*len)
template <const bool Optimized = true>
char* generatePwd(char* pwdBuf, const size_t len = 16U,
                  const GeneratePwdOptions& options = DEFAULT_GENERATE_PWD_OPTIONS_,
                  size_t charPerSet[GeneratePwdOptions::CHAR_SET_COUNT_] = nullptr) throw()
{
  static const auto MIN_PWD_LEN_ = 8U;
  static_assert('z' > 'a' && 'Z' > 'A' && '9' > '0', "Incorrect char codes");
  // [33, 47] U [58, 64] U [91, 96] U [123, 126]
  static const char SPEC_SYMBOLS[] = {"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"};

  if (!pwdBuf || !len) return nullptr;
  *pwdBuf = '\0';
  if (len < MIN_PWD_LEN_) return pwdBuf;

  const size_t timeSeed =
    static_cast<size_t>(std::chrono::system_clock::now().time_since_epoch().count());
  const std::mt19937 engine(timeSeed);

  const std::uniform_int_distribution<size_t> charIdxDistribution(size_t(), len - 1U);
  auto rollCharIdx = std::bind(charIdxDistribution, engine); // callable

  auto charSetCount = size_t();
  if (options.useAlpha) ++charSetCount;
  if (options.useDigits) ++charSetCount;
  if (options.useSymbols) ++charSetCount;
  
  if (!charSetCount) return pwdBuf; // error: charset NOT specified
  
  // Each set can place a strictly limited amount of chars
  const auto maxCharPerSetCount = len / charSetCount;
  // At least 1 char form each set
  const std::uniform_int_distribution<size_t> charPerSetDistribution(1U, maxCharPerSetCount);
  auto rollCharPerSetCount = std::bind(charPerSetDistribution, engine); // callable
  
  size_t localCharPerSet[GeneratePwdOptions::CHAR_SET_COUNT_] = {0}; // statistics
  auto const currCharPerSet = charPerSet ? charPerSet : localCharPerSet; // replace if NOT provided
  
  struct Randomizer_ {
    Randomizer_() throw() { // 'srand' will be called ONLY once during initialiation
      srand(static_cast<unsigned int>(time(nullptr)));
    }
  };
  static const Randomizer_ R_; // static init. is a thread safe in C++11

  auto charSetLeftCount = charSetCount;
  size_t charPlacedCount = size_t(); // total amount of chars already placed in the buf.

  auto getCharCountPerSet = [&]() throw() {
    // If last - fill the rest of the buf.
    return charSetLeftCount ? rollCharPerSetCount() : (len - charPlacedCount);
  };

  // Also updates statistics for the specified charset
  auto getRandomChar = [&](const GeneratePwdOptions::ECharSetType charSetType) throw() {
    size_t firstIdx = size_t(), lastIdx = size_t();
    switch (charSetType) {
      case GeneratePwdOptions::CST_ALPHA: lastIdx = 'z' - 'a'; break;
      case GeneratePwdOptions::CST_DIGITS: lastIdx = '9' - '0'; break;
      case GeneratePwdOptions::CST_SYMBOLS:
        lastIdx = (sizeof(SPEC_SYMBOLS) / sizeof(*SPEC_SYMBOLS)) - 2U; break; // skip '\0'
    }
    const auto idx = firstIdx + (rand() % (lastIdx - firstIdx + 1U)); // from set

    auto actualChar = '\0';
    switch (charSetType) {
      case GeneratePwdOptions::CST_ALPHA:
        // Randomize case (if neeeded)
        actualChar = options.caseSensetive ? (rand() % 2 ? 'A' : 'a') : 'a';
        actualChar += idx;
        ++currCharPerSet[0U];
      break;
      case GeneratePwdOptions::CST_DIGITS:
        actualChar = '0' + idx;
        ++currCharPerSet[1U];
      break;
      case GeneratePwdOptions::CST_SYMBOLS:
        actualChar = SPEC_SYMBOLS[idx];
        ++currCharPerSet[2U];
      break;
    }
    return actualChar;
  };
  
  // Places random count of a random chars from the specified set at random blank positions
  auto addCharsA = [&](const GeneratePwdOptions::ECharSetType charSetType) throw() {
    --charSetLeftCount;
    const auto charCount = getCharCountPerSet();
    
    size_t charIdx = size_t(); // actual idx. in the array
    // Add random chars from the curr. set
    for (size_t charNumber = size_t(); charNumber < charCount; ++charNumber) {
      while (pwdBuf[charIdx = rollCharIdx()]); // roll while NOT free space for the symbol
      pwdBuf[charIdx] = getRandomChar(charSetType);
    }
    charPlacedCount += charCount;
  };
  
  // Places random count of a random chars from the specified set at predefined positions
  //  (sequentially)
  auto addCharsB = [&](const GeneratePwdOptions::ECharSetType charSetType) throw() {
    --charSetLeftCount;
    const auto charCount = getCharCountPerSet();
    
    // Add random chars from the curr. set: O(charCount)
    for (size_t charNumber = size_t(); charNumber < charCount; ++charNumber, ++charPlacedCount) {
      pwdBuf[charPlacedCount] = getRandomChar(charSetType);
    }
  };
  
  switch (Optimized) {
    case false:
      memset(pwdBuf, 0, len + 1U);
      if (options.useDigits) addCharsA(GeneratePwdOptions::ECharSetType::CST_DIGITS);
      if (options.useSymbols) addCharsA(GeneratePwdOptions::ECharSetType::CST_SYMBOLS);
      if (options.useAlpha) addCharsA(GeneratePwdOptions::ECharSetType::CST_ALPHA);
    break;
    default: // true
      if (options.useDigits) addCharsB(GeneratePwdOptions::ECharSetType::CST_DIGITS);
      if (options.useSymbols) addCharsB(GeneratePwdOptions::ECharSetType::CST_SYMBOLS);
      if (options.useAlpha) addCharsB(GeneratePwdOptions::ECharSetType::CST_ALPHA);
      // Random shuffle: O(charPlacedCount)
      for (size_t charNumber = size_t(); charNumber < charPlacedCount; ++charNumber) {
        std::swap(pwdBuf[charNumber], pwdBuf[rollCharIdx()]);
      }
      pwdBuf[charPlacedCount] = '\0';
  }
  return pwdBuf;
}

#endif // StringUtilsH