#ifndef PerformanceTesterH
#define PerformanceTesterH

//// [!] Version 1.002 [!]

#include <ctime>
#include <cassert>

#include <chrono>
#include <iostream>

class PerformanceTester {

public:
  
  struct TestResults {
    typedef decltype(std::chrono::steady_clock::now()) TTimePoint;
    typedef decltype((TTimePoint() - TTimePoint()).count()) TTimeCount;
    
    void clear() throw() {
      time1 = TTimeCount(), time2 = TTimeCount();
    }
    
    volatile TTimeCount time1 = TTimeCount(), time2 = TTimeCount();
  };
  
  // Returns diff. (2/1)
  template <class TFunctor1, class TFunctor2, const bool onScreen = true>
  static long double test(TFunctor1& subj1, TFunctor2& subj2,
                          const size_t testCount, TestResults& results) {
    results.clear();
    if (!testCount) return 0.0L;
    
    auto time1 = TestResults::TTimePoint(), time2 = TestResults::TTimePoint();
    volatile clock_t cTime1, cTime2; // use also old C timers, as the 'chrono' is broken in the MS VS 2013
    volatile long long int cCounts[2U];
    volatile auto testIdx = size_t();
    
    static_assert(sizeof(clock_t) >= 4U, "Time duration number type is too small");
    static_assert(sizeof((time2 - time1).count()) >= 4U, "Time duration number type is too small");
    auto testSubj = [&](volatile const bool isSecondSubj, volatile long long int& cCount) throw() {
      cTime1 = clock(); // returns the processor time (in clock ticks) consumed by the program
      time1 = std::chrono::steady_clock::now();
      for (testIdx = size_t(); testIdx < testCount; ++testIdx) {
        switch (isSecondSubj) {
          case true: subj2(); break;
          default: subj1(); // false
        }
      }
      time2 = std::chrono::steady_clock::now();
      cTime2 = clock();
      
      assert(cTime2 > cTime1);
      cCount = cTime2 - cTime1;
      return (time2 - time1).count();
    };
    
    if (onScreen) std::cout << "\nTesting...";
    results.time1 = testSubj(false, cCounts[0U]);
    if (onScreen) std::cout << " 1/2";
    results.time2 = testSubj(true, cCounts[1U]);
    if (onScreen) std::cout << " 2/2";
    volatile const auto diff = static_cast<long double>(results.time2) / results.time1;
    
    if (onScreen) {
      volatile auto const potfix = (diff < 1.0L) ? "faster" : "slower";
      volatile const auto diffReinterpreted = (diff < 1.0L) ? (1.0L / diff) : diff;
      std::cout << ". Second is " << diffReinterpreted << " times " << potfix << '\n'
                << " [" << *cCounts << " / " << cCounts[1U] << " = "
                << static_cast<long double>(*cCounts) / cCounts[1U]
                << " | " << CLOCKS_PER_SEC << ']' << std::endl;
    }
    return diff;
  }
};

#endif // PerformanceTesterH