#include "BitUtils.h"

const long double MathUtils::DEFAULT_CMP_EPSILON_NEIGHBORHOOD_ = 1e-9L;
const MathUtils::ByteOrderTester MathUtils::ByteOrderTester::INSTANCE;

//// [!] Beware of the static initialization order fiasco ('BitPackageOrder' uses 'ByteOrderTesterEx') [!]
const BitUtils::ByteOrderTesterEx BitUtils::ByteOrderTesterEx::INSTANCE;
const BitUtils::BitPackageOrder BitUtils::BitPackageOrder::INSTANCE;