#include <stdio.h>
#include "image.h"
#include "decode.h"

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
    // print_image(image);

    int threshold = otsu_threshold(image->data, image->height * image->width);
    printf("Threshold: %d\n", threshold);
    binarization(image, threshold);

    // save_image_png(image, "images/output.png");
    int module = find_module(&image->data[image->width * (image->height / 2)], image->width);
    printf("Module: %d\n", module);

    // Free the image memory
    close_image(image);

    return 0;
}
