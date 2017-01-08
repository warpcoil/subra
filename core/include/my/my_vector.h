/** Simple container class as replacement for std::vector
 
  It might be useful as a training example for novices, or if you have to avoid
  the standard library for any reason, e.g. for link problems in an application
  using mixed old and new C++ libraries.

  A lot of stuff is missing, but you can extend it easily, if needed.

  Copyright: Karsten Burger. Placed in the public domain by the author.
 
  There is no guarantee of any kind.
 
  Please report bugfixes via  http://sourceforge.net/projects/simplecstringclass.

*/

#include "memory.h" // realloc
#include "oslib/assert.h"
#include "oslib/error.h"
#include "oslib/hw-func.h"


namespace my {


template <class T>
class Vector {
  T* data_;            // array of pointers
  unsigned capacity_;  // allocated
  unsigned size_;      // used
  enum { initialSize=4 };


public:
  typedef T* iterator;

  Vector() :data_(0), capacity_(0), size_(0) {
    reserve(initialSize);
  }

  Vector(unsigned n) :data_(0), capacity_(0), size_(0) {
    resize(n);
  }

  ~Vector() {
    clear();
  }

  void clear() {
    delete[] data_;
    data_ = 0;
  }

  bool     empty()     const { return size_==0; }
  unsigned size()      const { return size_; }
  unsigned capacity()  const { return capacity_; }

  void reserve( unsigned n) {
    if ( n > capacity_ ) {
      // reserve more memory
      T* p = new T[n];

      // copy old data
      for (unsigned i=0; i < size_; ++i) {
        new (p+i) T( *(data_ + i) );  // in-place copy-construct
      }
      delete[] data_;
      data_     = p;
      capacity_ = n;
    }
  }

  void resize(unsigned n) {
    if (n < size_) {
      // destroy last elements
      for (unsigned i=n; i < size_; ++i) {
        (data_ + i)->~T(); // in-place destroy
      }
      size_ = n;
    }

    if ( n > capacity_ ) {
      reserve( n );
    }

    // default-create new elements
    for (unsigned i=size_; i < n; ++i) {
      new(data_+i) T(); // in-place construct
    }
    size_ = n;
  }

  void push_back( const T& t ) {
    assert(capacity_ != 0);

    if (size_ == capacity_ ) {
      reserve( capacity_ * 2 );
    }

    new(data_+size_) T( t ); // in-place construct
    ++size_;
  }


  T& operator[](unsigned i)
  {
    if (i<size_)
      return *(data_+i);
    else {
        message("my_array: operator[]: index out of range");
        halt();
    }
  }


  const T& operator[](unsigned i) const
  {
    if (i<size_)
      return *(data_+i);
    else {
        message("my_array: operator[]: index out of range");
        halt();
    }
  }

  iterator begin() { return data_; }

  iterator end()  { return data_ + size_; }

}; // class



} // namespace
