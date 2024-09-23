# benchmark-dhrystone

"DHRYSTONE" Benchmark Program by Reinhold P. Weicker

Dhrystone code was taken from https://github.com/sifive/benchmark-dhrystone and modified so all Dhrystone code is in one C file.
The implementation of unresolved dependencies used in the code, like strcmp and strcpy, were copied from https://github.com/openbsd/src/tree/master/sys/lib/libsa as their version of said functions are simple and have no dependencies.
Dependencies from GCCs libgcc implementations of e.g. multiplication and division are taken from the libgcc mirror https://github.com/gcc-mirror/gcc/blob/master/libgcc/
