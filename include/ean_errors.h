#pragma once

/**
 * @enum EAN8Error
 * @brief Error codes returned by EAN-8 library functions.
 */
typedef enum {
    /** @brief Success. No error occurred. */
    EAN8_ERROR_NONE = 0,
    /** @brief Memory allocation failed (malloc/calloc). */
    EAN8_ERROR_MEMORY_ALLOCATION = 1,
    /** @brief Invalid argument passed to the function (e.g., NULL pointer). */
    EAN8_ERROR_INVALID_INPUT = 2,
    /** @brief The check digit (8th digit) is incorrect. */
    EAN8_ERROR_INVALID_CHECKSUM = 3,
    /** @brief Invalid EAN-8 segment format (wrong size or missing guard patterns). */
    EAN8_ERROR_INVALID_FORMAT = 4,
    /** @brief Failed to decode bar patterns into digits (unknown pattern). */
    EAN8_ERROR_INVALID_DECODE = 5
} EAN8Error;

/**
 * @brief Converts an EAN8Error code into a human-readable string.
 * * @param error The error code to translate.
 * @return A pointer to a constant string describing the error.
 */
const char* ean8_error_to_string(EAN8Error error);
