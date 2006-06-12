#include "base64.h"
#include <stdlib.h>

void encodeblock( unsigned char in[3], unsigned char out[4], int len )
{
  out[0] = cb64[ in[0] >> 2 ];
  out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
  out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
  out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

// Following code is written by Moritz Eberl (meberl@vscope.de)
unsigned char* encode_b64( unsigned char* in, int len )
{
  int loops = len/3+len%3;
  unsigned char* out = (unsigned char*)malloc(sizeof(unsigned char)*(loops*4));
  int i,j;
  for(i=0;i<loops-1;i++)
  {
    if(len-i*3>=3)
    {
      j = 3;
    }else
    {
      j = len-i*3;
    }
    encodeblock( in+i*3, out+4*i,j );
  }
  return out;
	
}
	
