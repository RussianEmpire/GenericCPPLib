#ifndef SynchUtilsH
#define SynchUtilsH

//// [!] Version 1.000 [!]

namespace SynchUtils {
  // Fake mutex, used as the replacer to the real mutex in the conditionally synchronized templates
  class MockMutex {
  public:
    MockMutex() = default;
    ~MockMutex() = default;

    // While 'std::mutex' is neither copyable NOR movable, this is
    MockMutex(const MockMutex&) = default;
    MockMutex(MockMutex&&) = default;

    void lock() throw() {}
    void unlock() throw() {}
  };
}

#endif // SynchUtilsH