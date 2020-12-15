#include <stdlib.h>
#include <memory.h>
#include "asm_sha256.h"
#include <stdio.h>

/****************************** MACROS ******************************/
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

#ifdef INSTRUMENTED
    #define TIC asm volatile ("rdcycle %0" : "=r" (tic)) // TIC
    #define TOC { asm volatile ("rdcycle %0" : "=r" (toc)); \
                ctr += (toc-tic);                          \
    }
/**************************** COUNTERS *****************************/
register long *tic asm("t5");
register long *toc asm("t6");
#else
    #define TIC
    #define TOC
#endif

unsigned long ctr;

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_pad(SHA256_CTX *ctx, const BYTE data[], size_t len)
{
    WORD i;
	ctx->datalen = 64*((len+8)/64 + 1); // Length of the final data in bytes (now multiple of 512 bits (64-bytes))
    ctx->bitlen = 8*len; // Length of the string in bits (l)
    ctx->data = (BYTE*) malloc(ctx->datalen); // Allocate the data array

    // Copy the string to data array
    for (i = 0; i < len; ++i)
		ctx->data[i] = data[i];

    // Padding
    i = len; // this is probably not needed after above loop, but in any case
    ctx->data[i++] = 0x80; // Append 1 right after the string (followed by zeros)
	for ( ; i < ctx->datalen - 8; ++i)
        ctx->data[i] = 0x00; // Pad the rest of the zeros until the last 8 bytes

    // Finally append the message's length in bits to the final 8 bytes
	ctx->data[ctx->datalen - 1] = ctx->bitlen;
	ctx->data[ctx->datalen - 2] = ctx->bitlen >> 8;
	ctx->data[ctx->datalen - 3] = ctx->bitlen >> 16;
	ctx->data[ctx->datalen - 4] = ctx->bitlen >> 24;
	ctx->data[ctx->datalen - 5] = ctx->bitlen >> 32;
	ctx->data[ctx->datalen - 6] = ctx->bitlen >> 40;
	ctx->data[ctx->datalen - 7] = ctx->bitlen >> 48;
	ctx->data[ctx->datalen - 8] = ctx->bitlen >> 56;
}

void sha256_init()
{	
	ctr = 0;
    TIC;
    //asm volatile ("rdcycle %0" : "=r" (tic)); // TIC

    asm volatile
    (
        "sha256init   zero, zero, zero\n\t"
        : 
        : 
    );

    TOC;
    //asm volatile ("rdcycle %0" : "=r" (toc)); // TOC
    //ctr += (toc-tic);
}

void sha256_transform(const BYTE data[])
{
	WORD i, j;
    WORD m32[16];
    DWORD m[8];

    TIC;
    //asm volatile ("rdcycle %0" : "=r" (tic)); // TIC

    for (i = 0, j = 0; i < 16; ++i, j += 4)
		m32[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    
    m[0] = ((DWORD)m32[0] << 32) | ((DWORD)m32[1]);
    m[1] = ((DWORD)m32[2] << 32) | ((DWORD)m32[3]);
    m[2] = ((DWORD)m32[4] << 32) | ((DWORD)m32[5]);
    m[3] = ((DWORD)m32[6] << 32) | ((DWORD)m32[7]);
    m[4] = ((DWORD)m32[8] << 32) | ((DWORD)m32[9]);
    m[5] = ((DWORD)m32[10] << 32) | ((DWORD)m32[11]);
    m[6] = ((DWORD)m32[12] << 32) | ((DWORD)m32[13]);
    m[7] = ((DWORD)m32[14] << 32) | ((DWORD)m32[15]);

    asm volatile
    (
        "sha256sch0   zero, %[rs1], %[rs2]\n\t"
        : 
        : [rs1] "r" (m[0]), [rs2] "r" (m[1])
    );
    asm volatile
    (
        "sha256sch1   zero, %[rs1], %[rs2]\n\t"
        : 
        : [rs1] "r" (m[2]), [rs2] "r" (m[3])
    );
    asm volatile
    (
        "sha256sch2   zero, %[rs1], %[rs2]\n\t"
        : 
        : [rs1] "r" (m[4]), [rs2] "r" (m[5])
    );
    asm volatile
    (
        "sha256sch3   zero, %[rs1], %[rs2]\n\t"
        : 
        : [rs1] "r" (m[6]), [rs2] "r" (m[7])
    );

    TOC;
    //asm volatile ("rdcycle %0" : "=r" (toc)); // TOC
    //ctr += (toc-tic);
}

void sha256_update(SHA256_CTX *ctx)
{
	WORD i;
    WORD bytecount = 0;
    BYTE data[64];

	for (i = 0; i < ctx->datalen; ++i) { // Transform (sch0-3) in 64-byte chunks
		data[bytecount] = ctx->data[i];
		bytecount++;
		if (bytecount == 64) {
			sha256_transform(data);
			bytecount = 0;
		}
	}
}

unsigned long sha256_final(SHA256_CTX *ctx, BYTE hash[])
{
	WORD i;
    DWORD digest[4]; // digest output

    //asm volatile ("rdcycle %0" : "=r" (tic)); // TIC
    TIC;

    asm volatile
    (
        "sha256fin0   %[rd], zero, zero\n\t"
        : [rd] "=r" (digest[0])
        :
    );
    asm volatile
    (
        "sha256fin1   %[rd], zero, zero\n\t"
        : [rd] "=r" (digest[1])
        :
    );
    asm volatile
    (
        "sha256fin2   %[rd], zero, zero\n\t"
        : [rd] "=r" (digest[2])
        :
    );
    asm volatile
    (
        "sha256fin3   %[rd], zero, zero\n\t"
        : [rd] "=r" (digest[3])
        :
    );

    //asm volatile ("rdcycle %0" : "=r" (toc)); // TOC
    //ctr += (toc-tic);

    TOC;

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash

    for (i = 0; i < 8; ++i) {
		hash[i]      = (digest[0] >> (56 - i * 8)) & 0x0000ffff;
		hash[i + 8]  = (digest[1] >> (56 - i * 8)) & 0x0000ffff;
		hash[i + 16] = (digest[2] >> (56 - i * 8)) & 0x0000ffff;
		hash[i + 24] = (digest[3] >> (56 - i * 8)) & 0x0000ffff;
	}

    free(ctx->data);
    
    unsigned long final_cycle_count = ctr;
    return final_cycle_count;
}

