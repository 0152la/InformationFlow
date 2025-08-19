/*
 * rle_libc.h
 * https://gitlab.com/bztsrc/rle
 *
 * Copyright (C) 2020 bzt (bztsrc@gitlab)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * @brief An extremely simple (as it should be) RLE library, libc version
 *
 */

#ifndef _RLE_LIBC_H_
#define _RLE_LIBC_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>     /* realloc */
#include <string.h>     /* memcmp, memcpy */

/**
 * Encode arbitrary sized members
 */
unsigned char *rle_enc(unsigned char *inbuff, int inlen, int membsize, unsigned char *outbuff, int *outlen)
{
    int i, k, l, o, a = !outbuff;

    if(!inbuff || membsize < 1 || inlen < membsize || inlen % membsize || !outlen) return (void*)0;

    /* allocate memory for the worst case scenario */
    if(a) { outbuff = (unsigned char *)realloc((void*)0, 2 * inlen + 1); if(!outbuff) return (void*)0; }
    k = o = 0; outbuff[o++] = 0;
    for(i = 0; i < inlen; i += membsize) {
        for(l = 1; l < 128 && i + l < inlen && !memcmp(inbuff + i, inbuff + i + l*membsize, membsize); l++);
        if(l > 1) {
            l--; if(outbuff[k]) { outbuff[k]--; outbuff[o++] = 0x80 | l; } else outbuff[k] = 0x80 | l;
            memcpy(outbuff + o, inbuff + i, membsize); o += membsize; k = o; outbuff[o++] = 0; i += l*membsize; continue;
        }
        outbuff[k]++; memcpy(outbuff + o, inbuff + i, membsize); o += membsize;
        if(outbuff[k] > 127) { outbuff[k]--; k = o; outbuff[o++] = 0; }
    }
    if(!(outbuff[k] & 0x80)) { if(outbuff[k]) outbuff[k]--; else o--; }
    *outlen = o;
    /* free the extra memory */
    if(a) outbuff = (unsigned char*)realloc(outbuff, o);
    return outbuff;
}

/**
 * Decode arbitrary sized members
 */
unsigned char *rle_dec(unsigned char *inbuff, int inlen, int membsize, unsigned char *outbuff, int *outlen)
{
    int l, o = 0, a = !outbuff, s = 0;
    unsigned char *end = inbuff + inlen;

    if(!inbuff || inlen < 1 + membsize || membsize < 1 || !outlen) return (void*)0;

    while(inbuff < end) {
        l = ((*inbuff++) & 0x7F) + 1;
        /* we don't know the required buffer size in advance, so allocate memory in 64k blocks */
        if(a && o + l*membsize + 1 > s) { s+=65536; outbuff = (unsigned char*)realloc(outbuff,s); if(!outbuff) return (void*)0; }
        if(inbuff[-1] & 0x80) {
            while(l--) { memcpy(outbuff + o, inbuff, membsize); o += membsize; }
            inbuff += membsize;
        } else { l *= membsize; memcpy(outbuff + o, inbuff, l); o += l; inbuff += l; }
    }
    *outlen = o;
    /* free the extra memory */
    if(a && s != o) outbuff = (unsigned char *)realloc(outbuff, o);
    return outbuff;
}

#ifdef  __cplusplus
}
#endif

#endif

