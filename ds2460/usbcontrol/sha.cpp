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


