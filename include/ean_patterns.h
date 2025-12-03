#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum {
    EDGE_GUARD = 0b101,
    MIDDLE_GUARD = 0b01010
} SegmentGuard;

typedef struct {
    size_t start;
    size_t middle;
    size_t end;
    size_t length;
    uint8_t* data;
} SegmentEAN;

// Size of an EAN-8
extern const size_t EAN8_LENGTH;
// Size of an EAN-8 part (Left and Right)
extern const size_t EAN8_SET_LENGTH;
// Size of an EAN-8 code (Left and Right)
extern const size_t EAN8_CODE_LENGTH;

// Coding Table for EAN-8 (Set A)
extern const int L_CODE[10];
// Coding Table for EAN-8 (Set B)
extern const int R_CODE[10];

SegmentEAN* create_segment_ean(const uint8_t* data, size_t length, int module);
void destroy_segment_ean(SegmentEAN* segment);

void print_segment_ean(SegmentEAN* segment);

/**
 * Validate the structural layout of an EAN-8 barcode.
 *
 * This function verifies whether `data[index ... index + EAN8_LENGTH]`
 * matches the mandatory EAN-8 guard patterns and bit layout.
 *
 * The EAN-8 format is defined as:
 *
 *   Start guard  : 101        (3 bits)
 *   Left digits  : 4 × 7 bits = 28 bits
 *   Middle guard : 01010      (5 bits)
 *   Right digits : 4 × 7 bits = 28 bits
 *   End guard    : 101        (3 bits)
 *
 * Total length: 67 bits.
 *
 * It is assumed that `data` has already been:
 *   - binarized,
 *   - resampled to one bit per module,
 *   - oriented horizontally.
 *
 * @param data   Pointer to the binary bitstream (values 0 or 1 only).
 * @param length Length of the bitstream in bits.
 * @param index  Starting position where the EAN-8 structure is expected.
 *
 * @return `true` if the start guard, middle guard, and end guard
 *         are present at the expected locations; otherwise `false`.
 */
bool is_valid_structure(const uint8_t* data, size_t length, size_t index);

int compute_check_digit(const SegmentEAN* segment);

int decode_code_ean8(const uint8_t* data, const int codes[10]);
int* decode_left_set_ean8(const SegmentEAN* segment);
int* decode_right_set_ean8(const SegmentEAN* segment);
int* decode_ean8(const SegmentEAN* segment);
