#include "ean_errors.h"

const char* ean8_error_to_string(EAN8Error error) {
    switch (error) {
        case EAN8_ERROR_NONE: return "No error";
        case EAN8_ERROR_MEMORY_ALLOCATION: return "Memory allocation error";
        case EAN8_ERROR_INVALID_INPUT: return "Invalid input";
        case EAN8_ERROR_INVALID_CHECKSUM: return "Invalid checksum";
        case EAN8_ERROR_INVALID_FORMAT: return "Invalid format";
        case EAN8_ERROR_INVALID_DECODE: return "Invalid decode";
        default: return "Unknown error";
    }
}
