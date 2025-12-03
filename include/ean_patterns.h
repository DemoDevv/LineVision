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

extern const size_t EAN8_LENGTH;
extern const size_t EAN8_SET_LENGTH;
extern const size_t EAN8_CODE_LENGTH;

extern const int L_CODE[10];
extern const int R_CODE[10];

SegmentEAN* create_segment_ean(const uint8_t* data, size_t length, int module);
void destroy_segment_ean(SegmentEAN* segment);

void print_segment_ean(SegmentEAN* segment);

bool is_valid_structure(const uint8_t* data, size_t length, size_t index);

int compute_check_digit(const SegmentEAN* segment);

int decode_code_ean8(const uint8_t* data, const int codes[10]);
int* decode_left_set_ean8(const SegmentEAN* segment);
int* decode_right_set_ean8(const SegmentEAN* segment);
int* decode_ean8(const SegmentEAN* segment);
