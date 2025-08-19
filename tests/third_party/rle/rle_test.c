/* gcc -ansi -pedantic -Wall -Wextra -O3 test.c -o test */

#include <stdio.h>

#include "rle_opt.h"
#include "rle_libc.h"

int main(int argc, char **argv)
{
    char in[] = "aaaabbccccccdefgggggghhh";
    unsigned char out[256], out2[256], *out3, *out4;
    int len, i;

    rle_enc8((unsigned char*)&in, 24, (unsigned char*)&out, &len);
    printf("rle_enc8 encoded len: %d\n", len);

    memset(out2, 0, sizeof(out2));
    rle_dec8((unsigned char*)&out, len, (unsigned char*)&out2, &len);
    printf("rle_dec8 decoded len: %d %s\n\n", len, !memcmp(in, out2, 24) ? "OK" : "NOT MATCH");

    rle_enc16((unsigned char*)&in, 24, (unsigned char*)&out, &len);
    printf("rle_enc16 encoded len: %d\n", len);

    memset(out2, 0, sizeof(out2));
    rle_dec16((unsigned char*)&out, len, (unsigned char*)&out2, &len);
    printf("rle_dec16 decoded len: %d %s\n\n", len, !memcmp(in, out2, 24) ? "OK" : "NOT MATCH");

    rle_enc24((unsigned char*)&in, 24, (unsigned char*)&out, &len);
    printf("rle_enc24 encoded len: %d\n", len);

    memset(out2, 0, sizeof(out2));
    rle_dec24((unsigned char*)&out, len, (unsigned char*)&out2, &len);
    printf("rle_dec24 decoded len: %d %s\n\n", len, !memcmp(in, out2, 24) ? "OK" : "NOT MATCH");

    rle_enc32((unsigned char*)&in, 24, (unsigned char*)&out, &len);
    printf("rle_enc32 encoded len: %d\n", len);

    memset(out2, 0, sizeof(out2));
    rle_dec32((unsigned char*)&out, len, (unsigned char*)&out2, &len);
    printf("rle_dec32 decoded len: %d %s\n\n", len, !memcmp(in, out2, 24) ? "OK" : "NOT MATCH");

    for(i = 1; i < 5; i++) {
        rle_enc((unsigned char*)&in, 24, i, (unsigned char*)&out, &len);
        printf("static rle_enc(%d) encoded len: %d\n", i, len);

        memset(out2, 0, sizeof(out2));
        rle_dec((unsigned char*)&out, len, i, (unsigned char*)&out2, &len);
        printf("static rle_dec(%d) decoded len: %d %s\n", i, len, !memcmp(in, out2, 24) ? "OK" : "NOT MATCH");

        out3 = rle_enc((unsigned char*)&in, 24, i, (void*)0, &len);
        printf("malloc rle_enc(%d) encoded len: %d\n", i, len);

        out4 = rle_dec(out3, len, i, (void*)0, &len);
        printf("malloc rle_dec(%d) decoded len: %d %s\n\n", i, len, !memcmp(in, out4, 24) ? "OK" : "NOT MATCH");
        free(out3);
        free(out4);
    }

    return 0;
}
