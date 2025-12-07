#include "decode.h"
#include <stdlib.h>

size_t find_module(const uint8_t* segment, int length) {
    if (!segment || length <= 0) return 0;

    int max_module_width = length / 10;
    int* hist = calloc(max_module_width + 1 , sizeof(int));
    if (!hist) return 0;

    int counter = 0;
    uint8_t current_color = segment[0];

    for (int i = 0; i < length; i++) {
        if (segment[i] == current_color) {
            counter++;
        } else {
            // Transition detected
            if (current_color == 0 && counter > 0 && counter <= max_module_width) {
                hist[counter]++;
            }
            current_color = segment[i];
            counter = 1;
        }
    }

    if (current_color == 0 && counter > 0 && counter <= max_module_width) {
        hist[counter]++;
    }

    int max_count = 0;
    size_t module_width = 1;

    // find the max
    for (int i = 1; i <= max_module_width; i++) {
        if (hist[i] > max_count) {
            max_count = hist[i];
            module_width = i;
        }
    }

    free(hist);
    return module_width;
}
