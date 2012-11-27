//---------------------------------------------------------------------------
// Copyright (C) 2005 Dallas Semiconductor Corporation, All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// Except as contained in this notice, the name of Dallas Semiconductor
// shall not be used except as stated in the Dallas Semiconductor
// Branding Policy.
//---------------------------------------------------------------------------
//
//  DS2460.C - SHA-1 algorithm
//
//  Version:   1.00
//  History:
//

//#include "SHA.h"

#include <string.h>
static long KTN (int n);
static long NLF (long B, long C, long D, int n);
//----------------------------------------------------------------------
// Compute the 160-bit MAC
//
// 'MT'  - input data, 64-byte defined respectively in the datasheet of DS2460, DS28E01-100 or DS2432
// 'A'   - part of the 160 bits of MAC
// 'B'   - part of the 160 bits of MAC
// 'C'   - part of the 160 bits of MAC
// 'D'   - part of the 160 bits of MAC
// 'E'   - part of the 160 bits of MAC
//
// Note: This algorithm is the SHA-1 algorithm as specified in the
// datasheet for the DS2460, DS28E01-100, DS2432, where the last step of the official
// FIPS-180 SHA routine is omitted (which only involves the addition of
// constant values).
//
//
void ComputeSHAEE(unsigned char *MT,long *A,long *B, long *C, long *D,long *E)
{
   unsigned long MTword[80];
   int i;
   long ShftTmp;
   long Temp;

   for(i=0;i<16;i++)
      MTword[i] = (MT[i*4] << 24) | (MT[i*4+1] << 16) |
                  (MT[i*4+2] << 8) | MT[i*4+3];

   for(i=16;i<80;i++)
   {
      ShftTmp = MTword[i-3] ^ MTword[i-8] ^ MTword[i-14] ^ MTword[i-16];
      MTword[i] = ((ShftTmp << 1) & 0xfffffffe) |
                  ((ShftTmp >> 31) & 0x00000001);
   }

   *A=0x67452301;
   *B=0xefcdab89;
   *C=0x98badcfe;
   *D=0x10325476;
   *E=0xc3d2e1f0;

   for(i=0;i<80;i++)
   {
      ShftTmp = ((*A << 5) & 0xffffffe0) | ((*A >> 27) & 0x0000001f);
      Temp = NLF(*B,*C,*D,i) + *E + KTN(i) + MTword[i] + ShftTmp;
      *E = *D;
      *D = *C;
      *C = ((*B << 30) & 0xc0000000) | ((*B >> 2) & 0x3fffffff);
      *B = *A;
      *A = Temp;
   }
}

// calculation used for the MAC
static long KTN (int n)
{
   if(n<20)
      return 0x5a827999;
   else if (n<40)
      return 0x6ed9eba1;
   else if (n<60)
      return 0x8f1bbcdc;
   else
      return 0xca62c1d6;
}

// calculation used for the MAC
static long NLF (long B, long C, long D, int n)
{
   if(n<20)
      return ((B&C)|((~B)&D));
   else if(n<40)
      return (B^C^D);
   else if(n<60)
      return ((B&C)|(B&D)|(C&D));
   else
      return (B^C^D);
}

static unsigned char SHAVM_Message[64];
// Hash buffer, in wrong order for Dallas MAC
static unsigned long  SHAVM_Hash[5];
// Temp vars for SHA calculation
static unsigned long SHAVM_MTword[80];
static long SHAVM_Temp;
static int SHAVM_cnt;
static long SHAVM_KTN[4];

static void SHAVM_Compute(unsigned char *SHAVM_MAC)
{
   SHAVM_KTN[0]=(long)0x5a827999;
   SHAVM_KTN[1]=(long)0x6ed9eba1;
   SHAVM_KTN[2]=(long)0x8f1bbcdc;
   SHAVM_KTN[3]=(long)0xca62c1d6;
   for(SHAVM_cnt=0; SHAVM_cnt<16; SHAVM_cnt++)
   {
      SHAVM_MTword[SHAVM_cnt]
         = (((long)SHAVM_Message[SHAVM_cnt*4]&0x00FF) << 24L)
         | (((long)SHAVM_Message[SHAVM_cnt*4+1]&0x00FF) << 16L)
         | (((long)SHAVM_Message[SHAVM_cnt*4+2]&0x00FF) << 8L)
         |  ((long)SHAVM_Message[SHAVM_cnt*4+3]&0x00FF);
   }

   for(; SHAVM_cnt<80; SHAVM_cnt++)
   {
      SHAVM_Temp
         = SHAVM_MTword[SHAVM_cnt-3]  ^ SHAVM_MTword[SHAVM_cnt-8]
         ^ SHAVM_MTword[SHAVM_cnt-14] ^ SHAVM_MTword[SHAVM_cnt-16];
      SHAVM_MTword[SHAVM_cnt]
         = ((SHAVM_Temp << 1) & 0xFFFFFFFE)
         | ((SHAVM_Temp >> 31) & 0x00000001);
   }

   SHAVM_Hash[0] = 0x67452301;
   SHAVM_Hash[1] = 0xEFCDAB89;
   SHAVM_Hash[2] = 0x98BADCFE;
   SHAVM_Hash[3] = 0x10325476;
   SHAVM_Hash[4] = 0xC3D2E1F0;

   for(SHAVM_cnt=0; SHAVM_cnt<80; SHAVM_cnt++)
   {
      SHAVM_Temp
         = ((SHAVM_Hash[0] << 5) & 0xFFFFFFE0)
         | ((SHAVM_Hash[0] >> 27) & 0x0000001F);
      if(SHAVM_cnt<20)
         SHAVM_Temp += ((SHAVM_Hash[1]&SHAVM_Hash[2])|((~SHAVM_Hash[1])&SHAVM_Hash[3]));
      else if(SHAVM_cnt<40)
         SHAVM_Temp += (SHAVM_Hash[1]^SHAVM_Hash[2]^SHAVM_Hash[3]);
      else if(SHAVM_cnt<60)
         SHAVM_Temp += ((SHAVM_Hash[1]&SHAVM_Hash[2])
                       |(SHAVM_Hash[1]&SHAVM_Hash[3])
                       |(SHAVM_Hash[2]&SHAVM_Hash[3]));
      else
         SHAVM_Temp += (SHAVM_Hash[1]^SHAVM_Hash[2]^SHAVM_Hash[3]);
      SHAVM_Temp += SHAVM_Hash[4] + SHAVM_KTN[SHAVM_cnt/20]
                  + SHAVM_MTword[SHAVM_cnt];
      SHAVM_Hash[4] = SHAVM_Hash[3];
      SHAVM_Hash[3] = SHAVM_Hash[2];
      SHAVM_Hash[2]
         = ((SHAVM_Hash[1] << 30) & 0xC0000000)
         | ((SHAVM_Hash[1] >> 2) & 0x3FFFFFFF);
      SHAVM_Hash[1] = SHAVM_Hash[0];
      SHAVM_Hash[0] = SHAVM_Temp;
   }

   //iButtons use LSB first, so we have to turn
   //the result around a little bit.  Instead of
   //result A-B-C-D-E, our result is E-D-C-B-A,
   //where each letter represents four bytes of
   //the result.
   for(SHAVM_cnt=0; SHAVM_cnt<5; SHAVM_cnt++)
   {
      SHAVM_Temp = SHAVM_Hash[4-SHAVM_cnt];
      SHAVM_MAC[((SHAVM_cnt)*4)+0] = (unsigned char)SHAVM_Temp;
      SHAVM_Temp >>= 8;
      SHAVM_MAC[((SHAVM_cnt)*4)+1] = (unsigned char)SHAVM_Temp;
      SHAVM_Temp >>= 8;
      SHAVM_MAC[((SHAVM_cnt)*4)+2] = (unsigned char)SHAVM_Temp;
      SHAVM_Temp >>= 8;
      SHAVM_MAC[((SHAVM_cnt)*4)+3] = (unsigned char)SHAVM_Temp;
   }
   
}

void get_shamac(unsigned char *read_sha_input, unsigned char *cur_secrect, unsigned char *mac)
{
	memcpy(SHAVM_Message, read_sha_input, 55);
	memcpy(SHAVM_Message, cur_secrect, 4);
	memcpy(&SHAVM_Message[48], &cur_secrect[4], 4);

	SHAVM_Message[55] = 0x80;
	memset(&SHAVM_Message[56], 0x00, 6);
	SHAVM_Message[62] = 0x01;
	SHAVM_Message[63] = 0xB8;
	SHAVM_Compute(mac);
}
