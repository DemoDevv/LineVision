#include "image.h"
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image* open_image(const char* filename) {
    Image* image = malloc(sizeof(Image));
    if (!image)
        return NULL;

    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 1);
    if (!data)
        return NULL;

    image->width = width;
    image->height = height;
    image->channels = channels;
    image->data = data;

    return image;
}

void close_image(Image* image) {
    if (!image)
        return;

    stbi_image_free(image->data);
    free(image);
}
