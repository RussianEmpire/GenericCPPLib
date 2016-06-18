#include "BitUtils.h"

const long double MathUtils::DEFAULT_CMP_EPSILON_NEIGHBORHOOD_ = 1e-9L;

//// [!] Beware of the static initialization order fiasco ('BitPackageOrder' uses 'ByteOrderTester') [!]
const MathUtils::ByteOrderTester MathUtils::ByteOrderTester::INSTANCE;
const BitUtils::BitPackageOrder BitUtils::BitPackageOrder::INSTANCE;