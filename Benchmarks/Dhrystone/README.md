# benchmark-dhrystone

"DHRYSTONE" Benchmark Program by Reinhold P. Weicker

Code taken from https://github.com/sifive/benchmark-dhrystone and modified so all code is in one C file, without any unresolved dependencies to other libraries. This includes removing timing and printing functions.
The implementation of unresolved dependencies, like strcmp and strcpy, were copied from https://github.com/openbsd/src/tree/master/sys/lib/libsa as their version of said functions have no dependencies.
