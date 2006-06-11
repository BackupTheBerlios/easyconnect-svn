/*
 * Taken from Bob Trower`s b64.h 
 * So for everybody here is the license
 
LICENCE:        Copyright (c) 2001 Bob Trower, Trantor Standard Systems Inc.
 
                 Permission is hereby granted, free of charge, to any person
                 obtaining a copy of this software and associated
                 documentation files (the "Software"), to deal in the
                 Software without restriction, including without limitation
                 the rights to use, copy, modify, merge, publish, distribute,
                 sublicense, and/or sell copies of the Software, and to
                 permit persons to whom the Software is furnished to do so,
                 subject to the following conditions:
 
                 The above copyright notice and this permission notice shall
                 be included in all copies or substantial portions of the
                 Software.
 
                 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
                 KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
                 WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
                 PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
                 OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
                 OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
                 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
                 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */
/*
 * Translation Table as described in RFC1113
 */
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
 * Translation Table to decode (created by author)
 */
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

/*
 * encodeblock
 *
 * encode 3 8-bit binary bytes as 4 '6-bit' characters
 */
void encodeblock( unsigned char in[3], unsigned char out[4], int len );


// Following code is written by Moritz Eberl (meberl@vscope.de)
unsigned char* encode_b64( unsigned char* in, int len );

	

