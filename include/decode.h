#pragma once
#include <stdint.h>

uint8_t* construct_segment(const uint8_t* data, int length);
int find_module(const uint8_t* segment, int length);
