/**
 * @file main.cpp tests address sanitizer in CMake which can be opened
 * by QtCreator.
 *
 * To use valgrind, simply run
 * valgrind --tool=memcheck
 * ~/Documents/docker/sanitizer_test/build/sanitizer_test
 * Do not use valgrind on a program built with sanitizer which may cause crashes.
 *
 * To use address sanitizer, add line include(sanitizers.cmake) to
 * CMakeLists.txt. No need to link against asan lib. The ASAN_OPTIONS env
 * variable can specify which type of errors to catch, e.g.,
 * export ASAN_OPTIONS=detect_stack_use_after_return=1:detect_invalid_pointer_pairs=1
 * see
 * https://kristerw.blogspot.com/2018/06/useful-gcc-address-sanitizer-checks-not.html
 * and https://github.com/google/sanitizers/wiki/AddressSanitizerFlags.
 *
 * Then build the program in Debug mode so that the stack traces have exact line
 * numbers. Then execute the program. It will exit with stack traces once a
 * memory bug is hit. The stack traces from the debug version of the program
 * should be easy to interpret. So there is no need for llvm-symbolizer or
 * asan_symbolize.py or c++filt as mentioned at
 * https://github.com/google/sanitizers/wiki/AddressSanitizerCallStack.
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
