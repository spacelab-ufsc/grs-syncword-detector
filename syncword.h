#ifndef SYNCWORD_H
#define SYNCWORD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * Represents a sync word expanded into a bit array for fast comparison.
 */
typedef struct {
    bool*  bits;
    size_t bit_len;
} SyncWord;

/**
 * Create a SyncWord from a byte array.
 *
 * @param bytes      Raw sync word bytes
 * @param byte_count Number of bytes
 * @param lsb_first  Bit order: true = LSB first, false = MSB first
 * @return           Heap-allocated SyncWord, or NULL on allocation failure
 */
SyncWord* syncword_create(uint8_t* bytes, size_t byte_count, bool lsb_first);

/**
 * Search for a sync word in a bit stream.
 *
 * @param stream_bits      Incoming bit stream (one bool per bit)
 * @param syncword         The sync word to search for
 * @param max_errors       Maximum bit mismatches to tolerate (typically 1-2)
 * @param size_stream_bits Length of stream_bits in bits
 * @return                 Index of first bit AFTER the sync word,
 *                         or -1 if not found
 */
int syncword_detect(bool* stream_bits, SyncWord* syncword,
                    int max_errors, int size_stream_bits);

/**
 * Free a SyncWord created by syncword_create.
 */
void syncword_destroy(SyncWord* sw);

#endif /* SYNCWORD_H */