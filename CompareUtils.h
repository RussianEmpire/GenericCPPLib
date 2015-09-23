#ifndef CompareUtilsH
#define CompareUtilsH

enum EComparisonType {
  EQ,
  NOT_EQ,
  LESS,
  GREATER,
  LESS_OR_EQ,
  GREATER_OR_EQ
};

template <const EComparisonType>
// Functor
struct Compare {};

template<>
struct Compare<EComparisonType::EQ> {
  template <typename T, class U>
  bool operator()(const T& first, const U& second) throw() {
    return first == second;
  }
};

template<>
struct Compare<EComparisonType::NOT_EQ> {
  template <typename T, class U>
  bool operator()(const T& first, const U& second) throw() {
    return first != second;
  }
};

template<>
struct Compare<EComparisonType::LESS> {
  template <typename T, class U>
  bool operator()(const T& first, const U& second) throw() {
    return first < second;
  }
};

template<>
struct Compare<EComparisonType::LESS_OR_EQ> {
  template <typename T, class U>
  bool operator()(const T& first, const U& second) throw() {
    return first <= second;
  }
};

template<>
struct Compare<EComparisonType::GREATER> {
  template <typename T, class U>
  bool operator()(const T& first, const U& second) throw() {
    return first > second;
  }
};

template<>
struct Compare<EComparisonType::GREATER_OR_EQ> {
  template <typename T, class U>
  bool operator()(const T& first, const U& second) throw() {
    return first >= second;
  }
};

#endif // CompareUtilsH