#include "decode.h"
#include <stdlib.h>

int find_module(const uint8_t* segment, int length) {
    int* hist = calloc(length, sizeof(int));
    int counter = 0;
    int max = 0, max_value = 0;

    for (int i = 0; i < length; i++) {
        if (segment[i] == 255 && counter > 0) {
            hist[counter]++;

            if (hist[counter] > max) {
                max = hist[counter];
                max_value = counter;
            }

            counter = 0;
        } else if (segment[i] == 0) {
            counter++;
        }
    }

    if (counter > 0) {
        hist[counter]++;
        if (hist[counter] > max) {
            max = hist[counter];
            max_value = counter;
        }
    }

    free(hist);
    return max_value;
}
