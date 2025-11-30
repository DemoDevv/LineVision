#include <stdio.h>
#include "image.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    char* image_file = argv[1];

    Image* image = open_image(image_file);
    if (!image) {
        printf("Failed to load image file: %s\n", image_file);
        return 1;
    }

    printf("Image loaded successfully!\n");
    printf("width: %d\n", image->width);
    printf("height: %d\n", image->height);
    printf("channels: %d\n", image->channels);

    // Free the image memory
    close_image(image);

    return 0;
}
