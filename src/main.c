#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "decode.h"
#include "ean_patterns.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    char* image_file = argv[1];

    Image* image = open_image(image_file, 1);
    if (!image) {
        printf("Failed to load image file: %s\n", image_file);
        return 1;
    }

    printf("Image loaded successfully!\n");
    print_image_info(image);

    // binarization
    int threshold = otsu_threshold(image->data, image->height * image->width);
    printf("Threshold: %d\n", threshold);
    binarization(image, threshold);


    uint8_t* middle_segment = &image->data[image->width * (image->height / 2)];
    int module = find_module(middle_segment, image->width);
    printf("Module: %d\n", module);

    SegmentEAN* segment = create_segment_ean(middle_segment, image->width, module);

    print_segment_ean(segment);
    printf("Segment start: %lu\n", segment->start);
    printf("Segment middle: %lu\n", segment->middle);
    printf("Segment end: %lu\n", segment->end);

    // decode CAB
    int* cab = decode_ean8(segment);

    for (int i = 0; i < 8; i++) {
        printf("CAB[%d]: %d\n", i, cab[i]);
    }

    free(cab);

    destroy_segment_ean(segment);

    // save_image_png(image, "images/output.png");

    // Free the image memory
    close_image(image);

    return 0;
}
