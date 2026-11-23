/*
 * syncword_test.c
 *
 * Unit tests for the sync word detector.
 *
 * Build and run:
 *     cd tests && make && ./syncword_unit_test
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>

#include <cmocka.h>

#include "syncword.h"

/* NGHam preamble + sync word, the pattern the ground station looks for. */
static uint8_t SYNC[4] = {0x5D, 0xE6, 0x2A, 0x7E};

size_t count_ones_in_byte(uint8_t byte);

static void count_ones_in_byte_test(void **state)
{
    assert_int_equal(count_ones_in_byte(0x00), 0);
    assert_int_equal(count_ones_in_byte(0x01), 1);
    assert_int_equal(count_ones_in_byte(0x80), 1);
    assert_int_equal(count_ones_in_byte(0xFF), 8);
    assert_int_equal(count_ones_in_byte(0x5D), 5);
}

static void syncword_create_test(void **state)
{
    SyncWord *sw = syncword_create(SYNC, sizeof(SYNC));

    assert_non_null(sw);
    assert_int_equal(sw->byte_len, sizeof(SYNC));
    assert_memory_equal(sw->bytes, SYNC, sizeof(SYNC));

    syncword_destroy(sw);
}

/* The sync word must own its bytes: the caller's buffer may be a local that
 * goes out of scope, or may be reused for the next word. */
static void syncword_create_copies_the_caller_bytes_test(void **state)
{
    uint8_t caller_bytes[4] = {0x5D, 0xE6, 0x2A, 0x7E};
    SyncWord *sw = syncword_create(caller_bytes, sizeof(caller_bytes));

    assert_non_null(sw);
    memset(caller_bytes, 0xFF, sizeof(caller_bytes));

    assert_memory_equal(sw->bytes, SYNC, sizeof(SYNC));

    syncword_destroy(sw);
}

static void syncword_detect_at_start_test(void **state)
{
    uint8_t stream[8] = {0x5D, 0xE6, 0x2A, 0x7E, 0x11, 0x22, 0x33, 0x44};
    SyncWord *sw = syncword_create(SYNC, sizeof(SYNC));

    assert_int_equal(syncword_detect(sw, 0, sizeof(stream), stream), 4);

    syncword_destroy(sw);
}

static void syncword_detect_in_the_middle_test(void **state)
{
    uint8_t stream[8] = {0xAA, 0xBB, 0x5D, 0xE6, 0x2A, 0x7E, 0x33, 0x44};
    SyncWord *sw = syncword_create(SYNC, sizeof(SYNC));

    /* Returns the index of the first payload byte, right after the word. */
    assert_int_equal(syncword_detect(sw, 0, sizeof(stream), stream), 6);

    syncword_destroy(sw);
}

static void syncword_detect_absent_test(void **state)
{
    uint8_t stream[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    SyncWord *sw = syncword_create(SYNC, sizeof(SYNC));

    assert_int_equal(syncword_detect(sw, 0, sizeof(stream), stream), -1);

    syncword_destroy(sw);
}

static void syncword_detect_stream_shorter_than_word_test(void **state)
{
    uint8_t stream[2] = {0x5D, 0xE6};
    SyncWord *sw = syncword_create(SYNC, sizeof(SYNC));

    assert_int_equal(syncword_detect(sw, 0, sizeof(stream), stream), -1);

    syncword_destroy(sw);
}

/* One corrupted bit is the normal case on a noisy link: 0x5C differs from
 * 0x5D by a single bit. */
static void syncword_detect_tolerates_one_bit_error_test(void **state)
{
    uint8_t stream[8] = {0x5C, 0xE6, 0x2A, 0x7E, 0x11, 0x22, 0x33, 0x44};
    SyncWord *sw = syncword_create(SYNC, sizeof(SYNC));

    assert_int_equal(syncword_detect(sw, 1, sizeof(stream), stream), 4);
    assert_int_equal(syncword_detect(sw, 0, sizeof(stream), stream), -1);

    syncword_destroy(sw);
}

static void syncword_detect_rejects_too_many_errors_test(void **state)
{
    /* 0xA2 differs from 0x5D in all eight bits. */
    uint8_t stream[8] = {0xA2, 0xE6, 0x2A, 0x7E, 0x11, 0x22, 0x33, 0x44};
    SyncWord *sw = syncword_create(SYNC, sizeof(SYNC));

    assert_int_equal(syncword_detect(sw, 2, sizeof(stream), stream), -1);
    assert_int_equal(syncword_detect(sw, 8, sizeof(stream), stream), 4);

    syncword_destroy(sw);
}

static void syncword_detect_returns_the_first_match_test(void **state)
{
    uint8_t stream[12] = {0x5D, 0xE6, 0x2A, 0x7E, 0x11,
                          0x5D, 0xE6, 0x2A, 0x7E, 0x22, 0x33, 0x44};
    SyncWord *sw = syncword_create(SYNC, sizeof(SYNC));

    assert_int_equal(syncword_detect(sw, 0, sizeof(stream), stream), 4);

    syncword_destroy(sw);
}

static void syncword_destroy_accepts_null_test(void **state)
{
    syncword_destroy(NULL);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(count_ones_in_byte_test),
        cmocka_unit_test(syncword_create_test),
        cmocka_unit_test(syncword_create_copies_the_caller_bytes_test),
        cmocka_unit_test(syncword_detect_at_start_test),
        cmocka_unit_test(syncword_detect_in_the_middle_test),
        cmocka_unit_test(syncword_detect_absent_test),
        cmocka_unit_test(syncword_detect_stream_shorter_than_word_test),
        cmocka_unit_test(syncword_detect_tolerates_one_bit_error_test),
        cmocka_unit_test(syncword_detect_rejects_too_many_errors_test),
        cmocka_unit_test(syncword_detect_returns_the_first_match_test),
        cmocka_unit_test(syncword_destroy_accepts_null_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
