#ifndef HashCodeCheckerH
#define HashCodeCheckerH

// Assister class, static singleton (C++11 synched)
// Does some work at the program launch
class HashCodeChecker {

public:
  
  // Static init. is a thread safe in C++11
  static const HashCodeChecker INSTANCE;
  
  const bool hashOfLargerStrLarger;
  
private:
  
  // Will be launched at the very beginning of the program execution
  HashCodeChecker() throw()
    : hashOfLargerStrLarger(ishashOfLargerStrLarger()) {}
  
  ~HashCodeChecker() = default;
  
  HashCodeChecker(const HashCodeChecker&) throw() = delete;
  HashCodeChecker(HashCodeChecker&&) throw() = delete;
  HashCodeChecker& operator=(const HashCodeChecker&) throw() = delete;
  HashCodeChecker& operator=(HashCodeChecker&&) throw() = delete;
  
  struct HashChecker; // forward declaration
  
  template <const bool SkipStatistics = true>
  static bool ishashOfLargerStrLarger() throw();
};

#endif // HashCodeCheckerH