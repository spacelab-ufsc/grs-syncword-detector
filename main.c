#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define _SYNC_WORD_MSB "msb"
#define _SYNC_WORD_LSB "lsb"

typedef struct {
    uint8_t*    bits;
    size_t      bit_len;
    char* endianess; 
} SyncWord;

void syncword_destroy(SyncWord* sw) {
    free(sw->bits);
    free(sw);
}

void byte_to_bitfield(uint8_t val, bool out[8], const char* endianess) {
    for(int i = 0; i < 8; i++) {
        int bit = (val >> (7 - i)) & 1;
        out[i] = bit;
    }

    /* If endianess is LSB, mirror the bitfield */
    if (strcmp(endianess, _SYNC_WORD_LSB) == 0) {
        for (int i = 0; i < 4; i++) {
            bool tmp = out[i];
            out[i] = out[7 - i];
            out[7 - i] = tmp;
        }
    }
}

int syncword_match(const SyncWord* sw, const uint8_t* window_bits) {
    int equal_bits = 0;

    for(size_t i = 0; i < sw->bit_len; i++) {
        if(sw->bits[i] == window_bits[i]) {
            equal_bits++;
        }
    }

    return equal_bits;
}

SyncWord* syncword_create(uint8_t* bytes, size_t byte_count, char* endianess) {
    SyncWord* sw = malloc(sizeof(SyncWord));
    if (!sw) return NULL;

    sw->bit_len = byte_count * 8;
    sw->bits = malloc(sw->bit_len * sizeof(bool));
    sw->endianess = endianess;

    for (size_t b = 0; b < byte_count; b++) {
        bool buf[8];
        byte_to_bitfield(bytes[b], buf, endianess);

        memcpy(sw->bits + (b * 8), buf, 8 * sizeof(bool));
    }

    return sw;
}

int main() {
    return 0;
}
