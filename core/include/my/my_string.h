/* Karsten Burger 2014
 *
 * Sourceforge project "Simple C++ String Class"
 * http://sourceforge.net/projects/simplecstringclass/

 * This a simple C++ string class based on class my_string by
 * Christian Stigen Larsen, 2007, http://csl.name/programming/my_string/
 *
 * It only uses the C-string functions and is thus independent of the
 * standard C++ library.
 *
 * It is public domain, in the hope, that you find it useful.
 * Please note that there is no guarantee of any kind: it is supplied
 * without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.
 *
 * You can probably replace std::string with this one in many
 * cases, but a lot of stuff is missing, and I would recommend
 * you stick to std::string anyway.
 *
 * I want to point out that there is nothing fancy about this class.
 * It keeps every string in its own buffer, and copies as often as
 * needed.
 * The data always contains a trailing NUL char.
 *
 * However, I believe that is a good approach.  For instance, it
 * uses malloc rather than new, which makes it possible to use
 * realloc.  On many systems, realloc will try to use up "invisible"
 * space that was used by malloc to pad a string for memory alignment.
 * That makes it potentially fast for small concatenations.
 *
 * I don't propose to use this class for anything practical, since
 * we already have std::string, but it may be an interesting read
 * for C++ novices at the very least. Also, additional functions can
 * easily be expanded.
 *
 * Also I met a case, where I had to avoid std::string because of
 * link problems with an application using mixed libraries, especially
 * one compiled with an old Intel compiler icc 7.
 *
 * Bugs/suggestions to info [at) dr-burger ]dot[ com
 * or via the Sourceforge project page.
 *
 */

//#include <sys/types.h> // size_t
//#include <stdexcept>
#include "oslib/types.h"

namespace my
{

  /** @brief Simple C++ string class, useful as replacement for
   std::string if this cannot be used, or just for fun.

   */
  class String
  {
    char*     p;           ///< The data
    size_t    allocated_;  ///< The allocated memory size (including trailing NUL)
    size_t    size_;       ///< The currently used memory size (excluding trailing NUL)

  public:
    typedef size_t size_type;
    static const   size_type npos;

    String();
    ~String();
    String(const String&);
    String(const char*);

    String&  operator=(const char*);
    String&  operator=(const String&);

    String&  operator+=(const String&);
    String&  operator+=(const char*);
    String&  operator+=(char);
    void     push_back(char);

    friend String
    operator+(const String& lhs, const String& rhs);

    bool     operator==(const char*) const;
    bool     operator==(const String&) const;

    void     clear();       // set string to empty string (memory remains reserved)
    void     clearMemory(); // set string to empty string (memory is free'd)

    String&      toUpper(); //To UpperCase
    String&      toLower(); //To LowerCase

    size_type size()   const   { return size_; }   ///< size without terminating NUL
    size_type length() const   { return size_; }   ///< as size()

    /// size if fully used
    size_type capacity() const { return allocated_-1; }

    bool      empty() const    { return size_ == 0; }

    const char*  c_str() const { return p; } ///< raw data

    /** Reserve internal string memory so that n characters can be put into the
        string (plus 1 for the NUL char). If there is already enough memory,
        nothing happens, if not, the memory will be realloated to exactly this
        amount.
        */
    void reserve( size_type n);

    /** Resize string. If n is less than the current size, the string will be truncated.
        If n is larger, then the memory will be reallocated to exactly this amount, and
        the additional characters will be NUL characters.
        */
    void resize( size_type n);

    /** Resize string. If n is less than the current size, the string will be truncated.
        If n is larger, then the memory will be reallocated to exactly this amount, and
        the additional characters will be c characters.
        */
    void resize( size_type n, char c);

    /// swap contents
    void swap( String& );

    ssize_t lastIndexOf(char c);

    String   substr(const size_type pos, size_type length) const;

    // unchecked access:
    char&    operator[](const size_type i)       { return p[i]; }
    char     operator[](const size_type i) const { return p[i]; }
    // checked access:
    char&    at(const size_type i);
    char     at(const size_type i) const;

    /// erase len characters at position pos
    String& erase(size_type pos, size_type len);
    /// Append n characters of a string
    String& append(const char* str, size_type n);

    int     compare( size_type pos, size_type len, const String& str ) const;
    int     compare( size_type pos, size_type len, const char*   str ) const;

  private:
    // reallocate the internal memory
    void  my_realloc( size_type n);
    char* strdup_never_null(const char* other);

  };
  // class

  bool
  operator<(const String&, const String&);

} // namespace
