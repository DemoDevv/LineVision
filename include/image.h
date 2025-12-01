#pragma once
#include <stdint.h>

typedef struct {
    int width;
    int height;
    int channels;
    unsigned char* data;
} Image;

Image* open_image(const char* filename, int desired_channels);
void close_image(Image* image);

void print_image(Image* image);
void print_image_info(Image* image);

int otsu_threshold(const uint8_t* gray, int length);
void binarization(Image* image, int threshold);

void save_image_png(Image* image, const char* filename);
