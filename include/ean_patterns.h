/**
 * @file ean_patterns.h
 * @brief EAN-8 Barcode Decoding Library
 *
 * This library provides the necessary tools to decode EAN-8 barcodes from
 * binary data representing barcode modules.
 *
 * EAN-8 Barcode Structure:
 * - Total length: 67 modules
 * - Start guard: 101 (3 modules)
 * - 4 digits encoded with L-set: 28 modules (4 × 7)
 * - Center guard: 01010 (5 modules)
 * - 4 digits encoded with R-set: 28 modules (4 × 7)
 * - End guard: 101 (3 modules)
 *
 * @author DemoDevv
 * @version 1.0
 */
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * @enum SegmentGuard
 * @brief EAN-8 barcode guard patterns
 *
 * Defines the binary patterns for the three guard bars in an EAN-8 barcode.
 * These guards delimit and separate the encoded digit sections.
 */
typedef enum {
    /** @brief Binary value of start/end guard (101) */
    EDGE_GUARD = 0b101,
    /** @brief Binary value of center guard (01010) */
    MIDDLE_GUARD = 0b01010
} SegmentGuard;

/**
 * @struct SegmentEAN
 * @brief Represents a decoded EAN-8 barcode segment
 *
 * This structure contains the binary barcode data along with the positions
 * of the guards (start, middle, end) identified during analysis.
 */
typedef struct {
    size_t start;
    size_t middle;
    size_t end;
    size_t length;
    uint8_t* data;
} SegmentEAN;

/** @brief Total length of an EAN-8 barcode in modules */
extern const size_t EAN8_LENGTH;
/** @brief Length of a set of 4 encoded digits (4 × 7 modules) */
extern const size_t EAN8_SET_LENGTH;
/** @brief Length of an individual code in modules */
extern const size_t EAN8_CODE_LENGTH;

/**
 * @brief Encoding table for L-set (left-side digits)
 *
 * Each index corresponds to a digit (0-9) and contains its 7-bit
 * encoding according to the EAN-8 standard.
 */
extern const int L_CODE[10];
/**
 * @brief Encoding table for R-set (right-side digits)
 *
 * Each index corresponds to a digit (0-9) and contains its 7-bit
 * encoding according to the EAN-8 standard.
 */
extern const int R_CODE[10];

/**
 * @brief Creates and initializes an EAN segment from raw pixel data
 *
 * This function performs sampling of the input data according to the specified
 * module width, then automatically searches for a valid EAN-8 structure (guards)
 * in the sampled data.
 *
 * The input data should be binarized pixels where:
 * - 0 represents a black pixel (barcode bar)
 * - 255 (or non-zero) represents a white pixel (barcode space)
 *
 * The function converts this to barcode representation where:
 * - 1 represents a bar (black)
 * - 0 represents a space (white)
 *
 * @param data Pointer to binarized pixel data (0 or 255 values)
 * @param length Total length of pixel data
 * @param module Width of one barcode module in pixels (sampling rate)
 *
 * @return Pointer to a dynamically allocated new SegmentEAN with valid structure,
 *         or NULL if memory allocation fails or no valid EAN-8 structure is found
 *
 * @note Allocated memory must be freed with destroy_segment_ean()
 * @note Data is sampled by taking one pixel every 'module' pixels
 * @note Pixel values are converted: 0 (black pixel) → 1 (bar), non-zero (white pixel) → 0 (space)
 * @note The function guarantees that a non-NULL return always contains valid
 *       start, middle, and end guard positions
 */
SegmentEAN* create_segment_ean(const uint8_t* data, size_t length, size_t module);

/**
 * @brief Frees the memory allocated for an EAN segment
 *
 * @param segment Pointer to the segment to destroy
 *
 * @note This function is safe with a NULL pointer
 */
void destroy_segment_ean(SegmentEAN* segment);

/**
 * @brief Prints the binary data of a segment to standard output
 *
 * @param segment Pointer to the segment to display
 *
 * @note Prints a sequence of 0s and 1s followed by a newline
 */
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

int compute_check_digit(const int* segment, const size_t size);

/**
 * @brief Decodes an individual 7-bit code
 *
 * Compares the provided 7 bits with the encoding tables to identify
 * the corresponding digit.
 *
 * @param data Pointer to 7 bits of data (uint8_t[7])
 * @param codes Encoding table to use (L_CODE or R_CODE)
 *
 * @return The decoded digit (0-9), or -1 if no match is found
 */
int decode_code_ean8(const uint8_t* data, const int codes[10]);

/**
 * @brief Decodes the 4 digits of the left set (L-set)
 *
 * @param segment Pointer to the EAN segment to decode
 *
 * @return Dynamically allocated array of 4 integers containing the decoded digits,
 *         or NULL if failure occurs (allocation or invalid decoding)
 *
 * @note Returned memory must be freed by the caller with free()
 * @note Uses segment->start to locate the beginning of data
 */
int* decode_left_set_ean8(const SegmentEAN* segment);

/**
 * @brief Decodes the 4 digits of the right set (R-set)
 *
 * @param segment Pointer to the EAN segment to decode
 *
 * @return Dynamically allocated array of 4 integers containing the decoded digits,
 *         or NULL if failure occurs (allocation or invalid decoding)
 *
 * @note Returned memory must be freed by the caller with free()
 * @note Uses segment->middle to locate the beginning of data
 */
int* decode_right_set_ean8(const SegmentEAN* segment);

/**
 * @brief Decodes a complete EAN-8 barcode
 *
 * This main function decodes all 8 digits of an EAN-8 barcode by successively
 * calling decode_left_set_ean8 and decode_right_set_ean8.
 *
 * @param segment Pointer to the EAN segment to decode
 *
 * @return Dynamically allocated array of 8 integers containing all decoded digits
 *         (4 from L-set followed by 4 from R-set), or NULL if failure occurs
 *
 * @note Returned memory must be freed by the caller with free()
 * @note This function automatically frees temporary memory used for
 *       left and right sets
 *
 * @warning Does not validate the EAN-8 checksum, only performs raw decoding
 */
int* decode_ean8(const SegmentEAN* segment);
