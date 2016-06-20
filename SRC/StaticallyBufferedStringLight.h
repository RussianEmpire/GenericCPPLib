#ifndef StaticallyBufferedStringLightH
#define StaticallyBufferedStringLightH

//// [!] Version 1.044 [!]

#include "FuncUtils.h"
#include "HashUtils.h"
#include "MemUtils.h"
#include "MathUtils.h"
#include "AllocatorInterface.h"
#include "GenericRAIterator.h"
#include "HashCodeChecker.h"

//// Define guard used to avoid multiple definitions

#ifndef getHashIfKnown_EXEC_MEMBER_FUNC_IF_PRESENT_
  #define getHashIfKnown_EXEC_MEMBER_FUNC_IF_PRESENT_
  EXEC_MEMBER_FUNC_IF_PRESENT(getHashIfKnown, size_t())
#endif

#ifndef hashAlgoID_EXEC_MEMBER_FUNC_IF_PRESENT_
  #define hashAlgoID_EXEC_MEMBER_FUNC_IF_PRESENT_
  EXEC_MEMBER_FUNC_IF_PRESENT(hashAlgoID, size_t())
#endif

#ifndef setHash_EXEC_MEMBER_FUNC_IF_PRESENT_
  #define setHash_EXEC_MEMBER_FUNC_IF_PRESENT_
  EXEC_MEMBER_FUNC_IF_PRESENT(setHash, false)
#endif

//// Forward declaration
class HashCodeChecker;

template<typename TElemType = char, const size_t MaxLen = 127U>
class StaticallyBufferedStringLight; // forward declaration of the template class

// 'TElemType' - char / wchar_t / char16_t / char32_t (provides POD C str. if 'TElemType' is a char)
// [!] WARNING! Hash calc. mechanic currently would only work correct woth the classic chars [!]
template<typename TElemType, const size_t MaxLen>
// NOT thread safe, NO leaks, NO-throw guarantee
// [!] Prefer using this class over the POD C strs AND STL strings
//      due to the performance AND safety advantage [!]
class StaticallyBufferedStringLight {
  // Original class 'StaticallyBufferedString' provides also some other sevices
  // [!] Add std::string compatiblity AND iterators & initializer list
  //     support implementation to the original class [!]
public:
  
  // Share access
  template<typename TOtherElemType, const size_t OtherMaxLen>
  friend class StaticallyBufferedStringLight;

  typedef StaticallyBufferedStringLight TThisType;

  ADD_HASHER_FOR(TThisType, hasher)

  typedef TElemType value_type;
  // ONLY char, wchar_t, char16_t AND char32_t are supported!
  static_assert(std::is_same<char, TElemType>::value ||
                std::is_same<wchar_t, TElemType>::value ||
                std::is_same<char16_t, TElemType>::value ||
                std::is_same<char32_t, TElemType>::value,
                "'TElemType' SHOULD be 'char' / 'wchar_t' / 'char16_t' / 'char32_t'");
  //// [?!] Is FNV1a works correclty ONLY wit the 1 byt chars?? [?!]
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
  ////  TO DO: provide a 'rehash' (recalc hash) / 'restore' (object state - recalc len. etc)
  ////   OR provide a proxy object by the subscript operator?? NOT sure if it is really needed
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
  static const auto BUF_SIZE = // better align to the sizeof(std::uintptr_t)
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
  
  // Replace the default generated version (just a copy of the templated version)
  StaticallyBufferedStringLight(const StaticallyBufferedStringLight& str) throw() {
    static_assert(!(BUF_SIZE % 8U), "Incorrect 'BUF_SIZE'");
    *this = str; // invoking 'operator=(const TStorageType& str)'
  }
  
  // Can't 'steal' resources, so just copying
  //  [http://en.cppreference.com/w/cpp/language/move_constructor]
  template<typename TStorageType>
  StaticallyBufferedStringLight(const TStorageType&& str) throw() {
    static_assert(!(BUF_SIZE % 8U), "Incorrect 'BUF_SIZE'");
    *this = std::move(str); // invoking 'operator=(const TStorageType&& str)'
  }

  // Replace the default generated version (just a copy of the templated version)
  StaticallyBufferedStringLight(const StaticallyBufferedStringLight&& str) throw() {
    static_assert(!(BUF_SIZE % 8U), "Incorrect 'BUF_SIZE'");
    *this = std::move(str); // invoking 'operator=(const TStorageType&& str)'
  }
  
  // Copies the portion of' str' that begins at the character position 'pos' and spans 'len' characters
  //  (OR until the end of 'str' OR until the end of this. buf.).
  // Zero 'count' means to attempt to copy ALL the possible elems from a 'str' (default)
  template<typename TStorageType>
  StaticallyBufferedStringLight(const TStorageType& str, const size_t pos,
                                const size_t len = std::string::npos) throw() {
    static_assert(!(BUF_SIZE % 8U), "Incorrect 'BUF_SIZE'");
    clear();
    const auto otherLen = str.length();
    if (pos >= otherLen) return;
    append(str.c_str() + pos, len);
    tryShareHash(str); // ONLY if fully copied
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
  //  AND provides null-terminated str. by the public const member 'c_str'
  //   (like std::string, StaticallyBufferedStringLight<ANY SIZE> etc)
  template<typename TStorageType>
  StaticallyBufferedStringLight& operator=(const TStorageType& str) throw() {
    // Raw copy should be faster [OPTIMIZATION]
    if (!tryRawCopyFrom(str)) { // if NOT - do standart copy (with the hash calc.)
      *this = str.c_str(); // invoking 'operator=(const TElemType* const str)'
      tryShareHash(str);
    }
    return *this;
  }

  // Replace the default generated version
  StaticallyBufferedStringLight& operator=(const StaticallyBufferedStringLight& str) throw() {
    // Invoking 'operator=(const TStorageType& str)'
    return operator=<StaticallyBufferedStringLight>(str);
  }

  // Can't 'steal' resources (transfer ownership, responsibility AND control), so just copying
  //  (does NOT alters an incoming object, allowing so to past a const)
  //  [http://en.cppreference.com/w/cpp/language/move_operator]
  // [!] A move assignment operator of class T is a non-template non-static member function
  //      with the name 'operator=' that takes exactly one parameter of type
  //       'T&&', 'const T&&', 'volatile T&&', or 'const volatile T&&' [!]
  // [!] Copying data, so works MUCH slower then the real move of the dynamic objects (std::string) [!]
  template<typename TStorageType>
  StaticallyBufferedStringLight& operator=(const TStorageType&& str) throw() {
    // Note that a trivial (NOT the default!) move assignment operator performs the same action
    //  as the trivial copy assignment operator, that is, makes a copy of the object
    *this = str; // invoking 'operator=(const TStorageType& str)'
    return *this;
  }
  
  // Replace the default generated version
  StaticallyBufferedStringLight& operator=(const StaticallyBufferedStringLight&& str) throw() {
    // Invoking 'operator=(const TStorageType&& str)'
    return operator=<StaticallyBufferedStringLight>(std::move(str));
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
  template<class TStorageType,
           typename = typename std::enable_if<std::is_class<TStorageType>::value>::type>
  StaticallyBufferedStringLight& operator+=(const TStorageType& str) throw() {
    *this += str.c_str(); // invoking 'operator+=(const TElemType* const str)'
    tryShareHash(str);
    return *this;
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
  StaticallyBufferedStringLight& append(const TValueType value,
                                        size_t* const actuallyAppended = nullptr) throw()
  {
    if (actuallyAppended) *actuallyAppended = size_t(); // reset
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
    
    auto const originEnd = data_ + length_;
    #ifdef _MSC_VER // MS VS specific
      // Number of characters stored in buffer, not counting the terminating null character
      auto count = _snprintf_s(originEnd, spaceLeft, _TRUNCATE, mask, value);
      if (count < 0) { // if NOT enough space left
        count = spaceLeft - 1U;
        truncated_ = true;
      }
    #else // NOT MS VS
      // Number of characters that properly SHOULD have been written (except terminating null character)
      auto count = snprintf(originEnd, spaceLeft, mask, value);
      if (count < 0) {
        data_[length_] = '\0'; // ensure NO actual changes
        return *this; // encoding error
      }
      if ((count + 1U) > spaceLeft) { // +1 for the str. terminator
        count = spaceLeft - 1U;
        truncated_ = true;
      }
    #endif
    // If actually updated AND hash WAS already known -
    //  update hash, using accumulation mechanics [OPTIMIZATION]
    if (!modified_ && count > 0) // accumulte if was't empty
      hash_ = MathUtils::getFNV1aHash(originEnd, hash_, size_t() < length_);
    // [!] ^Would NOT work correctly with the multibyte chars, as stoppes on the first zero byte [!]
    length_ += count;
    if (actuallyAppended) *actuallyAppended = static_cast<size_t>(count);
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
  
  //// OPTIMIZATION/REDESIGN HINT: add parallel hash. code calc. during the strs. comparing
  ////  (AND other similar funcs)
  //// Partial hash. can be also calculated (using 'std::pair' like 'size_t charIdx', 'size_t partHash')

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
    // HINT: static. check also if the storage str. type size <= sizeof(size_t) (to fit hash.)
    static const auto SAME_HASHING_ = (hashAlgoID() == hashAlgoID::ExecIfPresent(str));
    if (SAME_HASHING_) {
      if (!modified_) { // if this hash is relevant
        // If 'TStorageType' provides hash code 
        //  AND hash code is ALREADY calculated (cached) AND relevant
        //   for the BOTH compared objects - if codes does NOT equal -> 
        //    objects is clearly diffirent (return false)
        // Called ONLY if exists (if NOT - called surrogate which is ALWAYS return zero)
        auto otherHashKnown = false;
        const auto otherHash = getHashIfKnown::ExecIfPresent(str, otherHashKnown);
        // REMEMBER that hash code equivalence does NOT actually means that object are equal
        //  due to the non-nill collison probabilty
        if (otherHashKnown && otherHash != hash_) return false; // other hash is known AND relevant
      }
    }
    static const auto SAME_CHAR_TYPE_ = // remove cv and ref.
      std::is_same<typename std::decay<decltype(*c_str())>::type,
                   typename std::decay<decltype(*str.c_str())>::type>::value;
    auto result = false;

    switch (SAME_CHAR_TYPE_) {
      case true: result = isEqualMemD<>(data_, str.c_str(), sizeof(*data_) * length()); break;
      // Diff. types: call 'operator==(const TOtherElemType* const str)'
      default: result = (*this == str.c_str());
    }
    if (SAME_HASHING_ && result) { // uses same hashig algo. AND equal
      if (!modified_) setHash::ExecIfPresent(str, hash_); // if relevant - share
    }
    return result;
  }

  template<typename TStorageType>
  bool operator!=(const TStorageType& str) const throw() {
    return !(*this == str); // invoking 'operator==(const TStorageType& str)'
  }
  
  // [!] OPTIMIZATION: better use intrinsic form of the 'strcpy' here
  //      as it SHOULD have an intrinsic form on the ALL architectures
  //       (https://msdn.microsoft.com/ru-ru/library/5704bbxw(v=vs.100).aspx) [!]
  // HINT: we can also provide actually appended elems count
  // Zero 'count' means to attempt to copy ALL the possible elems from a 'str' (default)
  StaticallyBufferedStringLight& append(const TElemType* str, size_t count = -1,
                                        size_t* const actuallyAppended = nullptr) throw() {
    if (actuallyAppended) *actuallyAppended = size_t(); // reset
    if (!str || !*str) return *this;
    if (length_ >= MAX_LEN_) { // buff. end
      truncated_ = true;
      return *this;
    }
    if (!count) count = std::numeric_limits<decltype(count)>::max(); // replace value [!IMPORTANT!]

    auto const strEnd = data_ + length_; // original
    if (checkAddrIfInternal(str) && str < strEnd) { // if 'str' points to the substr. of 'data_'
      const decltype(count) restrictCount = strEnd - str; // restrict the count of chars to copy
      if (count > restrictCount) count = restrictCount; // update limit (if req.)
    }
    auto actuallyCopiedCount = size_t();
    do {
      if (!push_back(*str++)) break;
      if (++actuallyCopiedCount >= count) break; // stop at 'count'
    } while (true);
    if (actuallyAppended) *actuallyAppended = actuallyCopiedCount;
    return *this;
  }

  void clear() throw() {
    truncated_ = false;
    modified_ = false;

    length_ = size_t();
    hash_ = size_t(); // hash of an empty str. SHOULD be zero

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
  
  // Returns false at ANY error [ALSO CAN set truncation flag on invalid params]
  // If 'n' < curr. str. len. - the str. is shortened [AND the 'c' is ignored]
  //  otherwise the str. is extended with the 'c' symbols [if 'c' is NOT a str. terminator]
  bool resize(size_t n, const TElemType c) throw() {
    if (n == length()) return true; // do nothing
    if (!n) {
      clear();
      return true;
    }
    if (n < length()) { // smaller, BUT NOT zero
      length_ = n;
      data_[length_] = TElemType();
      modified_ = true;
      truncated_ = false;
      return true;
    }
    //// 'n' is clearly bigger, then the curr. len.
    if (full()) {
      truncated_ = true;
      return false; // NO space
    }
    if (!c) return false; // can NOT append NULL chars
    if (n > max_size()) { // too large
      n = max_size(); // reduce
      truncated_ = true;
    } else truncated_ = false;
    n -= length(); // how many we need to append
    for (; n; --n) assert(push_back(c)); // SHOULD be NO problems here
    return true;
  }

  // 'Modifier' should be 'const' OR empty
  #define AT(Modifier, CompromiseHashFlag) \
  /* The first character in a string is denoted by a value of 0 (not 1)
  If 'pos' is too high - returns the last (on empty str. returns after end)*/\
  Modifier TElemType& at(const size_t pos) Modifier throw() {\
    if (pos >= length())\
      return (length() ? data_[length() - 1U] : *data_); /*no such char - return smth. else*/\
    if (CompromiseHashFlag)\
      modified_ = true; /* compromise hash, as there is NO guarantee the content was NOT changed */\
    return data_[pos];\
  };
  
  #define PLACEHOLDER // to remove the warning
  AT(PLACEHOLDER, true); // [!] compromises hash, so use ONLY if you really want to change smth. [!]
  AT(const, false); // use when you want to just access a specific symbol [does NOT compromises hash]
  
  //// [!] If using none-const version of the 'at' OR 'operator[]' etc 
  ////      beware of the inadequate actions such as altering the actual str. len.
  ////       by placing terminator char. in the middle of a sequence [!]
  //// Better use 'setCharAt', which does handling this 
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
    return at(size_t());
  }
  
  // Returns the first elem. of the string
  const TElemType& front() const throw() {
    return at(size_t());
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

    // If hash WAS already known - update, using accumulation mechanics [OPTIMIZATION]
    if (!modified_) {
      if (decltype(length_)(1U) == length_)// if WAS empty 
        hash_ = MathUtils::getFNV1aStdOffsetBasis(); // prepare hash
      MathUtils::FNV1aAccumulate(hash_, c); // add last
    }
    return true;
  }
  
  // Erases (AND returning) the last character of the string, 
  //  effectively reducing its length by one
  TElemType pop_back() throw() {
    if (!length_) return TElemType();
    
    const auto c = data_[--length_];
    data_[length_] = TElemType();
    // [!] We can NOT simply rollback the hash here, as the number could be easily overflowed
    //      during the hash code calculation (deaccumulation mechanics MAY did NOT work correctly) [!]
    if (length_ <= decltype(length_)()) {
      clear();
    } else modified_ = true;

    return c;
  }
  
  //// [!] Hashing algo. SHOULD never be changed at the runtime (must be deterministic) [!]

  // Uses FNV-1a algorithm (ONLY for the one byte chars - char / unsigned char!!)
  size_t hash() const throw() {
    static_assert(1U == sizeof(TElemType), "'TElemType' SHOULD be 1 byte long!");

    if (modified_) { // recalc. rather then returning cached value
      hash_ = MathUtils::getFNV1aHash(data_); // [!] FNV-1a algo. used NOT ONLY in that func. [!]
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
    // OPTIMIZATION HINT: use C++11 'constexpr' here
    static const size_t ID_ = 'F' + 'N' + 'V' + '-' + '1' + 'a';
    return ID_;
  }

  // Used in some specific optimizations
  //  (do NOT alter unless you are clearly understands what are you doing)
  static bool isIdealHash() throw() {
    /*
    For a 32 bit (4 byte) hash AND
     a str. which consists of 10 arabic numbers (0..9),
      26 low XOR upper (ONLY ONE of them) case english symbols (a..z OR A..Z) AND
       32 types of specific characters like sentence delimiters (.!?) AND others (@#$%^ etc)
        (total of 68 unique chars)
         count of unique combinations of 32 bits is 2^32
          (^ means power, NOT bitwise XOR) = 4 294 967 296

    So we got a simple equaition: 4 294 967 296 = 68^N
     => N = log68(4 294 967 296)

    According to the logarithm transformation rule 'logX(Y) = logZ(Y)/logZ(X)'
     (where Z is some positive natural number)
      transforms the equaition: N = log10(4 294 967 296) / log10(68)

    The answer is: N = 9,633 / 1,833 ~ 5

    So within this restrictions to the available alphabet AND hash size,
     NO hashing algo. could produce unqie hashes to the strings 6+ symbols long
    */
    return false; // still FNV-1a seems to have NO collisions on low-length (1-4) strs
                  //  (further testing & researching needed)
  }

  // NEVER recalculates hash
  // Returns zero (AND sets 'isKnown' to false) if actual hash is unknown OR if str. is empty
  size_t getHashIfKnown(bool& isKnown) const throw() {
    // See https://en.wikipedia.org/wiki/Comma_operator#Complex_return
    return modified_ ? (isKnown = false, size_t()) : (isKnown = true, hash_);
  }

  // [!] Unsafe! Do NOT use OR use at your own risk [!]
  bool setHash(const size_t hash) const throw() {
    if (!modified_) // curr. hash value is relevant
      assert(hash_ == hash); // DEBUG check ONLY
    // Check actual hash is equal to new [ONLY in the debug mode]
    assert((modified_ = true,       // compromise, force recalc.
            this->hash() == hash)); // check new (AND old) hash is correct

    hash_ = hash;
    modified_ = false;
    return true;
  }

  // Manually setting '\0' is NOT allowed
  // Prolongation of the str. is NOT allowed
  bool setCharAt(const size_t pos, const char c) throw() {
    if (!c || pos >= length_) return false; // invalid char AND/OR pos
    if (c == data_[pos]) return true; // already

    data_[pos] = c;
    // NO way to easily rehash here
    //  (store a table of the intermediate hashes to simplify the hash code recalc.??)
    modified_ = true;
    return true;
  }
  
  // Construction, assertion OR appending strs can cause data truncation 
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
      if (!modified_) { // if this hash is already known
        auto otherHashKnown = false;
        const auto otherHashCached = getHashIfKnown::ExecIfPresent(str, otherHashKnown);
        if (otherHashKnown) { // other cached too
          // Equal caches does NOT necessary means the strs is actually equal, 
          //  due to collison probability
          return hash_ < otherHashCached;
        }
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

  //// [!] Using a none-const iterators to change the str. content
  ////      CAN break hash caching mechanics, so use this at your own risk [!]
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
  
  template<class TStorageType>
  // If possible; strs. SHOULD be equal AND use the same hashing algo!
  bool tryShareHash(const TStorageType& storage) const throw() {
    // Using hash code. algo. ID to identify if the same algo. is used by the 'storage' instance
    // OPTIMIZATION HINT: use C++11 'constexpr' here
    // HINT: static. check also if the storage str. type size <= sizeof(size_t) (to fit hash.)
    static const auto SAME_HASHING_ = (hashAlgoID() == hashAlgoID::ExecIfPresent(storage));
    if (!SAME_HASHING_) return false; // diff. algo.

    if (static_cast<const void*>(this) == static_cast<const void*>(std::addressof(storage)))
      return true; // same obj.

    if (modified_ || truncated_ || length() != storage.length())
      return false; // NOT relevant hash, NOT fully copied, NOT equal
    
    return setHash::ExecIfPresent(storage, hash_); // if relevant - share
  }

  template<typename TStorageType>
  // Compatible with the ANY storage class which has 'TElemType' as an elem. type 
  //  AND provides null-terminated str. by the public const member 'c_str'
  //   (like std::string, StaticallyBufferedStringLight<ANY SIZE> etc)
  // Returns false if impossible (use standart 'operator=(const TElemType* const str)' then)
  bool tryRawCopyFrom(const TStorageType& str) throw() {
    /* The main idea
    If 'TStorageType' provides hash code [+other conditions, see below]
     AND hash code is ALREADY calculated (cached) AND relevant
      we does NOT need to do step-by-step copying (with the hash code calculation AND then sharing)
       we can just fast copy whole memory chunk AND inherit other fields value [OPTIMIZATION]
    */
    // 'std::decay<const T&>' == 'T'
    if (!std::is_same<typename std::decay<decltype(*str.c_str())>::type, TElemType>::value)
      return false; // diff. elem. type (diff. memory representation)
    
    // OPTIMIZATION HINT: use C++11 'constexpr' here
    static const auto SAME_HASHING_ = (hashAlgoID() == hashAlgoID::ExecIfPresent(str));
    if (!SAME_HASHING_) return false; // diff. algos
    
    if (static_cast<const void*>(this) == static_cast<const void*>(std::addressof(str)))
      return true; // same obj. (nothing to do)
    
    auto otherHashKnown = false;
    const auto otherHash = getHashIfKnown::ExecIfPresent(str, otherHashKnown);
    if (!otherHashKnown) return false; // if other hash is NOT known NOR relevant
    
    const auto otherLen = str.length();
    if (otherLen > max_size()) return false; // could NOT copy ALL the data
    
    // Rare AND specific optimization, skipped in a very most cases
    // If NO collisions AND BOTH hash AND length of the two objects are equal
    //  [AND same hash algo. is used AND both hashes is known AND relevant]
    //   considering this objects already equal too (so NO action required, skip copying)
    if (isIdealHash() && length() == otherLen && !modified_ && hash_ == otherHash) return true;
    // [!] REMEMBER that hash code equivalence does NOT actually means that object are equal
    //      due to the non-nill collison probabilty [!]

    #ifndef _CRT_SECURE_NO_WARNINGS
      #define CRT_SECURE_NO_WARNINGS_UNDEF_ // if NOT already defined
      #define _CRT_SECURE_NO_WARNINGS       // MS VS specific (get rid of warning)
    #endif
    
    // 'std::memcpy' is the fastest library routine for memory-to-memory copy
    //  (SHOULD have an intrinsic form - use '/Oi' MS VS compiler command)
    //  It is usually more efficient than 'std::strcpy', which must scan the data it copies
    //   OR 'std::memmove', which must take precautions to handle overlapping inputs
    std::memcpy(data_, str.c_str(), sizeof(TElemType) * (otherLen + 1U)); // incl. str. terminator
    
    #ifdef CRT_SECURE_NO_WARNINGS_UNDEF_
      #undef _CRT_SECURE_NO_WARNINGS
    #endif
    
    truncated_ = false;
    modified_ = false;
    length_ = otherLen;
    hash_ = otherHash;

    return true;
  }

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
  mutable size_t hash_; // treat hash. code properly on adding new str. changing routines

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

#endif // StaticallyBufferedStringLightH