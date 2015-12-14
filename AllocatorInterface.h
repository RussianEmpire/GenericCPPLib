#ifndef AllocatorInterfaceH
#define AllocatorInterfaceH

#include <memory>

// C++ allocator concept: http://en.cppreference.com/w/cpp/concept/Allocator
// Fake allocator, which can NOT AND would NOT work!
// Derived classes SHOULD specify the following properties:
//  'propagate_on_container_copy_assignment', 'propagate_on_container_move_assignment',
//   'propagate_on_container_swap' AND 'is_always_equal' (if req.)
template <class T>
class AllocatorInterface : public std::allocator<T> { // better derive from the 'allocator_traits'??
  
public:
  //// MS VS does NOT need this aliases, but GCC does
  typedef typename std::allocator<T> TBaseAllocatorType;
  typedef typename TBaseAllocatorType::pointer pointer;
  typedef typename TBaseAllocatorType::const_pointer const_pointer;
  typedef typename TBaseAllocatorType::reference reference;
  typedef typename TBaseAllocatorType::const_reference const_reference;
  typedef typename TBaseAllocatorType::size_type size_type;

  //// Three constructor versions must be defined (even if they do nothing) 
  ////  to allow for copy-constructions from allocator objects of other types

  AllocatorInterface() = default;

  AllocatorInterface(const AllocatorInterface&) = default;

  template <class U>
  AllocatorInterface(AllocatorInterface<U>&&) throw() {}

  // [!] NO member of the standard default allocator class template shall introduce data races,
  //  calls to member functions that allocate or deallocate storage shall occur in a single total order 
  //   and each such deallocation shall happen before the next allocation (if any) in this order [!]

  virtual ~AllocatorInterface() = default;
  
  virtual pointer address(reference ref) const throw() {
    return std::allocator<T>::address(ref);
  }

  virtual const_pointer address(const_reference ref) const throw() {
    return std::allocator<T>::address(ref);
  }

  //// Pure virtual
  virtual pointer allocate(size_type count, std::allocator<void>::const_pointer hint = nullptr) = 0;
  virtual void deallocate(pointer addr, size_type count) = 0;

  virtual size_type max_size() const throw() {
    return size_type();
  }

  //// 'construct ' AND 'destroy' are both template funcs.
  ////  so can NOT be virtual (they will be used from a 'std::allocator')

  //// Custom allocators may contain state:
  ////  each container or another allocator-aware object stores an instance of the supplied allocator
  ////   and controls allocator replacement through 'std::allocator_traits'

private:
};

template <typename T, class U>
bool operator==(const AllocatorInterface<T>&, const AllocatorInterface<U>&) throw() {
  return false;
}

template <typename T, class U>
bool operator!=(const AllocatorInterface<T>& alloc1, const AllocatorInterface<U>& alloc2) throw() {
  return !(alloc1 == alloc2);
}

#endif // AllocatorInterfaceH