#include "siphash24.h"

#ifdef INSTRUMENTED
/**************************** COUNTERS *****************************/
register long *tic asm("t5");
register long *toc asm("t6");
#endif
unsigned long ctr;

/*********************** FUNCTION DEFINITIONS **********************/
unsigned long siphash(const uint8_t *in, const uint64_t inlen, const uint8_t *k, uint8_t *out) {
    uint64_t k0 = U8TO64_LE(k);
    uint64_t k1 = U8TO64_LE(k + 8);
    int i;
    const uint64_t reglen = (inlen / (sizeof(uint64_t))) + 1; // denotes how many registers it would take to represent the message
    const uint64_t looplen = reglen/2;
    const int extra = reglen % 2;
    uint64_t m[reglen];
    const int left = inlen & 7;
    uint64_t b = inlen << 56; // the message is b-bytes, so this puts b mod 256 at the top byte of m(w-1)

    // Formatting and padding the message
    for(i=0;i<(reglen-1);++i,in+=8)
        m[i] = U8TO64_LE(in);

    switch (left) {
    case 7:
        b |= ((uint64_t)in[6]) << 48;
    case 6:
        b |= ((uint64_t)in[5]) << 40;
    case 5:
        b |= ((uint64_t)in[4]) << 32;
    case 4:
        b |= ((uint64_t)in[3]) << 24;
    case 3:
        b |= ((uint64_t)in[2]) << 16;
    case 2:
        b |= ((uint64_t)in[1]) << 8;
    case 1:
        b |= ((uint64_t)in[0]);
        break;
    case 0:
        break;
    }
    m[i] = b;

    ctr = 0;
    TIC;

    asm volatile
    (
        "siph24i   zero, %[rs1], %[rs2]\n\t"
        : 
        : [rs1] "r" (k0), [rs2] "r" (k1)
    );

    for (i=0;i<looplen;i++) {
        asm volatile
        (
        "siph24c   zero, %[rs1], %[rs2]\n\t"
        : 
        : [rs1] "r" (m[i*2]), [rs2] "r" (m[(i*2)+1])
        );
    }
    if (extra) {
        asm volatile
        (
        "siph24c   zero, %[rs1], zero\n\t"
        : 
        : [rs1] "r" (m[i*2])
        );
    }

    asm volatile
    (
        "siph24f   %[rd], zero, zero\n\t"
        : [rd] "=r" (b)
        :
    );

    TOC;

    U64TO8_LE(out, b);
    
    return ctr;  
}