// Code found at https://github.com/gcc-mirror/gcc/blob/master/libgcc

// In the degree it's possible we want functions from the riscv configuration. However, since riscv has these in assembly only we need to take their
// C source code from elsewhere

// libgcc/udivmodsi4.c
unsigned long __udivmodsi4(unsigned long num, unsigned long den, int modwanted)
{
    unsigned long bit = 1;
    unsigned long res = 0;

    while (den < num && bit && !(den & (1L << 31)))
    {
        den <<= 1;
        bit <<= 1;
    }
    while (bit)
    {
        if (num >= den)
        {
            num -= den;
            res |= bit;
        }
        bit >>= 1;
        den >>= 1;
    }
    if (modwanted)
        return num;
    return res;
}

// libgcc/divmod.c
long __divsi3(long a, long b)
{
    int neg = 0;
    long res;

    if (a < 0)
    {
        a = -a;
        neg = !neg;
    }

    if (b < 0)
    {
        b = -b;
        neg = !neg;
    }

    res = __udivmodsi4(a, b, 0);

    if (neg)
        res = -res;

    return res;
}

// libcc/Epiphany/mulsi3.c
// GCC only has assembly code only for __mulsi3 for the riscv config, but that assembly code acts like this code which is taken from the
// Epiphany configuration
unsigned int __mulsi3(unsigned int a, unsigned int b)
{
    unsigned int r = 0;

    while (a)
    {
        if (a & 1)
            r += b;
        a >>= 1;
        b <<= 1;
    }
    return r;
}