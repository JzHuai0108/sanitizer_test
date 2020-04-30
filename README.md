
Address sanitizer and valgrind test

## Clone and build
```
git clone --recursive sanitizer_test
cd sanitizer_test
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```


## Code analysis with valgrind

The above build steps will produce an executable sanitizer_test/build/sanitizer_test 
which is not instrumented with address sanitizer.
And it is supposed to demonstrate how to use valgrind.

valgrind can be installed via
```
sudo apt-get install valgrind
```

To analyze the exectuable sanitizer_test with valgrind, run in a terminal.
```
valgrind --tool=memcheck ./sanitizer_test
```

The expected output is like below, noting two errors after the executable printed messages
 "Accessing a destroyed stack object" and "Deleting an deleted ptr".

```
==26323== Memcheck, a memory error detector
==26323== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==26323== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==26323== Command: ./sanitizer_test
==26323== 
Accessing a destroyed stack object
==26323== Conditional jump or move depends on uninitialised value(s)
==26323==    at 0x4F43B2A: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x4F50074: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x108A8C: main (main.cpp:23)
==26323== 
==26323== Use of uninitialised value of size 8
==26323==    at 0x4F4362E: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x4F43B53: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x4F50074: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x108A8C: main (main.cpp:23)
==26323== 
==26323== Conditional jump or move depends on uninitialised value(s)
==26323==    at 0x4F4363B: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x4F43B53: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x4F50074: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x108A8C: main (main.cpp:23)
==26323== 
==26323== Conditional jump or move depends on uninitialised value(s)
==26323==    at 0x4F43B86: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x4F50074: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==26323==    by 0x108A8C: main (main.cpp:23)
==26323== 
0
Deleting an deleted ptr
==26323== Invalid read of size 4
==26323==    at 0x108AEF: main (main.cpp:29)
==26323==  Address 0x5b7e0c0 is 0 bytes inside a block of size 4 free'd
==26323==    at 0x4C3123B: operator delete(void*) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==26323==    by 0x108AD4: main (main.cpp:28)
==26323==  Block was alloc'd at
==26323==    at 0x4C3017F: operator new(unsigned long) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==26323==    by 0x108ABE: main (main.cpp:27)
==26323== 
Value in deleted pointer 200
Exiting the program.
==26323== 
==26323== HEAP SUMMARY:
==26323==     in use at exit: 0 bytes in 0 blocks
==26323==   total heap usage: 3 allocs, 3 frees, 73,732 bytes allocated
==26323== 
==26323== All heap blocks were freed -- no leaks are possible
==26323== 
==26323== For counts of detected and suppressed errors, rerun with: -v
==26323== Use --track-origins=yes to see where uninitialised values come from
==26323== ERROR SUMMARY: 5 errors from 5 contexts (suppressed: 0 from 0)

```

## Code analysis with address sanitizer

To use address sanitizer, we need to run cmake with arguments as
```
cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZER=Address
make

./sanitizer_test
```

To reveal the stack use after return error, specify the ASAN_OPTIONS env variable by
```
export ASAN_OPTIONS=detect_stack_use_after_return=1
```
Then run the executale sanitizer_test again, the error "stack_use_after_return" will show up.

The expected output is like below.
```
Accessing a destroyed stack object
187
Deleting an deleted ptr
=================================================================
==26773==ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000010 at pc 0x558b06c97497 bp 0x7fffc42a7e00 sp 0x7fffc42a7df0
READ of size 4 at 0x602000000010 thread T0
    #0 0x558b06c97496 in main /home/user/Documents/docker/sanitizer_test/main.cpp:29
    #1 0x7f271bce0b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #2 0x558b06c97079 in _start (/home/user/Documents/docker/sanitizer_test/build/sanitizer_test+0x1079)

0x602000000010 is located 0 bytes inside of 4-byte region [0x602000000010,0x602000000014)
freed by thread T0 here:
    #0 0x7f271c51a2c0 in operator delete(void*) (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xe12c0)
    #1 0x558b06c97446 in main /home/user/Documents/docker/sanitizer_test/main.cpp:28
    #2 0x7f271bce0b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)

previously allocated by thread T0 here:
    #0 0x7f271c519448 in operator new(unsigned long) (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xe0448)
    #1 0x558b06c973f3 in main /home/user/Documents/docker/sanitizer_test/main.cpp:27
    #2 0x7f271bce0b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)

SUMMARY: AddressSanitizer: heap-use-after-free /home/user/Documents/docker/sanitizer_test/main.cpp:29 in main
Shadow bytes around the buggy address:
  0x0c047fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c047fff8000: fa fa[fd]fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==26773==ABORTING

```

## Use address sanitizer in another cmake project
1. Add line "include(sanitizers.cmake)" to CMakeLists.txt. No need to link against asan lib. 
2. Pass "-DUSE_SANITIZER=Address" to cmake command. Build in debug mode is recommended for clear numbering in stack traces.
3. Build the executable.
4. Export the ASAN_OPTIONS env variable if needed.
5. Run the executable and it will abort once a memory bug is hit.
6. If you find the stack traces hard to interpret, try llvm-symbolizer or
asan_symbolize.py or c++filt as mentioned at [here](https://github.com/google/sanitizers/wiki/AddressSanitizerCallStack).

## References
[1] [Useful GCC address sanitizer checks not enabled by default](https://kristerw.blogspot.com/2018/06/useful-gcc-address-sanitizer-checks-not.html).

[2] [AddressSanitizerFlags in google/sanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizerFlags)


