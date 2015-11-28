#ifndef PerformanceTesterH
#define PerformanceTesterH

#include <chrono>
#include <iostream>

class PerformanceTester {

public:
  
  struct TestResults {
    typedef decltype(std::chrono::system_clock::now()) TTimePoint;
    typedef decltype((TTimePoint() - TTimePoint()).count()) TTimeCount;

    void clear() throw() {
      time1 = TTimeCount(), time2 = TTimeCount();
    }

    TTimeCount time1 = TTimeCount(), time2 = TTimeCount();
  };

  // Returns diff. (2/1)
  template <class TFunctor1, class TFunctor2, const bool onScreen = true>
  static double test(TFunctor1& subj1, TFunctor2& subj2,
                     const size_t testCount, TestResults& results)
  {
    results.clear();
    if (!testCount) return 0.0;
    
    auto time1 = TestResults::TTimePoint(), time2 = TestResults::TTimePoint();
    auto testIdx = size_t();
    
    auto testSubj = [&](const bool isSecondSubj) throw() {
      time1 = std::chrono::system_clock::now();
      for (testIdx = size_t(); testIdx < testCount; ++testIdx) {
        switch (isSecondSubj) {
          case true: subj2(); break;
          default: subj1(); // false
        }
      }
      time2 = std::chrono::system_clock::now();

      return (time2 - time1).count();
    };

    results.time1 = testSubj(false);
    results.time2 = testSubj(true);
    const auto diff = static_cast<double>(results.time2) / results.time1;
    
    if (onScreen) {
      auto const potfix = (diff < 1.0) ? "faster" : "slower";
      const auto diffReinterpreted = (diff < 1.0) ? (1.0 / diff) : diff;
      std::cout << "\nSecond is " << diffReinterpreted << " times " << potfix << std::endl;
    }
    return diff;
  }
};

#endif // PerformanceTesterH