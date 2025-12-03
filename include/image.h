#pragma once
#include <stdint.h>

typedef struct {
    int width;
    int height;
    int channels;
    unsigned char* data;
} Image;

// Open an image file and return a pointer to the Image struct
Image* open_image(const char* filename, int desired_channels);
// Close the image and free the memory
void close_image(Image* image);

void print_image(Image* image);
// Print the image information
void print_image_info(Image* image);

// find the best threshold for binarization with Otsu's method
int otsu_threshold(const uint8_t* gray, int length);
// Binarize the image using the given threshold
void binarization(Image* image, int threshold);

// Save the image to a PNG file
void save_image_png(Image* image, const char* filename);
