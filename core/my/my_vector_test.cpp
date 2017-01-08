// Test program for my_vector.h
#include <string>
#include <iostream>
#include "my_vector.h"

using namespace std;

void test1() {
  my::Vector<int> vec;
  assert( vec.empty() );
  assert( vec.size()==0 );
  assert( vec.capacity() > 0 );

  vec.push_back( 1);
  assert( !vec.empty() );
  assert( vec.size()==1 );
  assert( vec[0] == 1 );

  vec[0] = 2;
  assert( vec[0] == 2 );

  vec[0] = 1;
  assert( vec[0] == 1 );
  vec.push_back(2);
  assert( !vec.empty() );
  assert( vec.size()==2 );
  assert( vec[1] == 2 );

  vec.resize(0);
  assert( vec.empty() );
  assert( vec.size()==0 );
  assert( vec.capacity() > 0 );

  vec.resize(2);
  vec[0] = 1;
  vec[1] = 2;
  vec.resize(10);
  assert( !vec.empty() );
  assert( vec.size()==10 );
  assert( vec.capacity() == 10 );

  assert( vec[0] == 1 );
  assert( vec[1] == 2 );
  assert( vec[2] == 0 );
  assert( vec[3] == 0 );
  assert( vec[4] == 0 );
  assert( vec[5] == 0 );
  assert( vec[6] == 0 );
  assert( vec[7] == 0 );
  assert( vec[8] == 0 );
  assert( vec[9] == 0 );


  {
    my::Vector<int> vec2(5);
    assert( !vec2.empty() );
    assert( vec2.size() == 5 );
    assert( vec2.capacity() == 5 );
    for (unsigned i=0; i < vec2.size(); ++i ) {
      assert( vec2[i] == 0);
    }
  }
} // test1

void test2() {
  using std::string;
  {
    my::Vector<string> vec;
    assert( vec.empty() );
    assert( vec.size() == 0 );
    assert( vec.capacity() > 0 );

    vec.push_back( string("abc") );
    assert( !vec.empty() );
    assert( vec.size() == 1 );
    assert( vec[0] == "abc" );

    vec.push_back( string("def") );
    assert( vec.size() == 2 );
    assert( vec[0] == "abc" );
    assert( vec[1] == "def" );

    vec.resize(10);
    assert( vec.size() == 10 );
    assert( vec[0] == "abc" );
    assert( vec[1] == "def" );
    assert( vec[2] == "" );
    assert( vec[3] == "" );
    assert( vec[4] == "" );
    assert( vec[5] == "" );
    assert( vec[6] == "" );
    assert( vec[7] == "" );
    assert( vec[8] == "" );
    assert( vec[9] == "" );

  }
}

void test3() {
  my::Vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  my::Vector<int>::iterator it = vec.begin();
  assert( *it == 1 );
  ++it;
  assert( *it == 2 );
  ++it;
  assert( *it == 3 );
}

int main() {
  test1();
  test2();
  test3();
  cout << "everything ok." << endl;
}
