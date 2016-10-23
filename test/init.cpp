#include <stack.hpp>
#include <catch.hpp>
#include <iostream>
using namespace std;

SCENARIO("count", "[count]"){
  Stack<int> s;
  s.push(1);
  REQUIRE(s.count()==1);
}

SCENARIO("push", "[push]"){
  Stack<int> s;
  s.push(1);
  REQUIRE(s.count() == 1);
  REQUIRE(s.top() == 1);
}

SCENARIO("pop", "[pop]"){
  Stack<int> s;
  s.push(1);
  REQUIRE(s.count() == 1);
  REQUIRE(s.top() == 1);
}

SCENARIO("prisv", "[prisv]"){
  Stack<int> s;
  s.push(1);
  Stack<int> s2;
  s2=s;
  REQUIRE(s.count() == s2.count());
  REQUIRE(s.top() == s2.top());
}

SCENARIO("copy", "[copy]"){
  Stack<int> s;
  s.push(3);
  Stack<int> s2(s);
  REQUIRE(s2.count() == 1);
  REQUIRE(s2.top() == 3);
}
