/*
   SipHash reference C implementation
   Copyright (c) 2012-2016 Jean-Philippe Aumasson
   <jeanphilippe.aumasson@gmail.com>
   Copyright (c) 2012 Daniel J. Bernstein <djb@cr.yp.to>
   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.
   You should have received a copy of the CC0 Public Domain Dedication along
   with
   this software. If not, see
   <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <stdio.h>
#include <string.h>
#include "siphash24.h"

#ifdef INSTRUMENTED
#define PRINTHASH                                                              \
    printf("SipHash24 Digest: ");                                              \
    for (int j = 0; j < 8; ++j) {                                              \
        printf("%02x", out[j]);                                                \
    }                                                                          \
    printf("\n");
#else
#define PRINTHASH
#endif

#ifdef INSTRUMENTED
int siphash_test(uint8_t *in, uint8_t *k, FILE* outstream) {
    uint8_t out[8];
    uint64_t inlen = (uint64_t)strlen(in);
    unsigned long cycles;
    cycles = siphash(in, inlen, k, out);
    PRINTHASH;
    printf("\nTotal cycles: %lu\n",cycles);
	fprintf(outstream,"%lu ",cycles);
}
#else
int siphash_test(uint8_t *in, uint8_t *k) {
    uint8_t out[8];
    uint64_t inlen = (uint64_t)strlen(in);
    unsigned long cycles;
    cycles = siphash(in, inlen, k, out);
}
#endif

int main(int argc, char* argv[])
{
	int len;
	int n = 0;
	FILE* instream;
	if(argc <= 3){
		printf("Not enough arguments, exiting. Usage is siphash24test <length of input string> <full path of input text file> <key string>.\n");
		return(0);
	}
	else if(argc == 4){
		len = atoi(argv[1]);
		instream = fopen(argv[2],"r");
		if(instream==NULL){
			printf("File path incorrect, exiting.\n");
			return(0);
		}
        if(strlen(argv[3]) != 16){
            printf("Key must be 16 characters long, exiting.\n");
            return(0);
        }
	}
	else {
		printf("Too many arguments, exiting. Usage is siphash24test <length of input string> <full path of input text file> <key string>.\n");
		return(0);
	}
	uint8_t text[len+1];
	while(n<len)
		text[n++] = (uint8_t)(fgetc(instream));
	text[n] = '\0';
	fclose(instream);

    uint8_t key[16];
    char *buf = malloc(17);
    strcpy(buf,argv[3]);
    for(n=0;n<16;n++)
        key[n] = buf[n];   

    free(buf);
#ifdef INSTRUMENTED
	FILE* outstream;    
	outstream = fopen("/home/volkan/Dropbox/7352/Project/Code/benchmark_scripts/sipoutput.txt", "a+"); 
	siphash_test(text,key,outstream);
	fclose(outstream);
#else
    siphash_test(text,key);
#endif
	return(0);
}
