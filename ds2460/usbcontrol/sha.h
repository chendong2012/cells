#ifndef __SHA_H__
#define __SHA_H__
void ComputeSHAEE(unsigned char *MT,long *A,long *B, long *C, long *D,long *E);
void get_shamac(unsigned char *read_sha_input, unsigned char *cur_secrect, unsigned char *mac);
#endif
