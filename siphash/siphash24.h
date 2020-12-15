/****************************** MACROS ******************************/
#define U32TO8_LE(p, v)                                                        \
    (p)[0] = (uint8_t)((v));                                                   \
    (p)[1] = (uint8_t)((v) >> 8);                                              \
    (p)[2] = (uint8_t)((v) >> 16);                                             \
    (p)[3] = (uint8_t)((v) >> 24);

#define U64TO8_LE(p, v)                                                        \
    U32TO8_LE((p), (uint32_t)((v)));                                           \
    U32TO8_LE((p) + 4, (uint32_t)((v) >> 32));

#define U8TO64_LE(p)                                                           \
    (((uint64_t)((p)[0])) | ((uint64_t)((p)[1]) << 8) |                        \
     ((uint64_t)((p)[2]) << 16) | ((uint64_t)((p)[3]) << 24) |                 \
     ((uint64_t)((p)[4]) << 32) | ((uint64_t)((p)[5]) << 40) |                 \
     ((uint64_t)((p)[6]) << 48) | ((uint64_t)((p)[7]) << 56))

#ifdef INSTRUMENTED
    #define TIC asm volatile ("rdcycle %0" : "=r" (tic)) // TIC
    #define TOC { asm volatile ("rdcycle %0" : "=r" (toc)); \
                ctr += (toc-tic);                          \
    }
#else
    #define TIC
    #define TOC
#endif
/*************************** HEADER FILES ***************************/
#include <stdint.h>
#include <stdlib.h>

/*********************** FUNCTION DECLARATIONS **********************/
unsigned long siphash(const uint8_t *in, const uint64_t inlen, const uint8_t *k, uint8_t *out);