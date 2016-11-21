#include <stack.hpp>
#include <catch.hpp>
#include <iostream>

SCENARIO("count", "[count]"){
  Stack<int> s;
  s.push(1);
  REQUIRE(s.count()==1);
}

SCENARIO("push", "[push]"){
  Stack<int> s;
  s.push(1);
  REQUIRE(s.count()==1);

}

SCENARIO("top", "[top]"){
  Stack<int> s;
  s.push(1);
  s.push(2);
  s.push(3);
  s.pop();

  REQUIRE(s.top()==2);
}
SCENARIO("operprisv", "[operprisv]"){
  Stack<int> s1;
  s1.push(1);
  Stack<int> s2;
  s2=s1;
  REQUIRE(s1.count()==s2.count());
}

SCENARIO("const", "[constr]"){
  Stack<int> s1;
  s1.push(1);
  Stack<int> s2=s1;
  REQUIRE(s1.count()==s2.count());
}

SCENARIO("empty", "[empty]"){
  Stack<int> s1;
  s1.push(1);
  REQUIRE(s1.empty()==false);
}

SCENARIO("empty2", "[empty2]"){
  Stack<int> s1;
  s1.push(1);
  s1.pop();
  REQUIRE(s1.empty()==true);
}

SCENARIO("empty3", "[empty3]"){
  Stack<int> s1;
  s1.push(1);
  s1.push(2);
  s1.pop();
  s1.top();

  REQUIRE(s1.empty()==false);
}

class Test
{
public:
  static ssize_t  counter;
  Test()
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    ++counter;
  }
  Test( Test const & )
  {
   std::cout << __PRETTY_FUNCTION__ << std::endl;
    ++counter;
  }
  ~Test()
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    --counter;
  }
};

ssize_t Test::counter = 0;

SCENARIO("Allocator::~Allocator") {

  {
    Allocator<Test> allocator{ 4 };
    allocator.construct( allocator.get() + 1, Test() );
    allocator.construct( allocator.get() + 2, Test() );

    REQUIRE( Test::counter == 2 );
  }

  REQUIRE( Test::counter == 0 );
}
