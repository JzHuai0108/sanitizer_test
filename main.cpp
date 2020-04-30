/**
 * @file main.cpp tests address sanitizer and valgrind in CMake.
 */
#include <iostream>

using namespace std;

struct Foo {
  Foo(const int& bar) : bar(bar) {}

  const int& bar;
};

Foo generate() {
  int i = 99;
  return Foo(i);
}


int main() {
  Foo foo = generate();
  std::cout << "Accessing a destroyed stack object\n";
  std::cout << foo.bar
            << std::endl;  // This bug is not detected by address sanitizer.

  std::cout << "Deleting an deleted ptr\n";
  int* a = new int(200);
  delete a;
  std::cout << "Value in deleted pointer " << *a << std::endl;
  std::cout << "Exiting the program.\n";
}
