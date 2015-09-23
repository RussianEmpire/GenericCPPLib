#ifndef TypeHelpersH
#define TypeHelpersH

#include <cstddef>
#include <cstdint>
#include <type_traits>

// bool val. to bool type
template <const bool Bool>
struct BoolType {};

template<>
struct BoolType<true> {
  typedef std::true_type Type;
};

template<>
struct BoolType<false> {
  typedef std::false_type Type;
};

#define BOOL_TYPE(BoolVal) BoolType<BoolVal>::Type

// HINT: std::add_const / std::remove_const also can be used
template <typename T, const bool Constant>
struct AddRemoveConst {};

template <typename T>
struct AddRemoveConst<T, true> {
  typedef const T Type;
};

template <typename T>
struct AddRemoveConst<T, false> {
  typedef T Type;
};

#define ADD_REMOVE_CONST(Type, StaticPredicate) AddRemoveConst<Type, StaticPredicate>::Type

// std::is_fundamental [http://www.cplusplus.com/reference/type_traits/is_fundamental/]
enum class ECFundamentalTypeTags {
  UNIDENTIFIED,
  BOOL,
  SIGNED_CHAR,
  UNSIGNED_CHAR,
  // Signedness of wchar_t is unspecified
  //  [http://stackoverflow.com/questions/11953363/wchar-t-is-unsigned-or-signed]
  WCHAR,

  //// 'char16_t' AND 'char32_t' SHOULD be a keywords since the C++11,
  ////  BUT MS VS Community 2013 Upd 5 does NOT supports that
  ////  AND specifys 'char16_t' AND 'char32_t' as a typdef aliases instead
  ////  (so they are NOT presented here)

  SIGNED_SHORT_INT,
  UNSIGNED_SHORT_INT,
  SIGNED_INT,
  UNSIGNED_INT,
  SIGNED_LONG_INT,
  UNSIGNED_LONG_INT,
  SIGNED_LONG_LONG_INT, // C++11
  UNSIGNED_LONG_LONG_INT, // C++11
  FLOAT,
  DOUBLE,
  LONG_DOUBLE,
  VOID_,
  NULLPTR // C++11 std::nullptr_t
};

template <typename T, class TypeTags = ECFundamentalTypeTags>
struct TypeTag {
  static const auto TAG = TypeTags::UNIDENTIFIED;
};

template <class TypeTags>
struct TypeTag<bool, TypeTags> {
  static const auto TAG = TypeTags::BOOL;
};

template <class TypeTags>
struct TypeTag<signed char, TypeTags> {
  static const auto TAG = TypeTags::SIGNED_CHAR;
};

template <class TypeTags>
struct TypeTag<unsigned char, TypeTags> {
  static const auto TAG = TypeTags::UNSIGNED_CHAR;
};

template <class TypeTags>
struct TypeTag<wchar_t, TypeTags> {
  static const auto TAG = TypeTags::WCHAR;
};

template <class TypeTags>
struct TypeTag<signed short int, TypeTags> {
  static const auto TAG = TypeTags::SIGNED_SHORT_INT;
};

template <class TypeTags>
struct TypeTag<unsigned short int, TypeTags> {
  static const auto TAG = TypeTags::UNSIGNED_SHORT_INT;
};

template <class TypeTags>
struct TypeTag<signed int, TypeTags> {
  static const auto TAG = TypeTags::SIGNED_INT;
};

template <class TypeTags>
struct TypeTag<unsigned int, TypeTags> {
  static const auto TAG = TypeTags::UNSIGNED_INT;
};

template <class TypeTags>
struct TypeTag<signed long int, TypeTags> {
  static const auto TAG = TypeTags::SIGNED_LONG_INT;
};

template <class TypeTags>
struct TypeTag<unsigned long int, TypeTags> {
  static const auto TAG = TypeTags::UNSIGNED_LONG_INT;
};

template <class TypeTags>
struct TypeTag<signed long long int, TypeTags> {
  static const auto TAG = TypeTags::SIGNED_LONG_LONG_INT;
};

template <class TypeTags>
struct TypeTag<unsigned long long int, TypeTags> {
  static const auto TAG = TypeTags::UNSIGNED_LONG_LONG_INT;
};

template <class TypeTags>
struct TypeTag<float, TypeTags> {
  static const auto TAG = TypeTags::FLOAT;
};

template <class TypeTags>
struct TypeTag<double, TypeTags> {
  static const auto TAG = TypeTags::DOUBLE;
};

template <class TypeTags>
struct TypeTag<long double, TypeTags> {
  static const auto TAG = TypeTags::LONG_DOUBLE;
};

template <class TypeTags>
struct TypeTag<void, TypeTags> {
  static const auto TAG = TypeTags::VOID_;
};

template <class TypeTags>
struct TypeTag<std::nullptr_t, TypeTags> {
  static const auto TAG = TypeTags::NULLPTR;
};

#define TYPE_TAG(Object) TypeTag<std::decay<decltype(Object)>::type>::TAG

// Size is in bytes
// Fixed width integer types (since C++11): http://en.cppreference.com/w/cpp/types/integer
// See also: http://www.viva64.com/en/t/0012/
template <const size_t Size, const bool Signed>
struct IntegralTypeBySize {
  static const auto TAG = ECFundamentalTypeTags::UNIDENTIFIED;
};

template<>
struct IntegralTypeBySize<1U, true> {
  typedef int8_t Type;
  static const auto TAG = TypeTag<Type, ECFundamentalTypeTags>::TAG;
};

template<>
struct IntegralTypeBySize<2U, true> {
  typedef int16_t Type;
  static const auto TAG = TypeTag<Type, ECFundamentalTypeTags>::TAG;
};

template<>
struct IntegralTypeBySize<4U, true> {
  typedef int32_t Type;
  static const auto TAG = TypeTag<Type, ECFundamentalTypeTags>::TAG;
};

template<>
struct IntegralTypeBySize<8U, true> {
  typedef int64_t Type;
  static const auto TAG = TypeTag<Type, ECFundamentalTypeTags>::TAG;
};

template<>
struct IntegralTypeBySize<1U, false> {
  typedef uint8_t Type;
  static const auto TAG = TypeTag<Type, ECFundamentalTypeTags>::TAG;
};

template<>
struct IntegralTypeBySize<2U, false> {
  typedef uint16_t Type;
  static const auto TAG = TypeTag<Type, ECFundamentalTypeTags>::TAG;
};

template<>
struct IntegralTypeBySize<4U, false> {
  typedef uint32_t Type;
  static const auto TAG = TypeTag<Type, ECFundamentalTypeTags>::TAG;
};

template<>
struct IntegralTypeBySize<8U, false> {
  typedef uint64_t Type;
  static const auto TAG = TypeTag<Type, ECFundamentalTypeTags>::TAG;
};

#endif // TypeHelpersH