#include "image.h"
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image* open_image(const char* filename, int desired_channels) {
    Image* image = malloc(sizeof(Image));
    if (!image)
        return NULL;

    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, desired_channels);
    if (!data) {
        free(image);
        return NULL;
    }

    image->width = width;
    image->height = height;
    image->channels = (desired_channels > 0) ? desired_channels : channels;
    image->data = data;

    return image;
}

void close_image(Image* image) {
    if (!image) return;

    stbi_image_free(image->data);
    free(image);
}

void print_image_info(Image* image) {
    if (!image) return;

    printf("Image Info:\n");
    printf("Width: %d\n", image->width);
    printf("Height: %d\n", image->height);
    printf("Channels: %d\n", image->channels);
}

void print_image(Image* image) {
    if (!image) return;

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            for (int c = 0; c < image->channels; c++) {
                printf("%u ", image->data[y * image->width * image->channels + x * image->channels + c]);
            }
        }
        printf("\n");
    }
}
