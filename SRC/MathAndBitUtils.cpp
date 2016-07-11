#include "BitUtils.h"

const long double MathUtils::DEFAULT_CMP_EPSILON_NEIGHBORHOOD_ = 1e-9L;
const MathUtils::ByteOrderTester MathUtils::ByteOrderTester::INSTANCE;

namespace BitUtils {
  //// [!] Beware of the static initialization order fiasco
  ////      ('BitPackageOrderTester' uses 'ByteOrderTesterEx') [!]

  /* GCC [up to 7.0] bogus error: 'accessing value through a glvalue in a constant expression'
     https://gcc.gnu.org/bugzilla/show_bug.cgi?id=71504
  */
  #if (__GNUC__ >= 7 && __GNUC_MINOR__ > 0) || !defined(__GNUC__)
    CONSTEXPR_11_
  #endif
  const EEndiannes ByteOrderTesterEx::endiannes = ByteOrderTesterEx::getCurrentEndiannes();
  const ByteOrderTesterEx ByteOrderTesterEx::INSTANCE;

  /* GCC [up to 6.1] internal compiler error: in 'cxx_eval_bit_field_ref'
     https://gcc.gnu.org/bugzilla/show_bug.cgi?id=57335
  */
  #if (__GNUC__ >= 6 && __GNUC_MINOR__ > 1) || !defined(__GNUC__)
    CONSTEXPR_14_
  #endif
  const EBitPackageOrder BitPackageOrderTester::order = BitPackageOrderTester::determineOrder();
  const BitPackageOrderTester BitPackageOrderTester::INSTANCE;
};