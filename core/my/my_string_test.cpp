/* Tests for my_string.h, 2014-02
 */

#include <cassert>
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>
#include "my_string.h"

using namespace std;
void test_basic_functionality()
{
  // ctor
  {
    my::String m;
    assert( m.c_str() );
    assert(*m.c_str() == '\0');
    assert(m.empty() );
    assert(m.size()     == 0);
    assert(m.capacity() == 0);
  }

  // ctor 2
  {
    my::String m2("ab");
    assert(m2.size() == 2);
    assert(m2.empty() == false);
    assert(m2[0] == 'a');
    assert(m2[1] == 'b');
    assert(m2[2] == 0  );
  }

  // copy ctor
  {
    my::String m1("ab"), m2(m1);
    assert(m2.size() == 2);
    assert(m2[0] == 'a');
    assert(m2[1] == 'b');
    assert(m2[2] == 0  );

  }

  // operator= and index operator
  {
    const char* p = "ab";
    my::String m;
    m = p;
    assert(strlen(m.c_str()) == m.size());
    assert(m.size() == 2);
    assert(m.empty() == false);
    assert(m.c_str() != p);            // unique buffer
    assert(m[0] == 'a');
    assert(m[1] == 'b');
    assert(m[2] == 0  );
    assert(strlen(m.c_str()) == 2);
    assert(strcmp(m.c_str(), "ab") == 0);

    my::String m2;
    m2.operator=(m);
    assert(m2 == m);
    assert(m2.size() == m.size() );
    assert(m2.c_str() != m.c_str()); // unique buffers
  }

  // reserve, resize
  {
    my::String m;
    m.reserve(5);
    assert( m.size() == 0 );
    assert( m.capacity() == 5 );
    assert(*m.c_str() == '\0');
    m.resize(3);
    assert( m.size() == 3 );
    assert( m.capacity() == 5 );
    assert(*(m.c_str()  ) == '\0');
    assert(*(m.c_str()+1) == '\0');
    assert(*(m.c_str()+2) == '\0');
    assert(*(m.c_str()+3) == '\0');
    m.resize(5, ' ');
    assert( m.size() == 5 );
    assert( m.capacity() == 5 );
    assert(*(m.c_str()+3) == ' ');
    assert(*(m.c_str()+4) == ' ');
    m.resize(0);
    assert( m.size() == 0 );
    assert( m.capacity() == 5 );
    assert(*(m.c_str()  ) == '\0');
  }

  // index access with write
  {
    my::String m2("abc");
    m2[1]='d';
    assert( m2 == "adc" );
    m2.at(1) = 'b';
    assert( m2 == "abc" );
    assert( m2[1] == 'b' );
  }

  // operator==
  {
    my::String m("ab");
    my::String m2(m);
    assert( m2 == m );
    assert( !(m2 == "") );
    assert( !(m2 == "a") );
    assert( !(m2 == "ac") );

    // clear
    m.clear();
    assert(m.empty());
    assert(m.size()==0);
  }

  // operator +=
  {
    my::String m2;
    m2 += 'a';
    assert( m2 == "a" );
    assert( m2.size() == 1 );
    assert( m2[1] == 0 );

    m2 += 'b';
    assert( m2 == "ab" );
    assert( m2.size() == 2 );
    assert( m2[2] == 0 );

    m2 += "cd";
    assert( m2 == "abcd" );
    assert( m2.size() == 4 );
    assert( m2[4] == 0 );

    m2 += my::String("ef");
    assert( m2 == "abcdef" );
    assert( m2.size() == 6 );
    assert( m2[6] == 0 );
  }

  // operator+
  {
    my::String m2, m3("cd");
    m2 += 'a';
    assert( m2 == "a" );
    assert( m2.size() == 1 );
    assert( m2[1] == 0 );

    m2 += 'b';
    assert( m2 == "ab" );
    assert( m2.size() == 2 );
    assert( m2[2] == 0 );

    assert( (m2+m3) == "abcd" );
    assert( (m2+m3).size() == 4 );
    assert( (m2+m3)[4] == 0 );

    m2 += m3;
    assert( m2 == "abcd" );
    assert( m2.size() == 4 );
    assert( m2[4] == 0 );
  }

  // substr
  {
    my::String m("ab");
    assert(m.substr(1,1) == "b");
    assert(m.substr(1,1).size() == 1);
    assert(m.substr(1,1)[0] == 'b');
    assert(m.substr(1,1)[1] == 0  );
    assert(m.substr(0,1) == "a");
    assert(m.substr(0,2) == "ab");
    assert(m.substr(0,2).size() == 2);
    assert(m.substr(0,2)[2] == 0);

    // check ignore overflow, same as in std::string
    assert(m.substr(0,3) == "ab");
    assert(m.substr(0,333) == "ab");
    assert(m.substr(0,3).size() == 2);
    //cout << "size=" << m.substr(0,3).raw_data().size() << endl;

    // should throw out_of_range
    try {
      my::String().substr(1,0);
      assert(false);
    }
    //catch(const std::out_of_range&) {
    catch(const char*) {
    }

  } // substr

  // operator+ also with overlapping memory
  {
    my::String m4 = "akkabakka";
    assert(m4 == "akkabakka");

    m4 += m4.c_str() + 4; // overlapping memory
    assert(m4 == "akkabakkabakka");

    my::String m5;
    m5 = m4 + "bonka"; // friend operator+(String, String)
    assert(m5 == "akkabakkabakkabonka");

    m5 = "bonka" + m4; // friend operator+(String, String)
    assert(m5 == "bonkaakkabakkabakka");

    m5 = my::String("abba") + " disco";
    assert(m5 == "abba disco");

    m5 = "ab" + my::String("cd") + "ef";
    assert(m5 == "abcdef");

    assert(m5.size() == 6 );
    assert(m5[0] == 'a');
    assert(m5[5] == 'f');
    assert(m5[6] == '\0');
      // at function
    assert(m5.at(0) == 'a');
    assert(m5.at(5) == 'f');
    assert(m5.at(6) == '\0');

    try {
      m5.at(7);
        assert(false); // should not reach this
    }
    //catch(const std::out_of_range&) {
    catch(const char*) {
    }

    my::String m6 = "hello";
    m6 = (m6.c_str() + 1); // overlapping assignment
    assert(m6 == "ello");

    m6 += (m6.c_str() + 1); // overlaping append
    assert(!strcmp(m6.c_str(), "ellollo"));
  }

  { // erase
    my::String m7 = "123456789";
    assert(m7.erase(0, 2) == "3456789"); // erase beginning
    assert(m7.erase(5, 2) == "34567"); // erase end
    assert(m7.erase(1, 3) == "37"); // erase middle
    assert(m7.erase(0, 0) == "37"); // no effect
    assert(m7.erase(1, 0) == "37");
    assert(m7.erase(2, 0) == "37");
    assert(m7.erase(1, 1) == "3");

    try {
      m7.erase(200, 100);
      assert(false);
    }
    //catch(const std::out_of_range&) {
    catch(const char*) {
    }

    m7 = "hello";
    assert(m7.erase(1, 200) == "h"); // overflow
  }

  // append  + push_back
  {
    my::String m2;
    m2.append( "abcd", 0);
    assert( m2.empty() );
    m2.append( "abcd", 1);
    assert( m2 == my::String("a") );
    m2.append( "bcd", 99);
    assert( m2 == my::String("abcd") );
    m2.push_back( 'e');
    assert( m2 == my::String("abcde") );
  }

  // compare( pos, len, stdstring)
  {
    assert( std::string("abcd").compare( 0, 1, std::string("a" ) )  == 0 );
    assert( std::string("abcd").compare( 0, 2, std::string("ab") )  == 0 );
    assert( std::string("abcd").compare( 1, 2, std::string("bc") )  == 0 );
    assert( std::string("abcd").compare( 2, 2, std::string("cd") )  == 0 );
    assert( std::string("abcd").compare( 3, 1, std::string("d" ) )  == 0 );

    assert( std::string("abcd").compare( 0, 2, std::string("bc") )  <  0 );
    assert( std::string("abcd").compare( 2, 2, std::string("bcd"))  >  0 );
    // other string is shorter:
    assert( std::string("abcd").compare( 1, 2, std::string("b")  )  >  0 );
    assert( std::string("abcd").compare( 1, 1, std::string("b")  )  == 0 );
    // this  string is shorter:
    assert( std::string("ab")  .compare( 1, 2, std::string("bc") )  <  0 );
    assert( std::string("ab")  .compare( 2, 2, std::string("bc") )  <  0 );
    assert( std::string("a")   .compare( 0, 2, std::string("abc") ) <  0 );
    assert( std::string("ab")  .compare( 0, 1, std::string("abc") ) <  0 );
    assert( std::string("ab")  .compare( 0, 2, std::string("abc") ) <  0 );
    assert( std::string("aa")  .compare( 0, 2, std::string("abc") ) <  0 );
    assert( std::string("b" )  .compare( 0, 2, std::string("abc") ) >  0 );
    try {
      std::string("ab").compare( 3, 2, std::string("bc") );
      assert(false);
    }
    catch(const std::out_of_range&) { // ok
    }

    assert( my::String("abcd").compare( 0, 1, my::String("a" ) )  == 0 );
    assert( my::String("abcd").compare( 0, 2, my::String("ab") )  == 0 );
    assert( my::String("abcd").compare( 1, 2, my::String("bc") )  == 0 );
    assert( my::String("abcd").compare( 2, 2, my::String("cd") )  == 0 );
    assert( my::String("abcd").compare( 3, 1, my::String("d" ) )  == 0 );

    assert( my::String("abcd").compare( 0, 2, my::String("bc") )  <  0 );
    assert( my::String("abcd").compare( 2, 2, my::String("bcd"))  >  0 );
    // other string is shorter:
    assert( my::String("abcd").compare( 1, 2, my::String("b")  )  >  0 );
    assert( my::String("abcd").compare( 1, 1, my::String("b")  )  == 0 );
    // this  string is shorter:
    assert( my::String("ab")  .compare( 1, 2, my::String("bc") )  <  0 );
    assert( my::String("ab")  .compare( 2, 2, my::String("bc") )  <  0 );
    assert( my::String("a")   .compare( 0, 2, my::String("abc") ) <  0 );
    assert( my::String("ab")  .compare( 0, 1, my::String("abc") ) <  0 );
    assert( my::String("ab")  .compare( 0, 2, my::String("abc") ) <  0 );
    assert( my::String("aa")  .compare( 0, 2, my::String("abc") ) <  0 );
    assert( my::String("b" )  .compare( 0, 2, my::String("abc") ) >  0 );

    try {
      my::String("ab").compare( 3, 2, my::String("bc") );
      assert(false);
    }
    // catch(const std::out_of_range&) { // ok
    catch(const char*) {
    }

  }

  // compare( pos, len, char*)
  {
    assert( std::string("abcd").compare( 0, 1, "a"  )  == 0 );
    assert( std::string("abcd").compare( 0, 2, "ab" )  == 0 );
    assert( std::string("abcd").compare( 1, 2, "bc" )  == 0 );
    assert( std::string("abcd").compare( 2, 2, "cd" )  == 0 );
    assert( std::string("abcd").compare( 3, 1, "d"  )  == 0 );

    assert( std::string("abcd").compare( 0, 2, "bc" )  <  0 );
    assert( std::string("abcd").compare( 2, 2, "bcd")  >  0 );
    // other string is shorter:
    assert( std::string("abcd").compare( 1, 2, "b"  )  >  0 );
    assert( std::string("abcd").compare( 1, 1, "b"  )  == 0 );
    // this  string is shorter:
    assert( std::string("ab")  .compare( 1, 2, "bc" )  <  0 );
    assert( std::string("ab")  .compare( 2, 2, "bc" )  <  0 );
    assert( std::string("a")   .compare( 0, 2, "abc" ) <  0 );
    assert( std::string("ab")  .compare( 0, 1, "abc" ) <  0 );
    assert( std::string("ab")  .compare( 0, 2, "abc" ) <  0 );
    assert( std::string("aa")  .compare( 0, 2, "abc" ) <  0 );
    assert( std::string("b" )  .compare( 0, 2, "abc" ) >  0 );
    try {
      std::string("ab").compare( 3, 2, "bc" );
      assert(false);
    }
    catch(const std::out_of_range&) { // ok
    }

    assert( my::String("abcd").compare( 0, 1, "a"  )  == 0 );
    assert( my::String("abcd").compare( 0, 2, "ab" )  == 0 );
    assert( my::String("abcd").compare( 1, 2, "bc" )  == 0 );
    assert( my::String("abcd").compare( 2, 2, "cd" )  == 0 );
    assert( my::String("abcd").compare( 3, 1, "d"  )  == 0 );

    assert( my::String("abcd").compare( 0, 2, "bc" )  <  0 );
    assert( my::String("abcd").compare( 2, 2, "bcd")  >  0 );
    // other string is shorter:
    assert( my::String("abcd").compare( 1, 2, "b"  )  >  0 );
    assert( my::String("abcd").compare( 1, 1, "b"  )  == 0 );
    // this  string is shorter:
    assert( my::String("ab")  .compare( 1, 2, "bc" )  <  0 );
    assert( my::String("ab")  .compare( 2, 2, "bc" )  <  0 );
    assert( my::String("a")   .compare( 0, 2, "abc" ) <  0 );
    assert( my::String("ab")  .compare( 0, 1, "abc" ) <  0 );
    assert( my::String("ab")  .compare( 0, 2, "abc" ) <  0 );
    assert( my::String("aa")  .compare( 0, 2, "abc" ) <  0 );
    assert( my::String("b" )  .compare( 0, 2, "abc" ) >  0 );

    try {
      my::String("ab").compare( 3, 2, "bc" );
      assert(false);
    }
    // catch(const std::out_of_range&) { // ok
    catch(const char*) {
    }

  }



} // test_basic_functionality()


void test_compatibility()
{
  const char* str = "goonigogo";
  const size_t len = strlen(str);
  std::string s = str;
  my::String  m = str;

  assert(s == m.c_str());

  for ( size_t x=0; x < len; ++x )
    for ( size_t y=0; y < len*2; ++y ) {
      //fprintf(stderr, "substr(%ld,%ld) ", x, y);
      assert(s.substr(x,y) == m.substr(x,y).c_str());
    }
  //fprintf(stderr, "\n");

  for ( size_t x=0; x < len; ++x )
    for ( size_t y=0; y < len*2; ++y ) {
      s = str;
      m = str;
      //fprintf(stderr, "erase(%ld,%ld) ", x, y);
      assert(s.erase(x,y) == m.erase(x,y).c_str());
    }
  //fprintf(stderr, "\n");

  s = str;
  m = str;
  for ( size_t x=0; x < len; ++x ) {
    //fprintf(stderr, "at(%ld) ", x);
    assert(s.at(x) == m.at(x));

    //fprintf(stderr, "[%ld] ", x);
    assert(s[x] == m[x]);
  }
  //fprintf(stderr, "\n");

  for ( size_t x=0; x < len; ++x )
    for ( size_t y=0; y < len*2; ++y ) {
      s = str;
      m = str;
      //fprintf(stderr, "+=substr(%ld,%ld) ", x, y);
      s += s.substr(x,y);
      m += m.substr(x,y);
      assert(s == m.c_str());
    }
  //fprintf(stderr, "\n");

  for ( size_t x=0; x < len; ++x )
    for ( size_t y=0; y < len*2; ++y ) {
      s = str;
      m = str;
      //fprintf(stderr, "+substr(%ld,%ld) ", x, y);
      s = "abc" + s.substr(x,y);
      m = "abc" + m.substr(x,y);
      assert(s == m.c_str());
    }
  //fprintf(stderr, "\n");
}

int main()
try
{
  test_basic_functionality();
  test_compatibility();

  printf("\neverything ok.\n");
  return 0;
}
catch(const std::exception& e) {
  fprintf(stderr, "std error: %s\n", e.what());
  throw;
}
catch(const char* e) {
  fprintf(stderr, "error: %s\n", e);
  throw;
}
