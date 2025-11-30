#pragma once

typedef struct {
    int width;
    int height;
    int channels;
    unsigned char* data;
} Image;

Image* open_image(const char* filename, int desired_channels);
void close_image(Image* image);
