#include "ean_patterns.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t EAN8_LENGTH = 67;
const size_t EAN8_SET_LENGTH = 28;
const size_t EAN8_CODE_LENGTH = 7;

const int L_CODE[10] = {
  0b0001101,
  0b0011001,
  0b0010011,
  0b0111101,
  0b0100011,
  0b0110001,
  0b0101111,
  0b0111011,
  0b0110111,
  0b0001011,
};

const int R_CODE[10] = {
  0b1110010,
  0b1100110,
  0b1101100,
  0b1000010,
  0b1011100,
  0b1001110,
  0b1010000,
  0b1000100,
  0b1001000,
  0b1110100,
};

SegmentEAN* create_segment_ean(const uint8_t* data, size_t length, size_t module) {
    SegmentEAN* segment = malloc(sizeof(SegmentEAN));
    if (!segment) return NULL;

    size_t n_modules = length / module;
    segment->data = malloc(n_modules * sizeof(uint8_t));
    if (!segment->data) {
        free(segment);
        return NULL;
    }

    segment->length = n_modules;

    // binarization of grayscale pixels (white = 0 and black = 1)
    for (size_t i = 0; i < n_modules; i++) {
        segment->data[i] = data[i * module] == 0 ? 1 : 0;
    }

    segment->start = 0;
    segment->middle = 0;
    segment->end = 0;

    bool is_valid = false;

    // find the EAN structure
    for (size_t i = 0; i < segment->length; i++) {
        if (is_valid_structure(segment->data, segment->length, i)) {
            is_valid = true;
            segment->start = i;
            segment->middle = i + 3 + EAN8_SET_LENGTH;
            segment->end = i + 3 + 5 + EAN8_SET_LENGTH * 2;
            break;
        }
    }

    if (!is_valid) {
        free(segment->data);
        free(segment);
        return NULL;
    }

    return segment;
}

bool is_valid_structure(const uint8_t* data, size_t length, size_t index) {
    if (index + EAN8_LENGTH > length) return false;

    // start guard (101)
    int start_guard = (data[index] << 2) | (data[index + 1] << 1) | data[index + 2];
    if (start_guard != EDGE_GUARD) return false;

    index += 3 + EAN8_SET_LENGTH;

    // Middle guard (01010)
    int middle_guard = (data[index]     << 4) |
                       (data[index + 1] << 3) |
                       (data[index + 2] << 2) |
                       (data[index + 3] << 1) |
                        data[index + 4];

    if (middle_guard != MIDDLE_GUARD) return false;

    index += 5 + EAN8_SET_LENGTH;

    // End guard (101)
    int end_guard = (data[index] << 2) | (data[index + 1] << 1) | data[index + 2];
    if (end_guard != EDGE_GUARD) return false;

    return true;
}

void destroy_segment_ean(SegmentEAN* segment) {
    if (!segment) return;
    free(segment->data);
    free(segment);
}

void print_segment_ean(SegmentEAN* segment) {
    for (size_t i = 0; i < segment->length; i++) {
        printf("%d", segment->data[i]);
    }
    printf("\n");
}

int decode_code_ean8(const uint8_t* data, const int codes[10]) {
    int value = 0;
    for (size_t i = 0; i < EAN8_CODE_LENGTH; i++) {
        value = (value << 1) | (data[i]);
    }

    for (int i = 0; i < 10; i++) {
        if (value == codes[i]) {
            return i;
        }
    }

    return -1;
}

int* decode_left_set_ean8(const SegmentEAN* segment) {
    if (!segment) return NULL;

    int* result = malloc(4 * sizeof(int));
    if (!result) return NULL;

    for (int i = 0; i < 4; i++) {
        int value = decode_code_ean8(&segment->data[segment->start + 3 + i * EAN8_CODE_LENGTH], L_CODE);

        if (value == -1) {
            free(result);
            return NULL;
        }

        result[i] = value;
    }

    return result;
}

int* decode_right_set_ean8(const SegmentEAN* segment) {
    if (!segment) return NULL;

    int* result = malloc(4 * sizeof(int));
    if (!result) return NULL;

    for (int i = 0; i < 4; i++) {
        int value = decode_code_ean8(&segment->data[segment->middle + 5 + i * EAN8_CODE_LENGTH], R_CODE);

        if (value == -1) {
            free(result);
            return NULL;
        }

        result[i] = value;
    }

    return result;
}

int* decode_ean8(const SegmentEAN* segment) {
    if (!segment) return NULL;

    int* result = malloc(8 * sizeof(int));
    if (!result) return NULL;

    int* left_set = decode_left_set_ean8(segment);
    if (!left_set) {
        free(result);
        return NULL;
    }

    int* right_set = decode_right_set_ean8(segment);
    if (!right_set) {
        free(result);
        return NULL;
    }

    memcpy(result, left_set, 4 * sizeof(int));
    memcpy(result + 4, right_set, 4 * sizeof(int));

    free(left_set);
    free(right_set);

    return result;
}

int compute_check_digit(const int* segment, const size_t size) {
    int sum = 0;

    for (size_t i = 0; i < size - 1; i++) {
        sum += (i % 2 == 0) ? segment[i] * 3 : segment[i];
    }

    int check_digit = 10 - sum % 10;

    return check_digit == 10 ? 0 : check_digit;
}
