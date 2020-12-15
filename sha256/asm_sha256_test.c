/*********************************************************************
* Filename:   sha256_test.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Performs known-answer tests on the corresponding SHA1
	          implementation. These tests do not encompass the full
	          range of available test vectors, however, if the tests
	          pass it is very, very likely that the code is correct
	          and was compiled properly. This code also serves as
	          example usage of the functions.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "asm_sha256.h"

/*********************** FUNCTION DEFINITIONS ***********************/
int sha256_test()
{
	BYTE text1[] = {"abc"};
	BYTE text2[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
	/* BYTE text3[] = {"aaaaaaaaaa"}; */
	BYTE hash1[SHA256_BLOCK_SIZE] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
	                                 0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
	BYTE hash2[SHA256_BLOCK_SIZE] = {0x24,0x8d,0x6a,0x61,0xd2,0x06,0x38,0xb8,0xe5,0xc0,0x26,0x93,0x0c,0x3e,0x60,0x39,
	                                 0xa3,0x3c,0xe4,0x59,0x64,0xff,0x21,0x67,0xf6,0xec,0xed,0xd4,0x19,0xdb,0x06,0xc1};
	/* BYTE hash3[SHA256_BLOCK_SIZE] = {0xcd,0xc7,0x6e,0x5c,0x99,0x14,0xfb,0x92,0x81,0xa1,0xc7,0xe2,0x84,0xd7,0x3e,0x67,
	                                 0xf1,0x80,0x9a,0x48,0xa4,0x97,0x20,0x0e,0x04,0x6d,0x39,0xcc,0xc7,0x11,0x2c,0xd0}; */
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int idx;
	int pass = 1;
    sha256_pad(&ctx, text1, strlen(text1));
	sha256_init();
	sha256_update(&ctx);
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash1, buf, SHA256_BLOCK_SIZE);

    sha256_pad(&ctx, text2, strlen(text2));
	sha256_init();
	sha256_update(&ctx);
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash2, buf, SHA256_BLOCK_SIZE);

    /* sha256_pad(&ctx, text3, strlen(text3));
	sha256_init();
	for (idx = 0; idx < 100000; ++idx)
	   sha256_update(&ctx);
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash3, buf, SHA256_BLOCK_SIZE); */

	return(pass);
}

#ifdef INSTRUMENTED
void sha256_conf(BYTE* text, FILE* outstream)
{
	// BYTE text[] = {"neu/ndn/vid/s00/f00"};
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int idx;
	unsigned long cycles;
    sha256_pad(&ctx, text, strlen(text));
	sha256_init();
	sha256_update(&ctx);
	cycles = sha256_final(&ctx, buf);
	printf("SHA-256 digest: ");
	for(idx=0; idx<SHA256_BLOCK_SIZE; idx++)
		printf("%02x",buf[idx]);
	printf("\nTotal cycles: %lu\n",cycles);
	fprintf(outstream,"%lu ",cycles);
}
#else
void sha256_conf(BYTE* text)
{
	// BYTE text[] = {"neu/ndn/vid/s00/f00"};
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int idx;
	unsigned long cycles;
    sha256_pad(&ctx, text, strlen(text));
	sha256_init();
	sha256_update(&ctx);
	cycles = sha256_final(&ctx, buf);
}
#endif


int main(int argc, char* argv[])
{
	// printf("SHA-256 tests: %s\n", sha256_test() ? "SUCCEEDED" : "FAILED");
	int len;
	int n = 0;
	FILE* instream;
	if(argc <= 2){
		printf("Not enough arguments, exiting. Usage is asm_sha256test <length of input string> <full path of input text file>\n");
		return(0);
	}
	else if(argc == 3){
		len = atoi(argv[1]);
		instream = fopen(argv[2],"r");
		if(instream==NULL){
			printf("File path incorrect, exiting.\n");
			return(0);
		}
	}
	else {
		printf("Too many arguments, exiting.\n");
		return(0);
	}
	BYTE text[len+1];
	while(n<len)
		text[n++] = (BYTE)(fgetc(instream));
	text[n] = '\0';
	fclose(instream);
#ifdef INSTRUMENTED
	FILE* outstream;
	outstream = fopen("/home/volkan/Dropbox/7352/Project/Code/benchmark_scripts/output.txt", "a+"); 
	sha256_conf(text,outstream);
	fclose(outstream);
#else
	sha256_conf(text);
#endif
	return(0);
}
