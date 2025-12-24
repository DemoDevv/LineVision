#pragma once

typedef enum {
    EAN8_ERROR_NONE = 0,
    EAN8_ERROR_MEMORY_ALLOCATION = 1,
    EAN8_ERROR_INVALID_INPUT = 2,
    EAN8_ERROR_INVALID_CHECKSUM = 3,
    EAN8_ERROR_INVALID_FORMAT = 4,
    EAN8_ERROR_INVALID_DECODE = 5
} EAN8Error;

const char* ean8_error_to_string(EAN8Error error);
