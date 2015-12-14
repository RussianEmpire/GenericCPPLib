#ifndef StaticallyBufferedStringLightH
#define StaticallyBufferedStringLightH

#include <cstdio>
#include <string>
#include <ostream>
#include <istream>

#include "..\..\FuncUtils.h"
#include "..\..\HashUtils.h"
#include "..\..\MemUtils.h"
#include "MathUtils.h"
#include "AllocatorInterface.h"
#include "GenericRAIterator.h"

EXEC_MEMBER_FUNC_IF_PRESENT(getHashIfKnown, size_t())
EXEC_MEMBER_FUNC_IF_PRESENT(hashAlgoID, size_t())

//// Forward declaration
class HashCodeChecker;

template<typename TElemType = char, const size_t MaxLen = 127U>
class StaticallyBufferedStringLight; // forward declaration of the template class

// 'TElemType' - char / wchar_t / char16_t / char32_t (provides POD C str. if 'TElemType' is a char)
template<typename TElemType, const size_t MaxLen>
// NOT thread safe, NO leaks, NO-throw guarantee
// [!] Prefer using this class over the POD C strs AND STL strings
//      due to the performance AND safety advantage [!]
class StaticallyBufferedStringLight {
  // Original class 'StaticallyBufferedString' provides also some other sevices
  // [!] Add std::string compatiblity AND iterators & initializer list
  //     support implementation to the original class [!]
public:
  
  typedef StaticallyBufferedStringLight TThisType;

  ADD_HASHER_FOR(TThisType, hasher)

  typedef TElemType value_type;
  // ONLY char, wchar_t, char16_t AND char32_t are supported!
  static_assert(std::is_same<char, TElemType>::value ||
                std::is_same<wchar_t, TElemType>::value ||
                std::is_same<char16_t, TElemType>::value ||
                std::is_same<char32_t, TElemType>::value,
                "'TElemType' SHOULD be 'char' / 'wchar_t' / 'char16_t' / 'char32_t'");
  typedef typename std::char_traits<TElemType> traits_type;

  typedef AllocatorInterface<TElemType> allocator_type; // mock object type
  
  typedef TElemType& reference;
  typedef const TElemType& const_reference;
  typedef TElemType* pointer;
  // Ptr. to the const. value, NOT the constant pointer!!
  typedef const TElemType* const_pointer;

  //// 'StaticallyBufferedStringLight' probably should NOT 
  ////  let it's symbols to be changed through the iterator
  ////   (as it CAN probably break some internal state logic)
  typedef GenericRAIterator<StaticallyBufferedStringLight,
                            value_type, false, false> iterator;
  typedef GenericRAIterator<StaticallyBufferedStringLight,
                            value_type, true, false> reverse_iterator;
  
  typedef GenericRAIterator<StaticallyBufferedStringLight,
                            value_type, false, true> const_iterator;
  typedef GenericRAIterator<StaticallyBufferedStringLight,
                            value_type, true, true> const_reverse_iterator;

  typedef ptrdiff_t difference_type;
  typedef size_t size_type;

  // Based on the the 'MaxLen', auto adjusted, to have a 8 byte alignment
  static const auto BUF_SIZE =
    MaxLen + 1U + (((MaxLen + 1U) % 8U) ? (8U - (MaxLen + 1U) % 8U) : 0U);
  
  // +1 for the str. terminator (always NULL-TERMINATED)
  static const auto MAX_LEN_ = BUF_SIZE - 1U;

  // Accepts null-terminated str. as an input
  StaticallyBufferedStringLight(const TElemType* const str = "") throw() {
    static_assert(!(BUF_SIZE % 8U), "Incorrect 'BUF_SIZE'");
    *this = str; // invoking 'operator=(const TElemType* const str)'
  }
  
  // Compatible with the ANY storage class which has 'TElemType' as an elem. type 
  //  AND provides the null-terminated str. by the public const member 'c_str'
  //   (like std::string, StaticallyBufferedStringLight<ANY SIZE> etc)
  template<typename TStorageType>
  StaticallyBufferedStringLight(const TStorageType& str) throw() {
    static_assert(!(BUF_SIZE % 8U), "Incorrect 'BUF_SIZE'");
    *this = str; // invoking 'operator=(const TStorageType& str)'
  }

  // Can't 'steal' resources, so just copying
  //  [http://en.cppreference.com/w/cpp/language/move_constructor]
  template<typename TStorageType>
  StaticallyBufferedStringLight(const TStorageType&& str) throw() {
    static_assert(!(BUF_SIZE % 8U), "Incorrect 'BUF_SIZE'");
    *this = str; // invoking 'operator=(const TStorageType&& str)'
  }
  
  // Copies the portion of' str' that begins at the character position 'pos' and spans 'len' characters
  //  (OR until the end of 'str' OR until the end of this. buf.).
  // Zero 'count' means to attempt to copy ALL the possible elems from a 'str' (default)
  template<typename TStorageType>
  StaticallyBufferedStringLight(const TStorageType& str, const size_t pos,
                                const size_t len = std::string::npos) throw() {
    static_assert(!(BUF_SIZE % 8U), "Incorrect 'BUF_SIZE'");
    clear();
    if (pos >= str.length()) return;
    append(str.c_str() + pos, len);
  }

  //// Special implementations with the 'nullptr_t' is required 
  ////  coze other way that type will be tried to proceeded by
  ////   'template<typename TStorageType> <some type> <some func>(const TStorageType& str)'
  ////    which is obviously does NOT support it

  StaticallyBufferedStringLight(const nullptr_t) throw() {
    static_assert(!(BUF_SIZE % 8U), "Incorrect 'BUF_SIZE'");
    *this = nullptr; // invoking 'operator=(const nullptr_t)'
  }

  ~StaticallyBufferedStringLight() = default;
  
  // Accepts null-terminated str. as an input
  StaticallyBufferedStringLight& operator=(const TElemType* const str) throw() {
    // Prevents "self-copy" error
    //  (through it does NOT checks buf. overlapping, this check is just enough)
    if (str == data_) return *this; 
    clear();
    return (*this += str); // invoking 'operator+=(const TElemType* const str)'
  }
  
  // Compatible with the ANY storage class which has 'TElemType' as an elem. type 
  //  AND provides the null-terminated str. by the public const member 'c_str'
  //   (like std::string, StaticallyBufferedStringLight<ANY SIZE> etc)
  template<typename TStorageType>
  StaticallyBufferedStringLight& operator=(const TStorageType& str) throw() {
    /* REDUNDANT due to the existing 'if (str == data_)' check
        in the 'operator=(const TElemType* const str)'

    // prevents the possible self-copying
    if (static_cast<void*>(this) == static_cast<const void*>(&str)) return;
    */
    return (*this = str.c_str()); // invoking 'operator=(const TElemType* const str)'
  }
  
  // Can't 'steal' resources, so just copying
  //  [http://en.cppreference.com/w/cpp/language/move_operator]
  template<typename TStorageType>
  StaticallyBufferedStringLight& operator=(const TStorageType&& str) throw() {
    return (*this = str.c_str()); // invoking 'operator=(const TElemType* const str)'
  }
  
  // Make ONE symbol str.
  StaticallyBufferedStringLight& operator=(const TElemType symb) throw() {
    // OPTIMIZATION HINT: remove overhead
    clear();
    push_back(symb);
    return *this;
  }

  StaticallyBufferedStringLight& operator=(const nullptr_t) throw() {
    clear();
    return *this;
  }

  // Accepts null-terminated str. as an input
  StaticallyBufferedStringLight& operator+=(const TElemType* const str) throw() {
    append(str);
    return *this;
  }
  
  // Compatible with the ANY storage class which provides the null-terminated str. 
  //  by the public const member 'c_str'
  //   (like std::string, StaticallyBufferedStringLight<ANY SIZE> etc)
  template<class TStorageType>
  StaticallyBufferedStringLight& operator+=(const TStorageType& str) throw() {
    return (*this += str.c_str()); // invoking 'operator+=(const TElemType* const str)'
  }
  
  // Adds ONE symbol
  StaticallyBufferedStringLight& operator+=(const TElemType symb) throw() {
    push_back(symb);
    return *this;
  }

  StaticallyBufferedStringLight& operator+=(const nullptr_t) throw() {
    return *this;
  }
  
  // Adds a WHOLE number in a str. representation
  //  (be carefull here, do NOT miss with the 'operator+=(const TElemType symb)')
  template<typename TValueType,
           class = typename // remove from the overload resolution to avoid an ambiguity
             std::enable_if<std::is_arithmetic<TValueType>::value &&
                            !std::is_pointer<TValueType>::value && // 'std::nullptr_t' is OK
                            !std::is_same<TValueType, TElemType>::value>::type>
  StaticallyBufferedStringLight& append(const TValueType value) throw() {
    const auto spaceLeft = std::extent<decltype(data_)>::value - length_;
    if (spaceLeft < 2U) { // NO actual space left (1 for the str. terminator)
      truncated_ = true;
      return *this;
    }
    const char* mask = "";
    auto returnFlag = false; // true if return immediately
    auto getMask = [&]() throw() {
      typedef typename TypeTag<decltype(value)>::TypeTags_ Tags;
      switch (TYPE_TAG(value)) {
        // OPTIMIZATION thoughts: reduce the mask count?
        //  use 'if std::is_floatinng<decltype(value)>' for two mask types - float|integral?

        default: assert(false); // unidentified type
        case Tags::NULLPTR: returnFlag = true; break;

        case Tags::BOOL: mask = value ? "true" : "false"; break;
        
        case Tags::SIGNED_CHAR: mask = "%hhi"; break;
        case Tags::SIGNED_SHORT_INT: mask = "%hi"; break;
        case Tags::SIGNED_INT:
        case Tags::WCHAR: // signedness of wchar_t is unspecified
          mask = "%i"; break;
        case Tags::SIGNED_LONG_INT: mask = "%li"; break;
        case Tags::SIGNED_LONG_LONG_INT: mask = "%lli"; break;

        case Tags::UNSIGNED_CHAR: mask = "%hhu"; break;
        case Tags::UNSIGNED_SHORT_INT: mask = "%hu"; break;
        case Tags::UNSIGNED_INT: mask = "%u"; break;
        case Tags::UNSIGNED_LONG_INT: mask = "%lu"; break;
        case Tags::UNSIGNED_LONG_LONG_INT: mask = "%llu"; break;

        case Tags::FLOAT:
        case Tags::DOUBLE:
          mask = "%f"; break;
        case Tags::LONG_DOUBLE: mask = "%Lf";
      }
    };
    getMask();
    if (returnFlag) return *this;
    
    #ifdef _MSC_VER // MS VS specific
      // Number of characters stored in buffer, not counting the terminating null character
      auto count = _snprintf_s(data_ + length_, spaceLeft, _TRUNCATE, mask, value);
      if (count < 0) { // if NOT enough space left
        count = spaceLeft - 1U;
        truncated_ = true;
      }
    #else
      // Number of characters that properly SHOULD have been written (except terminating null character)
      auto count = snprintf(data_ + length_, spaceLeft, mask, value);
      if (count < 0) {
        data_[length_] = '\0'; // ensure NO actual changes
        return *this; // encoding error
      }
      if ((count + 1U) > spaceLeft) { // +1 for the str. terminator
        count = spaceLeft - 1U;
        truncated_ = true;
      }
    #endif

    length_ += count;
    modified_ = true;
    return *this;
  }

  // Same as the 'append(const TValueType value)'
  template<typename TValueType,
           class = typename // remove from the overload resolution to avoid an ambiguity
             std::enable_if<std::is_arithmetic<TValueType>::value &&
                            !std::is_pointer<TValueType>::value && // 'std::nullptr_t' is OK
                            !std::is_same<TValueType, TElemType>::value>::type>
  StaticallyBufferedStringLight& operator<<(const TValueType value) throw() {
    append(value);
    return *this;
  }
  
  StaticallyBufferedStringLight& operator<<(const TElemType* const str) throw() {
    append(str);
    return *this;
  }
  
  // 'str' SHOULD points to the POD C str.
  // [!] Less effective, then 'operator==(const TElemType (&str)[ArrayElemCount])' [!]
  bool operator==(const TElemType* const str) const throw() {
    if (!*str) return !length_;
    if (str == data_) return true;
    // Probably points to the substr. of the str. contatining in 'data_'
    if (checkAddrIfInternal(str)) return false;
    
    return !strCmp<>(data_, str);
  }
  
  bool operator!=(const TElemType* const str) const throw() {
    return !(*this == str); // invoking 'operator==(const TElemType* const str)'
  }

  // 'str' SHOULD contain the POD C str.
  // [!] More efficient then 'operator==(POD C str.)' (coze providing array size),
  //  BUT less effective, then 'operator==(const TStorageType& str)' [!]
  template<const size_t ArrayElemCount>
  bool operator==(const TElemType (&str)[ArrayElemCount]) const throw() {
    if (str == data_) return true;
    // Probably points to the substr. of the str. contatining in 'data_'
    if (checkAddrIfInternal(str)) return false;
    
    if (ArrayElemCount < (length_ + 1U)) return false; // optimization
    
    const auto thisStrMemChunkSize = sizeof(*data_) * (length_ + 1U); // in bytes
    return isEqualMemD<>(data_, str, thisStrMemChunkSize);
  }

  //// Warning! 'operator<([Array])' looks like isn't called

  template<const size_t ArrayElemCount>
  bool operator!=(const TElemType (&str)[ArrayElemCount]) const throw() {
    return !(*this == str); // invoking 'operator==(const TElemType (&str)[ArrayElemCount])'
  }

  bool operator==(const nullptr_t) const throw() {
    return !length_;
  }

  bool operator!=(const nullptr_t) const throw() {
    return !(*this == nullptr); // invoking 'operator==(const nullptr_t)'
  }
  
  // Compatible with the ANY storage class which has 'TElemType' as an elem. type 
  //  AND provides the null-terminated str. by the public const member 'c_str'
  //   AND returning actual str. len. with the public const member 'length'
  //    (like std::string, StaticallyBufferedStringLight<ANY SIZE> etc)
  // [!] The most effective type of comparing strs is 
  //      comparing a one 'StaticallyBufferedStringLight' to another (coze providing hash) [!]
  template<class TStorageType>
  bool operator==(const TStorageType& str) const throw() {
    if (str.length() != length()) return false;
    
    // Using hash code. algo. ID to identify if the same algo. is used by the 'str' instance
    // OPTIMIZATION HINT: use C++14 'constexpr' here
    static const auto SAME_HASHING_ = (hashAlgoID() == hashAlgoID::ExecIfPresent(str));
    if (SAME_HASHING_) {
      const auto thisHash = getHashIfKnown(); // ONLY if already calculated
      if (thisHash) { // if this hash is relevant
        // If 'TStorageType' provides hash code 
        //  AND hash code is ALREADY calculated (cached) AND relevant
        //   for the BOTH compared objects - if codes does NOT equal -> 
        //    objects is clearly diffirent (return false)
        // Called ONLY if exists (if NOT - called surrogate which is ALWAYS return zero)
        const auto otherHash = getHashIfKnown::ExecIfPresent(str);
        // REMEMBER that hash code equivalence does NOT actually means that object are equal
        //  due to the non-nill collison probabilty
        if (otherHash && otherHash != thisHash) return false; // if other hash is known AND relevant
      }
    }
    static const auto SAME_CHAR_TYPE_ = // remove cv and ref.
      std::is_same<typename std::decay<decltype(*c_str())>::type,
                   typename std::decay<decltype(*str.c_str())>::type>::value;
    switch (SAME_CHAR_TYPE_) {
      case true: return isEqualMemD<>(data_, str.c_str(), sizeof(*data_) * length());
      // Diff. types: call 'operator==(const TOtherElemType* const str)'
      default: return *this == str.c_str();
    }
  }

  template<typename TStorageType>
  bool operator!=(const TStorageType& str) const throw() {
    return !(*this == str); // invoking 'operator==(const TStorageType& str)'
  }
  
  // HINT: we can also provide actually appended elems count
  // Zero 'count' means to attempt to copy ALL the possible elems from a 'str' (default)
  StaticallyBufferedStringLight& append(const TElemType* str, size_t count = -1) throw() {
    if (!str || !*str) return *this;
    if (length_ >= MAX_LEN_) { // buff. end
      truncated_ = true;
      return *this;
    }
    if (!count) count = std::numeric_limits<decltype(count)>::max(); // replace value

    auto const strEnd = data_ + length_;
    if (checkAddrIfInternal(str) && str < strEnd) { // if 'str' points to the substr. of 'data_'
      const decltype(count) restrictCount = strEnd - str; // restrict the count of chars to copy
      if (count > restrictCount) count = restrictCount; // update limit (if req.)
    }
    auto currElem = *str++;
    auto actuallyCopiedCount = size_t();
    do {
      // IN FUTURE: use 'push_back' instead of manually adding an elem. 
      //  (if so, req. to modify this code - remove redundant checks)
      data_[length_++] = currElem;
      if (++actuallyCopiedCount >= count) break; // stop at 'count'
      
      currElem = *str++;
      if (!currElem) break; // stop at '\0'

      if (length_ >= MAX_LEN_) { // buff. end
        truncated_ = true;
        break;
      }
    } while (true);
    data_[length_] = '\0';
    modified_ = actuallyCopiedCount > size_t();

    return *this;
  }

  void clear() throw() {
    truncated_ = false;
    modified_ = false;

    length_ = size_t();
    hash_ = size_t();

    *data_ = TElemType();
  }
  
  //// 'c_str' AND 'data' returns null-terminated str.
  //// [!] OPTIMIZATION hint: allow to provice non-const unsafe char*
  ////      which allows to provide a raw pointer like c_str + len to the funcs like 'itoa' [!]

  const TElemType* c_str() const throw() {
    return data_; // 'data_' is ALWAYS null-terminated
  }

  const TElemType* data() const throw() {
    return c_str();
  }

  //// 'length' AND 'size' returns str. size in symbols, NOT in bytes like std::string!!

  size_t length() const throw() {
    return length_;
  }

  size_t size() const throw() {
    return length();
  }

  // Returns maximum potential length the string can reach
  size_t max_size() const throw() {
    return MAX_LEN_;
  }

  // Returns the size of the storage space currently allocated for the string in terms of bytes
  size_t capacity() const throw() {
    return MAX_LEN_;
  }

  bool empty() const throw() {
    return !length_;
  }
  
  // The request is non-binding, and the container implementation is free to optimize otherwise
  //  and leave the string with a capacity greater than its size
  void shrink_to_fit() const throw() {}

  // Returns true if the specified capacity is reachable
  bool reserve(const size_t n) const throw() {
    return n <= MAX_LEN_;
  }
  
  // 'Modifier' should be 'const' OR empty
  #define AT(Modifier) \
  /* The first character in a string is denoted by a value of 0 (not 1)
  If 'pos' is too high - returns the last (on empty str. returns after end)*/\
  Modifier TElemType& at(const size_t pos) Modifier throw() {\
    if (pos >= length())\
      return (length() ? data_[length() - 1U] : *data_); /*no such char - return smth. else*/\
    return data_[pos];\
  };
  
  #define PLACEHOLDER // to remove the warning
  AT(PLACEHOLDER);
  AT(const);
  
  //// [!] If using none-const version of the 'at' OR 'operator[]' etc 
  ////      beware of the inadequate actions such as altering the actual str. len.
  ////       by placing terminator char. in the middle of a sequence [!]
  //// Bettre use 'setCharAt', which does handling this 
  ////  AND protects the instance of ANY harmfull effects

  TElemType& operator[](const size_t pos) throw() {
    return at(pos);
  }
  
  const TElemType& operator[](const size_t pos) const throw() {
    return at(pos);
  }
  
  // Returns the last elem. of the string
  TElemType& back() throw() {
    return at(length_ - 1U);
  }
  
  // Returns the last elem. of the string
  const TElemType& back() const throw() {
    return at(length_ - 1U);
  }

  // Returns the first elem. of the string
  TElemType& front() throw() {
    return at(0U);
  }
  
  // Returns the first elem. of the string
  const TElemType& front() const throw() {
    return at(0U);
  }

  // Appends character 'c' to the end of the string, increasing its length by one
  // Returns false if NOT succeeded
  //  (for example, str. terminator char. can NOT be manually added)
  bool push_back(const TElemType c) throw() {
    if (!c) return false;
    
    if (length_ >= MAX_LEN_) { // buff. end
      truncated_ = true;
      return false;
    }
    data_[length_++] = c;
    data_[length_] = TElemType();
    modified_ = true;

    return true;
  }
  
  // Erases (AND returning) the last character of the string, 
  //  effectively reducing its length by one
  TElemType pop_back() throw() {
    if (!length_) return TElemType();
    
    const char c = data_[length_ - 1U];
    data_[--length_] = TElemType();
    modified_ = true;

    return c;
  }
  
  //// [!] Hashing algo. SHOULD never be changed at the runtime (must be deterministic) [!]

  // Uses FNV-1a algorithm (ONLY for the one byte chars - char / unsigned char!!)
  size_t hash() const throw() {
    static_assert(1U == sizeof(TElemType), "'TElemType' SHOULD be 1 byte long!");

    if (modified_) { // recalc. rather then returning cached value
      hash_ = MathUtils::getFNV1aHash(data_);
      modified_ = false;
    }
    return hash_;
  }
  
  // Returns unique ID of the hashing algo. used to calculate
  //  a value returned by the 'hash' AND 'getHashIfKnown' routines
  // [!] SHOULD never return zero [!]
  // Define AND use this if you planning to compare this instance with the other instances
  //  which using the same hash code calculation algorithm [OPTIMIZATION]
  // OPTIMIZATION HINT : use C++14 'constexpr' here
  static size_t hashAlgoID() throw() {
    static const size_t ID_ = 'F' + 'N' + 'V' + '-' + '1' + 'a';
    return ID_;
  }

  // NEVER recalculates hash
  // Returns zero if actual hash is unknown OR if str. is empty
  size_t getHashIfKnown() const throw() {
    return modified_ ? size_t() : hash_;
  }

  // Manually setting '\0' is NOT allowed
  // Prolongation of the str. is NOT allowed
  bool setCharAt(const size_t pos, const char c) throw() {
    if (!c || pos >= length_) return false; // invalid char AND/OR pos
    if (c == data_[pos]) return true; // already

    data_[pos] = c;
    modified_ = true;
    return true;
  }
  
  // Constrcation, assertion OR appending strs can cause data truncation 
  //  due to the strictly limited size of the internal buffer
  //   in case of the incoming data truncation occures appropriated flag will be set 
  //    AND this func. will return true
  //     data truncation indication flag CAN be reseted by some operations
  bool truncated() const throw() {
    return truncated_;
  }
  
  bool full() const throw() {
    return size() == max_size();
  }
  
  // Returns true if the stored hash. code is NOT relevant AND should be recalculated
  bool modified() const throw() {
    return modified_;
  }

  /* 'std::string' comparing schem:
    < 0	 Either the value of the first character that does not match is lower in the compared string,
          or all compared characters match but the compared string is shorter
    > 0	 Either the value of the first character that does not match is greater in the compared string,
          or all compared characters match but the compared string is longer
  */

  // [!] Less effective, then 'operator<(const TElemType (&str)[ArrayElemCount])' [!]
  bool operator<(const TElemType* const str) const throw() {
    if (!str) return false;
    if (checkAddrIfInternal(str))
      return false; // probably points to the substr. of the str. contatining in 'data_'
    return std::ptrdiff_t() > strCmp<>(data_, str);
  }
  
  // [!] More efficient then 'operator<(POD C str.)' (coze providing array size),
  //  BUT CAN be less effective, then 'operator<(const TStorageType& str)' [!]
  // Incoming array SHOULD holds the POD C str.
  template <const size_t ArrayElemCount>
  bool operator<(const TElemType (&str)[ArrayElemCount]) const throw() {
    if (data_ == str) return false;
    if (checkAddrIfInternal(str))
      return false; // probably points to the substr. of the str. contatining in 'data_'
    // Comparing max. possible char. count
    return 0 > memcmp(data_, str, sizeof(*data_) * std::min<>(ArrayElemCount, length_ + 1U));
  }
  
  // [!] The most effective type of comparing strs is 
  //      comparing a one 'StaticallyBufferedStringLight' to another (coze providing hash) [!]
  // 'TStorageType' SHOULD provide POD C str. with it's 'data' function
  template<class TStorageType>
  bool operator<(const TStorageType& str) const throw() {
    // Using hash code. algo. ID to identify if the same algo. is used by the 'str' instance
    // OPTIMIZATION HINT: use C++14 'constexpr' here
    static const auto SAME_HASHING_ = (hashAlgoID() == hashAlgoID::ExecIfPresent(str));
    // If proved that the hash code of a larger str. is larger - 
    //  we can just check the hash code here [OPTIMIZATION]
    //   (ONLY if the hash code algo. is ALWAYS generates a lager hash for a larger str., FNV-1a is NOT)
    if (SAME_HASHING_ && HashCodeChecker::INSTANCE.hashOfLargerStrLarger) {
      // Get hash ONLY if already known [OPTIMIZATION]
      const auto thisHashCached = getHashIfKnown(), otherHashCached = str.getHashIfKnown();
      if (thisHashCached && otherHashCached && (thisHashCached != otherHashCached)) {
        // Equal caches does NOT necessary means the strs is actually equal, 
        //  due to collison probability
        return thisHashCached < otherHashCached;
      }
    }
    static const auto SAME_CHAR_TYPE_ = // remove cv and ref.
      std::is_same<typename std::decay<decltype(*c_str())>::type,
                   typename std::decay<decltype(*str.c_str())>::type>::value;
    switch (SAME_CHAR_TYPE_) {
      case true:
        if (static_cast<const void*>(data_) == static_cast<const void*>(str.data())) return false;
        return 0 > memcmp(data(), str.data(),
                          sizeof(*data_) * (std::min<>(length(), str.length()) + 1U));
      default:
        return *this < str.data(); // diff. types: call 'operator<(const TOtherElemType* const str)'
    }
  }

  iterator begin() throw() {
    return std::move(iterator(this));
  }

  iterator end() throw() {
    iterator iter(this);
    return std::move(iter += length()); // to past-the-end pos.
  }

  const_iterator begin() const throw() {
    return std::move(const_iterator(this));
  }

  const_iterator end() const throw() {
    const_iterator iter(this);
    return std::move(iter += length()); // to past-the-end pos.
  }

  reverse_iterator rbegin() throw() {
    return std::move(reverse_iterator(this, true));
  }

  reverse_iterator rend() throw() {
    reverse_iterator iter(this, true);
    return std::move(iter += length()); // to before-begin pos.
  }

  const_reverse_iterator rbegin() const throw() {
    return std::move(const_reverse_iterator(this, true));
  }

  const_reverse_iterator rend() const throw() {
    const_reverse_iterator iter(this, true);
    return std::move(iter += length()); // to before-begin pos.
  }

  const_iterator cbegin() const throw() {
    return std::move(begin());
  }

  const_iterator cend() const throw() {
    return std::move(end());
  }

  const_reverse_iterator crbegin() const throw() {
    return std::move(rbegin());
  }

  const_reverse_iterator crend() const throw() {
    return std::move(rend());
  }
  
private:
  
  // Returns true if the provided address lies withing the internal buffer
  // Used to check buffer overlapping
  bool checkAddrIfInternal(const TElemType* const addr) const throw() {
    // Better compare two void* casted ptrs
    return (addr >= data_ && addr < (data_ + std::extent<decltype(data_)>::value));
  }
  
  // True if the last operation modifying the str.
  //  leads to the data truncation due to the hard storage limitation
  bool truncated_;
  // Internal state flag (caching support), true if the hash. code should be recalculated
  mutable bool modified_;

  size_t length_;
  mutable size_t hash_;

  TElemType data_[BUF_SIZE];
};

namespace std {

  template<typename TElemType, const size_t MaxLen>
  bool operator==(const std::string& dStr,
                  const StaticallyBufferedStringLight<TElemType, MaxLen>& sStr) throw() {
    return sStr == dStr;
  }

  template<typename TElemType, const size_t MaxLen>
  bool operator==(const TElemType* const cStr,
                  const StaticallyBufferedStringLight<TElemType, MaxLen>& sStr) throw() {
    return sStr == cStr;
  }
  
  template<typename TElemType, const size_t MaxLen>
  std::ostream& operator<<(std::ostream& stream,
                           const StaticallyBufferedStringLight<TElemType, MaxLen>& str) {
    stream << str.c_str(); // OPTIMIZATION: reserve internal buffer first?
    return stream;
  }
  
  template<typename TElemType, const size_t MaxLen>
  std::istream& operator>>(std::istream& stream,
                           StaticallyBufferedStringLight<TElemType, MaxLen>& str) {
    auto symb = stream.get(); // skip first (it is '\n')
    while (true) {
      symb = stream.get(); // on EOF - 'failbit' flag is set
      switch (symb) {
        case '\n': case '\r': return stream; // escape char.
      }
      if (!stream) break; // true if either 'failbit' or 'badbit' flag is set
      if (!str.push_back(symb)) break;
    }
    return stream;
  }
};

typedef StaticallyBufferedStringLight<> StrLight;

// Assister class, static singleton (C++11 synched)
// Does some work at the program launch
class HashCodeChecker {

public:

  // Static init. is a thread safe in C++11
  static const HashCodeChecker INSTANCE;

  const bool hashOfLargerStrLarger = false;

private:

  // Will be launched at the very beginning of the program execution
  HashCodeChecker() throw()
    : hashOfLargerStrLarger(ishashOfLargerStrLarger()) {}

  ~HashCodeChecker() = default;

  HashCodeChecker(const HashCodeChecker&) throw() = delete;
  HashCodeChecker(HashCodeChecker&&) throw() = delete;
  HashCodeChecker& operator=(const HashCodeChecker&) throw() = delete;
  HashCodeChecker& operator=(HashCodeChecker&&) throw() = delete;

  struct HashChecker {
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
    static size_t STATS[2U];
    static size_t PREV_HASH; // will be initially zero as a static
    static bool NEXT_HASH_LARGER_OR_EQ; // initially true!!
    static StaticallyBufferedStringLight<char, 7U> STR;
  };

  template <const bool SkipStatistics = true>
  static bool ishashOfLargerStrLarger() throw() {
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
};

const HashCodeChecker HashCodeChecker::INSTANCE;

bool HashCodeChecker::HashChecker::COLLECT_STATS;
size_t HashCodeChecker::HashChecker::STATS[2U];
size_t HashCodeChecker::HashChecker::PREV_HASH;
bool HashCodeChecker::HashChecker::NEXT_HASH_LARGER_OR_EQ = true;
StaticallyBufferedStringLight<char, 7U> HashCodeChecker::HashChecker::STR;

#endif // StaticallyBufferedStringLightH