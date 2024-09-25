#include <limits.h>

/* Nonzero if either X or Y is not aligned on a "long" boundary.  */
#define UNALIGNED(X, Y) \
    (((long)X & (sizeof(long) - 1)) | ((long)Y & (sizeof(long) - 1)))

/* DETECTNULL returns nonzero if (long)X contains a NULL byte. */
#if LONG_MAX == 2147483647L
#define DETECTNULL(X) (((X) - 0x01010101) & ~(X) & 0x80808080)
#else
#if LONG_MAX == 9223372036854775807L
#define DETECTNULL(X) (((X) - 0x0101010101010101) & ~(X) & 0x8080808080808080)
#else
#error long int is not a 32bit or 64bit type.
#endif
#endif

#ifndef DETECTNULL
#error long int is not a 32bit or 64bit byte
#endif

int strcmp(const char *s1, const char *s2)
{
#if defined(PREFER_SIZE_OVER_SPEED) || defined(__OPTIMIZE_SIZE__)
    while (*s1 != '\0' && *s1 == *s2)
    {
        s1++;
        s2++;
    }

    return (*(unsigned char *)s1) - (*(unsigned char *)s2);
#else
    unsigned long *a1;
    unsigned long *a2;

    /* If s1 or s2 are unaligned, then compare bytes. */
    if (!UNALIGNED(s1, s2))
    {
        /* If s1 and s2 are word-aligned, compare them a word at a time. */
        a1 = (unsigned long *)s1;
        a2 = (unsigned long *)s2;
        while (*a1 == *a2)
        {
            /* To get here, *a1 == *a2, thus if we find a null in *a1,
           then the strings must be equal, so return zero.  */
            if (DETECTNULL(*a1))
                return 0;

            a1++;
            a2++;
        }

        /* A difference was detected in last few bytes of s1, so search bytewise */
        s1 = (char *)a1;
        s2 = (char *)a2;
    }

    while (*s1 != '\0' && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1) - (*(unsigned char *)s2);
#endif /* not PREFER_SIZE_OVER_SPEED */
}

char *strcpy(char *dst0, const char *src0)
{
#if defined(PREFER_SIZE_OVER_SPEED) || defined(__OPTIMIZE_SIZE__)
    char *s = dst0;

    while (*dst0++ = *src0++)
        ;

    return s;
#else
    char *dst = dst0;
    const char *src = src0;
    long *aligned_dst;
    const long *aligned_src;

    /* If SRC or DEST is unaligned, then copy bytes.  */
    if (!UNALIGNED(src, dst))
    {
        aligned_dst = (long *)dst;
        aligned_src = (long *)src;

        /* SRC and DEST are both "long int" aligned, try to do "long int"
           sized copies.  */
        while (!DETECTNULL(*aligned_src))
        {
            *aligned_dst++ = *aligned_src++;
        }

        dst = (char *)aligned_dst;
        src = (char *)aligned_src;
    }

    while ((*dst++ = *src++))
        ;
    return dst0;
#endif /* not PREFER_SIZE_OVER_SPEED */
}