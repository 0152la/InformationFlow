/*
 * rle_opt.h
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
 * @brief An extremely simple (as it should be) RLE library, dependency-free versions
 *
 */

/**
 * Encode bytes
 */
void rle_enc8(unsigned char *inbuff, int inlen, unsigned char *outbuff, int *outlen)
{
    int i, k, l, o;

    if(!inbuff || inlen < 1 || !outbuff || !outlen) return;

    k = o = 0; outbuff[o++] = 0;
    for(i = 0; i < inlen; i++) {
        for(l = 1; l < 128 && i + l < inlen && inbuff[i] == inbuff[i + l]; l++);
        if(l > 1) {
            l--; if(outbuff[k]) { outbuff[k]--; outbuff[o++] = 0x80 | l; } else outbuff[k] = 0x80 | l;
            outbuff[o++] = inbuff[i]; k = o; outbuff[o++] = 0; i += l; continue;
        }
        outbuff[k]++; outbuff[o++] = inbuff[i];
        if(outbuff[k] > 127) { outbuff[k]--; k = o; outbuff[o++] = 0; }
    }
    if(!(outbuff[k] & 0x80)) { if(outbuff[k]) outbuff[k]--; else o--; }
    *outlen = o;
}

/**
 * Encode double bytes
 */
void rle_enc16(unsigned char *inbuff, int inlen, unsigned char *outbuff, int *outlen)
{
    unsigned short int *data = (unsigned short int *)inbuff;
    int i, k, l, o;

    inlen >>= 1;
    if(!inbuff || inlen < 1 || !outbuff || !outlen) return;

    k = o = 0; outbuff[o++] = 0;
    for(i = 0; i < inlen; i++) {
        for(l = 1; l < 128 && i + l < inlen && data[i] == data[i + l]; l++);
        if(l > 1) {
            l--; if(outbuff[k]) { outbuff[k]--; outbuff[o++] = 0x80 | l; } else outbuff[k] = 0x80 | l;
            *((unsigned short int*)(outbuff + o)) = data[i]; o += 2; k = o; outbuff[o++] = 0; i += l; continue;
        }
        outbuff[k]++; *((unsigned short int*)(outbuff + o)) = data[i]; o += 2;
        if(outbuff[k] > 127) { outbuff[k]--; k = o; outbuff[o++] = 0; }
    }
    if(!(outbuff[k] & 0x80)) { if(outbuff[k]) outbuff[k]--; else o--; }
    *outlen = o;
}

/**
 * Encode triple bytes
 */
void rle_enc24(unsigned char *inbuff, int inlen, unsigned char *outbuff, int *outlen)
{
    int i, k, l, o;

    if(!inbuff || inlen < 3 || !outbuff || !outlen) return;

    k = o = 0; outbuff[o++] = 0;
    for(i = 0; i < inlen; i += 3) {
        for(l = 1; l < 128 && i + l < inlen && inbuff[i] == inbuff[i + l*3] && inbuff[i+1] == inbuff[i+1 + l*3] &&
            inbuff[i+2] == inbuff[i+2 + l*3]; l++);
        if(l > 1) {
            l--; if(outbuff[k]) { outbuff[k]--; outbuff[o++] = 0x80 | l; } else outbuff[k] = 0x80 | l;
            outbuff[o] = inbuff[i]; outbuff[o+1] = inbuff[i+1]; outbuff[o+2] = inbuff[i+2]; o += 3; k = o; outbuff[o++] = 0;
            i += l*3;
            continue;
        }
        outbuff[k]++; outbuff[o] = inbuff[i]; outbuff[o+1] = inbuff[i+1]; outbuff[o+2] = inbuff[i+2]; o += 3;
        if(outbuff[k] > 127) { outbuff[k]--; k = o; outbuff[o++] = 0; }
    }
    if(!(outbuff[k] & 0x80)) { if(outbuff[k]) outbuff[k]--; else o--; }
    *outlen = o;
}

/**
 * Encode quad bytes
 */
void rle_enc32(unsigned char *inbuff, int inlen, unsigned char *outbuff, int *outlen)
{
    unsigned int *data = (unsigned int *)inbuff;
    int i, k, l, o;

    inlen >>= 2;
    if(!inbuff || inlen < 1 || !outbuff || !outlen) return;

    k = o = 0; outbuff[o++] = 0;
    for(i = 0; i < inlen; i++) {
        for(l = 1; l < 128 && i + l < inlen && data[i] == data[i + l]; l++);
        if(l > 1) {
            l--; if(outbuff[k]) { outbuff[k]--; outbuff[o++] = 0x80 | l; } else outbuff[k] = 0x80 | l;
            *((unsigned int*)(outbuff + o)) = data[i]; o += 4; k = o; outbuff[o++] = 0; i += l; continue;
        }
        outbuff[k]++; *((unsigned int*)(outbuff + o)) = data[i]; o += 4;
        if(outbuff[k] > 127) { outbuff[k]--; k = o; outbuff[o++] = 0; }
    }
    if(!(outbuff[k] & 0x80)) { if(outbuff[k]) outbuff[k]--; else o--; }
    *outlen = o;
}

/**
 * Decode bytes
 */
void rle_dec8(unsigned char *inbuff, int inlen, unsigned char *outbuff, int *outlen)
{
    int l, o = 0;
    unsigned char *end = inbuff + inlen;

    if(!inbuff || inlen < 2 || !outbuff || !outlen) return;

    while(inbuff < end) {
        l = ((*inbuff++) & 0x7F) + 1;
        if(inbuff[-1] & 0x80) {
            while(l--) outbuff[o++] = *inbuff;
            inbuff++;
        } else while(l--) outbuff[o++] = *inbuff++;
    }
    *outlen = o;
}

/**
 * Decode double bytes
 */
void rle_dec16(unsigned char *inbuff, int inlen, unsigned char *outbuff, int *outlen)
{
    int l, o = 0;
    unsigned char *end = inbuff + inlen;

    if(!inbuff || inlen < 3 || !outbuff || !outlen) return;

    while(inbuff < end) {
        l = ((*inbuff++) & 0x7F) + 1;
        if(inbuff[-1] & 0x80) {
            while(l--) { *((unsigned short int*)(outbuff + o)) = *((unsigned short int*)(inbuff)); o += 2; }
            inbuff += 2;
        } else while(l--) { *((unsigned short int*)(outbuff + o)) = *((unsigned short int*)(inbuff)); o += 2; inbuff += 2; }
    }
    *outlen = o;
}

/**
 * Decode triple bytes
 */
void rle_dec24(unsigned char *inbuff, int inlen, unsigned char *outbuff, int *outlen)
{
    int l, o = 0;
    unsigned char *end = inbuff + inlen;

    if(!inbuff || inlen < 4 || !outbuff || !outlen) return;

    while(inbuff < end) {
        l = ((*inbuff++) & 0x7F) + 1;
        if(inbuff[-1] & 0x80) {
            while(l--) { outbuff[o] = inbuff[0]; outbuff[o+1] = inbuff[1]; outbuff[o+2] = inbuff[2]; o += 3; }
            inbuff += 3;
        } else { l *= 3; while(l--) outbuff[o++] = *inbuff++; }
    }
    *outlen = o;
}

/**
 * Decode quad bytes
 */
void rle_dec32(unsigned char *inbuff, int inlen, unsigned char *outbuff, int *outlen)
{
    int l, o = 0;
    unsigned char *end = inbuff + inlen;

    if(!inbuff || inlen < 5 || !outbuff || !outlen) return;

    while(inbuff < end) {
        l = ((*inbuff++) & 0x7F) + 1;
        if(inbuff[-1] & 0x80) {
            while(l--) { *((unsigned int*)(outbuff + o)) = *((unsigned int*)(inbuff)); o += 4; }
            inbuff += 4;
        } else
            while(l--) { *((unsigned int*)(outbuff + o)) = *((unsigned int*)(inbuff)); o += 4; inbuff += 4; }
    }
    *outlen = o;
}
