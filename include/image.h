#pragma once

typedef struct {
    int width;
    int height;
    int channels;
    unsigned char* data;
} Image;

Image* open_image(const char* filename);
void close_image(Image* image);
