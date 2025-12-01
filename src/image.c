#include "image.h"
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

// find the best threshold for binarization
int otsu_threshold(const uint8_t* gray, int length) {
    int histogram[256] = {0};

    // compute grey scale histogram
    for (int i = 0; i < length; i++) {
        histogram[gray[i]]++;
    }

    long sum_total = 0;
    for (int i = 0; i < 256; i++) {
        sum_total += i * histogram[i];
    }

    int wB = 0;
    double sumB = 0.0, meanB, meanF, maxInterVar = 0.0;

    int best_threshold = 0;

    for (int i = 0; i < 256; i++) {
        // no need to use probability here, just pixels number
        wB += histogram[i];
        if (wB == 0) continue;

        int wF = length - wB;
        if (wF == 0) break;

        sumB += i * histogram[i];

        meanB = sumB / wB;
        // we can find sumF by subtracting sumB from sum_total
        double sumF = sum_total - sumB;
        meanF = sumF / wF;

        // calculate inter-class variance
        double var = wB * wF * (meanB - meanF) * (meanB - meanF);

        if (var > maxInterVar) {
            maxInterVar = var;
            best_threshold = i;
        }
    }

    return best_threshold;
}

void binarization(Image* image, int threshold) {
    if (!image) return;

    for (int i = 0; i < image->width * image->height * image->channels; i++) {
        image->data[i] = image->data[i] > threshold ? 255 : 0;
    }
}

void save_image_png(Image* image, const char* filename) {
    stbi_write_png(filename, image->width, image->height, image->channels, image->data, image->width * image->channels);
}
