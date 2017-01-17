/*
 * IMPLEMENTATION NOTES
 *
 * My goal was to create a small, and conceptually simple
 * string class.  I allocate unique memory for every object
 * created, instead of doing anything fancy.
 *
 * I've taken care about the order in which I free memory,
 * in case we are copying overlapping regions.
 *
 */

#include "my_string.h"
//#include <string.h> // strlen
//#include <stdlib.h> // malloc, realloc
//#include <assert.h>

#include "oslib/string.h" //strlen
#include "memory.h" //malloc, realloc
#include "oslib/error.h" //message
#include "oslib/hw-func.h" //halt

namespace my {

  const String::size_type String::npos = static_cast<size_t>(-1);

  template<class T>
  const T& min(const T& a, const T& b)
  {
      return (b < a) ? b : a;
  }

  template<class T>
  void iswap(T& a, T& b) {
      T c = a;
      a=b;
      b=c;
  }

  /*
   * Like the 'new' operator, we want to guarantee that we NEVER
   * return NULL. Loop until there is free memory.
   *
   */
  static char* malloc_never_null(const size_t b)
  {
      char *p;

      do {
          p = static_cast<char*>(dlmalloc(b));
      } while ( p == NULL );

      return p;
  }

  /**
   * Allocates memory for the copy the string at the same time sets "this->allocated".
   * @param s
   * @return
   */
  char*
  String::strdup_never_null(const char* s)
  {
      const size_t len = strlen(s)+1;
      char *p2 = malloc_never_null(len);
      memcpy(p2, s, len);
      allocated_=len;
      size_=len-1;
      return p2;
  }

  String::String()
  : p( strdup_never_null("") )
  {
  }

  String::~String()
  {
      dlfree(p);
  }

  String::String(const String& s)
    : p(0)
  {
    p = malloc_never_null( s.size_ + 1 );  // copy only used part
    allocated_ = s.size_ + 1;
    size_      = s.size_;
    memcpy(p, s.p, size_ + 1);
  }

  String::String(const char* s)
  : p(strdup_never_null(s))
  {
  }

  String& String::operator=(const char* s)
  {
      if ( p != s ) {
        // s could point into our own string, so we have to allocate a new string
        const size_t len = strlen(s);
        char* copy = (char*)dlmalloc( len + 1);
        memmove(copy, s, len+1); // trailing 0
        dlfree( p );
        p = copy;
        size_ = len;
        allocated_ = len+1;
      }

      return *this;
  }

  String& String::operator=(const String& s)
  {
      return operator=(s.p);
  }

  String& String::operator+=(const String& s)
  {
    if (s.size_ > 0){
      this->reserve(size_ + s.size_);
      memmove(p+size_, s.p, s.size_+1); // trailing 0
      size_ += s.size_;
    }
    return *this;
  }

  // since p and s may overlap, we have to copy our own string first
  String& String::operator+=(const char* s)
  {
      const size_type lens = strlen(s);
      if (lens > 0){
        if (size_ + lens + 1 <= allocated_) {
          memmove(p+size_, s, lens+1); // trailing 0
          size_ += lens;
        }
        else {
          String s2( *this );  // copy own data
          s2.reserve(size_ + lens);
          memmove(s2.p+size_, s, lens+1); // trailing 0
          s2.size_ = size_ + lens;
          this->swap( s2 );
        }
      }
      return *this;
  }

  String& String::operator+=(const char c)
  {
      push_back(c);
      return *this;
  }


  void String::push_back(const char c) {

    if (size_ == allocated_ - 1) {
      size_t more =  (allocated_* 3) / 2; // factor 1.5
      if ( more < 4 ) more = 4;
      reserve( size_ + more );
    }

    p[size_] = c;
    size_++;
    p[size_] = 0;
  }

  bool String::operator==(const char* s) const
  {
      return !strcmp(p, s);
  }

  bool String::operator==(const String& s) const
  {
      return !strcmp(p, s.p);
  }

  void String::clearMemory()
  {
    String s;
    this->swap( s );
  }

  void String::clear()
  {
      size_ = 0;
      p[0]  = 0;
  }

  String& String::toUpper() {
      p = strupr(p);
      return *this;
  }

  String& String::toLower() {
      p = strlwr(p);
      return *this;
  }

  String operator+(const String& lhs, const String& rhs)
  {
      return String(lhs) += rhs;
  }

  ssize_t String::lastIndexOf(char c) {

      size_t len = strlen(p);

      for (size_t i = len - 1; i > 0; i--) {
          if (p[i] == c)
              return i;
      }

      if (p[0] == c)
          return 0;

      return -1;
  }

  String String::substr(const size_type pos, size_type length) const
  {
      String s;
      const size_type len = size_;

      if ( pos > len ) {
          message("my::String::substr: pos index out of range");
          halt();
      }

      size_type remain = len - pos;

      if ( length > remain )
          length = remain;

      s.reserve( length );

      memcpy(s.p, p + pos, length);
      s.p[length] = '\0';
      s.size_ = length;

      return s;
  }


  // checked access, accessing the NUL at end is allowed
  char& String::at(const size_type i)
  {
      if ( i > (size_type)strlen(p)) {
          message("my::String::at(): index out_of_range");
          halt();
      }

      return p[i];
  }
  char String::at(const size_type i) const
  {
      if ( i > (size_type)strlen(p)) {
          message("my::String::at(): index out_of_range");
          halt();
      }

      return p[i];
  }

  String& String::erase(size_type pos, size_type len)
  {
    if (len > 0) {

      if ( pos >= size_ ) { // user must not remove trailing 0
          message("my::String::erase: pos index out_of_range");
          halt();
      }

      int s2 = size_;
      int remain = s2 - (int)pos - len;

      if (remain > 0) {
        // erase by overwriting
        memmove(p + pos, p + pos + len, remain);
      }

      if ( remain < 0 ) remain = 0;

      // remove unused space
      this->resize( pos+remain );

    }
    return *this;
  }

  String& String::append( const char* str, size_type n) {
    if (str && n > 0) {
      size_t lens = strlen(str);
      if (n > lens)
        n = lens;
      size_t newlen = size_ + n;
      this->reserve( newlen );
      memmove(p+size_, str, n); // p and s.p MAY overlap
      p[newlen] = 0; // add NUL termination
      size_ = newlen;
    }
    return *this;
  }

  int
  String::compare( size_type pos, size_type len, const String& str ) const {
    if (pos > size_) {
        message("my::String::compare: pos index out of range");
        halt();
    }

    if ( len > size_ - pos)
      len = size_ - pos; // limit len to available length

    const size_type osize = str.size();
    const size_type len2   = min(len, osize);
    int r = strncmp( p + pos, str.p, len2);
    if (r==0) // equal so far, now compare sizes
	r = len < osize ? -1 : ( len == osize ? 0 : +1 );
    return r;
  }

  int
  String::compare( size_type pos, size_type len, const char* str ) const {
    if (pos > size_) {
        message("my::String::compare: pos index out of range");
        halt();
    }

    if ( len > size_ - pos)
      len = size_ - pos; // limit len to available length

    const size_type osize = strlen(str);
    const size_type len2   = min(len, osize);
    int r = strncmp( p + pos, str, len2);
    if (r==0) // equal so far, now compare sizes
        r = len < osize ? -1 : ( len == osize ? 0 : +1 );
    return r;
  }


  void
  String::my_realloc( size_type n) {
    if (n > 0 ) {
      char* pnew = static_cast<char*>(dlrealloc(p, n)); // could return NULL
      if (pnew)
        p = pnew;
      else {
          message("my::String::my_realloc out of memory");
          halt();
      }
    }
   }


  void
  String::reserve( const size_type n) {
    if (n >= allocated_ ) {
      this->my_realloc(n + 1);
      allocated_ = n + 1;
    }
  }

  void
  String::resize( const size_type n) {
    this->resize( n, 0 );
  }

  void
  String::resize( const size_type n, const char c) {
    if (n < size_ ) {
      p[n] = 0;
      size_ = n;
    }
    else if (n >  size_ ) {
      this->reserve( n );
      for (size_type i=size_; i < n; ++i )
        p[i] = c;
      p[n] = 0;
      size_ = n;
    }
  }

  void String::swap( String& s ) {
    iswap( allocated_, s.allocated_ );
    iswap( size_,      s.size_      );
    iswap( p,          s.p          );
  }


  // Comparison
  bool operator<( const String& s1, const String& s2 ) {
    return strcmp( s1.c_str(), s2.c_str() ) < 0;
  }

} // namespace
