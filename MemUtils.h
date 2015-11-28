#ifndef MemUtilsH
#define MemUtilsH

#include <cstring>
#include <cassert>

#include "TypeHelpers.h"
#include "HardwareUtils.h"

// HINT: Size of 'std::ptrdiff_t' AND 'uintptr_t'
//  types depends on compiler settings

template<const size_t CmpChunkSize = 2U, // in bytes sizeof(std::uintptr_t)
         const bool SignedCmpChunk = false, // as 'strcmp' by default [7.21.4/1 (C99)]
         typename TElem1Type, typename TElem2Type>
typename IntegralTypeBySize<CmpChunkSize, true>::Type
// Used to compare strings with the diff. char types
// Works almost as fast as a 'strcmp' (at least for 'char' AND on release)
// [!] Does NOT checks an incoming args. on validity [!]
  strCmp(const TElem1Type* mem1, const TElem2Type* mem2) throw()
{
  typedef typename IntegralTypeBySize<sizeof(*mem1), false>::Type TUE1T; // unsigned both
  typedef typename IntegralTypeBySize<sizeof(*mem2), false>::Type TUE2T;
  typedef typename IntegralTypeBySize<CmpChunkSize, true>::Type TReturnType;
  // See http://stackoverflow.com/questions/1356741/strcmp-and-signed-unsigned-chars
  static_assert(sizeof(TReturnType) > sizeof(*mem1) &&
                sizeof(TReturnType) > sizeof(*mem2), "Incorrect elem. type size");

  if (mem1 == mem2) return TReturnType(); // optimization
  typename IntegralTypeBySize<CmpChunkSize, SignedCmpChunk>::Type elem1, elem2;
  while (true) {
    elem1 = static_cast<decltype(elem1)>(static_cast<TUE1T>(*mem1));
    elem2 = static_cast<decltype(elem2)>(static_cast<TUE2T>(*mem2));
    
    if (!elem1 || elem1 != elem2)
      return static_cast<TReturnType>(elem1) - elem2;
    ++mem1, ++mem2;
  }
}

template<const size_t CmpChunkSize = sizeof(std::uintptr_t), // in bytes
         // As 'memcmp' by default (http://www.cplusplus.com/reference/cstring/memcmp/)
         const bool SignedCmpChunk = false>
// [!] Does NOT checks an incoming args. on validity [!]
// Works ~as fast as 'memcmp'
long long int memCmp(const void* const mem1, const void* const mem2, const size_t iterCount) throw() {
  typedef long long int TReturnType;
  typedef typename IntegralTypeBySize<CmpChunkSize, SignedCmpChunk>::Type TCmpChunkType;
  static_assert(sizeof(TCmpChunkType) < sizeof(TReturnType), "Incorrect comparing type size");

  if (mem1 == mem2) return TReturnType(); // optimization
  auto mem1re = static_cast<const TCmpChunkType*>(mem1); // reinterpreted
  auto mem2re = static_cast<const TCmpChunkType*>(mem2);
  
  const auto end = mem1re + iterCount;
  while (mem1re < end) {
    if (*mem1re != *mem2re)
      return static_cast<TReturnType>(*mem1re) - *mem2re;
    ++mem1re, ++mem2re;
  }
  return TReturnType();
}

template<const size_t CmpChunkSize = sizeof(std::uintptr_t), // in bytes
         // As 'memcmp' by default (http://www.cplusplus.com/reference/cstring/memcmp/)
         const bool SignedCmpChunk = false>
// [!] Does NOT checks an incoming args. on validity [!]
// Use this when the fact itself of a difference of the two memory chunks is meaningfull,
//  NOT the actual difference value
// [!] Works a bit faster then 'memcmp' [!]
bool isEqualMem(const void* const mem1, const void* const mem2, const size_t iterCount) throw() {
  typedef typename IntegralTypeBySize<CmpChunkSize, SignedCmpChunk>::Type TCmpChunkType;

  if (mem1 == mem2) return true; // optimization
  auto mem1re = static_cast<const TCmpChunkType*>(mem1); // reinterpreted
  auto mem2re = static_cast<const TCmpChunkType*>(mem2);
  
  const auto end = mem1re + iterCount;
  while (mem1re < end) {
    if (*mem1re != *mem2re) return false;
    ++mem1re, ++mem2re;
  }
  return true;
}

// As 'memcmp' by default (http://www.cplusplus.com/reference/cstring/memcmp/)
template<const bool SignedCmpChunk = false>
// 'memSize' is in bytes
// D - Dispatcher
//  (uses either 64 OR 32 bit chunks, depending on the CPU type, to better fit the register size)
// [!] Faster then 'memCmp<>' OR 'memcmp', use instead of 'isEqualMem' [!]
bool isEqualMemD(const void* const mem1, const void* const mem2, const size_t memSize) throw() {
  typedef const typename IntegralTypeBySize<8U, SignedCmpChunk>::Type T64Bit;
  typedef const typename IntegralTypeBySize<4U, SignedCmpChunk>::Type T32Bit;
  typedef const typename IntegralTypeBySize<1U, SignedCmpChunk>::Type T8Bit;
  
  if (memSize < 8U) {
    return !memcmp(mem1, mem2, memSize);
  }
  switch (CPUInfo::INSTANCE.is64BitCPU) {
    case true: // 64 bit
      assert(memSize >= 8U);
      if (!isEqualMem<8U, SignedCmpChunk>(mem1, mem2, memSize / 8U)) return false;
      // Check the remain (NOT fitted) bytes; sizeof(char) == 1
      return *(reinterpret_cast<T64Bit*>(static_cast<T8Bit*>(mem1) + memSize - 8U)) ==
             *(reinterpret_cast<T64Bit*>(static_cast<T8Bit*>(mem2) + memSize - 8U));
      
    default: // 32 bit
      assert(memSize >= 4U);
      if (!isEqualMem<4U, SignedCmpChunk>(mem1, mem2, memSize / 4U)) return false;
      return *(reinterpret_cast<T32Bit*>(static_cast<T8Bit*>(mem1) + memSize - 4U)) ==
             *(reinterpret_cast<T32Bit*>(static_cast<T8Bit*>(mem2) + memSize - 4U));
  }
}

/*
  Standart 'memcmp' SHOULD be implemented as an architecture-independent built-in by the compiler,
   loke 8-byte "repz cmpsb" (http://stackoverflow.com/questions/855895/intrinsic-memcmp)
  http://stackoverflow.com/questions/21106801/why-is-memcmp-so-much-faster-than-a-for-loop-check
*/
//// NOTE: 'Shuffle' looks like meanigless, better use reversed cmp. (from the end) instead

#endif // MemUtilsH