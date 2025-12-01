#include "ean_patterns.h"

// Coding Table for EAN-8 (Set A)
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

// Coding Table for EAN-8 (Set B)
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
